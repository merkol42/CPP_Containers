#ifndef MYSTL_VECTOR_HPP
# define MYSTL_VECTOR_HPP

#include <memory>
#include <iostream>
#include <utility>
#include "../iterators/random_access_iterator.hpp"
#include "../iterators/reverse_iterator.hpp"
#include "../auxiliary/information_printer.hpp"
#include "../aux_templates/algorithm.hpp"
#include "../aux_templates/utils.hpp"
#include "../memory/memory.hpp"

// for test
// #include <chrono>
// #include <thread>

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

		T*& internalPtr() { return mCapacityAllocator.first; }
		T* const& internalPtr() const { return mCapacityAllocator.first; }
		allocator_type& internalAllocator() { return mCapacityAllocator.second; }
		const allocator_type& internalAllocator() const { return mCapacityAllocator.second; }

	public:
		vectorBase();
		vectorBase(const allocator_type& allocator);
		//vectorBase(size_type n);
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
		// check_vector_assertion<T, Allocator>();
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
		internalPtr() = mpBegin + n;
	}

	template<typename T, typename Allocator>
	inline vectorBase<T, Allocator>::~vectorBase()
	{
		print_info("merkol::vectorBase::destructor");
		// std::this_thread::sleep_for(std::chrono::seconds(3));
		if (mpBegin)
			internalAllocator().deallocate(mpBegin, (internalPtr() - mpBegin) * sizeof(T));
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
			T*	ptr = (T*)internalAllocator().allocate(n * sizeof(T));
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
		explicit vector(const Allocator& alloc) M_NOEXCEPT;
		explicit vector(size_type n, const allocator_type& allocator = Allocator());
		vector(size_type n, const value_type& val, const Allocator& allocator = Allocator());
		vector(const this_type& other);
		
		// note: this has pre-C++11 semantics:
		// this constructor is equivalent to the constructor vector(static_cast<size_type>(first), static_cast<value_type>(last), allocator) if InputIterator is an integral type.
		// SFINAE Required
		template<typename InputIterator>
		vector(InputIterator first, InputIterator last, const Allocator& alloc = Allocator(), typename merkol::enable_if<!merkol::is_integral<InputIterator>::value>::type* = 0);
		
		~vector();

		// Copy assignment operator
		this_type&	operator=(const this_type& other);
		
		//base_type->get_allocator();
		
		void	assign(size_type n, const value_type& value);

		template<typename InputIterator> //SFINAE Required
		void	assign(InputIterator first, InputIterator last);

		// Iterators
		iterator		begin() M_NOEXCEPT;
		const_iterator	begin() const M_NOEXCEPT;

		iterator		end() M_NOEXCEPT;
		const_iterator	end() const M_NOEXCEPT;

		reverse_iterator		rbegin() M_NOEXCEPT;
		const_reverse_iterator	rbegin() const M_NOEXCEPT;

		reverse_iterator		rend() M_NOEXCEPT;
		const_reverse_iterator	rend() const M_NOEXCEPT;

		// Element access
		pointer			data() M_NOEXCEPT;
		const_pointer	data() const M_NOEXCEPT;

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
		void		clear() M_NOEXCEPT;
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
	inline merkol::vector<T, Allocator>::vector(const Allocator& alloc) M_NOEXCEPT
	: base_type(alloc)
	{
		print_info("vector::allocator_constructor");
		// Empty
	}

	template<typename T, typename Allocator>
	inline merkol::vector<T, Allocator>::vector(size_type n, const allocator_type& allocator)
	: base_type(n, allocator)
	{
		merkol::uninitialized_value_construct_n(this->mpBegin, n);
		//std::uninitialized_fill(this->__begin_, this->__begin_ + n, value_type());
		this->mpEnd = this->mpBegin + n;
	}


	template<typename T, typename Allocator>
	inline merkol::vector<T, Allocator>::vector(size_type n, const value_type& val, const Allocator& alloc)
	: base_type(n, alloc)
	{
		print_info("vector(size_type n, const value_type& val, const Allocator& alloc)");
		std::uninitialized_fill(this->mpBegin, this->mpBegin + n, val);
		this->mpEnd = this->mpBegin + n;
		// for (size_type i = 0; i < n; i++)
		// 	std::cout << *(this->mpBegin + i) << std::endl;
	}
	
	// note: this has pre-C++11 semantics:
	// this constructor is equivalent to the constructor vector(static_cast<size_type>(first), static_cast<value_type>(last), allocator) if InputIterator is an integral type.
	// SFINAE Required
	template<typename T, typename Allocator>
	template<typename InputIterator>
	inline merkol::vector<T, Allocator>::vector(InputIterator first, InputIterator last, const Allocator& alloc,
													typename merkol::enable_if<!merkol::is_integral<InputIterator>::value>::type*)
	: base_type(static_cast<size_type>(merkol::distance(first, last), alloc))
	{
		print_info("Input iter constructor");
		this->mpEnd = std::uninitialized_copy(first, last, this->mpBegin);
	}
	
	template<typename T, typename Allocator>
	inline merkol::vector<T, Allocator>::vector(const this_type& other)
	: base_type(other.size(), other.internalAllocator())
	{
		print_info("Copy constructor");
		this->mpEnd = std::uninitialized_copy(other.mpBegin, other.mpEnd, this->mpBegin);
	}
	
	template <typename T, typename Allocator>
	inline merkol::vector<T, Allocator>::~vector()
	{
		print_info("merkol::vector::destructor");
		merkol::destruct(this->mpBegin, this->mpEnd);
	}

	// Copy assignment operator
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::this_type&
	merkol::vector<T, Allocator>::operator=(const this_type& other)
	{
		print_info("merkol::vector::operator=");
		if (this != &other)
		{
			
		}
		return (*this);
	}

	template<typename T, typename Allocator>
	void merkol::vector<T, Allocator>::assign(size_type n, const value_type& value)
	{
		if (n > size_type(this->internalAllocator() - this->mpBegin)) // if n > capacity
		{
			this_type temp(n, value); // We have little choice but to reallocate with new memory.
			swap(temp);
		}
		else if (n > size_type(this->mpEnd - this->mpBegin)) // if n > size
		{
			std::fill(this->mpBegin, this->mpEnd, value);
			std::uninitialized_fill_n(this->mpEnd, n - size_type(this->mpEnd - this->mpBegin), value);
			this->mpEnd += n - size_type(this->mpEnd - this->mpBegin);
		}
		else // else 0 <= n <= size
		{
			std::fill_n(this->mpBegin, n, value);
			// erase(this->mpBegin + n, this->mpEnd);
		}

	}


	// template<typename InputIterator> //SFINAE Required
	// void	assign(InputIterator first, InputIterator last);



	// Iterators
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::iterator 
		merkol::vector<T, Allocator>::begin() M_NOEXCEPT
	{
		return this->mpBegin;
	}

	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::const_iterator 
		merkol::vector<T, Allocator>::begin() const M_NOEXCEPT
	{
		return this->mpBegin;

	}

	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::iterator 
		merkol::vector<T, Allocator>::end() M_NOEXCEPT
	{
		return this->mpEnd;

	}

	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::const_iterator 
		merkol::vector<T, Allocator>::end() const M_NOEXCEPT
	{
		return this->mpEnd;
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::reverse_iterator 
		merkol::vector<T, Allocator>::rbegin() M_NOEXCEPT
	{
		return reverse_iterator(this->mpEnd);
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::const_reverse_iterator 
		merkol::vector<T, Allocator>::rbegin() const M_NOEXCEPT
	{
		return const_reverse_iterator(this->mpEnd);
	}

	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::reverse_iterator 
		merkol::vector<T, Allocator>::rend() M_NOEXCEPT
	{
		return reverse_iterator(this->mpBegin);
	}

	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::const_reverse_iterator 
		merkol::vector<T, Allocator>::rend() const M_NOEXCEPT
	{
		return const_reverse_iterator(this->mpBegin);
	}
	


	// Element access

	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::pointer 
		merkol::vector<T, Allocator>::data() M_NOEXCEPT
	{
		return this->mpBegin;
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::const_pointer 
		merkol::vector<T, Allocator>::data() const M_NOEXCEPT
	{
		return this->mpBegin;
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::reference 
		merkol::vector<T, Allocator>::at(size_type n)
	{
		if (n >= static_cast<size_type>(this->mpEnd - this->mpBegin))
			throw std::out_of_range("merkol::vector::at -- out of range");
		return *(this->mpBegin + n);
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::const_reference 
		merkol::vector<T, Allocator>::at(size_type n) const
	{
		if (n >= static_cast<size_type>(this->mpEnd - this->mpBegin))
			throw std::out_of_range("merkol::vector::at -- out of range");
		return *(this->mpBegin + n);
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::reference 
		merkol::vector<T, Allocator>::front()
	{
		if ((this->mpBegin == nullptr) || (this->mpEnd <= this->mpBegin))
			std::cerr << "merkol::vector::front() -- empty vector" << std::endl;
		return *this->mpBegin;
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::const_reference 
		merkol::vector<T, Allocator>::front() const
	{
		if ((this->mpBegin == nullptr) || (this->mpEnd <= this->mpBegin))
			std::cerr << "merkol::vector::front() -- empty vector" << std::endl;
		return *this->mpBegin;
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::reference 
		merkol::vector<T, Allocator>::back()
	{
		if ((this->mpBegin == nullptr) || (this->mpEnd <= this->mpBegin))
			std::cerr << "merkol::vector::front() -- empty vector" << std::endl;
		return *(this->mpEnd - 1);
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::const_reference 
		merkol::vector<T, Allocator>::back() const
	{
		if ((this->mpBegin == nullptr) || (this->mpEnd <= this->mpBegin))
			std::cerr << "merkol::vector::front() -- empty vector" << std::endl;
		return *(this->mpEnd - 1);
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::reference 
		merkol::vector<T, Allocator>::operator[](size_type n)
	{
		if (n >= static_cast<size_type>(this->mpEnd - this->mpBegin))
			throw std::out_of_range("merkol::vector::operator[] -- out of range");
		return *(this->mpBegin + n);
	}
	
	template<typename T, typename Allocator>
	typename merkol::vector<T, Allocator>::const_reference 
		merkol::vector<T, Allocator>::operator[](size_type n) const
	{
		if (n >= static_cast<size_type>(this->mpEnd - this->mpBegin))
			throw std::out_of_range("merkol::vector::operator[] -- out of range");
		return *(this->mpBegin + n);
	}


	// Capacity
	template<typename T, typename Allocator>
	bool merkol::vector<T, Allocator>::empty() const
	{
		return (this->mpBegin == this->mpEnd);
	}

	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::size_type
	merkol::vector<T, Allocator>::size() const
	{
		return (size_type)(this->mpEnd - this->mpBegin);
	}
	// size_type	capacity() const;
	// void		reserve(size_type n);
	// //base_type->max_size();


	// // Modifiers
	// void		clear() M_NOEXCEPT;
	// iterator	insert(const_iterator pos, const T& value);
	// iterator	insert(const_iterator pos, size_type count, const T& value);
	// // note: this has pre-C++11 semantics:
	// // this function is equivalent to insert(const_iterator position, static_cast<size_type>(first), static_cast<value_type>(last)) if InputIterator is an integral type.
	// // ie. same as insert(const_iterator position, size_type n, const value_type& value)
	// template<typename InputIterator> // SFINAE Required
	// iterator	insert(const_iterator pos, InputIterator first, InputIterator last);
	// iterator	erase(iterator pos);
	// iterator	erase(iterator first, iterator last);
	// void		push_back(const value_type& val);
	// void		pop_back(void);
	// void		resize(size_type count);
	// void		resize(size_type count, const value_type& value);
	
	template<typename T, typename Allocator>
	void merkol::vector<T, Allocator>::swap(vector& other)
	{
		merkol::swap(this->mpBegin, other.mpBegin);
		merkol::swap(this->mpEnd, other.mpEnd);
		merkol::swap(this->mCapacityAllocator, other.mCapacityAllocator);
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