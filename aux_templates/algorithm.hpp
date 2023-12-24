#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

namespace merkol
{
	/// equal
	///
	/// Returns: true if for every iterator i in the range [first1, last1) the
	/// following corresponding conditions hold: predicate(*i, *(first2 + (i - first1))) != false.
	/// Otherwise, returns false.
	///
	/// Complexity: At most last1 first1 applications of the corresponding predicate.
	///
	/// To consider: Make specializations of this for scalar types and random access
	/// iterators that uses memcmp or some trick memory comparison function.
	/// We should verify that such a thing results in an improvement.
	template<typename InputIterator1, typename InputIterator2>
	inline bool
	equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		for (; first1 != last1; ++first1, (void) ++first2)
		{
			if (!(*first1 == *first2))
				return false;
		}
		return true;
	}

	/// equal
	///
	/// Returns: true if for every iterator i in the range [first1, last1) the
	/// following corresponding conditions hold: pred(*i, *(first2 + (i first1))) != false.
	/// Otherwise, returns false.
	///
	/// Complexity: At most last1 first1 applications of the corresponding predicate.
	///
	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
	inline bool
	equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate predicate)
	{
		for (; first1 != last1; ++first1, (void) ++first2)
		{
			if (!predicate(*first1, *first2))
				return false;
		}
		return true;
	}

	template<typename InputIterator1, typename InputIterator2>
	inline bool
	lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		for(; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
		{
			if (*first1 < *first2)
				return true;
			if (*first2 < *first1)
				return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	// why the cast to void? 
	// the cast to void protects against element types that overload operator comma. https://en.cppreference.com/w/Talk:cpp/algorithm/lexicographical_compare
	template<typename InputIterator1, typename InputIterator2, typename Compare>
	inline bool
	lexicographical_compare(InputIterator1 first1, InputIterator1 last1, 
							InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		for(; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
		{
			if (comp(*first1, *first2))
				return true;
			if (comp(*first2, *first1))
				return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

} // namespace merkol



#endif // ALGORITHM
