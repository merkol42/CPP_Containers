#ifndef NORMAL_ITERATOR
# define NORMAL_ITERATOR

//#include "iterator_traits.hpp"
#include <cstddef> // std::ptrdiff_t
#include "iterator.hpp"
#include "../aux_templates/nullptr.hpp"

/**
 * // Forward iterator requirements
 * operator*();
 * operator->();
 * operator++; pre and post
 * // Bidirectional iterator requirements
 * operator--; pre and post
 * // Random access iterator requirements
 * operator[]
 * operator+=();
 * operator+();
 * operator-=();
 * operator-();
 * base();
*/


namespace merkol
{
	template <typename T>
	class normal_iterator
	{
	public:
		typedef merkol::normal_iterator<T>				this_type;
		typedef T										value_type;
		typedef T*										pointer;
		typedef T&										reference;
		typedef std::ptrdiff_t							difference_type;
		typedef merkol::random_access_iterator_tag		iterator_category;
		typedef merkol::normal_iterator<const T>		const_iterator;
	private:
		pointer	mPointer;
	public:
		// constructors
		normal_iterator() : mPointer(nullptr) { std::cout << "merkol::normal_iterator::default_constructor" << std::endl; };
		explicit normal_iterator(const pointer& ref) : mPointer(ref) { std::cout << "merkol::normal_iterator::pointer_constructor" << std::endl; }; // avoid implicitly call
		normal_iterator(const normal_iterator& instance) { *this = instance; /*       */std::cout << "merkol::normal_iterator::copy_constructor" << std::endl; };
		~normal_iterator() { std::cout << "merkol::normal_iterator::destructor" << std::endl; };

		// copy assignment
		normal_iterator& operator=(const normal_iterator& rhs)
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
		normal_iterator&	operator++(void)
		{
			this->mPointer++;
			return *this;
		} // pre-increment

		normal_iterator	operator++(int)
		{
			normal_iterator temp(*this);
			this->mPointer++;
			return temp;
		} // post-increment
		
		normal_iterator&	operator--(void)
		{
			this->mPointer--;
			return *this;
		} // pre-decrement

		normal_iterator	operator--(int)
		{
			normal_iterator temp(*this);
			this->mPointer--;
			return temp;
		} // post-decrement

		// convertion to const maybe unnecessary
		operator const_iterator() const
		{
			std::cout << "operator const_iterator" << std::endl;
			return const_iterator(*this);
		}

		// arithmetic operators
		normal_iterator operator+(difference_type rhs)
		{
			return normal_iterator(this->mPointer + rhs);
		}

		normal_iterator operator-(difference_type rhs)
		{
			return normal_iterator(this->mPointer - rhs);
		}

		normal_iterator& operator+=(difference_type rhs)
		{
			this->mPointer += rhs;
			return (*this);
		}

		normal_iterator& operator-=(difference_type rhs)
		{
			this->mPointer -= rhs;
			return (*this);
		}

		// reference && dereference operator
		reference operator*() const { // const dene
			return (*this->mPointer);
		}

		pointer operator->() const {
			return (&(*this->mPointer)); // this->mPointer
		}

		reference operator[](difference_type n) const {
			return (this->mPointer[n]);
		}

	};
	
	// non-member relational operators overload
	template <typename Iterator1, typename Iterator2>
	inline bool operator==(const normal_iterator<Iterator1>& lhs, const normal_iterator<Iterator2>& rhs)
	{
		return (lhs.base() == rhs.base());
	}

	template <typename Iterator1, typename Iterator2>
	inline bool operator!=(const normal_iterator<Iterator1>& lhs, const normal_iterator<Iterator2>& rhs)
	{
		return (lhs.base() != rhs.base());
	}

	template <typename Iterator1, typename Iterator2>
	inline bool operator<=(const normal_iterator<Iterator1>& lhs, const normal_iterator<Iterator2>& rhs)
	{
		return (lhs.base() <= rhs.base());
	}

	template <typename Iterator1, typename Iterator2>
	inline bool operator>=(const normal_iterator<Iterator1>& lhs, const normal_iterator<Iterator2>& rhs)
	{
		return (lhs.base() >= rhs.base());
	}

	template <typename Iterator1, typename Iterator2>
	inline bool operator<(const normal_iterator<Iterator1>& lhs, const normal_iterator<Iterator2>& rhs)
	{
		return (lhs.base() < rhs.base());
	}
	
	template <typename Iterator1, typename Iterator2>
	inline bool operator>(const normal_iterator<Iterator1>& lhs, const normal_iterator<Iterator2>& rhs)
	{
		return (lhs.base() > rhs.base());
	}

} // namespace merkol


#endif