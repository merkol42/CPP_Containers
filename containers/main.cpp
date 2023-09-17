#include "vector.hpp"
#include <iostream>
#include <vector>
// #include <unordered_map>
#include <utility>
#include <set>
#include <sstream>
#include <memory>
#include <algorithm>
#include <string>
#include "../auxiliary/auxiliary.hpp"
#include <string.h>
#include <iomanip>
#include <ios>
// #include <chrono>
// #include <thread>

// void f(long) {std::cout << "null" << std::endl;}
// void f(int *a) {std::cout << " null*" << std::endl;}

// int	main()
// {
// 	std::vector<int> ab(__gnu_cxx::__numeric_traits<long>::__max);
// 	merkol::vectorBase<int, std::allocator<int>> a(__gnu_cxx::__numeric_traits<long>::__max, std::allocator<int>());

// 	int	*ptr = NULL;
// 	std::cout << (ptr == nullptr) << std::endl;
// 	f(nullptr);
// }

// template <typename T>
// void	moveMemberValue(std::vector<T>& vec, T value) {
// 	typedef typename std::vector<T>::value_type valueType;
// 	std::for_each(std::remove_if(vec.begin(), vec.end(), [&value](valueType &val) { return val == value;}), vec.end(), [&value](valueType &val) { val = value;});
// }

#include "../iterators/random_access_iterator.hpp"
#include "../iterators/iterator_traits.hpp"
#include "../aux_templates/nullptr.hpp"
#include <typeinfo>

#include "../iterators/iterator.hpp"

int main()
{
	

	std::cout << typeid(std::iterator_traits<std::vector<int>::iterator>::iterator_category).name() << std::endl;
	std::cout << typeid(merkol::iterator_traits<merkol::random_access_iterator<int> >::iterator_category).name() << std::endl;
	// std::cout << "asdfasdfasdfasdfasdfasdf" << std::endl << "asdfasdfasdfasdfasdfasdfasdfasdfasdfasdf";
	// std::this_thread::sleep_for(std::chrono::seconds(2));
	// getchar();
}
