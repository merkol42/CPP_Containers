#ifndef UTILS_HPP
# define UTILS_HPP

#include "type_traits.hpp"
#include <stdlib.h>

namespace merkol
{
	template <typename T, typename Alloc>
	void check_vector_assertion()
	{
		typedef typename merkol::remove_cv<T>::type non_cv_type;

		if (!merkol::is_same<non_cv_type, T>::value) {
			std::cerr << "std::vector must have a non-const, non-volatile value_type\n";
		}

		#if __cplusplus < 201103L || defined __STRICT_ANSI__
		if (!merkol::is_same<typename Alloc::value_type, T>::value) {
			std::cerr << "std::vector must have the same value_type as its allocator\n";
		}
		#endif
	}
} // namespace merkol




#endif