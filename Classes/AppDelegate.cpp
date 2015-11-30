/*!
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment: cocos2dx version is 2.2.3
 */
//--------------------------------------------------------
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"

#include "TestListControllerLayer.h"
#include "Common/FKCommon.h"
//--------------------------------------------------------
USING_NS_CC;
using namespace CocosDenshion;
//--------------------------------------------------------
AppDelegate::AppDelegate() {

}
//--------------------------------------------------------
AppDelegate::~AppDelegate() 
{
	cocos2d::extension::CCArmatureDataManager::purge();
	cocos2d::extension::DictionaryHelper::purgeDictionaryHelper();
}
//--------------------------------------------------------
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
    // turn on display FPS
    pDirector->setDisplayStats(true);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	// create FK windows console
	CreateWinConsole();

    // create a scene. it's an autorelease object
    CCScene *pScene = CCScene::create();
	CCLayer* pLayer = new CTestListControllerLayer();
	pLayer->autorelease();
	pScene->addChild( pLayer );
    // run
    pDirector->runWithScene(pScene);

    return true;
}
//--------------------------------------------------------
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() 
{
	CCDirector::sharedDirector()->stopAnimation();
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}
//--------------------------------------------------------
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	CCDirector::sharedDirector()->startAnimation();
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
//--------------------------------------------------------