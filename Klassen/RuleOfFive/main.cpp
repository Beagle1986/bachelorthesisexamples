#include <iostream>

class Resource {public:
    ~Resource() {
        std::cout << "Resource Destruktor (Address " << (this) <<")\n";
    }
};

class Example {
public:
    explicit Example(int size, Resource* res)
            : resource{res},buffer{new int[size]} {
    }

    /*Beispiel wie der vom Compiler generierte Kopierkonstruktor aussieht /+\label{line:copycc}+/
    inline Example(Example const &rhs) {
        resource = rhs.resource;  //Nur Zeiger wird kopiert
        array_of_int = rhs.resource; //Nur der Zeiger auf das erste Feld wird kopiert
    */

    ~Example() {
        delete resource;
        delete[] buffer;
    }

private:
    Resource *resource;   //Alternative unique_ptr<Resource> oder shared_ptr<Resource>
    int *buffer;      //Alternative vector<int>
};

int main()
{
    {
        Example a(5, new Resource());
        Example b{a}; //Flache Kopie durch impliziten Kopierkonstruktor /+\label{line:flatcopy}+/
    } //Destruktor bzw. delete auf Rsesourcen zwei Mal aufgerufen /+\label{line:doubledelete}+/
      //Ressource-Leck und undefiniertes Verhalten
    return 0;
}