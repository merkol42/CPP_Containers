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
#include "../iterators/iterator.hpp"
#include "../iterators/reverse_iterator.hpp"
#include <typeinfo>
#include <cstddef>
#include <typeinfo>


template <typename T>
void	printValue(T val) {
	std::cout << val << std::endl;
}

void	iterator_traits_test()
{
	int arr[] = {1, 2, 3, 4, 5};
	int* ptr = arr;
	merkol::iterator_traits<int*>::value_type value;  // Değer türünü almak
	merkol::iterator_traits<int*>::difference_type diff; // Fark türünü almak
	merkol::iterator_traits<int*>::iterator_category category; // Iterator kategorisini almak
	merkol::iterator_traits<int*>::pointer pointer; // İşaretçiyi almak
	merkol::iterator_traits<int*>::reference reference = arr[2]; // Referansı almak

	// Elde edilen özellikleri yazdıralım.
	std::cout << "Value Type: " << typeid(value).name() << std::endl;
	std::cout << "Difference Type: " << typeid(diff).name() << std::endl;
	std::cout << "Iterator Category: " << typeid(category).name() << std::endl;
	std::cout << "Pointer Type: " << typeid(pointer).name() << std::endl;
	std::cout << "Reference Type: " << typeid(reference).name() << std::endl;
}

void vector_iter_test() {
	int *ptr = new int();
	merkol::random_access_iterator<int> it1(ptr);
	merkol::random_access_iterator<int> it2;

	
	std::vector<int> vec;
	std::vector<int> vec1;

	std::cout << (it1 == it2) << std::endl;

	std::vector<int>::iterator it4 = vec.begin();
	std::vector<int>::iterator it3 = vec1.begin();
	
	std::cout << (it4 == it3) << std::endl;

	std::cout << it1.base() << "      " << it2.base() << std::endl;
	if (it2.base() == nullptr) {
		std::cout << "operator==" << std::endl;
	}

	if (ptr != nullptr)
	{
		printValue("nullptr test");
	}

	if (it1 == it2)
		std::cout << "operator==" << std::endl;
}

void random_access_iterator_compare_test() {
	int val = 169090600;
	char *ptr = reinterpret_cast<char*>(&val);
	

	merkol::random_access_iterator<char> x(reinterpret_cast<char*>(ptr++));
	merkol::random_access_iterator<char> y(reinterpret_cast<char*>(ptr++));
	merkol::random_access_iterator<char> z(reinterpret_cast<char*>(ptr++));
	merkol::random_access_iterator<char> t(reinterpret_cast<char*>(ptr));
	

	std::cout
		<< std::boolalpha
		<< "*x == " << (int)(*x) << '\n' // 40
		<< "*y == " << (int)(*y) << '\n' // 30
		<< "*z == " << (int)(*z) << '\n' // 20
		<< "*t == " << (int)(*t) << '\n' // 10
		<< "x == y ? " << (x == y) << '\n' // false
		<< "x != y ? " << (x != y) << '\n' // true
		<< "x <  y ? " << (x <  y) << '\n' // true
		<< "x <= y ? " << (x <= y) << '\n' // true
		<< "x == z ? " << (x == z) << '\n' // false
		<< "x != z ? " << (x != z) << '\n' // true
		<< "x <  z ? " << (x <  z) << '\n' // true
		<< "x <= z ? " << (x <= z) << '\n' // true
		;
}

void reverse_iterator_test() {
	// std::reverse_iterator<int*> deneme;
	int * ptr = new int();
	std::cout << ptr << std::endl;
	std::reverse_iterator<char*> za((nullptr));
	merkol::reverse_iterator<char*> ri1((nullptr));
	// std::cout << "std:base "<< za.base() << std::endl;
	// std::cout << "merkol:base "<< ri1.base() << std::endl;
	// std::reverse_iterator<int*> deneme = 2 + za;
	// merkol::reverse_iterator<int*> ri3 = 2 + ri1; // Şablon yapıcı fonksiyonu ile dönüşüm yapılıyor

	// std::cout << "after std:base "<< deneme.base() << std::endl;
	// std::cout << "after merkol:base "<< ri3.base() << std::endl;

	// std::cout << "ri1.base() = " << ri1.base() << std::endl;
	// std::cout << "ri2.base() = " << ri2.base() << std::endl;
	// std::cout << "ri3.base() = " << ri3.base() << std::endl;
}

void vector_test() {
	// merkol::vector<int> vec1(std::allocator<int>());
	int val();
	std::cout << val << std::endl;
}


int main()
{
	print_title();
	vector_test();
	// reverse_iterator_test();
	// vector_iter_test();
	// random_access_iterator_compare_test();
	// typeid(merkol::iterator_traits<decltype(it)>::value_type).name();

	// iterator_traits_test();

	// std::vector<int>::iterator a = std::vector<int>().begin();
	// std::cout << std::addressof(a) << std::endl;
	// printValue(a.base());
	// a += sizeof(int);
	// std::cout << std::addressof(a) << std::endl;
	// printValue(a.base());

	// int* sa = nullptr;

	// if (sa == nullptr)
	// {
	// 	std::cout << "sa\n";
	// }

	// std::vector<int> za;

	// if (za.data() == nullptr)
	// {
	// 	// throw merkol::InvalidIteratorException<int>();
	// 	std::cout << "nullptr size: " << sizeof(merkol::nullptr_t) << std::endl;
	// 	std::cout << typeid(merkol::nullptr_t).name() << std::endl;
	// }

	// std::cout << merkol::is_my_iterator_tagged<merkol::input_iterator_tag>::value << std::endl;
	// std::cout << merkol::is_my_iterator_tagged<std::input_iterator_tag>::value << std::endl;


	// std::cout << typeid(std::iterator_traits<std::vector<int>::iterator>::iterator_category).name() << std::endl;
	// std::cout << typeid(merkol::iterator_traits<merkol::random_access_iterator<int> >::iterator_category).name() << std::endl;
	// std::cout << "asdfasdfasdfasdfasdfasdf" << std::endl << "asdfasdfasdfasdfasdfasdfasdfasdfasdfasdf";
	// std::this_thread::sleep_for(std::chrono::seconds(2));
	// getchar();
}
