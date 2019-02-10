#include <iostream>
#include <memory>

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
    explicit Example()
            : resource1 {new Resource()},
              resource2 {new Resource()}
    {
    }

private:
    Resource* resource1; //std::unique_ptr<Resource> resource1;
    Resource* resource2; //std::unique_ptr<Resource> resource2;
};


int main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}