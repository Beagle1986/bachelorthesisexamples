#include <iostream>
#include <memory>

class Robot {};

void moveRobot(std::shared_ptr<Robot> robot, int speed) {}

int calculateSpeed() { } //wirft eventuell eine Exception (ZB. Division durch 0)

int main()
{
    //Potentielles Ressource-Leck wegen nicht deterministischer
    //Argument-Abarbeitungsreihenfolge
    moveRobot(std::shared_ptr<Robot>(new Robot), calculateSpeed()); //  /+\label{line:sharedptrpossibleResourceLeak}+/

    //Ressource-Leck kann nicht mehr auftreten
    moveRobot(std::make_shared<Robot>(), calculateSpeed()); // /+\label{line:sharedptrNoResourceLeak}+/

    //Von Meyers vorgeschlagene Best Practice bei Verwendung einer benutzerdefinierten Freigabefunktion
    auto deleter { [](Robot *pw){delete pw;} }; // /+\label{line:sharedptrdeleterBegin}+/
    auto robot { std::shared_ptr<Robot>(new Robot, deleter)};

    //Übergabe per rvalue effizienter als per lvalue. Daher Cast mit std::move
    moveRobot(std::move(robot), calculateSpeed()); //  /+\label{line:sharedptrdeleterEnd}+/

    return 0;
}