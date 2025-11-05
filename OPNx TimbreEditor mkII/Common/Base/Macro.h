#pragma	once



#define	Auto			decltype(auto)



#define	nil8			static_cast<u8>(~0)
#define	nil16			static_cast<u16>(~0)
#define	nil32			static_cast<u32>(~0)
#define	nil64			static_cast<u64>(~0)
#define	nilT			static_cast<std::size_t>(~0)

#define	max8			static_cast<u8>(~0)
#define	max16			static_cast<u16>(~0)
#define	max32			static_cast<u32>(~0)
#define	max64			static_cast<u64>(~0)
#define	maxT			static_cast<std::size_t>(~0)



#if !defined(numof)//[
#define	numof(x)		(sizeof(x) / sizeof((x)[0]))
#endif//]



#if !defined(member_offset)//[
#define	member_offset(T, m)		to_t(&static_cast<T*>(nullptr)->m)
#endif//]



#if !defined(__PRETTY_FUNCTION__)//[
#define	__PRETTY_FUNCTION__		__FUNCTION__
#endif//]



#if !defined(attribute_packed)//[
#if DefKit_Msvc//[
#define	attribute_packed(...)\
{ __pragma(pack(push, 1)) __VA_ARGS__ __pragma(pack(pop)) }
#else//][
#define	attribute_packed(...)\
{ __VA_ARGS__ } __attribute__((packed))
#endif//]
#endif//]



#define	rawT(c)			std::remove_reference<decltype(c)>::type
#define	ptrT(c)			std::remove_reference<decltype(*c)>::type



#if DefPlatform_PS4//[
#if DefScheme_Resident//[
#define	FWM	__declspec(dllexport)
#else//][
#define	FWM	__declspec(dllimport)
#endif//]
#else//][
#define	FWM
#endif//]
