#ifndef NULL_PTR_HPP
# define NULL_PTR_HPP

namespace merkol
{
	// non-complete
	struct nullptr_t
	{
	// For conversion to any type of null non-member pointer.
	template<class T>
	operator T*() const { return (0); }

	// For conversion to any type of null member pointer.
	template<class C, class T>
	operator T C::*() const { return (0); }
	private:
		// It's impossible to get an adress of a nullptr
		void operator&() const;
	} null_ptr = {};
	
	// typedef decltype(merkol::u_nullptr) null_ptr;

} // namespace merkol


#endif