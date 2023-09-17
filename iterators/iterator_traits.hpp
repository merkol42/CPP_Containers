#ifndef ITERATORS_TRAITS_HPP
# define ITERATORS_TRAITS_HPP

//**stl_iterator_base_types.h line 160**

namespace merkol
{
	template <typename T>
	struct iterator_traits {
		typedef typename T::value_type			value_type;
		typedef typename T::difference_type		difference_type;
		typedef typename T::iterator_category	iterator_category;
		typedef typename T::pointer				pointer;
		typedef typename T::reference			reference;
	};

	template <typename T>
	struct iterator_traits<T*>
	{
		typedef std::random_access_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef T*								pointer;
		typedef T&								reference;
		typedef std::ptrdiff_t					difference_type;
	};

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
