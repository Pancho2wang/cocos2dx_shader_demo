/*!
 * \author:	FreeKnight
 * \date:	2014-7-2
 * \comment:
 */
//--------------------------------------------------------
#include "Wave1D.h"
#include "Common/FKCommon.h"
#include "Common/StringConvert.h"
//--------------------------------------------------------
#define VECTEX_NUM 64
//--------------------------------------------------------
CWave1DNode::CWave1DNode()
{

}
//--------------------------------------------------------
CWave1DNode::~CWave1DNode()
{
	CC_SAFE_FREE( m_pH1 );
	CC_SAFE_FREE( m_pH2 );
}
//--------------------------------------------------------
CWave1DNode* CWave1DNode::create( CCRect p_Bounds, int p_nCount, float p_fDamping, float p_fDiffuse )
{
	CWave1DNode* pWave1DNode = new CWave1DNode;  
	if (pWave1DNode && pWave1DNode->initWithBounds(p_Bounds, p_nCount, p_fDamping, p_fDiffuse))
	{  
		pWave1DNode->autorelease();  
		return pWave1DNode;  
	}	
	CC_SAFE_RELEASE(pWave1DNode);  
	return NULL;  
}
//-------------------------------------------------------
CWave1DNode* CWave1DNode::initWithBounds( CCRect p_Bounds, int p_nCount, float p_fDamping, float p_fDiffuse )
{
	CCNode::init();

	m_Bounds = p_Bounds;
	m_nCount = p_nCount;
	m_fDamping = p_fDamping;
	m_fDiffusion = p_fDiffuse;

	m_pH1 = (float*)calloc( m_nCount, sizeof( float ) );
	m_pH2 = (float*)calloc( m_nCount, sizeof( float ) );

	return this;
}
//--------------------------------------------------------
// 牵引运动
// 参考：http://en.wikipedia.org/wiki/Verlet_integration
void CWave1DNode::Verlet()
{
	for(int i=0; i<m_nCount; i++)
	{
		m_pH1[i] = 2.0*m_pH2[i] - m_pH1[i];
	}
	float *temp = m_pH2;
	m_pH2 = m_pH1;
	m_pH1 = temp;
}
//--------------------------------------------------------
float CWave1DNode::Diffuse(float diff, float damp, float prev, float curr, float next)
{
	return (curr*diff + ((prev + next)*0.5f)*(1.0f - diff))*damp;
}
//--------------------------------------------------------
void CWave1DNode::Diffuse()
{
	float prev = m_pH2[0];
	float curr = m_pH2[0];
	float next = m_pH2[1];

	m_pH2[0] = Diffuse(m_fDiffusion, m_fDamping, prev, curr, next);

	for(int i=1; i<(m_nCount - 1); ++i)
	{
		prev = curr;
		curr = next;
		next = m_pH2[i + 1];

		m_pH2[i] = Diffuse(m_fDiffusion, m_fDamping, prev, curr, next);
	}

	prev = curr;
	curr = next;
	m_pH2[m_nCount - 1] = Diffuse(m_fDiffusion, m_fDamping, prev, curr, next);
}
//-------------------------------------------------------
float CWave1DNode::Dx()
{
	return m_Bounds.size.width/(GLfloat)( m_nCount - 1 );
}
//-------------------------------------------------------
void CWave1DNode::MakeSplashAt( float p_fX )
{
	// Changing the values of heightfield in h2 will make the waves move.
	// Here I only change one column, but you get the idea.
	// Change a bunch of the heights using a nice smoothing function for a better effect.
	FKLog( "落石X坐标位置：%f", p_fX );
	int index = MAX(0, MIN((int)(p_fX/Dx()), m_nCount - 1));
	m_pH2[index] += CCRANDOM_MINUS1_1()*20.0;
}
//-------------------------------------------------------
void CWave1DNode::draw()
{
	// It would be better to run these on a fixed timestep.
	// As an GFX only effect it doesn't really matter though.
	Verlet();
	Diffuse();

	// 计算高度
	GLfloat dx = Dx();
	GLfloat top = m_Bounds.size.height;

	// Build a vertex array and render it.
	ccVertex2F vects[VECTEX_NUM * 2];
	for(int i=0; i<m_nCount; i++)
	{
		GLfloat x = i*dx;
		// 底部顶点
		vects[2*i + 0] = vertex2(x, 0);
		// top + m_pH2[i] 为水深+对应的波浪起伏
		vects[2*i + 1] = vertex2(x, top + m_pH2[i]);
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_TEXTURE_2D);

	GLfloat r = 105.0f/255.0f;
	GLfloat g = 193.0f/255.0f;
	GLfloat b = 212.0f/255.0f;
	GLfloat a = 0.6f;
	glColor4f(r*a, g*a, b*a, a);

	glVertexPointer(2, GL_FLOAT, 0, vects);

	kmGLPushMatrix(); 
	{
		glScalef(CC_CONTENT_SCALE_FACTOR(), CC_CONTENT_SCALE_FACTOR(), 1.0);
		glTranslatef(m_Bounds.origin.x, m_Bounds.origin.y, 0.0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_nCount*2);
	}
	kmGLPopMatrix();

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//-------------------------------------------------------
// CWave1DNodeLayer
//-------------------------------------------------------
bool CWave1DNodeLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CWave1DNode* pNode = CWave1DNode::create( CCRectMake(0.0,0.0, winSize.width, winSize.height / 2),
		VECTEX_NUM, 0.99f, 0.99f );
	pNode->setTag( 10001 );
	pNode->setPosition( ccp(winSize.width /2, winSize.height / 2));
	addChild( pNode );

	// 开启Input消息
	this->setTouchEnabled(true);

	return true;
}
//-------------------------------------------------------
void CWave1DNodeLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CWave1DNode* pNode = (CWave1DNode*)getChildByTag( 10001 );
	if( pNode )
	{
		cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
		cocos2d::CCPoint touchLocation = pTouch->getLocation();
		pNode->MakeSplashAt( touchLocation.x );
	}
}
//-------------------------------------------------------
void CWave1DNodeLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

}
//-------------------------------------------------------
void CWave1DNodeLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

}
//-------------------------------------------------------
void CWave1DNodeLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}
//-------------------------------------------------------
// CWave1DNodeScene
//-------------------------------------------------------
CWave1DNodeScene::CWave1DNodeScene()
{
	//CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	//layer->setColor( ccWHITE );
	//layer->setTag( 9999 );
	//addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CWave1DNodeLayer* pWaveLayer  = CWave1DNodeLayer::create();
	this->addChild( pWaveLayer );
}
//-------------------------------------------------------
void CWave1DNodeScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//-------------------------------------------------------