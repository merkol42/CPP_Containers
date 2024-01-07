#ifndef NULL_PTR_HPP
# define NULL_PTR_HPP

// __cplusplus, C++ standardının bir tamsayı değerini içerir.
// C++11'den sonraki standartlar std::nullptr_t tipini tanımlar.
// Bu nedenle __cplusplus değeri 201103'ten büyükse, std::nullptr_t kullanılabilir.

#if __cplusplus < 201103L
namespace merkol
{
	// I'm not sure it's the right practice
	struct nullptr_t
	{
		// void *x = NULL; ???
	public: // default Access Specifier for struct is public
		// For conversion to any type of null non-member pointer.
		template<class T>
		operator T*() const { 
			std::cout << typeid(T).name() << " operatorT*" << std::endl;
			return (0); }

		// For conversion to any type of null member pointer.
		template<class C, class T>
		operator T C::*() const { 
			std::cout << typeid(T).name() << " operatorC::T*" << std::endl;
			return (0); }
	private:
		// It's impossible to get an adress of a nullptr
		void operator&() const {}
		
	};
	#define noexcept throw()
} // namespace merkol
#define nullptr merkol::nullptr_t()
	#endif // __cplusplus < 201103L
#endif // NULL_PTR_HPP