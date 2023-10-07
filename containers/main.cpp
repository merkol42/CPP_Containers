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
struct deneme
{
	int a;
	int	b;
}deneme = {42, 16};

std::ostream& operator<<(std::ostream& o, const struct deneme& a) {
	o << deneme.a << std::endl << deneme.b;
	return o;
}

#include <cstddef>
#include <typeinfo>
int main()
{
	int* sa = nullptr;

	if (sa == nullptr)
	{
		std::cout << "sa\n";
	}

	std::vector<int> za;

	if (za.data() == nullptr)
	{
		// throw merkol::InvalidIteratorException<int>();
		std::cout << "nullptr size: " << sizeof(merkol::nullptr_t) << std::endl;
		std::cout << typeid(merkol::nullptr_t).name() << std::endl;
	}

	std::cout << merkol::is_my_iterator_tagged<merkol::input_iterator_tag>::value << std::endl;
	std::cout << merkol::is_my_iterator_tagged<std::input_iterator_tag>::value << std::endl;


	// std::cout << typeid(std::iterator_traits<std::vector<int>::iterator>::iterator_category).name() << std::endl;
	// std::cout << typeid(merkol::iterator_traits<merkol::random_access_iterator<int> >::iterator_category).name() << std::endl;
	// std::cout << "asdfasdfasdfasdfasdfasdf" << std::endl << "asdfasdfasdfasdfasdfasdfasdfasdfasdfasdf";
	// std::this_thread::sleep_for(std::chrono::seconds(2));
	// getchar();
}
