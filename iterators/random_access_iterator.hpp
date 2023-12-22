#ifndef RANDOM_ACCESS_ITERATOR
# define RANDOM_ACCESS_ITERATOR

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
	class random_access_iterator
	{
	public:
		typedef merkol::random_access_iterator<T>			this_type;
		typedef T											value_type;
		typedef T*											pointer;
		typedef T&											reference;
		typedef std::ptrdiff_t								difference_type;
		typedef merkol::random_access_iterator_tag			iterator_category;
		typedef merkol::random_access_iterator<const T>		const_iterator;
	private:
		pointer	mPointer;
	public:
		// constructors
		random_access_iterator() : mPointer(nullptr) { std::cout << "merkol::random_access_iterator::default_constructor" << std::endl; };
		explicit random_access_iterator(const pointer& ref) : mPointer(ref) { std::cout << "merkol::random_access_iterator::pointer_constructor" << std::endl; }; // avoid implicitly call
		random_access_iterator(const random_access_iterator& instance) { *this = instance; /*       */std::cout << "merkol::random_access_iterator::copy_constructor" << std::endl; };
		~random_access_iterator() { std::cout << "merkol::random_access_iterator::destructor" << std::endl; };

		// copy assignment
		random_access_iterator& operator=(const random_access_iterator& rhs)
		{
			if (this != &rhs)
				this->mPointer = rhs.mPointer;
			return (*this);
		}

		// get underlying pointer address
		pointer base() const
		{
			return (this->mPointer);
		}

		//*** Forward iterator requirements ***///
		//--------------------------------------------------//
		reference operator*() const { // const dene
			return (*this->mPointer);
		}

		pointer operator->() const {
			return (&(*this->mPointer));
		}

		random_access_iterator&	operator++(void)
		{
			this->mPointer++;
			return (*this);
		} // pre-increment

		random_access_iterator	operator++(int)
		{
			random_access_iterator temp(*this);
			++(*this);
			return temp;
		} // post-increment

		//--------------------------------------------------//
		

		//*** Bidirectional iterator requirements ***///
		//--------------------------------------------------//
		random_access_iterator&	operator--(void)
		{
			this->mPointer--;
			return (*this);
		} // pre-decrement

		random_access_iterator	operator--(int)
		{
			random_access_iterator temp(*this);
			this->mPointer--;
			return (temp);
		} // post-decrement

		//--------------------------------------------------//


		// convertion to const
		operator const_iterator() const
		{
			return (const_iterator(mPointer));
		}


		//*** Random access iterator requirements ***///
		//--------------------------------------------------//
		reference operator[](difference_type n) const
		{
			return (this->mPointer[n]);
		}

		random_access_iterator operator+(difference_type rhs)
		{
			return (random_access_iterator(this->mPointer + rhs));
		}

		random_access_iterator operator-(difference_type rhs)
		{
			return (random_access_iterator(this->mPointer - rhs));
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
		//--------------------------------------------------//

	};
	
	// non-member relational operators overload
	template <typename Iterator1, typename Iterator2>
	inline bool operator==(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.base() == rhs.base());
	}

	template <typename Iterator1, typename Iterator2>
	inline bool operator!=(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.base() != rhs.base());
	}

	template <typename Iterator1, typename Iterator2>
	inline bool operator<=(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.base() <= rhs.base());
	}

	template <typename Iterator1, typename Iterator2>
	inline bool operator>=(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.base() >= rhs.base());
	}

	template <typename Iterator1, typename Iterator2>
	inline bool operator<(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.base() < rhs.base());
	}
	
	template <typename Iterator1, typename Iterator2>
	inline bool operator>(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.base() > rhs.base());
	}

} // namespace merkol


#endif