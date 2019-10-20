#pragma once
#include <tuple>
#include <fstream>
#include <string>
#include <vector>

template <size_t I, typename TupleType, typename T, typename... Ty>
struct TupleStreamReader
{
	static void readTupleFromStream(std::istream& is, TupleType& tuple)
	{
		is >> std::get<std::tuple_size<TupleType>::value - I>(tuple);
		TupleStreamReader<I - 1, TupleType, Ty...>::readTupleFromStream(is, tuple);
	}
};

template <typename TupleType, typename... Ty>
struct TupleStreamReader<1, TupleType, Ty...>
{
	static void readTupleFromStream(std::istream& is, TupleType& tuple)
	{
		is >> std::get<std::tuple_size<TupleType>::value - 1>(tuple);
	}
};

template <typename... Ty>
std::vector<std::tuple<Ty...>> readTuplesFromFile(const std::string& path)
{
	std::ifstream is(path);
	std::vector<std::tuple<Ty...>> res;
	while (!is.eof())
	{
		std::tuple<Ty...> cur;
		TupleStreamReader<sizeof...(Ty), std::tuple<Ty...>, Ty...>::readTupleFromStream(is, cur);
		res.push_back(cur);
	}
	return res;
};