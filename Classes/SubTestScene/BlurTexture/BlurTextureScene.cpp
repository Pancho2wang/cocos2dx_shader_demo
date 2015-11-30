/*!
 * \author:	FreeKnight
 * \date:	2014-6-30
 * \comment:
 */
//--------------------------------------------------------
#include "BlurTextureScene.h"
#include "Resource.h"
#include "Common/StringConvert.h"
//---------------------------------------------------------
// CFullFilterLayer
//---------------------------------------------------------
bool CFullFilterLayer::init()
{
	do{
		CC_BREAK_IF(!CCLayer::init());

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		// 必须使用RGBA8888格式的图片
		CCTexture2D::setDefaultAlphaPixelFormat( kCCTexture2DPixelFormat_RGBA8888 );

		// 创建 Mutable 纹理
		CCTexture2DMutable* pMutableTex = new CCTexture2DMutable();
		CCImage* pImage = new CCImage();
		if( pImage && pImage->initWithImageFile( s_szBlurTextureImg ))
		{
			pImage->autorelease();
		}
		if( pMutableTex && pMutableTex->initWithData( pImage->getData(), 
			kCCTexture2DPixelFormat_RGBA8888, pImage->getWidth(), pImage->getHeight(),
			CCSizeMake(pImage->getWidth(), pImage->getHeight() ) ) )
		{
			pMutableTex->autorelease();
		}
		else
		{
			return false;
		}
		// 创建 NoMutable 纹理
		CCTexture2D* pNoMutableTex = pMutableTex->copyMutable( false );
		pNoMutableTex->autorelease();

		// 给 Mutable 纹理混合模糊
		AWTextureFilter Filter;
		Filter.blur( pMutableTex, 8 );

		// 显示 Mutable 纹理 和 NoMutable 纹理
		CCSprite* pSprite = CCSprite::createWithTexture( pNoMutableTex );
		pSprite->setPosition( ccp( winSize.width / 2 - pSprite->getContentSize().width / 2 - 30,
			winSize.height / 2 ) );
		CCSprite* pBlurSprite = CCSprite::createWithTexture( pMutableTex );
		pBlurSprite->setPosition( ccp( winSize.width / 2 + pSprite->getContentSize().width / 2 + 30,
			winSize.height / 2 ) );

		this->addChild( pSprite );
		this->addChild( pBlurSprite );

		CCLabelTTF* pInfo = CCLabelTTF::create(FKStringConvert::a2u("原图").c_str(), "Arial", 36 );
		pInfo->setPosition( pSprite->getPosition() + ccp(50, 50));
		pInfo->setColor( ccRED );
		this->addChild(pInfo);

		CCLabelTTF* pInfo2 = CCLabelTTF::create(FKStringConvert::a2u("全图模糊").c_str(), "Arial", 36 );
		pInfo2->setPosition( pBlurSprite->getPosition() + ccp(50, 50));
		pInfo2->setColor( ccRED );
		this->addChild(pInfo2);

		return true;
	}while(0);
	return false;
}
//---------------------------------------------------------
// CPartialFilterLayer
//---------------------------------------------------------
bool CPartialFilterLayer::init()
{
	do{
		CC_BREAK_IF(!CCLayer::init());

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		// 必须使用RGBA8888格式的图片
		CCTexture2D::setDefaultAlphaPixelFormat( kCCTexture2DPixelFormat_RGBA8888 );

		// 创建 Mutable 纹理
		CCTexture2DMutable* pMutableTex = new CCTexture2DMutable();
		CCImage* pImage = new CCImage();
		if( pImage && pImage->initWithImageFile( s_szBlurTextureImg ))
		{
			pImage->autorelease();
		}
		if( pMutableTex && pMutableTex->initWithData( pImage->getData(), 
			kCCTexture2DPixelFormat_RGBA8888, pImage->getWidth(), pImage->getHeight(),
			CCSizeMake(pImage->getWidth(), pImage->getHeight() ) ) )
		{
			pMutableTex->autorelease();
		}
		else
		{
			return false;
		}

		// 开始部分模糊
		AWTextureFilter Filter;
		Filter.blur( pMutableTex, 8, CCRectMake( 50, 50, 120, 80 ));

		// 显示 Mutable 纹理
		CCSprite* pBlurSprite = CCSprite::createWithTexture( pMutableTex );
		pBlurSprite->setPosition( ccp( winSize.width / 2 + pBlurSprite->getContentSize().width / 2 + 30,
			winSize.height / 2 + pBlurSprite->getContentSize().height + 30 ) );

		this->addChild( pBlurSprite );

		CCLabelTTF* pInfo = CCLabelTTF::create(FKStringConvert::a2u("局部模糊").c_str(), "Arial", 36 );
		pInfo->setPosition( pBlurSprite->getPosition() + ccp(50, 50));
		pInfo->setColor( ccRED );
		this->addChild(pInfo);

		return true;
	}while(0);
	return false;
}
//---------------------------------------------------------
// CTextShadow
//---------------------------------------------------------
bool CTextShadow::init()
{
	do{
		CC_BREAK_IF(!CCLayer::init());

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		CCSprite* pBlurSprite = CCSprite::create( s_szBlurTextureImg );

		CCSprite* pLable = CCSprite::create();
		pLable->setPosition( ccp( winSize.width / 2 + pBlurSprite->getContentSize().width / 2 + 150,
			winSize.height / 2 - pBlurSprite->getContentSize().height - 30 ));
		pLable->setTag( 10001 );
		CCSprite* pShadowLable = CCSprite::create();
		pShadowLable->setPosition( ccp( winSize.width / 2 + pBlurSprite->getContentSize().width / 2 + 150 + 1,
			winSize.height / 2 - pBlurSprite->getContentSize().height - 30 + 1 ));
		pShadowLable->setColor( ccBLACK );
		pShadowLable->setTag( 10002 );

		this->addChild( pShadowLable, 1 );
		this->addChild( pLable, 2 );

		// 对比图
		CCSprite* pLableNormal = CCSprite::create();
		pLableNormal->setPosition( ccp( winSize.width / 2 + pBlurSprite->getContentSize().width / 2 - 150,
			winSize.height / 2 - pBlurSprite->getContentSize().height - 30 ));
		pLableNormal->setTag( 10003 );
		CCSprite* pShadowLableNormal = CCSprite::create();
		pShadowLableNormal->setPosition( ccp( winSize.width / 2 + pBlurSprite->getContentSize().width / 2 - 150 + 1,
			winSize.height / 2 - pBlurSprite->getContentSize().height - 30 + 1 ));
		pShadowLableNormal->setColor( ccBLACK );
		pShadowLableNormal->setTag( 10004 );

		this->addChild( pShadowLableNormal, 1 );
		this->addChild( pLableNormal, 2 );

		CCLabelTTF* pInfo = CCLabelTTF::create(FKStringConvert::a2u("双次绘制描底效果").c_str(), "Arial", 24 );
		pInfo->setPosition( pLableNormal->getPosition() + ccp(0, 50));
		pInfo->setColor( ccRED );
		this->addChild(pInfo);

		CCLabelTTF* pInfo2 = CCLabelTTF::create(FKStringConvert::a2u("描底模糊效果").c_str(), "Arial", 24 );
		pInfo2->setPosition( pLable->getPosition() + ccp(0, 50));
		pInfo2->setColor( ccRED );
		this->addChild(pInfo2);

		m_nNum = 0;

		// 开启帧循环
		this->scheduleUpdate();

		return true;
	}while(0);
	return false;
}
//---------------------------------------------------------
void CTextShadow::update( float delta )
{
	setText( CCString::createWithFormat("Num = %d", m_nNum ));
	m_nNum++;
}
//---------------------------------------------------------
void CTextShadow::setText( CCString* p_Text )
{
	//Create mutable texture
	CCTexture2DMutable *shadowTexture = new CCTexture2DMutable();
	if( shadowTexture && shadowTexture->initWithString( p_Text->m_sString.c_str(), "Arial", 48 ))
	{
		shadowTexture->autorelease();
	}

	// 普通纹理
	CCTexture2D *labelTexture = shadowTexture->copyMutable( false );
	labelTexture->autorelease();


	// 基础图 的清晰前景
	CCSprite* pLabelNormal = (CCSprite*)getChildByTag( 10003 );
	if( pLabelNormal )
	{
		pLabelNormal->setTexture( labelTexture );
		pLabelNormal->setTextureRect( CCRectMake(0, 0, 
			shadowTexture->getContentSize().width, shadowTexture->getContentSize().height));
	}
	// 不模糊阴影
	CCSprite* pShadowLabelNormal = (CCSprite*)getChildByTag( 10004 );
	if( pShadowLabelNormal )
	{
		pShadowLabelNormal->setTexture( labelTexture );
		pShadowLabelNormal->setTextureRect( CCRectMake(0, 0, 
			shadowTexture->getContentSize().width, shadowTexture->getContentSize().height));
	}
	
	// 模糊图 的 清晰前景
	CCSprite* pLabel = (CCSprite*)getChildByTag( 10001 );
	if( pLabel )
	{
		pLabel->setTexture( labelTexture );
		pLabel->setTextureRect( CCRectMake(0, 0, 
			shadowTexture->getContentSize().width, shadowTexture->getContentSize().height));
	}
	// 开始模糊
	AWTextureFilter Filter;
	Filter.blur( shadowTexture, 8 );

	CCSprite* pShadowLabel = (CCSprite*)getChildByTag( 10002 );
	if( pShadowLabel )
	{
		pShadowLabel->setTexture( shadowTexture );
		pShadowLabel->setTextureRect( CCRectMake(0, 0, 
			shadowTexture->getContentSize().width, shadowTexture->getContentSize().height));
	}
}
//---------------------------------------------------------
// CBlurTextureLayer
//---------------------------------------------------------
bool CBlurTextureLayer::init()
{
	do{
		CC_BREAK_IF(!CCLayer::init());

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		CCLayer* P1 = CFullFilterLayer::create();
		this->addChild( P1 );

		CCLayer* P2 = CPartialFilterLayer::create();
		this->addChild( P2 );

		CCLayer* P3 = CTextShadow::create();
		this->addChild( P3 );

		return true;
	}while(0);
	return false;
}
//--------------------------------------------------------
// CBlurTextureScene
//--------------------------------------------------------
CBlurTextureScene::CBlurTextureScene()
{
	CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	layer->setColor( ccGRAY );
	addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CBlurTextureLayer* pBlurTexLayer = (CBlurTextureLayer*)CBlurTextureLayer::create();
	if( pBlurTexLayer )
	{
		this->addChild( pBlurTexLayer );
	}
}
//--------------------------------------------------------
void CBlurTextureScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------