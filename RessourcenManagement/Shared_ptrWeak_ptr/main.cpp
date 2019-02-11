#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

class Company;

class Employee
{
public:
     ~Employee() {
        std::cout << "Destruktor Mitarbeiter aufgerufen" << '\n';
    }

    //Parameter kann auch std::weak_ptr<Company> const & sein
    void set_company(std::shared_ptr<Company> const & company) {
        this->company = company; //weak_ptr wird aus shared_ptr konstruiert
    }
private:
    //std::shared_ptr<Company> company; //Wäre ein Ressource-Leak
    std::weak_ptr<Company> company;
};

class Company
{
public:
    ~Company() {
        std::cout << "Destruktor Unternehmen aufgerufen" << '\n';
    }

    void add_employee(std::shared_ptr<Employee> employee) {
        employees.emplace_back(employee);
    }
private:
    std::vector<std::shared_ptr<Employee>> employees;
};

int main()
{
    {
        auto company{std::make_shared<Company>()};
        auto employee{std::make_shared<Employee>()};

        company->add_employee(employee);
        employee->set_company(company);
    }

    return 0;
}