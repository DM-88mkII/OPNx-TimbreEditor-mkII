#pragma	once



// 
template <class T>	constexpr const T* const_on(T* p) noexcept	{ return const_cast<const T*>(p); }
template <class T>	constexpr const T& const_on(T& r) noexcept	{ return const_cast<const T&>(r); }
template <class T>	constexpr T* const_off(const T* p) noexcept	{ return const_cast<T*>(p); }
template <class T>	constexpr T& const_off(const T& r) noexcept	{ return const_cast<T&>(r); }


// 
template <class T>	constexpr i8 to_i8(T v) noexcept			{ return static_cast<i8>(v); }
template <class T>	constexpr i16 to_i16(T v) noexcept			{ return static_cast<i16>(v); }
template <class T>	constexpr i32 to_i32(T v) noexcept			{ return static_cast<i32>(v); }
template <class T>	constexpr i64 to_i64(T v) noexcept			{ return static_cast<i64>(v); }

template <class T>	constexpr u8 to_u8(T v) noexcept			{ return static_cast<u8>(v); }
template <class T>	constexpr u16 to_u16(T v) noexcept			{ return static_cast<u16>(v); }
template <class T>	constexpr u32 to_u32(T v) noexcept			{ return static_cast<u32>(v); }
template <class T>	constexpr u64 to_u64(T v) noexcept			{ return static_cast<u64>(v); }

template <class T>	constexpr f32 to_f32(T v) noexcept			{ return static_cast<f32>(v); }
template <class T>	constexpr f64 to_f64(T v) noexcept			{ return static_cast<f64>(v); }

template <class T>	constexpr char to_char(T v) noexcept		{ return static_cast<char>(v); }
template <class T>	constexpr ascii to_ascii(T v) noexcept		{ return static_cast<ascii>(v); }
template <class T>	constexpr utf8 to_utf8(T v) noexcept		{ return static_cast<utf8>(v); }
template <class T>	constexpr utf16 to_utf16(T v) noexcept		{ return static_cast<utf16>(v); }
template <class T>	constexpr utf32 to_utf32(T v) noexcept		{ return static_cast<utf32>(v); }
template <class T>	constexpr ucs1 to_ucs1(T v) noexcept		{ return static_cast<ucs1>(v); }
template <class T>	constexpr ucs2 to_ucs2(T v) noexcept		{ return static_cast<ucs2>(v); }
template <class T>	constexpr ucs4 to_ucs4(T v) noexcept		{ return static_cast<ucs4>(v); }

template <class T>	constexpr bool to_b(T v) noexcept			{ return static_cast<bool>(v); }
template <class T>	constexpr std::size_t to_t(T p) noexcept	{ return reinterpret_cast<std::size_t>(p); }
template <class T>	constexpr void* to_p(T p) noexcept			{ return reinterpret_cast<void*>(const_off(p)); }


// 
template <class T>	constexpr i8* to_i8p(T p) noexcept			{ return reinterpret_cast<i8*>(const_off(p)); }
template <class T>	constexpr i16* to_i16p(T p) noexcept		{ return reinterpret_cast<i16*>(const_off(p)); }
template <class T>	constexpr i32* to_i32p(T p) noexcept		{ return reinterpret_cast<i32*>(const_off(p)); }
template <class T>	constexpr i64* to_i64p(T p) noexcept		{ return reinterpret_cast<i64*>(const_off(p)); }

template <class T>	constexpr u8* to_u8p(T p) noexcept			{ return reinterpret_cast<u8*>(const_off(p)); }
template <class T>	constexpr u16* to_u16p(T p) noexcept		{ return reinterpret_cast<u16*>(const_off(p)); }
template <class T>	constexpr u32* to_u32p(T p) noexcept		{ return reinterpret_cast<u32*>(const_off(p)); }
template <class T>	constexpr u64* to_u64p(T p) noexcept		{ return reinterpret_cast<u64*>(const_off(p)); }

template <class T>	constexpr f32* to_f32p(T p) noexcept		{ return reinterpret_cast<f32*>(const_off(p)); }
template <class T>	constexpr f64* to_f64p(T p) noexcept		{ return reinterpret_cast<f64*>(const_off(p)); }

template <class T>	constexpr char* to_charp(T p) noexcept		{ return reinterpret_cast<char*>(const_off(p)); }
template <class T>	constexpr ascii* to_asciip(T p) noexcept	{ return reinterpret_cast<ascii*>(const_off(p)); }
template <class T>	constexpr utf8* to_utf8p(T p) noexcept		{ return reinterpret_cast<utf8*>(const_off(p)); }
template <class T>	constexpr utf16* to_utf16p(T p) noexcept	{ return reinterpret_cast<utf16*>(const_off(p)); }
template <class T>	constexpr utf32* to_utf32p(T p) noexcept	{ return reinterpret_cast<utf32*>(const_off(p)); }
template <class T>	constexpr ucs1* to_ucs1p(T p) noexcept		{ return reinterpret_cast<ucs1*>(const_off(p)); }
template <class T>	constexpr ucs2* to_ucs2p(T p) noexcept		{ return reinterpret_cast<ucs2*>(const_off(p)); }
template <class T>	constexpr ucs4* to_ucs4p(T p) noexcept		{ return reinterpret_cast<ucs4*>(const_off(p)); }

template <class T>	constexpr bool* to_bp(T p) noexcept			{ return reinterpret_cast<bool*>(const_off(p)); }
template <class T>	constexpr std::size_t* to_tp(T p) noexcept	{ return reinterpret_cast<std::size_t*>(const_off(p)); }
template <class T>	constexpr void** to_pp(T p) noexcept		{ return reinterpret_cast<void**>(const_off(p)); }


// 
template <class T, class S>	constexpr T* to_ptr(const S* p)			{ return reinterpret_cast<T*>(const_off(p)); }
template <class T, class S>	constexpr T* to_ptr(const S& r)			{ return reinterpret_cast<T*>(const_off(&r)); }

template <class T, class S>	constexpr T* to_ptr(const uptr<S>& p)	{ return reinterpret_cast<T*>(p.get()); }
template <class T, class S>	constexpr T* to_ptr(const sptr<S>& p)	{ return reinterpret_cast<T*>(p.get()); }
template <class T, class S>	constexpr T* to_ptr(const lptr<S>& p)	{ return reinterpret_cast<T*>(p.get()); }


// 
template <class T, class S>	constexpr T& to_ref(const S* p)			{ return *reinterpret_cast<T*>(const_off(p)); }
template <class T, class S>	constexpr T& to_ref(const S& r)			{ return *reinterpret_cast<T*>(const_off(&r)); }

template <class T, class S>	constexpr T& to_ref(const uptr<S>& p)	{ return *reinterpret_cast<T*>(p.get()); }
template <class T, class S>	constexpr T& to_ref(const sptr<S>& p)	{ return *reinterpret_cast<T*>(p.get()); }
template <class T, class S>	constexpr T& to_ref(const lptr<S>& p)	{ return *reinterpret_cast<T*>(p.get()); }


// 
template <class T>	constexpr enum_type<T> enum_value(T v) noexcept	{ return static_cast<enum_type<T>>(v); }
