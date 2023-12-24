#ifndef MYSTL_VECTOR_HPP
# define MYSTL_VECTOR_HPP

#include <memory>
#include <iostream>
#include <utility>
#include "../iterators/random_access_iterator.hpp"
#include "../iterators/reverse_iterator.hpp"
#include "../auxiliary/information_printer.hpp"
#include "../aux_templates/algorithm.hpp"

namespace merkol
{
	/* vectorBase
		Bir VectorBase sınıfına sahip olmamızın nedeni, onun istisna işleme yapmasıdır.
		Bellek tahsisi tamamen bu sınıfta uygulandığı için uygulanması daha basittir.
		Bir kullanıcı, yapıcıda bellek ayırması gereken bir vektör oluşturursa, vectorBase bunu halleder.
		Ayırıcı tarafından bir istisna fırlatılırsa, istisna fırlatma kullanıcı koduna geri atlar ve vektörde veya vectorBase yapıcısında herhangi bir try/catch kodunun yazılmasına gerek yoktur.
		Vektör (vectorBase değil) yapıcısında bir istisna atılırsa, yürütme kullanıcı koduna geri atlamadan önce vectorBase için yıkıcı otomatik olarak çağrılır (ve ayrılan belleği boşaltır).
		Bununla birlikte, eğer vektör sınıfı hem tahsisi hem de başlatmayı idare edecek olsaydı, o zaman belleği tahsis eden tüm yollar için açık bir try/catch deyimi uygulamaktan başka seçeneği olmazdı.
		Bu, kod boyutunu artırır ve performansı düşürür ve kodun okunmasını ve bakımını biraz zorlaştırır.
	*/

	/*
		Vector taban sınıfı. Bu sınıf, %Vector'ün tahsisi için birleştirilmiş arayüzü sağlar. 
		Bu sınıfın yapıcısı ve yıkıcısı, depolamayı ayırır ve serbest bırakır (ancak başlatmaz).
		Bu, %istisna güvenliğini kolaylaştırır.

		Bu sınıftaki hiçbir şey gerçek bir Tp öğesini oluşturmaz veya yok etmez. (Vector bunu kendisi halleder.) 
		Yalnızca/Tüm bellek yönetimi burada gerçekleştirilir.
	*/

	/*
		C++ standardı (15.2 paragraf 2) şöyle der:
		"Kısmen oluşturulmuş veya kısmen yok edilmiş bir nesne, tamamen oluşturulmuş tüm alt nesneleri için,
		yani oluşturucunun yürütmeyi tamamladığı ve yok edicinin henüz tamamlamadığı alt nesneler için
		yıkıcıları çalıştıracaktır. infaz başladı."

		C++ standardı (15.3 paragraf 11) şunları belirtir:
		"Bir nesnenin tamamen oluşturulmuş temel sınıfları ve üyeleri,
		o blok için bir yapıcı veya yıkıcının işlev deneme bloğunun işleyicisine girmeden önce yok edilmelidir."
	*/
	template <typename T, typename Allocator>
	struct vectorBase
	{
		typedef Allocator			allocator_type;
		typedef std::size_t			size_type;
		typedef std::ptrdiff_t		difference_type;
	
		static const size_type npos		= (size_type)-1; // 'npos' means non-valid position or simply non-position.
		static const size_type kMaxSize	= (size_type)-2; // -1 is reserved for 'npos'. It also happens to be slightly beneficial that kMaxSize is a value less than -1, as it helps us deal with potential integer wraparound issues.

		size_type getNewCapacity(size_type currentCapacity);
	protected:
		T*								mpBegin;
		T*								mpEnd;
		std::pair<T*, allocator_type>	mCapacityAllocator;

		T*& internalCapacityPtr() { return mCapacityAllocator.first; }
		T* const& internalCapacityPtr() const { return mCapacityAllocator.first; }
		allocator_type& internalAllocator() { return mCapacityAllocator.second; }
		const allocator_type& internalAllocator() const { return mCapacityAllocator.second; }

	public:
		vectorBase();
		vectorBase(const allocator_type& allocator);
		vectorBase(size_type n, const allocator_type& allocator);

		~vectorBase();

		const allocator_type&	get_allocator() const;
		allocator_type&			get_allocator();
		void					set_allocator(const allocator_type& allocator);

	protected:
		T*		doAllocate(size_type n);
		void	doFree(T* p, size_type n);
	}; // vectorBase


	///////////////////////////////////////////////////////////////////////
	// VectorBase.imp.begin();											///
	///////////////////////////////////////////////////////////////////////

	template<typename T, typename Allocator>
	inline vectorBase<T, Allocator>::vectorBase() 
		: 
		mpBegin(NULL),
		mpEnd(NULL),
		mCapacityAllocator(NULL, Allocator())
	{
	}

