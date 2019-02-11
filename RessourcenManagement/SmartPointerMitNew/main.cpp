#include <iostream>
#include <memory>

class Product
{
public:
    void print() const {
        std::cout << "Print aufgerufen" << '\n';
    }
};

int main()
{
    //unique_ptr Beispiel
    //Lambda-Funktion als Freigabefunktion
    auto deleter = [](Product *p) {
        std::cout << "Produkt gelöscht" << '\n';
        delete p;
    };

    std::unique_ptr<Product, decltype(deleter)> up1(new Product, deleter); // /+\label{line:deleteruniqueptr}+/
    std::unique_ptr<Product, decltype(deleter)> up2{std::move(up1)}; //Aufruf Move-Konstruktor
    up1->print(); //Zugriffsverhalten undefiniert

    //shared_ptr Beispiel
    auto p {new Product}; // /+\label{line:shared_ptrDeleterBegin}+/
    {
        std::shared_ptr<Product> sp1{p,deleter};
        {
            std::shared_ptr<Product> sp2{p,deleter};
        }

        sp1->print(); //Zugriffsverhalten undefiniert
    }
    p->print(); //Zugriffsverhalten undefiniert p /+\label{line:shared_ptrDeleterEnd}+/

    return 0;
}