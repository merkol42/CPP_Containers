#ifndef MYSTL_VECTOR_HPP
# define MYSTL_VECTOR_HPP

#include <memory>
#include <iostream>
#include <utility>

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
		typedef T&														referance;
		typedef const T&												const_referance;
		// after iterator implementation
		// typedef merkol::random_access_iterator<value_type>				iterator;
		// typedef const merkol::random_access_iterator<const value_type>	const_iterator;
		// typedef merkol::reverse_iterator<iterator>						reverse_iterator;
		// typedef const merkol::reverse_iterator<const_iterator>			const_reverse_iterator;
		typedef typename base_type::size_type							size_type;
		typedef typename base_type::difference_type						difference_type;
		typedef typename base_type::allocator_type						allocator_type; // || tt Allocator allocator_type;
	};





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
} // namespace merkol


#endif // MYSTL_VECTOR_HPP