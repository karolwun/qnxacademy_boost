#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <iostream>

void manual_logging()
{
//this is where you end without logger
#if PRINT_INFO
    std::cout << "Info message \n";
#endif

#if PRINT_WARNING
    std::cerr << "Warning message \n";
#endif

#if PRINT_ERROR
    std::cerr << "Error message \n";
#endif
}

using namespace boost;

void trivial_logging_boost()
{
    auto log_something = []()
    {
        BOOST_LOG_TRIVIAL(trace) << "Trace log";
        BOOST_LOG_TRIVIAL(debug) << "Debug log";
        BOOST_LOG_TRIVIAL(info) << "Info log";
        BOOST_LOG_TRIVIAL(warning) << "Warning log";
        BOOST_LOG_TRIVIAL(error) << "Error log";
        BOOST_LOG_TRIVIAL(fatal) << "Fatal log";
        std::cout << "\n";
    };

    std::cout << "Trivial logging \n";
    log_something();
    log::core::get()->set_filter(log::trivial::severity >= log::trivial::info); //log::core is global filter
    log_something();
    log::core::get()->set_filter(log::trivial::severity >= log::trivial::error);
    log_something();
    log::core::get()->set_filter(log::trivial::severity == log::trivial::warning);
    log_something();     
    log::core::get()->set_filter(log::trivial::severity < log::trivial::error);
    log_something();   
}

void less_trivial_logging_boost()
{

}

int main()
{
    trivial_logging_boost();
    less_trivial_logging_boost();
    return 0;
}

