#pragma	once



// 
using i8 = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using f32 = float;
using f64 = double;


// 
using ascii = u8;

using utf8 = u8;
using utf16 = u16;
using utf32 = u32;

using ucs1 = u8;
using ucs2 = u16;
using ucs4 = u32;


// 
template <class T>	using uptr = std::unique_ptr<T>;
template <class T>	using sptr = std::shared_ptr<T>;
template <class T>	using wptr = std::weak_ptr<T>;
template <class T>	using lptr = local_ptr<T>;


// 
template <class T>	using enum_type = typename std::underlying_type<T>::type;
