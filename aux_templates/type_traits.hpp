#ifndef TYPE_TRAITS_TPP
# define TYPE_TRAITS_TPP

namespace merkol
{
	//integral_constant to make false/true_type typedef below
	template<typename _Tp, _Tp __v>
	struct integral_constant
	{
		static const _Tp					value = __v;

		typedef _Tp							value_type;
		typedef integral_constant<_Tp, __v>	type;

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

	//enable if
	template <bool Cond, class T = void>
	struct enable_if {};
	
	//Partial specialization for true.
	template <class T>
	struct enable_if<true, T> {typedef T type;};

	template<class T, class U>
	struct is_same : merkol::false_type {};

	template<class T>
	struct is_same<T, T> : merkol::true_type {};

}


#endif