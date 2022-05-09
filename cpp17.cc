#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <iostream>
#include <regex>
#include <vector>

// 'variant', 'optional', 'any' are similar in use to their std versions
boost::variant<int, float, std::string> getVariant(const std::string & s)
{
    boost::variant<int, float, std::string> result;
    if (s.find('.') != std::string::npos) //please never implement it this way! 
    {
        try {
            float f = std::stof(s);
            result = f;
        } catch(const std::invalid_argument &) {
            result = s;
        }
    }
    else
    {
        try {
            int r = std::stoi(s);
            result = r;
        } catch(const std::invalid_argument &) {
            result = s;
        }
    }
    return result;
}

void variant()
{
    class visitor: public boost::static_visitor<> {
    public:
        void operator()(std::string &v) const
        {
            std::cout << "this is string " << v << "\n";
        }

        void operator()(float &v) const
        {
            std::cout << "this is float " << v << "\n";
        }

        void operator()(int &v) const
        {
            std::cout << "this is int " << v << "\n";
        }
    };

    std::cout << "Variant: \n";
    auto v1 = getVariant("3");
    auto v2 = getVariant("3.7");
    auto v3 = getVariant("d");    
    boost::apply_visitor(visitor(), v1);
    boost::apply_visitor(visitor(), v2);
    boost::apply_visitor(visitor(), v3);
}

boost::optional<int> getOptional(const std::string & s)
{
    //ignore implemenation
    if (s == "a") {
        return 0;
    }
    else {
        return boost::none;
    }
}

void optional()
{
    std::cout << "Optional: \n";
    auto a = getOptional("a");
    auto b = getOptional("b");
    if (a) {
        std::cout << *a << "\n";
    }
    if (b) {
        std::cout << *b << "\n";
    }
}

namespace fs = boost::filesystem;
void filesystem()
{
    std::cout << "Filesystem: \n";
    std::cout << fs::file_size(__FILE__) << "\n"; //so nicer than 'stat'

    //very simple code to show which processess occupied cpu the most
    std::vector<std::pair<long, std::string>> processes;
    fs::path p("/proc");
    for (auto & x : fs::directory_iterator(p)) { //compare this with 'opendir' :)
        if (fs::is_directory(x) && std::regex_match(x.path().filename().c_str(), std::regex("[0-9]+"))) {
            auto sched_path = fs::path(x).append("schedstat");
            auto comm_path = fs::path(x).append("comm");
            unsigned long cpu_v;
            std::string comm_v;
            fs::ifstream ifs(sched_path);
            fs::ifstream ifc(comm_path);
            ifs >> cpu_v;
            ifc >> comm_v;
            processes.emplace_back(std::make_pair(cpu_v, comm_v));
        }
    }

    std::sort(processes.rbegin(), processes.rend());
    for (int i = 0; i < 5; i++) {
        std::cout << processes[i].second << ": " << processes[i].first << "\n";
    }    
}

int main(int argc, char **argv)
{
    variant();
    optional();
    filesystem();

    return 0;
}