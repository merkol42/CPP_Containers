#pragma once // 'pragma once' can be more optimized against 'include' ?
#include <cstddef>
#include "iterator_traits.hpp"

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


	/**
	 * @brief struct iterator base
	 * I didn't use iterator_base because it was removed in c++11.
	 * can be deleted afterwards
	*/
	template <typename Category, typename T, typename Distance = std::ptrdiff_t,
			  typename Pointer = T*, typename Reference = T&>
	struct iterator_base
	{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	template <bool is_valid, typename T>
	struct valid_iterator_tag_res
	{
		typedef T type;
		static const bool value = is_valid;
	};
	
	/*
	** @brief Basic to check if the typename given
	** is an input_iterator. Based on valid_iterator_tag_res.
	** In this if the typename is not from the possible
	** input iterator form, validity is set to false.
	*/
	template <typename T>
	struct is_input_iterator_tagged : public valid_iterator_tag_res<false, T> { };

	template <>
	struct is_input_iterator_tagged<merkol::random_access_iterator_tag>
		: public valid_iterator_tag_res<true, merkol::random_access_iterator_tag> { };

	template <>
	struct is_input_iterator_tagged<merkol::bidirectional_iterator_tag>
		: public valid_iterator_tag_res<true, merkol::bidirectional_iterator_tag> { };

	template <>
	struct is_input_iterator_tagged<merkol::forward_iterator_tag>
		: public valid_iterator_tag_res<true, merkol::forward_iterator_tag> { };

	template <>
	struct is_input_iterator_tagged<merkol::input_iterator_tag>
		: public valid_iterator_tag_res<true, merkol::input_iterator_tag> { };


	/*
	** @brief This will return a structure
	** that contain a boolean "value" true if the
	** iterator given is tagged with a merkol:: iterator
	** tag, otherwise "value" is false.
	*/
	template <typename T>
	struct is_my_iterator_tagged : public valid_iterator_tag_res<false, T> { };
	
	template <>
	struct is_my_iterator_tagged<merkol::random_access_iterator_tag>
		: public valid_iterator_tag_res<true, merkol::random_access_iterator_tag> { };

	template <>
	struct is_my_iterator_tagged<merkol::bidirectional_iterator_tag>
		: public valid_iterator_tag_res<true, merkol::bidirectional_iterator_tag> { };

	template <>
	struct is_my_iterator_tagged<merkol::forward_iterator_tag>
		: public valid_iterator_tag_res<true, merkol::forward_iterator_tag> { };

	template <>
	struct is_my_iterator_tagged<merkol::input_iterator_tag>
		: public valid_iterator_tag_res<true, merkol::input_iterator_tag> { };

	template <>
	struct is_my_iterator_tagged<merkol::output_iterator_tag>
		: public valid_iterator_tag_res<true, merkol::output_iterator_tag> { };


	template <typename _InputIterator>
	inline typename iterator_traits<_InputIterator>::difference_type _distance(
		_InputIterator first, _InputIterator last, merkol::input_iterator_tag)
	{
		typename iterator_traits<_InputIterator>::difference_type d(0);
		for (; first != last; ++first) ++d;
		return d;
	}

	template <typename _RandIterator>
	inline typename iterator_traits<_RandIterator>::difference_type _distance(
		_RandIterator first, _RandIterator last, merkol::random_access_iterator_tag)
	{
		return last - first;
	}

	template <typename _InputIterator>
	inline typename iterator_traits<_InputIterator>::difference_type distance(
		_InputIterator first, _InputIterator last)
	{
		return merkol::_distance(first, last, typename iterator_traits<_InputIterator>::iterator_category());
	}


	/**
	 * Call when the iterator tested does not meet demand.
	*/
	template <typename T>
	class InvalidIteratorException : public std::exception
	{
		private:
			std::string _msg;
		
		public :
			InvalidIteratorException () throw() { _msg = "Is invalid iterator tag : " + std::string(typeid(T).name()); }
			InvalidIteratorException (const InvalidIteratorException&) throw() {}
			InvalidIteratorException& operator= (const InvalidIteratorException&) throw() {}
			virtual ~InvalidIteratorException() throw() {}
			virtual const char* what() const throw() { return (_msg.c_str()); }
	};

} // namespace merkol
