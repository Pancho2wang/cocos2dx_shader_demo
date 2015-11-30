/*!
 * \author:	FreeKnight
 * \date:	2014-6-30
 * \comment:
 */
//--------------------------------------------------------
#include "SuckAction.h"
#include "Resource.h"
#include "Common/FKCommon.h"
//--------------------------------------------------------
CSuckAction* CSuckAction::create( CCSize p_tagSize, float p_fTime, CCPoint p_tagPos )
{
	CSuckAction* pRet = new CSuckAction();

	if (pRet)
	{
		if (pRet->initWithGrid( p_tagSize, p_fTime, p_tagPos ))
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
CSuckAction* CSuckAction::initWithGrid( CCSize p_tagSize, float p_fTime, CCPoint p_tagPos )
{
	do 
	{
		CC_BREAK_IF( !CCGrid3DAction::initWithDuration( p_fTime, p_tagSize ) );

		m_fAmplitudeRate = 1.0f;
		m_tagToPos = p_tagPos;
		return this;

	} while ( 0 );
	return NULL;
}
//--------------------------------------------------------
void CSuckAction::update(float time)
{
	int i, j;
	CCPoint target = m_tagToPos;
	int nWidth = getGrid()->getGridSize().width;
	int nHeight = getGrid()->getGridSize().height;
	for( i = 0; i < ( nWidth+1 ); i++ )
	{
		for( j = 0; j < ( nHeight+1 ); j++ )
		{
			ccVertex3F v = vertex( CCPoint( i, j ));

			float s = ccpDistance(ccp(v.x, v.y), target);
			float distx = abs(v.x - target.x);
			float disty = abs(v.y - target.y);

			if (s > 0.0f) 
			{
				s = s / (10/getDuration() * 8.0f);

				float valx = distx*time/s;
				if (valx > distx) valx = distx;
				if (valx < 0) valx = 0;
				float valy = disty*time/s;
				if (valy > disty) valy = disty;
				if (valy < 0) valy = 0;

				if (v.x < target.x)
					v.x += valx;
				if (v.x >= target.x)
					v.x -= valx;

				if (v.y < target.y)
					v.y += valy;
				if (v.y >= target.y)
					v.y -= valy;
			}
			setVertex( ccp(i,j), v );
		}
	}
}
//--------------------------------------------------------
// CSuckScene
//--------------------------------------------------------
CSuckScene::CSuckScene()
{
	CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	layer->setColor( ccGRAY );
	addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCLayer* pLayer = (CCLayer*)CCLayer::create();

	CCSprite* pPlayer = CCSprite::create( s_szCommonBackgroundImgPath );
	if( pPlayer )
	{
		pPlayer->setPosition( ccp(winSize.width / 2, winSize.height / 2 ) );
		pPlayer->setScale( 0.8f );
		pLayer->addChild( pPlayer );
		pPlayer->runAction( CCSequence::create(
			CCDelayTime::create( 1.0f ),
			CSuckAction::create( CCSizeMake( 100, 100 ), 10.0f, ccp(winSize.width / 2 - 100, winSize.height / 2 + 50) ),
			CCCallFuncN::create( this, callfuncN_selector(CSuckScene::RemoveSprite )),
			NULL ));
	}
	if( pLayer )
	{
		this->addChild( pLayer );
	}
}
//--------------------------------------------------------
void CSuckScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------
void CSuckScene::RemoveSprite( CCNode* p )
{
	if( p != NULL )
	{
		p->removeAllChildren();
		p->removeFromParentAndCleanup( true );
	}
}
//--------------------------------------------------------