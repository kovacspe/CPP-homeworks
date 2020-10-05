#include "du1.hpp"
#include "connection.hpp"
#include "dump.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>

int main(int argc, char * * argv)
{
	timetable tt;
	
	if (argc >= 2)
	{
		{
			std::cout << "Reading data " << argv[1] << std::endl;

			std::ifstream iff(argv[1]);

			read_timetable(tt, iff);
		}

		if (argc >= 3 && argv[2] == std::string{ "-sw"})
		{
			auto fn = argv[1] + std::string{ ".sw" };
			std::cout << "Dumping data stop-wise " << fn << std::endl;

			std::ofstream off(fn);
			dump::dump_stopwise(off, tt);
		}

		if (argc >= 3 && argv[2] == std::string{ "-tw" })
		{
			auto fn = argv[1] + std::string{ ".tw" };
			std::cout << "Dumping data trip-wise " << fn << std::endl;

			std::ofstream off(fn);
			dump::dump_tripwise(off, tt);
		}
	}
	print_platform_timetables(std::cout, tt, "Palmovka", "U529Z1");

	//print_platform_timetables(std::cout, tt, "Hellichova", "U138Z2");

	connection::print_shortest_connection(std::cout, tt, "Palmovka", "Florenc", pack_time(16, 00), 1);

	connection::print_shortest_connection(std::cout, tt, "Palmovka", "Hellichova", pack_time(16, 00), 1);

	connection::print_shortest_connection(std::cout, tt, "Palmovka", "Hellichova", pack_time(16, 00), 5);

	connection::print_shortest_connection(std::cout, tt, "Motol", "Kobylisy", pack_time(16, 00), 2);
	std::string s;
	std::cin >> s;
	return 0;
}
