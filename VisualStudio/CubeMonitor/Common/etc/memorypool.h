// memory pool  using boost pool library
//
#pragma once

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>

namespace common
{

	template<class T>
	class cMemoryPool
	{
	public:
		__forceinline void* operator new (size_t size)
		{
			return m_pool.malloc();
		}
		__forceinline void* operator new (size_t size, char *attachTypeName)
		{
			return m_pool.malloc();
		}
		__forceinline void* operator new[] (size_t size)
		{
			return malloc(size);
		}

		// Debug new
		__forceinline void* operator new (size_t size, char* lpszFileName, int nLine)
		{
			return m_pool.malloc();
		}
		// Debug new
		__forceinline void* operator new[] (size_t size, char* lpszFileName, int nLine)
		{
			return malloc(size);
		}

		__forceinline void* operator new(size_t size, const char* handle, const char * filename, int line)	
		{ 
			return m_pool.malloc();
		}

		__forceinline void operator delete (void *ptr)
		{
			m_pool.free(ptr);
		}
		__forceinline void operator delete (void *ptr, char* lpszFileName, int nLine)
		{
			m_pool.free(ptr);
		}
		__forceinline void operator delete (void *ptr, char *attachTypeName)
		{
			m_pool.free(ptr);
		}
		__forceinline void operator delete[] (void *ptr)
		{
			free(ptr);
		}
		__forceinline void	operator delete (void* ptr, const char*, const char *, int)
		{
			m_pool.free(ptr);
		}

	private:
		static boost::pool<> m_pool;
	};

	template<class T>
	boost::pool<> cMemoryPool<T>::m_pool(sizeof(T));

}
