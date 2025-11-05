#pragma	once



// value
static constexpr u64 bit(i32 v) noexcept	{ return 1ULL<<v; }

static constexpr u64 KiB(u64 v) noexcept	{ return v<<10; }
static constexpr u64 MiB(u64 v) noexcept	{ return v<<20; }
static constexpr u64 GiB(u64 v) noexcept	{ return v<<30; }
static constexpr u64 TiB(u64 v) noexcept	{ return v<<40; }
static constexpr u64 PiB(u64 v) noexcept	{ return v<<50; }
static constexpr u64 EiB(u64 v) noexcept	{ return v<<60; }


// id
static constexpr u32 id4(ucs1 c0, ucs1 c1, ucs1 c2, ucs1 c3) noexcept										{ return c0 | (c1<<8) | (c2<<16) | (c3<<24); }
static constexpr u64 id8(ucs1 c0, ucs1 c1, ucs1 c2, ucs1 c3, ucs1 c4, ucs1 c5, ucs1 c6, ucs1 c7) noexcept	{ return id4(c0, c1, c2, c3) | (to_u64(id4(c4, c5, c6, c7))<<32); }


// math
template <class T>	constexpr T abs(T v) noexcept											{ return (v < 0)? -v: v; }
template <class T>	constexpr T div(T a, decltype(a) b) noexcept							{ return (b == 0)? 0: (a / b); }

template <class T>	constexpr T minimum(T src, decltype(src) trg) noexcept					{ return (src > trg)? src: trg; }
template <class T>	constexpr T maximum(T src, decltype(src) trg) noexcept					{ return (src < trg)? src: trg; }

template <class T>	constexpr T clamp(T src, decltype(src) min, decltype(src) max) noexcept	{ return maximum(minimum(src, min), max); }


// t
static constexpr std::size_t bound_t(std::size_t v, std::size_t b) noexcept					{ return v & (~b + 1); }
static constexpr std::size_t align_t(std::size_t v, std::size_t a) noexcept					{ return bound_t(v + (a-1), a); }


// p
template <class T = void*>	constexpr T bound_p(const void* p, std::size_t b) noexcept		{ return reinterpret_cast<T>(bound_t(to_t(p), b)); }
template <class T = void*>	constexpr T align_p(const void* p, std::size_t a) noexcept		{ return reinterpret_cast<T>(align_t(to_t(p), a)); }
template <class T = void*>	constexpr T offset_p(const void* p, std::size_t o) noexcept		{ return reinterpret_cast<T>(to_t(p) + o); }

static constexpr std::size_t distance_p(const void* pHead, const void* pTail) noexcept		{ return to_t(pTail) - to_t(pHead); }


// exist
template <class T>	constexpr bool exist(const T* p) noexcept		{ return p; }
template <class T>	constexpr bool exist(const T& r) noexcept		{ return &r; }

template <class T>	constexpr bool exist(const uptr<T>& p) noexcept	{ return p.get(); }
template <class T>	constexpr bool exist(const sptr<T>& p) noexcept	{ return p.get(); }
template <class T>	constexpr bool exist(const lptr<T>& p) noexcept	{ return p.get(); }


// nullref
template <class T>	constexpr T& nullref() noexcept					{ return *static_cast<T*>(nullptr); }