	template<typename T, typename Allocator>
	inline vectorBase<T, Allocator>::vectorBase(const allocator_type& allocator)
		: 
		mpBegin(NULL),
		mpEnd(NULL),
		mCapacityAllocator(NULL, allocator)
	{
	}


	template<typename T, typename Allocator>
	inline vectorBase<T, Allocator>::vectorBase(size_type n, const allocator_type& allocator)
		: mCapacityAllocator(NULL, allocator)
	{
		mpBegin	= doAllocate(n);
		mpEnd	= mpBegin;
		internalCapacityPtr() = mpBegin + n;
	}

	template<typename T, typename Allocator>
	inline vectorBase<T, Allocator>::~vectorBase()
	{
		if (mpBegin)
			internalAllocator().deallocate(mpBegin, (internalCapacityPtr() - mpBegin) * sizeof(T));
	}

	template<typename T, typename Allocator>
	inline typename vectorBase<T, Allocator>::allocator_type&
	vectorBase<T, Allocator>::get_allocator()
	{
		return (internalAllocator());
	}

	template<typename T, typename Allocator>
	inline const typename vectorBase<T, Allocator>::allocator_type&
	vectorBase<T, Allocator>::get_allocator() const
	{
		return (internalAllocator());
	}

	template<typename T, typename Allocator>
	inline void vectorBase<T, Allocator>::set_allocator(const allocator_type& allocator)
	{
		internalAllocator() = allocator;
	}

	template<typename T, typename Allocator>
	inline T* vectorBase<T, Allocator>::doAllocate(size_type n)
	{
		if (n > internalAllocator().max_size())
			throw std::length_error("merkol::vector -- requested size exceeds max_size()");
		if (n)
		{
			T*	ptr = internalAllocator().allocate(n * sizeof(T));
			if (!ptr)
				throw std::runtime_error("merkol::vector -- memory allocation failed");
			return ptr;
		}
		return NULL;
	}

	template<typename T, typename Allocator>
	inline void vectorBase<T, Allocator>::doFree(T* p, size_type n)
	{
		if (p)
			internalAllocator().deallocate(p, n * sizeof(T));
	}

	template<typename T, typename Allocator>
	inline typename vectorBase<T, Allocator>::size_type
	vectorBase<T, Allocator>::getNewCapacity(size_type currentCapacity)
	{
		return (currentCapacity > 0) ? (currentCapacity * 2) : 1;
	}

	///////////////////////////////////////////////////////////////////////
	// VectorBase.imp.end();											///
	///////////////////////////////////////////////////////////////////////
	

	/**
	 * @brief vector
	 * Implements a dynamic array.
	 * 
	 * @tparam T value type
	 * @tparam Allocator allocator type
	 */
	template <typename T, typename Allocator = std::allocator<T> >
	class vector : public vectorBase<T, Allocator>
	{
		typedef	vectorBase<T, Allocator>	base_type;
		typedef	vector<T, Allocator>		this_type;
	// protected: std >= c++11 
	// 	using base_type::mpBegin;
	// 	using base_type::mpEnd;
	// 	using base_type::mCapacityAllocator;
	// 	using base_type::DoAllocate;
	// 	using base_type::DoFree;
	// 	using base_type::internalCapacityPtr;
	// 	using base_type::internalAllocator;
	public:
		typedef T														value_type;
		typedef T*														pointer;
		typedef const T*												const_pointer;
		typedef T&														reference;
		typedef const T&												const_reference;
		typedef merkol::random_access_iterator<value_type>				iterator;
		typedef const merkol::random_access_iterator<const value_type>	const_iterator;
		typedef merkol::reverse_iterator<iterator>						reverse_iterator;
		typedef const merkol::reverse_iterator<const_iterator>			const_reverse_iterator;
		typedef typename base_type::size_type							size_type;
		typedef typename base_type::difference_type						difference_type;
		typedef typename base_type::allocator_type						allocator_type; // || tt Allocator allocator_type;
	
	public:
		// Constructors
		vector();
		explicit vector(const Allocator& alloc);
		explicit vector(size_type count, const value_type& val = value_type(), const Allocator& alloc = Allocator());
		vector(const this_type& other);
		
		// note: this has pre-C++11 semantics:
		// this constructor is equivalent to the constructor vector(static_cast<size_type>(first), static_cast<value_type>(last), allocator) if InputIterator is an integral type.
		// SFINAE Required
		template<typename InputIterator>
		vector(InputIterator first, InputIterator last, const Allocator& alloc = Allocator());
		
		~vector();

		// Copy assignment operator
		this_type&	operator=(const this_type& other);
		
