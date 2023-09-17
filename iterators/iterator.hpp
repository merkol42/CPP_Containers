#pragma once
#include <cstddef>


namespace merkol
{
	// Iterator categories
	// Every iterator is defined as belonging to one of the iterator categories that
	// we define here. These categories come directly from the C++ standard.
	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag			: public input_iterator_tag { };
	struct bidirectional_iterator_tag	: public forward_iterator_tag { };
	struct random_access_iterator_tag	: public bidirectional_iterator_tag { };
	// struct contiguous_iterator_tag		: public random_access_iterator_tag { }; // CXX20

	// struct iterator
	template <typename Category, typename T, typename Distance = std::ptrdiff_t,
			  typename Pointer = T*, typename Reference = T&>
	struct iterator
	{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	// Helper to make iterator_traits SFINAE friendly as N3844 requires.
	template <typename Iterator, class = void>
	struct default_iterator_traits { };

	// template <typename Iterator>
	// struct default_iterator_traits<Iterator, void_t

} // namespace merkol
