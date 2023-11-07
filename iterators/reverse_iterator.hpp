#ifndef REVERSE_ITERATOR
# define REVERSE_ITERATOR

#include "iterator_traits.hpp"
#include "random_access_iterator.hpp"
#include <iostream>

namespace merkol
{

	template<typename T>
	class random_access_iterator;

	template<class Iter>
	class reverse_iterator
	{
	protected:
		Iter	mIterator;
		typedef merkol::iterator_traits<Iter>	traits_type;
	public:
		typedef Iter											iterator_type;
		typedef typename traits_type::iterator_category			iterator_category;
		typedef typename traits_type::value_type				value_type;
		typedef typename traits_type::difference_type			difference_type;
		typedef typename traits_type::pointer					pointer;
		typedef typename traits_type::reference					reference;
	public:
		reverse_iterator() : mIterator() { std::cout << "merkol::reverse_iterator::default_constructor" << std::endl; } // It's important that we construct mIterator, because if Iterator is a pointer, there's a difference between doing it and not.
		explicit reverse_iterator(iterator_type i) : mIterator(i) { std::cout << "merkol::reverse_iterator::iterator_constrcutor" << std::endl; }
		reverse_iterator(const reverse_iterator& ri) : mIterator(ri.mIterator) { std::cout << "merkol::reverse_iterator::copy_constrcutor" << std::endl; }

		template<typedef U>
		reverse_iterator(const reverse_iterator<U>& ri) : mIterator(ri.base()) { }

		template<typedef U> // try return type reverse_iterator<Iterator>& whats the diff
		reverse_iterator& operator=(const reverse_iterator<U>& other)
			{ mIterator = other.base(); return (*this);
				std::cout << "merkol::reverse_iterator::coopy_assignment_operator=" << std::endl; }

		iterator_type base() const
			{ return mIterator; }
		
		reference operator*() const
		{
			iterator_type tmp(mIterator);
			return *--tmp;
		}

		pointer	operator->() const
		{
			// iterator_type tmp(mIterator);
			// return --tmp;
			return &(operator*());
		}

		reverse_iterator& operator++(void)
		{
			--mIterator;
			return *this;
		}

		reverse_iterator operator++(int)
		{
			reverse_iterator tmp(*this);
			--mIterator;
			return tmp;
		}

		reverse_iterator& operator--(void)
		{
			++mIterator;
			return *this;
		}

		reverse_iterator operator--(int)
		{
			reverse_iterator tmp(*this);
			++mIterator;
			return tmp;
		}

		reverse_iterator operator+(difference_type n) const
		{
			return reverse_iterator(mIterator - n);
		}

		reverse_iterator& operator+=(difference_type n)
		{
			mIterator -= n;
			return *this;
		}

		reverse_iterator operator-(difference_type n) const
		{
			return reverse_iterator(mIterator + n);
		}

		reverse_iterator operator-=(difference_type n)
		{
			mIterator += n;
			return *this;
		}

		reference operator[](difference_type n)
		{
			return (mIterator[-n - 1]);
		}

	};

	// non-member relational operators overload
	// The C++ library working group has tentatively approved the usage of two
	// template parameters (Iterator1 and Iterator2) in order to allow reverse_iterators
	// and const_reverse iterators to be comparable. This is a similar issue to the
	// C++ defect report #179 regarding comparison of container iterators and const_iterators.
	//
	// libstdc++ reports that std::relops breaks the usage of two iterator types and if we
	// want to support relops then we need to also make versions of each of below with
	// a single template parameter to placate std::relops. But relops is hardly used due to
	// the troubles it causes and so we are avoiding support here until somebody complains about it.

	//Compares the underlying iterators. Inverse comparisons are applied in order to take into account that the iterator order is reversed. 

	template<typename Iterator1, typename Iterator2>
	inline bool 
	operator==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return (lhs.base() == rhs.base());
	}

	template<typename Iterator1, typename Iterator2>
	inline bool 
	operator!=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return (lhs.base() != rhs.base());
	}

	template<typename Iterator1, typename Iterator2>
	inline bool 
	operator>(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return (lhs.base() > rhs.base());
	}

	template<typename Iterator1, typename Iterator2>
	inline bool 
	operator>=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return (lhs.base() >= rhs.base());
	}

	template<typename Iterator1, typename Iterator2>
	inline bool 
	operator<(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return (lhs.base() < rhs.base());
	}

	template<typename Iterator1, typename Iterator2>
	inline bool 
	operator<=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return (lhs.base() <= rhs.base());
	}

	template<typename Iterator1, typename Iterator2>
	inline typename reverse_iterator<Iterator1>::difference_type
	operator-(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return (y.base() - x.base());
	}

	template<typename Iterator>
	inline reverse_iterator<Iterator>
	operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& x)
	{
		return (reverse_iterator<Iterator>(x.base() - n));
	}

} // namespace merkol



#endif