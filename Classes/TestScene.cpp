/*!
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment:
 */
//--------------------------------------------------------
#include "TestScene.h"
#include "TestListControllerLayer.h"
#include "Common\StringConvert.h"
//--------------------------------------------------------
CTestScene::CTestScene( bool bPortrait /* = false */)
{
	CCScene::init();
}
//--------------------------------------------------------
void CTestScene::onEnter()
{
	CCScene::onEnter();
	CCLabelTTF* pLabel = CCLabelTTF::create(FKStringConvert::a2u("返回主界面").c_str(), "Arial", 36 );
	pLabel->setColor( ccBLUE );
	CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(pLabel, this, menu_selector(CTestScene::MainMenuCallback));
	CCMenu* pMenu = CCMenu::create(pMenuItem, NULL);
	pMenu->setPosition( CCPointZero );
	pMenuItem->setPosition( ccp( CVisibleRect::right().x - 100, CVisibleRect::bottom().y + 30) );
	addChild(pMenu, 1);
}
//--------------------------------------------------------
void CTestScene::MainMenuCallback(CCObject* pSender)
{
	CCScene* pScene = CCScene::create();
	CCLayer* pLayer = new CTestListControllerLayer();
	pLayer->autorelease();
	pScene->addChild(pLayer);
	CCDirector::sharedDirector()->replaceScene(pScene);
}
//--------------------------------------------------------