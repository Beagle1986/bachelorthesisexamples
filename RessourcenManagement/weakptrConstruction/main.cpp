#include <iostream>
#include <memory>

class User {};

int main()
{
    auto sp {std::make_shared<User>()};
    std::weak_ptr<User> wp{sp};

    //Raw-Pointer ohne Möglichkeit festzustellen ob Zeiger hängt
    User* rpu {sp.get()};

    //reset() um shared_ptr Ressource-Freigabe zu simulieren
    //sp.reset();

    if(!wp.expired()) {
        std::shared_ptr<User> spNew {wp.lock()};
    }
    else {
        std::cout << "Objekt auf das weak_ptr zeigt nicht mehr gültig.";
    }
    return 0;
}