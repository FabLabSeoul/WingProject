
#include "stdafx.h"
#include "indexbuffer.h"

using namespace graphic;


cIndexBuffer::cIndexBuffer() :
	m_pIdxBuff(NULL)
,	m_faceCount(0)
{
}

cIndexBuffer::~cIndexBuffer()
{
	Clear();
}


bool cIndexBuffer::Create(int faceCount)
{
	SAFE_RELEASE(m_pIdxBuff);

	if (FAILED(graphic::GetDevice()->CreateIndexBuffer(faceCount*3*sizeof(WORD), 
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff, NULL)))
	{
		return false;
	}

	m_faceCount = faceCount;
	return true;
}


void* cIndexBuffer::Lock()
{
	RETV(!m_pIdxBuff, NULL);

	WORD *indices = NULL;
	m_pIdxBuff->Lock(0, 0, (void**)&indices, 0);
	return indices;
}


void cIndexBuffer::Unlock()
{
	m_pIdxBuff->Unlock();
}


void cIndexBuffer::Bind() const
{
	GetDevice()->SetIndices(m_pIdxBuff);
}


void cIndexBuffer::Clear()
{
	m_faceCount = 0;
	SAFE_RELEASE(m_pIdxBuff);
}


cIndexBuffer& cIndexBuffer::operator=(cIndexBuffer &rhs)
{
	if (this != &rhs)
	{
		m_faceCount = rhs.m_faceCount;

		if (Create(rhs.m_faceCount))
		{
			if (BYTE* dest = (BYTE*)Lock())
			{
				if (BYTE *src = (BYTE*)rhs.Lock())
				{
					memcpy(dest, src, rhs.m_faceCount*3*sizeof(WORD));
					rhs.Unlock();
				}
				Unlock();
			}
		}
	}
	return *this;
}
