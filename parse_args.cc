#include <boost/program_options.hpp>
#include <getopt.h>
#include <iostream>

namespace po = boost::program_options;

void get_args_boost(int argc, char **argv)
{
    int v;
    po::options_description od("Options");
    od.add_options()
        ("help,h", "print help message") //automatically builds help message
        ("queue,q", "queue protocol")
        ("shm,s", po::value<int>(&v), "shared memory") //immediate validation and no need to call stoi
        ("file", po::value<std::string>()->required(), "input file"); //support for required options
        // but missing support for mutually exclusive options ;(

    po::positional_options_description p; //easy mixing with positional options
    p.add("file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(od).positional(p).run(), vm);
    if (vm.count("help")) {
        std::cout << od << "\n";
        return;
    }

    po::notify(vm);

    std::cout << vm["file"].as<std::string>() << std::endl;
}

void get_args_longopt(int argc, char **argv)
{
    int c;
    static struct option long_options[] =
        {
            {"help", no_argument, 0, 'h'},
            {"queue", no_argument, 0, 'q'},
            {"pipe", no_argument, 0, 'p'},
            {"shm", no_argument, 0, 's'},
            {"file", required_argument, 0, 'f'},
            {0, 0, 0, 0}};

    int option_index = 0;

    while (1)
    {
        c = getopt_long(argc, argv, "hqpsf:",
                        long_options, &option_index);

        if (c == -1)
            break;
        switch (c)
        {
        case 'h':
            std::cout << "help" << std::endl;
            break;

        case 'q':
            std::cout << "IPC Method: Queue" << std::endl;
            break;

        case 'p':
            std::cout << "IPC Method: Pipe" << std::endl;
            break;
        case 's':
            std::cout << "IPC Method: Shared Memory" << std::endl;
            break;
        case 'f':
            std::cout << "Filename: " << optarg << std::endl;
            break;
        case '?':
            std::cout << "Invalid arg" << std::endl;
            break;
        }
    }
}

int main(int argc, char **argv)
{
    //get_args_longopt(argc, argv);
    get_args_boost(argc, argv);
    return 0;
}

