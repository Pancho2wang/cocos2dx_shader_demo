/*!
 * \author:	FreeKnight
 * \date:	2014-6-27
 * \comment:
 */
//--------------------------------------------------------
#include "WaterWave.h"
#include "Resource.h"
//--------------------------------------------------------
CWaterWaveScene::CWaterWaveScene()
{
	CCLayerColor *layer = CCLayerColor::create();
	layer->setColor( ccGRAY );
	addChild(layer);

	CCSprite* pBK = CCSprite::create( s_szCommonBackgroundImgPath );
	if( pBK )
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		pBK->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		layer->addChild( pBK );
	}

	CCActionInterval* pRippleAction = CCWaves3D::create(6, CCSize(16,16), 10, 15);  
	pBK->runAction(pRippleAction);  
}
//--------------------------------------------------------
void CWaterWaveScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------