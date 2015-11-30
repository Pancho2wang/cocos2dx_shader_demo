/*!
 * \author:	FreeKnight
 * \date:	2014-6-28
 * \comment:
 */
//--------------------------------------------------------
#include "SpotLight.h"
#include "Resource.h"
#include "Common/FKCommon.h"
//--------------------------------------------------------
const static int SEGS = 45;
//--------------------------------------------------------
CSpotLight*	CSpotLight::InitWithRenderTexture( CCRenderTexture* pRenderTex,
									  float fRadius, ccColor4B tagRenderColor )
{
	do 
	{
		CC_BREAK_IF( !CCSprite::init() );

		m_pRenderTex = pRenderTex;
		m_fSpotLightRadius = fRadius;
		m_tagRenderColor = tagRenderColor;

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		setPosition( ccp( winSize.width / 2, winSize.height / 2 ) );

		m_pVertices = (GLfloat *)malloc( sizeof(GLfloat)*2*(SEGS));
		m_pCoordinates = (GLfloat *)malloc( sizeof(GLfloat)*2*(SEGS));
		m_pColors = (ccColor4B *)malloc( sizeof(ccColor4B)*(SEGS));

		// 开启帧循环
		schedule( schedule_selector( CSpotLight::Tick ));

		return this;
	} while ( 0 );
	return NULL;
}
//--------------------------------------------------------
CSpotLight* CSpotLight::Create( CCRenderTexture* pRenderTex,
	float fRadius, ccColor4B tagRenderColor )
{
	CSpotLight* pSpotLight = new CSpotLight();
	if(pSpotLight && pSpotLight->InitWithRenderTexture(pRenderTex, fRadius, tagRenderColor ))
	{
		pSpotLight->autorelease();
		return pSpotLight;
	}
	CC_SAFE_DELETE(pSpotLight);
	return NULL;
}
//--------------------------------------------------------
void CSpotLight::Tick(float dt)
{
	CC_NODE_DRAW_SETUP();

	memset(m_pVertices,0, sizeof(GLfloat)*2*(SEGS));
	memset(m_pCoordinates,0, sizeof(GLfloat)*2*(SEGS));
	m_pRenderTex->clear( m_tagRenderColor.r / 255.0f, 
		m_tagRenderColor.g / 255.0f, m_tagRenderColor.b / 255.0f, 
		m_tagRenderColor.a / 255.0f );

	m_pColors[0] = ccc4(0, 0, 0, 255);
	for (int i = 1; i < SEGS; i++)
	{
		m_pColors[i] = ccc4(0, 0, 0, 0);
	}

	const float coef = 2.0f * (float)M_PI/(SEGS-2) ;

	m_pVertices[0] = getPosition().x;
	m_pVertices[1] = getPosition().y;
	m_pCoordinates[0] = getPosition().x;
	m_pCoordinates[1] = (getContentSize().height-getPosition().y);
	for(int i=1;i<=SEGS;i++)
	{
		float rads = i*coef;
		float j = m_fSpotLightRadius * cosf(rads) + getPosition().x;
		float k = m_fSpotLightRadius * sinf(rads) + getPosition().y;

		m_pVertices[i*2] = j;
		m_pVertices[i*2+1] = k;

		m_pCoordinates[i*2] = (j);
		m_pCoordinates[i*2+1] = (getContentSize().height-k);
	}

	// Update the render texture
	m_pRenderTex->begin();

	ccGLBindTexture2D((GLuint)m_pRenderTex);
	ccGLBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );

	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_pVertices);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_pCoordinates);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, m_pColors);

	glDrawArrays(GL_TRIANGLE_FAN, 0, SEGS);
	ccGLBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

	m_pRenderTex->end();
}
//--------------------------------------------------------
void CSpotLight::RemoveSelf()
{
	FKLog("CSpotLight::RemoveSelf");
	schedule( schedule_selector( CSpotLight::SpreadOutTimer ));
}
//-------------------------------------------------------
void CSpotLight::SpreadOutTimer(float dt)
{
	m_fSpotLightRadius += 3.0f;

	if( m_fSpotLightRadius > 500.0f )
	{
		FKLog("实际删除灯光  %f", m_fSpotLightRadius );
		unscheduleAllSelectors();
		m_pRenderTex->removeFromParentAndCleanup( true );
		removeFromParentAndCleanup( true );

		//CC_SAFE_FREE(m_pVertices);
		//CC_SAFE_FREE(m_pCoordinates);
		//CC_SAFE_FREE(m_pColors);
	}
}
//-------------------------------------------------------
// CSpotLightLayer
//--------------------------------------------------------
bool CSpotLightLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCRenderTexture* pRenderLayer = CCRenderTexture::create( size.width, size.height );
	pRenderLayer->setPosition( ccp( size.width / 2, size.height / 2 ) );
	ccBlendFunc blendFunc = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
	pRenderLayer->getSprite()->setBlendFunc( blendFunc );
	this->addChild( pRenderLayer, 0 );

	CCSprite* pBackground = CCSprite::create( s_szCommonBackgroundImgPath );
	if( pBackground != NULL )
	{
		pBackground->setPosition( ccp(size.width / 2, size.height / 2 ));
		this->addChild( pBackground, -1 );
	}

	CSpotLight* pSpotLight = CSpotLight::Create( pRenderLayer, 150.0f, ccc4( 0, 0, 0, 200 ) );
	pSpotLight->setTag( 10001 );
	this->addChild( pSpotLight, 1 );

	// 开启Input消息
	this->setTouchEnabled(true);

	return true;
}
//--------------------------------------------------------
void CSpotLightLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CSpotLight* pSpotLight = (CSpotLight*)getChildByTag( 10001 );
	if( pSpotLight )
	{
		cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
		cocos2d::CCPoint touchLocation = pTouch->getLocation();
		pSpotLight->setPosition( touchLocation );
	}
}
//--------------------------------------------------------
void CSpotLightLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CSpotLight* pSpotLight = (CSpotLight*)getChildByTag( 10001 );
	if( pSpotLight )
	{
		cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
		cocos2d::CCPoint touchLocation = pTouch->getLocation();
		pSpotLight->setPosition( touchLocation );
	}
}
//--------------------------------------------------------
void CSpotLightLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	DelSpotLight();
}
//--------------------------------------------------------
void CSpotLightLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}
//--------------------------------------------------------
void CSpotLightLayer::DelSpotLight()
{
	CSpotLight* pSpotLight = (CSpotLight*)getChildByTag( 10001 );
	if( pSpotLight )
	{
		pSpotLight->RemoveSelf();
	}
}
//--------------------------------------------------------
// CSpotLightScene
//--------------------------------------------------------
CSpotLightScene::CSpotLightScene()
{
	CSpotLightLayer *layer = CSpotLightLayer::create();
	this->addChild(layer);
}
//--------------------------------------------------------
void CSpotLightScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------