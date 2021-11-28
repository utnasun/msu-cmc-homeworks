#include <iostream>
#include <cstring>

class Student
{
private:
    char* name;
    int age;
    bool isFemale;
public:
    Student();
    Student(const char *s, const int x, const bool gender);
    Student(const bool gender);
    Student(const Student & B);
    Student & operator= (const Student & B);
    ~Student();
    void print() const;
    void getname() const;
};
Student::Student ()
{
    age = 0;
    name = new char[1];
    name[0] = '\0';
    isFemale = false;
    std::cout << "Default constructor worked" << std::endl;
}
Student::Student (const bool gender)
{
    age = 0;
    name = new char[1];
    name[0] = '\0';
    isFemale = gender;
    std::cout << "Gender constructor worked" << std::endl;
}
Student::Student (const char *s, const int x, const bool gender)
{
    name = new char[strlen(s)+1];
    strcpy(name, s);
    age = x;
    isFemale = gender;
    std::cout << "Second constructor worked" << std::endl;
}
Student::~Student()
{
    delete [] this->name;
    std::cout << "Destructor worked" << std::endl;
}
Student::Student (const Student & B)
{
    name = new char [strlen(B.name)+1];
    strcpy(name, B.name);
    age = B.age;
    isFemale = B.isFemale;
    std::cout << "Copy worked" << std::endl;
}
Student& Student::operator= (const Student &B)
{
    if (this == &B)
        return *this;
    delete[] this->name;
    this->name = new char[strlen(B.name)+1];
    this->age = B.age;
    this->isFemale = B.isFemale;
    strcpy(this->name,B.name);
    std::cout << "Operator = worked" << std::endl;
    return *this;
}

void Student::getname() const
{
    std::cout << "getname function: " << name << std::endl;
}
void Student::print() const
{
    std::cout << "Info print: Name: " << name << "| age: " << age << "| gender ( 0 - MAN/ 1 -WOMAN): " << isFemale << std::endl;
}

int main()
{
    Student A;
    A.print();
    Student B("Utnasun", 19, false);
    B.print();
    B.getname();
    Student D(true);
    D.print();
    A = B;
    A.print();
    Student C(B);
    C.print();
    return 0;
}

