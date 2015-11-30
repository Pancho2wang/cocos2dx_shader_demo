/*!
 * \author:	FreeKnight
 * \date:	2014-7-2
 * \comment:
 */
//--------------------------------------------------------
#include "Distort.h"
#include "Resource.h"
//-------------------------------------------------------
CDistortLayer::CDistortLayer()
{

}
//-------------------------------------------------------
CDistortLayer::~CDistortLayer()
{
	unscheduleUpdate();

	free( m_pSpring );
	free( m_pMass );
}
//-------------------------------------------------------
bool CDistortLayer::init()
{
	do{
		CC_BREAK_IF(!CCLayer::init());
		
		m_fMouseX = m_fMouseY = 0.0f;
		m_bIsShake = m_bIsTakeImage = false;
		m_pTexture2D = CCTextureCache::sharedTextureCache()->addImage( s_szCommonBackgroundImgPath );
		m_pMass = NULL;
		m_pSpring = NULL;
		m_nGrab = -1;
		memset( &m_usIndices, 0, GRID_SIZE_X * GRID_SIZE_Y * 6 * sizeof(GLushort) );

		// 自己的初始化
		MyInit();

		// 开启帧更新
		this->scheduleUpdate();
		// 开启Input消息
		this->setTouchEnabled(true);

		return true;
	}while(0);
	return false;
}
//--------------------------------------------------------
void CDistortLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
	cocos2d::CCPoint touchLocation = pTouch->getLocation();

	touchLocation = this->convertToNodeSpace(touchLocation);
	m_fMouseX = touchLocation.x;
	m_fMouseY = touchLocation.y;

	m_nGrab = Grab( touchLocation.x, touchLocation.y );

}
//--------------------------------------------------------
void CDistortLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
	cocos2d::CCPoint touchLocation = pTouch->getLocation();

	touchLocation = this->convertToNodeSpace(touchLocation);
	m_fMouseX = touchLocation.x;
	m_fMouseY = touchLocation.y;
}
//--------------------------------------------------------
void CDistortLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	m_nGrab = -1;
}
//--------------------------------------------------------
void CDistortLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}
//--------------------------------------------------------
void CDistortLayer::update(float dt)
{
	MyUpdatePos( m_fMouseX, m_fMouseY );
}
//--------------------------------------------------------
void CDistortLayer::draw()
{
	glDisableClientState(GL_COLOR_ARRAY);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4ub(224,224,244,200);

	MyRedraw();

	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glEnableClientState(GL_COLOR_ARRAY);
}
//--------------------------------------------------------
void CDistortLayer::MyInit()
{
	GLint width = m_pTexture2D->getContentSizeInPixels().width;
	GLint height = m_pTexture2D->getContentSizeInPixels().height;
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;

	if (m_pMass == NULL)
	{
		m_pMass = (MASS *) malloc(sizeof(MASS)*GRID_SIZE_X*GRID_SIZE_Y);
		if (m_pMass == NULL)
		{
			fprintf(stderr, "rubber: Can't allocate memory.\n");	
			exit(-1);
		}
	}

	k = 0;
	for (i = 0; i < GRID_SIZE_X; i++)
	{
		for (j = 0; j < GRID_SIZE_Y; j++)
		{
			m_pMass[k].nail = (i == 0 || j == 0 || i == GRID_SIZE_X - 1
				|| j == GRID_SIZE_Y - 1);
			m_pMass[k].x[0] = i/(GRID_SIZE_X - 1.0)*width;
			m_pMass[k].x[1] = j/(GRID_SIZE_Y - 1.0)*height;
			m_pMass[k].x[2] = -(CLIP_FAR - CLIP_NEAR)/2.0;

			m_pMass[k].v[0] = 0.0;
			m_pMass[k].v[1] = 0.0;
			m_pMass[k].v[2] = 0.0;

			m_pMass[k].t[0] = i/(GRID_SIZE_X - 1.0);
			m_pMass[k].t[1] = j/(GRID_SIZE_Y - 1.0);

			k++;
		}
	}

	if (m_pSpring == NULL)
	{
		m_nSpringCount = (GRID_SIZE_X - 1)*(GRID_SIZE_Y - 2)
			+ (GRID_SIZE_Y - 1)*(GRID_SIZE_X - 2);

		m_pSpring = (SPRING *) malloc(sizeof(SPRING)*m_nSpringCount);
		if (m_pSpring == NULL)
		{
			fprintf(stderr, "rubber: Can't allocate memory.\n");	
			exit(-1);
		}
	}

	k = 0;
	for (i = 1; i < GRID_SIZE_X - 1; i++)
	{
		for (j = 0; j < GRID_SIZE_Y - 1; j++)
		{
			m = GRID_SIZE_Y*i + j;
			m_pSpring[k].i = m;
			m_pSpring[k].j = m + 1;
			if( m_pSpring[k].j >=  GRID_SIZE_X * GRID_SIZE_Y )
			{
				CC_ASSERT( false );
			}
			m_pSpring[k].r = (height - 1.0)/(GRID_SIZE_Y - 1.0);
			k++;
		}
	}

	for (j = 1; j < GRID_SIZE_Y - 1; j++)
	{
		for (i = 0; i < GRID_SIZE_X - 1; i++)
		{
			m = GRID_SIZE_Y*i + j;
			m_pSpring[k].i = m;
			m_pSpring[k].j = m + GRID_SIZE_X;
			if( m_pSpring[k].j >=  GRID_SIZE_X * GRID_SIZE_Y )
			{
				CC_ASSERT( false );
			}
			m_pSpring[k].r = (width - 1.0)/(GRID_SIZE_X - 1.0);
			k++;
		}
	}

	//k = 0;
	for ( i = 0; i < GRID_SIZE_Y - 1; i++ )
	{
		for ( j = 0; j < GRID_SIZE_X - 1; j++ )
		{	
			k = i * GRID_SIZE_X + j;
			m_usIndices[k * 6 + 0] = ( i     ) * GRID_SIZE_X + j;
			m_usIndices[k * 6 + 1] = ( i + 1 ) * GRID_SIZE_X + j;
			m_usIndices[k * 6 + 2] = ( i + 1 ) * GRID_SIZE_X + j + 1;
			m_usIndices[k * 6 + 3] = ( i     ) * GRID_SIZE_X + j;
			m_usIndices[k * 6 + 4] = ( i + 1 ) * GRID_SIZE_X + j + 1;
			m_usIndices[k * 6 + 5] = ( i     ) * GRID_SIZE_X + j + 1;
		}
	}
}
//--------------------------------------------------------
void CDistortLayer::MyRedraw()
{
	int k = 0;
	int i = 0;
	int j = 0;
	if(m_pMass == NULL) 
	{
		return;
	}
	glBindTexture(GL_TEXTURE_2D, m_pTexture2D->getName());

	k = 0;
	for (i = 0; i < GRID_SIZE_X - 1; i++)
	{
		for (j = 0; j < GRID_SIZE_Y - 1; j++)
		{
			GLfloat vertices[]= {
				m_pMass[k].x[0],m_pMass[k].x[1],m_pMass[k].x[2], 
				m_pMass[k + 1].x[0],m_pMass[k + 1].x[1],m_pMass[k + 1].x[2],
				m_pMass[k + GRID_SIZE_Y + 1].x[0],m_pMass[k + GRID_SIZE_Y + 1].x[1],m_pMass[k + GRID_SIZE_Y + 1].x[2], 
				m_pMass[k + GRID_SIZE_Y].x[0],m_pMass[k + GRID_SIZE_Y].x[1],m_pMass[k + GRID_SIZE_Y].x[2]
			};
			GLfloat tex[]={
				m_pMass[k].t[0], m_pMass[k].t[1], 
				m_pMass[k + 1].t[0], m_pMass[k + 1].t[1],
				m_pMass[k + GRID_SIZE_Y + 1].t[0], m_pMass[k + GRID_SIZE_Y + 1].t[1],
				m_pMass[k + GRID_SIZE_Y].t[0], m_pMass[k + GRID_SIZE_Y].t[1]
			};

			glVertexPointer(3, GL_FLOAT, 0, vertices);
			glTexCoordPointer(2, GL_FLOAT, 0, tex);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			k++;
		}
		k++;
	}
}
//--------------------------------------------------------
void CDistortLayer::MyUpdatePos( int p_nX, int p_nY )
{
	int k = 0;
	float d[3];
	d[0] = d[1] = d[2] = 0.0f;
	int i = 0;
	int j = 0;
	float l = 0.0f;
	float a = 0.0f;

	/* calculate all the spring forces acting on the mass points */

	for (k = 0; k < m_nSpringCount; k++)
	{
		i = m_pSpring[k].i;
		j = m_pSpring[k].j;

		d[0] = m_pMass[i].x[0] - m_pMass[j].x[0];
		d[1] = m_pMass[i].x[1] - m_pMass[j].x[1];
		d[2] = m_pMass[i].x[2] - m_pMass[j].x[2];

		l = sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);

		if (l != 0.0)
		{
			d[0] /= l;
			d[1] /= l;
			d[2] /= l;

			a = l - m_pSpring[k].r;

			m_pMass[i].v[0] -= d[0]*a*SPRING_KS;
			m_pMass[i].v[1] -= d[1]*a*SPRING_KS;
			m_pMass[i].v[2] -= d[2]*a*SPRING_KS;

			m_pMass[j].v[0] += d[0]*a*SPRING_KS;
			m_pMass[j].v[1] += d[1]*a*SPRING_KS;
			m_pMass[j].v[2] += d[2]*a*SPRING_KS;
		}
	}

	/* update the state of the mass points */

	for (k = 0; k < GRID_SIZE_X*GRID_SIZE_Y; k++)
	{
		if (!m_pMass[k].nail)
		{
			m_pMass[k].x[0] += m_pMass[k].v[0];
			m_pMass[k].x[1] += m_pMass[k].v[1];
			m_pMass[k].x[2] += m_pMass[k].v[2];

			m_pMass[k].v[0] *= (1.0 - DRAG);
			m_pMass[k].v[1] *= (1.0 - DRAG);
			m_pMass[k].v[2] *= (1.0 - DRAG);

			if (m_pMass[k].x[2] > -CLIP_NEAR - 0.01)
				m_pMass[k].x[2] = -CLIP_NEAR - 0.01;
			if (m_pMass[k].x[2] < -CLIP_FAR + 0.01)
				m_pMass[k].x[2] = -CLIP_FAR + 0.01;
		}

		/* if a mass point is grabbed, attach it to the mouse */

		if (m_nGrab != -1 && !m_pMass[m_nGrab].nail)
		{
			m_pMass[m_nGrab].x[0] = p_nX;
			m_pMass[m_nGrab].x[1] = p_nY;
			m_pMass[m_nGrab].x[2] = -(CLIP_FAR - CLIP_NEAR)/4.0;
		}
	}
}
//--------------------------------------------------------
int CDistortLayer::Grab( int p_nX, int p_nY )
{
	float dx[2];
	dx[0] = dx[1] = 0.0f;
	float d = 0.0f;
	float min_d = 0.0f;
	float min_i = 0.0f;
	int i = 0;

	for (i = 0; i < GRID_SIZE_X*GRID_SIZE_Y; i++)
	{
		dx[0] = m_pMass[i].x[0] - p_nX;
		dx[1] = m_pMass[i].x[1] - p_nY;
		d = sqrt(dx[0]*dx[0] + dx[1]*dx[1]);
		if (i == 0 || d < min_d)
		{
			min_i = i;
			min_d = d;
		}
	}

	return min_i;
}
//--------------------------------------------------------
// CDistortScene
//--------------------------------------------------------
CDistortScene::CDistortScene()
{
	CDistortLayer *layer = CDistortLayer::create();
	this->addChild(layer);
}
//--------------------------------------------------------
void CDistortScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------