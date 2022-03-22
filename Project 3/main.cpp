#include "provided.h"
#include <iostream>
#include <cassert>
using namespace std;


int main()
{
    HumanPlayer bp1("Bart");
    SmartPlayer bp2("Homer");
    Game g(7, 6, 4, &bp1, &bp2);
    g.play();
}
