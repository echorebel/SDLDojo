#include "res_path.h"
#include "cleanup.h"
#include "sdl.h"

#include "Spaceship.h" //TODO fix class

// TODO backbuffer handling

using namespace std;

int main()
{
   const int SCREEN_WIDTH = 640;
   const int SCREEN_HEIGHT = 460;

   // init SDL: SDL_INIT_EVERYTHING or only SDL_INIT_VIDEO
   if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
      logSDLError(std::cout, "SDL_INIT");
      return 1;
   }

   if(TTF_Init() !=0)
   {
      logSDLError(std::cout, "TTF_Init");
      SDL_Quit();
      return 1;
   }

   // fullscreen
   //SDL_Window *win = SDL_CreateWindow("Ronin", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_FULLSCREEN);
   SDL_Window *win = SDL_CreateWindow("Ronin", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
   if(win == nullptr)
   {
      logSDLError(std::cout, "CreateWindow");
      SDL_Quit();
      return 1;
   }

   SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if(ren == nullptr)
   {
      cleanup(win);
      logSDLError(std::cout, "CreateRenderer");
      SDL_Quit();
      return 1;
   }

   // not necessary happens automatically but it will delay start
   // so it's better to have it under control
   if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
   {
      logSDLError(std::cout, "IMG_Init");
      SDL_Quit();
      return 1;
   }

   const std::string resPath = getResourcePath("Ronin");

   SDL_Texture *shipTex = loadTexture(resPath + "spaceship.png", ren);
   // make sure they both loaded
   if(shipTex == nullptr)
   {
      cleanup(shipTex, ren, win);
      IMG_Quit();
      TTF_Quit();
      SDL_Quit();
      return 1;
   }

   // Color is in RGBA format
   SDL_Color color = {255,255,255,255};
   SDL_Texture *image = renderText("Ronin", resPath + "kenvector_future.ttf", color, 64, ren);
   if(image == nullptr)
   {
      cleanup(shipTex, ren, win);
      IMG_Quit();
      TTF_Quit();
      SDL_Quit();
      return 1;
   }

   bool quit = false;
   SDL_Event e;

   Spaceship ship;
   ship.y = 200;
   int speedX = 0;
   int speedY = 0;

   // get texture w/h so we can enter it in the screen
   int iW, iH;
   SDL_QueryTexture(image, NULL,NULL, &iW, &iH);
   int x = SCREEN_WIDTH / 2 - iW / 2;
   int y = SCREEN_HEIGHT / 2 - iH / 2;

   while(!quit)
   {
      // e is an SDL_Event variable we've declared before entering the main loop
      while(SDL_PollEvent(&e))
      {
         // read user input and handle it
         // If user closes the window
         if(e.type == SDL_QUIT)
         {
            quit = true;
         }
         // if user presses any key
         if(e.type == SDL_KEYDOWN)
         {
            //use number input to select which clip should be drawn
            switch (e.key.keysym.sym)
            {
               case SDLK_ESCAPE:
                  quit = true;
                  break;
               case SDLK_LEFT:
                    std::cout << "left" << '\n';
                  speedX = ship.maxSpeedX*-1;
                  break;
               case SDLK_RIGHT:
                    std::cout << "right" << '\n';
                  speedX = ship.maxSpeedX;
                  break;
               case SDLK_UP:
                  speedY = -3;
                  break;
               case SDLK_DOWN:
                  speedY = 3;
                  break;
               default:
                  cout << "button pressed: " << e.key.keysym.sym << endl;
                  break;
            }
         }
         if(e.type == SDL_KEYUP)
         {
            switch (e.key.keysym.sym)
            {
               case SDLK_RIGHT:
               case SDLK_LEFT:
                    std::cout << "reset horizontal throttle" << std::endl;
                  speedX = 0;
                  break;
               case SDLK_UP:
               case SDLK_DOWN:
                    std::cout << "reset vertical throttle" << std::endl;
                  speedY = 0;
                  break;
            }
         }
         // If user clicks the mouse
         if(e.type == SDL_MOUSEBUTTONDOWN)
         {
            cout << "mouse clicked" << endl;
         }
      }

      //update
      if(speedX!=0) ship.x += speedX;
      if(speedY!=0) ship.y += speedY;
      if(ship.x < 0) ship.x = 0;
      if(ship.x > SCREEN_WIDTH-25) ship.x = SCREEN_WIDTH-25;
      if(ship.y < 0) ship.y = 0;
      if(ship.y > SCREEN_HEIGHT-28) ship.y = SCREEN_HEIGHT-28;

      // render our scene
      SDL_RenderClear(ren);
      renderTexture(image, ren, x, y);
      renderTexture(shipTex, ren, ship.x, ship.y);
      SDL_RenderPresent(ren);

      //SDL_Delay(1);
   }

   cleanup(image, ren, win);
   TTF_Quit();
   SDL_Quit();

   return 0;
}
