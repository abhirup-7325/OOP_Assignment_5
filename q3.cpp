#include <iostream>
#include <string>
#include <vector>

class Employee {
protected:
    std::string empId;
    std::string name;
    std::string designation;
    double basicPay;

public:
    Employee(const std::string& empId, const std::string& name, const std::string& designation, double basicPay)
        : empId(empId), name(name), designation(designation), basicPay(basicPay) {}

    virtual ~Employee() {}

    virtual double calculateSalary() const = 0;

    void display() const {
        std::cout << "Employee ID: " << empId
                  << "\nName: " << name
                  << "\nDesignation: " << designation
                  << "\nSalary: " << calculateSalary() << "\n\n";
    }
};

class PermanentEmployee : public Employee {
public:
    PermanentEmployee(const std::string& empId, const std::string& name, const std::string& designation, double basicPay)
        : Employee(empId, name, designation, basicPay) {}

    double calculateSalary() const {
        return basicPay + (0.3 * basicPay) + (0.8 * basicPay);
    }
};

class ContractualEmployee : public Employee {
private:
    double allowance;

public:
    ContractualEmployee(const std::string& empId, const std::string& name, const std::string& designation, double basicPay, double allowance)
        : Employee(empId, name, designation, basicPay), allowance(allowance) {}

    double calculateSalary() const {
        return basicPay + allowance;
    }
};

int main() {
    std::vector<Employee*> employees;

    employees.push_back(new PermanentEmployee("P001", "Alice", "Manager", 50000));
    employees.push_back(new PermanentEmployee("P002", "Bob", "Engineer", 40000));
    employees.push_back(new ContractualEmployee("C001", "Charlie", "Consultant", 30000, 10000));
    employees.push_back(new ContractualEmployee("C002", "Daisy", "Designer", 35000, 15000));

    for (std::vector<Employee*>::iterator it = employees.begin(); it != employees.end(); ++it) {
        (*it)->display();
        delete *it;
    }

    return 0;
}
