#ifndef TYPE_TRAITS_TPP
# define TYPE_TRAITS_TPP

namespace merkol
{
	//integral_constant to make false/true_type typedef below
	/*
		std::integral_constant wraps a static constant of specified type. It is the base class for the C++ type traits.
		The behavior of a program that adds specializations for std::integral_constant is undefined.
	*/
	template<typename T, T v>
	struct integral_constant
	{
		static const T						value = v;

		typedef T							value_type;
		typedef integral_constant<T, v>		type;

		operator value_type() const { return value; }
	};

	//typedefs to is_integral implementation below
	typedef integral_constant<bool, false>	false_type;
	typedef integral_constant<bool, true>	true_type;

	//is_integral
	template<typename T>
	struct is_integral : false_type {};

	//specializations
	template<>
	struct is_integral<bool> : true_type {};

	template<>
	struct is_integral<char> : true_type {};
	
	template<>
	struct is_integral<unsigned char> : true_type {};
	
	template<>
	struct is_integral<int> : true_type {};
	
	template<>
	struct is_integral<unsigned int> : true_type {};

	template<>
	struct is_integral<long> : true_type {};
	
	template<>
	struct is_integral<unsigned long> : true_type {};

	template<>
	struct is_integral<long long> : true_type {};
	
	template<>
	struct is_integral<unsigned long long> : true_type {};

	// enable if
	template <bool Cond, class T = void>
	struct enable_if {};
	
	// Partial specialization for true.
	template <class T>
	struct enable_if<true, T> {typedef T type;};

	// is_same implementation
	template<class T, class U>
	struct is_same : merkol::false_type {};

	template<class T>
	struct is_same<T, T> : merkol::true_type {};

	// remove_cv, Provides the member typedef type which is the same as T
	// remove const or volatile, or both, if present.
	// https://en.cppreference.com/w/cpp/types/remove_cv
	template<class T>
	struct remove_cv { typedef T type; };

	template<class T>
	struct remove_cv<const T> { typedef T type; };

	template<class T>
	struct remove_cv<volatile T> { typedef T type; };

	template<class T>
	struct remove_cv<const volatile T> { typedef T type; };
	
	// the code below may not be useful? except readability
	template<class T>
	struct remove_const { typedef T type; };

	template<class T>
	struct remove_const<const T> { typedef T type; };
	
	template<class T>
	struct remove_volatile { typedef T type; };

	template<class T>
	struct remove_volatile<volatile T> { typedef T type; };

	// void_t implementation. !! not tested !!
	template<typename>
	struct void_t
	{
		typedef void type;
	};
	
}


#endif