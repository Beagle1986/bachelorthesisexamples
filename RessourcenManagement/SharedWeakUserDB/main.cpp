#include <iostream>
#include <memory>
#include <unordered_map>
#include <mutex>

class User {
public:
    void set_name(std::string const &name) {
        this->name = name;
    }

    auto &get_name() const {
        return name;
    }

    ~User() {
        std::cout << "Destruktor User aufgerufen" << '\n';
    }

private:
    std::string name {"User"};
};

std::unique_ptr<User> loadUser(int id){
    //Hier möglicher Zugriff auf Datenquelle
    return std::make_unique<User>();
}

std::shared_ptr<User> loadUser_cached(int id)
{
    static std::unordered_map<int, std::weak_ptr<User>> cache;
    static std::mutex mut;

    std::lock_guard<std::mutex> guard(mut);

    auto user {cache[id].lock()};

    if(user) {
        user->set_name("User aus Cache");
    }

    if(!user) { //Falls nicht im Cache
        user = loadUser(id); //Lade User
        user->set_name("User aus Datenbank");
        cache[id] = user; //Speichere User im Cache
    }

    return user;
}

int main()
{
    auto user1{loadUser_cached(42)};
    std::cout << "User: " << user1->get_name() << '\n';

    //Wird reset aufgerufen, wird der Zeiger im weak_ptr
    //der unordered_map ebenfalls ungültig.
    //user1.reset();

    auto user2{loadUser_cached(42)};
    std::cout << "User: " << user2->get_name() << '\n';

    return 0;
}