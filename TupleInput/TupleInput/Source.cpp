#include "TupleRead.h"
#include <sstream>
#include <iostream>
void main()
{
	std::istringstream iss("1 2 3 abc");
	std::tuple<int, unsigned int, double, std::string> t;
	TupleStreamReader<4, std::tuple<int, unsigned int, double, std::string>, int, unsigned int, double, std::string>::readTupleFromStream(iss, t);
	std::cout << ((std::get<0>(t) == 1 && std::get<1>(t) == 2 && std::get<2>(t) == 3 && std::get<3>(t) == "abc") ? "ok" : "not ok");
}