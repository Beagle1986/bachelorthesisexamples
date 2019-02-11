#include <iostream>
#include <memory>

class ProductBase
{
    public:
    double get_price() const { return price; }
        virtual void print() const = 0;
        explicit ProductBase(double p) : price{p}{

        }
    private:
        double price{0};
};

class Product1 : public ProductBase
{
    using inherited = ProductBase;
    protected:
    void print() const override {
        std::cout << "Produkt 1 Preis: " << get_price() << '\n';
    };

    public:
    explicit Product1(double preis) : inherited{preis}
    {

    }

};

class Product2 : public ProductBase
{
    using inherited = ProductBase;
protected:
    void print() const override {
        std::cout << "Produkt 1 Preis: " << get_price() << '\n';
    };

public:
    explicit Product2(double price) : inherited{price}
    {

    }
};


template <typename... Ts>
std::unique_ptr<ProductBase> create_product(int productNo, Ts &&...params)
{
    if(productNo == 1) {
        return std::make_unique<Product1>(params...); //std::forward nicht notwendig
    }
    else if(productNo == 2) {
        return std::make_unique<Product2>(params...);
    }

    return nullptr;
}

int main()
{
    std::unique_ptr<ProductBase> prod1{create_product(1, 3.2)};
    prod1->print();

    std::shared_ptr<ProductBase> prod2{create_product(2, 4.5)}; // /+\label{line:uniqueToSharedConversion}+/
    prod2->print();

    return 0;
}