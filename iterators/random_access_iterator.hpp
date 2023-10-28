#ifndef RANDOM_ACCESS_ITERATOR
# define RANDOM_ACCESS_ITERATOR

//#include "iterator_traits.hpp"
#include <cstddef> // std::ptrdiff_t
#include "iterator.hpp"
#include "../aux_templates/nullptr.hpp"

namespace merkol
{
	template <typename T>
	class random_access_iterator
	{
	public:
		typedef merkol::random_access_iterator<T>		this_type;
		typedef T										value_type;
		typedef T*										pointer;
		typedef T&										reference;
		typedef std::ptrdiff_t							difference_type;
		typedef merkol::random_access_iterator_tag		iterator_category;
		typedef merkol::random_access_iterator<const T>	const_iterator;
	private:
		pointer	mPointer;
	public:
		// constructors
		random_access_iterator() : mPointer(nullptr) { std::cout << "merkol::random_access_iterator::default_constructor" << std::endl; };
		explicit random_access_iterator(const pointer& ref) : mPointer(ref) { std::cout << "merkol::random_access_iterator::pointer_constructor" << std::endl; }; // avoid implicitly call
		random_access_iterator(const this_type& instance) { *this = instance; /*       */std::cout << "merkol::random_access_iterator::copy_constructor" << std::endl; };
		~random_access_iterator() { std::cout << "merkol::random_access_iterator::destructor" << std::endl; };

		// copy assignment
		this_type& operator=(const this_type& rhs)
		{
			this->mPointer = rhs.mPointer;
			return *this;
		}

		// get underlying pointer address
		pointer base() const
		{
			return this->mPointer;
		}

		// increment & decrement
		random_access_iterator&	operator++(void)
		{
			this->mPointer++;
			return *this;
		} // pre-increment

		random_access_iterator	operator++(int)
		{
			this_type temp(*this);
			this->mPointer++;
			return temp;
		} // post-increment
		
		random_access_iterator&	operator--(void)
		{
			this->mPointer--;
			return *this;
		} // pre-decrement

		random_access_iterator	operator--(int)
		{
			this_type temp(*this);
			this->mPointer--;
			return temp;
		} // post-decrement

		// convertion to const maybe unnecessary
		operator const_iterator() const
		{
			return const_iterator(*this);
		}

		// arithmetic operators
		random_access_iterator operator+(difference_type rhs)
		{
			return this_type(this->mPointer + rhs);
		}

		random_access_iterator operator-(difference_type rhs)
		{
			return this_type(this->mPointer - rhs);
		}

		random_access_iterator& operator+=(difference_type rhs)
		{
			this->mPointer += rhs;
			return (*this);
		}

		random_access_iterator& operator-=(difference_type rhs)
		{
			this->mPointer -= rhs;
			return (*this);
		}
	};
	
	// non-member relational operators overload
	template <typename T>
	inline bool operator==(const random_access_iterator<T>& lhs, const random_access_iterator<T>& rhs)
	{
		return (lhs.base() == rhs.base());
	}

	template <typename T>
	inline bool operator!=(const random_access_iterator<T>& lhs, const random_access_iterator<T>& rhs)
	{
		return (lhs.base() != rhs.base());
	}

	template <typename T>
	inline bool operator<=(const random_access_iterator<T>& lhs, const random_access_iterator<T>& rhs)
	{
		return (lhs.base() <= rhs.base());
	}

	template <typename T>
	inline bool operator>=(const random_access_iterator<T>& lhs, const random_access_iterator<T>& rhs)
	{
		return (lhs.base() >= rhs.base());
	}

	template <typename T>
	inline bool operator<(const random_access_iterator<T>& lhs, const random_access_iterator<T>& rhs)
	{
		return (lhs.base() < rhs.base());
	}
	
	template <typename T>
	inline bool operator>(const random_access_iterator<T>& lhs, const random_access_iterator<T>& rhs)
	{
		return (lhs.base() > rhs.base());
	}

} // namespace merkol


#endif