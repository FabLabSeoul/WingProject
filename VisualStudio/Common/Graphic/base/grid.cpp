
#include "stdafx.h"
#include "grid.h"

using namespace graphic;


cGrid::cGrid()
{
}

cGrid::~cGrid()
{
}


void cGrid::Create( const int rowCellCount, const int colCellCount, const float cellSize )
{
	// init member
	m_rowCellCount = rowCellCount;
	m_colCellCount = colCellCount;
	m_cellSize = cellSize;

	// Init Grid
	const int rowVtxCnt  = rowCellCount+1;
	const int colVtxCnt  = colCellCount+1;
	const int cellCnt = rowCellCount * colCellCount;
	const int vtxCount= rowVtxCnt * colVtxCnt;

	m_vtxBuff.Create( vtxCount, sizeof(sVertexDiffuse), sVertexDiffuse::FVF);
	{
		sVertexDiffuse *vertices = (sVertexDiffuse*)m_vtxBuff.Lock();
		const float startx = -cellSize*(colCellCount/2);
		const float starty = cellSize*(rowCellCount/2);
		const float endx = startx + cellSize*colCellCount;
		const float endy = starty - cellSize*rowCellCount;

		const float uCoordIncrementSize = 1.0f / (float)colCellCount;
		const float vCoordIncrementSize = 1.0f / (float)rowCellCount;

		int i=0;
		for (float y=starty; y >= endy; y -= cellSize, ++i)
		{
			int k=0;
			for (float x=startx; x <= endx; x += cellSize, ++k )
			{
				int index = (i * colVtxCnt) + k;
				vertices[ index].p = Vector3(x, 0, y);
				vertices[ index].c = 0xffcccccc;
			}
		}
		m_vtxBuff.Unlock();
	}	


	m_idxBuff.Create( cellCnt*2 );
	{
		WORD *indices = (WORD*)m_idxBuff.Lock();
		int baseIndex = 0;
		for( int i=0; i < rowCellCount; ++i )
		{
			for( int k=0; k < colCellCount; ++k )
			{
				indices[ baseIndex] = (i * colVtxCnt) + k;
				indices[ baseIndex + 1] = (i   * colVtxCnt) + k + 1;
				indices[ baseIndex + 2] = ((i+1) * colVtxCnt) + k;

				indices[ baseIndex + 3] = ((i+1) * colVtxCnt) + k;
				indices[ baseIndex + 4] = (i   * colVtxCnt) + k + 1;
				indices[ baseIndex + 5] = ((i+1) * colVtxCnt) + k + 1;

				// next quad
				baseIndex += 6;
			}
		}	
		m_idxBuff.Unlock();
	}

}


void cGrid::Render()
{
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE);

	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount());
}


void cGrid::RenderLinelist()
{
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE);

	m_vtxBuff.Bind();
	m_idxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount()*3/2);
}
