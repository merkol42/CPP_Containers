#ifndef MEMORY_HPP
# define MEMORY_HPP

#include <iostream>
#include <memory.h>

#define M_NOEXCEPT throw()

namespace merkol
{
	template<typename ForwardIt, typename Count>
	ForwardIt uninitialized_value_construct_n(ForwardIt first, Count n)
	{
		std::cout << "uninitialized_value_construct_n" << std::endl;
		typedef typename merkol::iterator_traits<ForwardIt>::value_type value_type;
		typename std::allocator<value_type> alloc;
		ForwardIt currentDest(first);

		try
		{
			for (; n > 0; --n, ++currentDest)
				alloc.construct(currentDest, value_type()); // ::new (static_cast<void*>(&*currentDest)) value_type(); ??
			return currentDest;
		}
		catch(const std::exception& e)
		{
			for (; first < currentDest; ++first)
				(*first).~value_type();
			std::cerr << "!!!!ERROR!!!! uninitialized_value_construct_n" << std::endl;
			throw;
		}
	}

	template<typename ForwardIt, typename Count, typename T>
	ForwardIt uninitialized_fill_n(ForwardIt first, Count n, T val)
	{
		std::cout << "uninitialized_fill_n" << std::endl;
		typedef typename merkol::iterator_traits<ForwardIt>::value_type value_type;
		typename std::allocator<value_type> alloc;
		ForwardIt currentDest(first);

		try
		{
			for (; n > 0; --n, ++currentDest)
				alloc.construct(currentDest, value_type(val)); // ::new (static_cast<void*>(&*currentDest)) value_type(); ??
			return currentDest;
		}
		catch(const std::exception& e)
		{
			for (; first < currentDest; ++first)
				(*first).~value_type();
			std::cerr << "!!!!ERROR!!!! uninitialized_fill_n" << std::endl;
			throw;
		}
	}

	// destruct(first, last)
	//
	template <typename ForwardIterator>
	inline void destruct_impl(ForwardIterator /*first*/, ForwardIterator /*last*/, merkol::true_type) // true means the type has a trivial destructor.
	{
		// Empty. The type has a trivial destructor.
	}

	template <typename ForwardIterator>
	inline void destruct_impl(ForwardIterator first, ForwardIterator last, merkol::false_type) // false means the type has a significant destructor.
	{
		typedef typename merkol::iterator_traits<ForwardIterator>::value_type value_type;

		for(; first != last; ++first)
			(*first).~value_type();
	}

	/// destruct
	///
	/// Calls the destructor on a range of objects.
	///
	/// We have a specialization for objects with trivial destructors, such as
	/// PODs. In this specialization the destruction of the range is a no-op.
	///
	template <typename ForwardIterator>
	inline void destruct(ForwardIterator first, ForwardIterator last)
	{
		typedef typename merkol::iterator_traits<ForwardIterator>::value_type value_type;
		destruct_impl(first, last, merkol::is_integral<value_type>());
	}

	/// addressof
	///
	/// From the C++11 Standard, section 20.6.12.1
	/// Returns the actual address of the object or function referenced by r, even in the presence of an overloaded operator&.
	///
	template<typename T>
	T* addressof(T& value) M_NOEXCEPT
	{
		return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(value)));
	}

} // namespace merkol


#endif // MEMORY_HPP
