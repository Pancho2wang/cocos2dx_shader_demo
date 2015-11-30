/*!
 * \author:	FreeKnight
 * \date:	2014-6-30
 * \comment:
 */
//--------------------------------------------------------
#include "LabelShuffleTile.h"
#include "Resource.h"
//--------------------------------------------------------
bool CLabelShuffleTileLayer::init()
{
	do{
		CC_BREAK_IF(!CCLayer::init());

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		// 创建字体对象
		CCLabelBMFont* pLabel = CCLabelBMFont::create( "FreeKnightDemo!", s_szLabelShuffleTileFontFile );
		pLabel->setPosition( ccp( winSize.width / 2, winSize.height / 2 ));
		pLabel->setTag( 10001 );
		addChild( pLabel );

		return true;
	}while(0);
	return false;
}
//--------------------------------------------------------
void CLabelShuffleTileLayer::onEnter()
{
	CCLayer::onEnter();

	// 执行动作
	CCLabelBMFont* p = ( CCLabelBMFont* )getChildByTag( 10001 );
	if( p == NULL )
		return;

	int nCount = 0;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(p->getChildren(), pObj)
	{
		CCSprite* pTmp = ( CCSprite* )pObj;
		pTmp->setScale( 10.0f );
		pTmp->setOpacity( 0 );
		pTmp->setRotation( -10.0f );
		pTmp->runAction( CCSequence::create(
			CCDelayTime::create( nCount * 0.3f ),
			CCSpawn::create( CCScaleTo::create( 0.3f, 1.0f, 1.0f ),
			CCFadeIn::create( 0.3f ),
			CCRotateBy::create( 0.3f, 10.0f, 10.0f ),
			NULL ),
			NULL ));
		nCount++;
	}

	// 执行破碎动作
	p->runAction( CCSequence::create(
		CCDelayTime::create( 0.3f * (nCount + 1 )),
		CCSpawn::create(
		CCShuffleTiles::create( 4.0f, CCSize( 100, 100 ), 1.0f ),
		CCFadeOut::create( 2.0f ),
		NULL),
		CCCallFuncN::create( this, callfuncN_selector(CLabelShuffleTileLayer::RemoveSprite )),
		NULL));
}
//--------------------------------------------------------
void CLabelShuffleTileLayer::RemoveSprite( CCNode* p )
{
	if( p != NULL )
	{
		p->removeAllChildren();
		p->removeFromParentAndCleanup( true );
	}
}
//--------------------------------------------------------
// CLabelShuffleTileScene
//--------------------------------------------------------
CLabelShuffleTileScene::CLabelShuffleTileScene()
{
	CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	layer->setColor( ccGRAY );
	addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CLabelShuffleTileLayer* pLabelShuffleTileLayer = (CLabelShuffleTileLayer*)CLabelShuffleTileLayer::create();
	if( pLabelShuffleTileLayer )
	{
		this->addChild( pLabelShuffleTileLayer );
	}
}
//--------------------------------------------------------
void CLabelShuffleTileScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------