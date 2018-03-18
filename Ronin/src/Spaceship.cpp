#include "Spaceship.h"
#include <iostream>

using namespace std;

Spaceship::Spaceship()
{
    cout << "Spaceship created" << endl;
    x = 100;
    y = 100;
    maxSpeedX = 3;
    maxSpeedY = 3;
    energy = 10;
}

void Spaceship::fire()
{
  cout << "fire" << endl;
}
