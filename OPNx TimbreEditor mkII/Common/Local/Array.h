#pragma	once



// Class

template <class T>
class local_array final {
	public:
		~local_array() noexcept											{ auto p = end(); for (auto n = mn; n; --n){ --p; p->~T(); } }
		
		explicit local_array(T* a, std::size_t n)						:ma(a),mn(n){ auto p = begin(); for (auto n = mn; n; --n){ new(p) T; ++p; } }
		
		local_array(local_array<T>&& Src) noexcept						{ ma = Src.ma; mn = Src.mn; }
		local_array<T>& operator =(local_array<T>&& Src) noexcept		{ ma = Src.ma; mn = Src.mn; return *this; }
		
		T& operator [](std::size_t o) const noexcept					{ return ma[o]; }
		T& at(std::size_t o) const										{ if (o >= mn){ throw std::exception("local_array : out_of_range"); } return ma[o]; }
	
	public:
		using value_type = T;
		using reference = const T&;
		using const_reference = const T&;
		using size_type = std::size_t;
		using iterator = T*;
		using const_iterator = const T*;
		
		constexpr std::size_t size() const noexcept						{ return mn; }
		constexpr iterator data() const noexcept						{ return ma; }
		constexpr iterator begin() const noexcept						{ return ma; }
		constexpr iterator end() const noexcept							{ return &ma[mn]; }
		constexpr const_iterator cbegin() const noexcept				{ return ma; }
		constexpr const_iterator cend() const noexcept					{ return &ma[mn]; }
	
	private:
		local_array(const local_array<T>& Src) noexcept					= delete;
		local_array<T>& operator =(const local_array<T>& Src) noexcept	= delete;
	
	
	// Variable
	private:
		T* ma;
		std::size_t mn;
};



// 

#define	local_array(T, n)		local_array<T>(local_alloc(T, sizeof(T) * n), n)
