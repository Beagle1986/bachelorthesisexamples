#include <iostream>

class Resource {
public:
    ~Resource() {
        std::cout << "Resource Destruktor (Address " << (this) <<")\n";
    }
};

class Example
{
public:
    explicit Example(int size, Resource* res) : resource{res}, size{size}, buffer{new int[size]} {
        std::cout << "Example Konstruktor\n";
    }

    Example(Example const& rhs) :
            resource{new Resource(*rhs.resource)}, //Tiefe Kopie
            size{rhs.size},
            buffer{new int[rhs.size]}
    {
        for(int i = 0; i < rhs.size; ++i) //Tiefe Kopie von Buffer
        {
            buffer[i] = rhs.buffer[i];
        }
        std::cout << "Example Kopierkonstruktor\n";
    }

    Example& operator=(Example const& rhs)
    {
        if (&rhs != this) //Schutz vor Selbstzuweisung
        {
            if (rhs.size != size) {
                delete [] buffer; buffer = new int[rhs.size];
            }

            size = rhs.size;
            buffer = rhs.buffer;
            resource = new Resource(*rhs.resource);

            for(int i = 0; i < rhs.size; ++i) //Tiefe Kopie von Buffer
            {
                buffer[i] = rhs.buffer[i];
            }
        }

        std::cout << "Example Kopier-Zuweisung\n";

        return *this;
    }

    Example (Example&& tmp) noexcept
    :   resource{tmp.resource},
        size{tmp.size},
        buffer{tmp.buffer}
    {
        std::cout << "Example Move-Konstruktor\n";
    }

    Example& operator=(Example&& tmp) noexcept
    {
        if (&tmp != this) //Schutz vor Selbstzuweisung
        {
            resource = tmp.resource,
            size = tmp.size,
            buffer = tmp.buffer;
        }

        std::cout << "Example Move-Zuweisung\n";

        return *this;
    }

    ~Example()
    {
        std::cout << "Example Destruktor\n";
        delete resource;
        delete[] buffer;
    }

private:
    Resource *resource;   //Alternative unique_ptr<Resource> oder shared_ptr<Resource>
    int *buffer;      //Alternative vector<int>
    int size;
};

int main()
{
    {
        Example a(5, new Resource());
        Example b{a};
    }

    return 0;
}