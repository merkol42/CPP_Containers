#ifndef ITERATORS_TRAITS_HPP
# define ITERATORS_TRAITS_HPP

//**stl_iterator_base_types.h line 160**
#include <typeinfo>

namespace merkol
{
	// since CXX17(void_t)
	// Helper to make iterator_traits SFINAE friendly as N3844 requires.
	// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3844.pdf

	// template <typename Iterator, class = void>
	// struct default_iterator_traits {};

	// template <typename Iterator>
	// struct default_iterator_traits<
	// 	Iterator,
	// 	void_t<
	// 		typename Iterator::iterator_category,
	// 		typename Iterator::value_type,
	// 		typename Iterator::difference_type,
	// 		typename Iterator::pointer,
	// 		typename Iterator::reference
	// 	>
	// >
	// {
	// 	typedef typename Iterator::iterator_category iterator_category;
	// 	typedef typename Iterator::value_type        value_type;
	// 	typedef typename Iterator::difference_type   difference_type;
	// 	typedef typename Iterator::pointer           pointer;
	// 	typedef typename Iterator::reference         reference;
	// };

	template <typename T>
	struct iterator_traits {
		typedef typename T::value_type			value_type;
		typedef typename T::difference_type		difference_type;
		typedef typename T::iterator_category	iterator_category;
		typedef typename T::pointer				pointer;
		typedef typename T::reference			reference;
	};

	// Partial specialization for pointer types.
	template <typename T>
	struct iterator_traits<T*>
	{
		typedef std::random_access_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef T*								pointer;
		typedef T&								reference;
		typedef std::ptrdiff_t					difference_type;
	};

	// Partial specialization for const pointer types.
	template <typename T>
	struct iterator_traits<const T*>
	{
		typedef std::random_access_iterator_tag		iterator_category;
		typedef T									value_type;
		typedef const T*							pointer;
		typedef const T&							reference;
		typedef std::ptrdiff_t						difference_type;
	};

} // namespace merkol

#endif
