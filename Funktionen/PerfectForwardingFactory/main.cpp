#include <iostream>
#include <memory>

class ProductBase {};

class Product1 : public ProductBase {
public:
    template <typename T,
            typename = typename std::enable_if_t <!std::is_same_v<Product1, std::decay_t<T>>>>
    explicit Product1(T&& param) : param_name{std::forward<T>(param)} {} // /+\label{line:TForwardRef1}+/
    //Nicht benötigt wegen Forwarding-Referenz Überladung
    //explicit Product1(std::string param) : param_name{std::move(param)} {}
    //explicit Product1(std::string const& param) : param_name{param} {}
    //explicit Product1(const char* param) : param_name{param} {}
    //explicit Product1(std::string&& param) : param_name{param} {}
private:
    std::string param_name;
};

template<typename T>
std::string get_type()
{
    std::string s;
    if (std::is_const_v<std::remove_reference_t<T>>)
        s += " const";

    if (std::is_volatile_v<std::remove_reference_t<T>>)
        s += " volatile";

    if (std::is_lvalue_reference<T>::value)
        s += " lvalue";

    if (std::is_rvalue_reference<T>::value)
        s += " rvalue";

    return s;
}

//Nicht benötigt wegen Forwarding-Referenz Überladung
/*bool check_parameter(std::string& param) {
    return true;
}

bool check_parameter(std::string const& param) {
    return true;
}*/

/*bool check_parameter(std::string&& param) {
    return true;
}*/

/*bool check_parameter(const char* param) {
    return true;
}*/

template <typename T,
        typename = std::enable_if_t<std::is_convertible_v <T,std::string>>>
bool check_parameter(T&& stringParam) { // /+\label{line:TForwardRefParam2}+/+/
    std::cout << "In check_parameter:" << '\n';
    std::cout << "param ist" << get_type<decltype(stringParam)>() << " \n";

    return true;
}


//Bindet Lvalues und ermöglicht Move-Semantik. Nachteil: Bindet nur LValues, Temporäres Objekt wird erzeugt für const char*
/*std::unique_ptr<ProductBase> create_product(int productID, std::string param)
{
    if(!check_parameter(param)){}

    if(productID == 1) {
        return std::make_unique<Product1>(param); // /+\label{line:OverloadRegularParam}+/
    }
}*/

//std::string const& bindet sowohl Lvalue als auch Rvalues sowie deren const Typen.
//Nachteil: Verhindert Move-Semantik, Temporäres Objekt wird erzeugt für const char*
/*std::unique_ptr<ProductBase> create_product(int productNo, std::string const& param)
{
    if(!check_parameter(param)) {}

    if (productID == 1) {
        return std::make_unique<Product1>(param); // /+\label{line:OverloadRegularConstParam}+/
    }
}*/

//Für param als std::string Rvalue um Move-Semantik zu ermöglichen
/*std::unique_ptr<ProductBase> create_product(int productNo, std::string&& param)
{
    if(!check_parameter(param)) {}

    if (productID == 1) {
        return std::make_unique<Product1>(param); // /+\label{line:OverloadRegularConstParam}+/
    }
}*/

/*std::unique_ptr<ProductBase> create_product(int productNo, const char* param)
{
    if(!check_parameter(param)) {}

    if (productID == 1) {
        return std::make_unique<Product1>(param); // /+\label{line:OverloadConstCharParam}+/
    }
}*/

template<typename T>
//std::unique_ptr<ProductBase> create_product(int productNo, T param) //per Wert. Temporäres Objekt
//std::unique_ptr<ProductBase> create_product(int productNo, T& param) //Keine Rvalues, weil nur Lvalue
//std::unique_ptr<ProductBase> create_product(int productNo, T const& param) //Kein Move
std::unique_ptr<ProductBase> create_product(int productNo, T&& param)
{
    std::cout << "In create_product..." << '\n';
    std::cout << "param ist" << get_type<decltype(param)>() << " \n";

    if (!check_parameter(param)) { /*Fehlerbehandlung*/ }

    std::cout << "In create_product after check_parameter..." << '\n';
      if (std::is_rvalue_reference<decltype(std::forward<T>(param))>::value) // /+\label{line:TForwardRef2}+/
        std::cout << "param mit forward wieder in Rvalue-Ref. umgewandelt" << '\n';

    if (!check_parameter(std::forward<T>(param))) { /*Fehlerbehandlung*/ } // /+\label{line:TForwardRef3}+/

    if (productNo == 1) {
        return std::unique_ptr<Product1>(new Product1(std::forward<T>(param))); // /+\label{line:TForwardInConstructor}+/
    }
    //if (productNo == 1) {
    //    return std::make_unique<Product1>(param); //std::forward nicht notwendig, weil intern // /+\label{line:TForwardInUnique}+/
   //}
}

int main()
{
    std::cout << "Lvalue Referenz auf const std::string" << '\n';
    const std::string cs {std::string("Produktname")};
    auto p1 {create_product(1, cs)}; // /+\label{line:LValueRef1}+/

    std::cout << '\n' << "Lvalue Referenz mit std::move in Rvalue umgewandelt" << '\n';
    std::string s {"Produktname"};
    auto p2 {create_product(1, std::move(s))}; //Ohne PF kein Move möglich // /+\label{line:RValueRef1}+/

    std::cout << '\n' << "Rvalue Referenz auf std::string" << '\n';
    auto p3 {create_product(1, std::string("Produktname"))}; // /+\label{line:RValueRef}+/

    std::cout << '\n' << "LValue Referenz auf const char*" << '\n';
    auto p4 {create_product(1, "Produktname")}; // /+\label{line:LValueRef2}+/

    //Ohne Type Traits im Konstruktor kompiliert das nicht,
    //da die Template Funktion eine bessere Übereinstimmung hat,
    //als der Kopierkonstruktor
    Product1 prod1("Produkt1");
    auto prod_clone1(prod1);

    const Product1 prod2("Produkt2");
    auto prod_clone2(prod2);
    return 0;
}