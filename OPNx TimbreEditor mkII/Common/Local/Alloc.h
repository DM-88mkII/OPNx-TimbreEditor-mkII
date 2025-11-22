#pragma	once



#if DefKit_Msvc//[
#define	alloca	_alloca
#endif//]

#define	local_alloc(T, s)	static_cast<T*>(alloca(s))
