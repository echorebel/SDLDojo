#include "Spaceship.h"
#include <iostream>

using namespace std;

Spaceship::Spaceship()
{
    cout << "Spaceship created" << endl;
    x = 100;
    y = 100;
    energy = 10;
}

Spaceship::fire()
{
  cout << "fire" << endl;
}
