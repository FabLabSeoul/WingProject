#pragma once

namespace common
{
	template <class T>
	class cSingleton
	{
	public:
		static T* m_pInstance;
		static T* Get()
		{
			if (!m_pInstance)
				m_pInstance = new T;
			return m_pInstance;
		}
		static void Release()
		{
			if (m_pInstance)
			{
				delete m_pInstance;
				m_pInstance = NULL;
			}
		}
	};
	template <class T>
	T* cSingleton<T>::m_pInstance = NULL;
}
