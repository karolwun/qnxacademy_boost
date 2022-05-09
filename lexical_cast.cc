#include <boost/lexical_cast.hpp>
#include <boost/locale.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using boost::lexical_cast;
using boost::bad_lexical_cast;

void simple_casts_cpp()
{
    std::cout << "simple casts cpp: \n";
    std::string a = std::to_string(3.253);
    double b = std::stod("3.253ghi");
    int c = std::stoi("3253ghi");
    std::cout << a << " " << b << " " << c << "\n";
}

void simple_casts_boost()
{
    std::cout << "simple casts boost:\n";
    std::string a = lexical_cast<std::string>(3.253);
    try {
        double b = lexical_cast<double>("3.253ghi"); //not be allowed this time
        b = b;
    }        
    catch (bad_lexical_cast &e) {
        std::cerr << e.what() << "\n";
    }
    double b = lexical_cast<double>("3.253");
    int c = lexical_cast<int>("3253");
    std::cout << a << " " << b << " " << c << "\n";
}

class SomeData
{
public:
    int a = 3;
    float b = 3.0;
    std::string c = "3";

    SomeData() {}
    SomeData(int a, float b, std::string c) : a(a), b(b), c(c) {}

    //no need for friend here, but usually used when fields are private
    friend std::ostream& operator<<(std::ostream& os, const SomeData & s) 
    {
        return os << s.a << " " << s.b << " " << s.c;
    }

    friend std::istream& operator>>(std::istream& is, SomeData & s)
    {
        char whitespace; //std::stringstream has ios_base::skipws flag as true, but lexical_cast sets it to false
        return is >> s.a >> whitespace >> s.b >> whitespace >> s.c;
    }
};

void complex_casts_cpp()
{
    std::cout << "complex cast cpp: \n";
    SomeData s;
    std::stringstream ss;
    ss << s.a << " " << s.b << " " << s.c;
    std::cout << ss.str() << "\n";

    std::stringstream ss2("7 7.0 7");
    int a;
    float b;
    std::string c;
    ss2 >> a >> b >> c;
    SomeData s2(a, b, c);
    std::cout << s2.a << " " << s2.b << " " << s2.c << "\n";
}

void complex_casts_boost()
{
    std::cout << "complex cast boost: \n";
    SomeData s;
    std::cout << lexical_cast<std::string>(s) << "\n";

    std::string s_in = "7 7.0 7";
    SomeData s2 = lexical_cast<SomeData>(s_in);
    std::cout << s2.a << " " << s2.b << " " << s2.c << "\n";
}

int main()
{
    simple_casts_cpp();
    simple_casts_boost();
    complex_casts_cpp();
    complex_casts_boost();
}