#include <iostream>
#include <memory>
#include <vector>

class Resource {
public:
    ~Resource()
    {
        //Um zu beweisen dass Resource kopiert wurde wird die Adresse ausgegeben
        std::cout << "Resource Destruktor (Address " << (this) << ")\n";
    }
private:
};

class Example
{
public:
    explicit Example(size_t size, Resource *res)
            : resource {*res}
    {
        buffer.resize(size);
    }

private:
    Resource resource;
    std::vector<int> buffer;
};

int main()
{
    {
        Example a(5, new Resource());
        Example b{a};
    }

    return 0;
}