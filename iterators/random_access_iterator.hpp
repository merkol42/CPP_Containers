#ifndef RANDOM_ACCESS_ITERATOR
# define RANDOM_ACCESS_ITERATOR

//#include "iterator_traits.hpp"
#include <cstddef> // std::ptrdiff_t

namespace merkol
{
	template <typename T>
	class random_access_iterator
	{
	public:
		typedef T									value_type;
		typedef T*									pointer;
		typedef T&									reference;
		typedef std::ptrdiff_t						difference_type;
		typedef std::random_access_iterator_tag		iterator_category;
		typedef merkol::random_access_iterator<T>	this_type;
	private:
		pointer	_pointer;
	public:
		random_access_iterator();
		explicit random_access_iterator(pointer& ref); // avoid implicitly call
		random_access_iterator(const this_type& instance);
		~random_access_iterator();

		random_access_iterator&	operator++(void); // pre-increment
		random_access_iterator	operator++(int); // post-increment
		
		random_access_iterator&	operator--(void); // pre-decrement
		random_access_iterator	operator--(int); // post-decrement
	};
	
} // namespace merkol


#endif