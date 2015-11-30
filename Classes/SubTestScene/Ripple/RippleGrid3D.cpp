/*!
 * \author:	FreeKnight
 * \date:	2014-6-26
 * \comment:
 */
//--------------------------------------------------------
#include "RippleGrid3D.h"
//--------------------------------------------------------
CRippleGrid3D* CRippleGrid3D::create(const CCSize& gridNum)
{
	CRippleGrid3D *pRet= new CRippleGrid3D();

	if (pRet)
	{
		if (pRet->initWithNum(gridNum))
		{
			pRet->autorelease();
		}
		else
		{
			delete pRet;
			pRet = NULL;
		}
	}
	return pRet;
}
//--------------------------------------------------------
CRippleGrid3D::CRippleGrid3D()
	: m_pBufNext(NULL)
	, m_pBufLast(NULL)
	, m_pBuf(NULL)
{

}
//--------------------------------------------------------
CRippleGrid3D::~CRippleGrid3D(void)
{
	CC_SAFE_FREE(m_pBuf);
}
//--------------------------------------------------------
bool CRippleGrid3D::initWithNum(const CCSize &gridNum)
{
	if( CCGrid3D::initWithSize(gridNum) ) 
	{
		m_unWidthNum = gridNum.width+1;
		m_unHeightNum = gridNum.height+1;

		// 创建顶点缓冲
		unsigned int numOfPoints = m_unWidthNum * m_unHeightNum;
		m_pBuf = (float*)malloc(numOfPoints * sizeof(ccVertex3F));
		m_pBufLast = (ccVertex3F*)m_pVertices;
		m_pBufNext = (ccVertex3F*)m_pBuf;
		memcpy(m_pBufNext, m_pBufLast, numOfPoints * sizeof(ccVertex3F));

		return true;
	}
	return false;
}
//--------------------------------------------------------
void CRippleGrid3D::update(float dt)
{
	// 每帧CPU修改顶点缓冲值
	int index = 0;
	for (int y = 1; y<m_unHeightNum-1; y++) 
	{
		for (int x = 1; x<m_unWidthNum-1; x++) 
		{
			index = x*m_unHeightNum + y;

			float zu = m_pBufLast[index - 1].z;
			float zr = m_pBufLast[index + m_unHeightNum].z;
			float zd = m_pBufLast[index + 1].z;
			float zl = m_pBufLast[index - m_unHeightNum].z;
			float z = m_pBufLast[index].z;

			z = (zu+zr+zd+zl)/2-z;
			z -= z/64;
			m_pBufLast[index].z = z;
		}
	}
	m_pBufLast = m_pBufNext;
	m_pBufNext = (ccVertex3F*)m_pVertices;
	m_pVertices = m_pBufLast;
}
//--------------------------------------------------------
void CRippleGrid3D::DropStone(int x, int y, int stoneSize, int stoneWeight)
{
	// 创建波源
	if ( x > m_unWidthNum || y > m_unHeightNum || x < 0 || y < 0)
	{        
		return;        
	}

	int radius = MIN(MIN(MIN(x-1, m_unWidthNum-x-1), MIN(y-1,m_unHeightNum-y-1)), stoneSize-1);

	int radius2 = radius*radius;
	float weight = -stoneWeight;

	for (int posx = x - stoneSize; posx < x + stoneSize; ++posx)
	{        
		for (int posy = y - stoneSize; posy < y + stoneSize; ++posy)
		{            
			if ((posx - x) * (posx - x) + (posy - y) * (posy - y) < radius2)                
			{                
				m_pBufLast[(m_unHeightNum * posx + posy)].z = weight;
			}            
		}        
	}
}
//--------------------------------------------------------