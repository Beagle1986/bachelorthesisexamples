#include <iostream>
#include <memory>
#include <vector>

class Ressource {public:
    ~Ressource() {
        std::cout << "Ressource Destruktor (Address " << (this) <<")\n";
}};

class Example
{
public:
    explicit Example(size_t size, std::unique_ptr<Ressource> res)
            : ressource {std::move(res)}
    {
        buffer.resize(size);
    }

private:
    std::unique_ptr<Ressource> ressource;
    std::vector<int> buffer;
};

int main()
{
    {
        Example a(5, std::make_unique<Ressource>());
        Example b{std::move(a)}; //Example wird durch unique_ptr zum Move-Only Typ
    }

    return 0;
}