		//base_type->get_allocator();
		
		void	assign(size_type n, const value_type& value);

		template<typename InputIterator> //SFINAE Required
		void	assign(InputIterator first, InputIterator last);

		// Iterators
		iterator		begin();
		const_iterator	begin() const;

		iterator		end();
		const_iterator	end() const;

		reverse_iterator		rbegin();
		const_reverse_iterator	rbegin() const;

		reverse_iterator		rend();
		const_reverse_iterator	rend() const;

		// Element access
		pointer			data();
		const_pointer	data() const;

		reference		at(size_type n);
		const_reference	at(size_type n) const;

		reference		front();
		const_reference	front() const;

		reference		back();
		const_reference	back() const;

		reference		operator[](size_type n);
		const_reference	operator[](size_type n) const;

		// Capacity
		bool		empty() const;
		size_type	size() const;
		size_type	capacity() const;
		void		reserve(size_type n);
		//base_type->max_size();

		// Modifiers
		void		clear();
		iterator	insert(const_iterator pos, const T& value);
		iterator	insert(const_iterator pos, size_type count, const T& value);

		// note: this has pre-C++11 semantics:
		// this function is equivalent to insert(const_iterator position, static_cast<size_type>(first), static_cast<value_type>(last)) if InputIterator is an integral type.
		// ie. same as insert(const_iterator position, size_type n, const value_type& value)
		template<typename InputIterator> // SFINAE Required
		iterator	insert(const_iterator pos, InputIterator first, InputIterator last);

		iterator	erase(iterator pos);
		iterator	erase(iterator first, iterator last);

		void		push_back(const value_type& val);
		void		pop_back(void);

		void		resize(size_type count);
		void		resize(size_type count, const value_type& value);

		void		swap(vector& other);
	};

	template<typename T, typename Allocator>
	inline merkol::vector<T, Allocator>::vector() : base_type()
	{
		print_info("vector::default_constructor");
		// Empty
	}

	template<typename T, typename Allocator>
	inline merkol::vector<T, Allocator>::vector(const Allocator& alloc) : base_type(alloc)
	{
		print_info("vector::allocator_constructor");
	}

	// template<typename T, typename Allocator>
	// inline merkol::vector<T, Allocator>::vector(size_type count, const value_type& val = value_type(), const Allocator& alloc = Allocator())
	// {
		
	// }
	
	
	// vector(const this_type& other);
	
	// // note: this has pre-C++11 semantics:
	// // this constructor is equivalent to the constructor vector(static_cast<size_type>(first), static_cast<value_type>(last), allocator) if InputIterator is an integral type.
	// // SFINAE Required
	// template<typename InputIterator>
	// vector(InputIterator first, InputIterator last, const Allocator& alloc = Allocator());
	
	template <typename T, typename Allocator>
	merkol::vector<T, Allocator>::~vector()
	{
		print_info("merkol::vector::destructor");
	}

	///////////////////////////////////////////////////////////////////////
	// non-member relational operators overload(vector global operators)///
	///////////////////////////////////////////////////////////////////////
	template<typename T, typename Allocator>
	inline bool
	operator==(const merkol::vector<T, Allocator>& a, const merkol::vector<T, Allocator>& b)
	{
		return ((a.size() == b.size()) && merkol::equal(a.begin(), a.end(), b.begin()));
		return 0;

	}

	template<typename T, typename Allocator>
	inline bool
	operator!=(const merkol::vector<T, Allocator>& a, const merkol::vector<T, Allocator>& b)
	{
		return ((a.size() == b.size()) && !merkol::equal(a.begin(), a.end(), b.begin()));
		return 0;
	}

	template<typename T, typename Allocator>
	inline bool
	operator<(const merkol::vector<T, Allocator>& a, const merkol::vector<T, Allocator>& b)
	{
		return merkol::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
		return 0;

	}

	template<typename T, typename Allocator>
	inline bool
	operator>(const merkol::vector<T, Allocator>& a, const merkol::vector<T, Allocator>& b)
	{
		return (b < a);
		return 0;

	}

	template<typename T, typename Allocator>
	inline bool
	operator<=(const merkol::vector<T, Allocator>& a, const merkol::vector<T, Allocator>& b)
	{
		return !(b < a);
		return 0;

	}

	template<typename T, typename Allocator>
	inline bool
	operator>=(const merkol::vector<T, Allocator>& a, const merkol::vector<T, Allocator>& b)
	{
		return !(a < b);
		return 0;

	}

	template <typename T, typename Allocator>
	inline void swap(vector<T, Allocator>& a, vector<T, Allocator>& b)
	{
		a.swap(b);
	}

} // namespace merkol


#endif // MYSTL_VECTOR_HPP