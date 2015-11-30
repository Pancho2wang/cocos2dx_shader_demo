/*!
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment:
 */
//--------------------------------------------------------
#include "TestListControllerLayer.h"
#include "GlobalMacro.h"
#include "Resource.h"
#include "TestScene.h"
#include "Common/FKCommon.h"
#include "Common/StringConvert.h"

#include "SubTestScene/Ripple/RippleScene.h"
#include "SubTestScene/RippleSprite/RippleSprite.h"
#include "SubTestScene/RenderTexture/RenderTexture.h"
#include "SubTestScene/ColorSprite/ColorSprite.h"
#include "SubTestScene/ShaderNode/ShaderNode.h"
#include "SubTestScene/WaterWave/WaterWave.h"
#include "SubTestScene/RemoveBKSprite/RemoveBKSprite.h"
#include "SubTestScene/Emboss/EmbossSprite.h"
#include "SubTestScene/SpotLight/SpotLight.h"
#include "SubTestScene/ActiveGrass/ActiveGrass.h"
#include "SubTestScene/ScratchOff/ScratchOff.h"
#include "SubTestScene/LaserLine/LaserLine.h"
#include "SubTestScene/SuckAction/SuckAction.h"
#include "SubTestScene/LabelShuffleTile/LabelShuffleTile.h"
#include "SubTestScene/BlurTexture/BlurTextureScene.h"
#include "SubTestScene/ImageProcess/ImageProcess.h"
#include "SubTestScene/Distort/Distort.h"
#include "SubTestScene/Wave1D/Wave1D.h"
//--------------------------------------------------------
static CCPoint s_tCurPos = CCPointZero;
//--------------------------------------------------------
static CTestScene* CreateTestScene( ENUM_TEST_SCENE_TYPE p_eType )
{
	// 清除全部缓存数据
	CCDirector::sharedDirector()->purgeCachedData();

	CTestScene* pScene = NULL;
	if( p_eType >= eTestSceneType_Max )
		return pScene;

	FKLog( "尝试创建SceneType: %d", static_cast<int>( p_eType ) );
	switch (p_eType)
	{
	case eTST_Ripple_UseGrid3D:
		pScene = new CRippleScene();
		break;
	case eTST_RippleSprite_UseShader:
		pScene = new CRippleSpriteScene();
		break;
	case eTST_RenderTexture:
		pScene = new CRenderTextureScene();
		break;
	case eTST_ColorSprite_UseShader:
		pScene = new CColorSpriteScene();
		break;
	case eTST_ShaderNode:
		pScene = new CShaderNodeScene();
		break;
	case eTST_Wave3DAction:
		pScene = new CWaterWaveScene();
		break;
	case eTST_RemoveBKSprite:
		pScene = new CRemoveBKSpriteScene();
		break;
	case eTST_EmbossSprite:
		pScene = new CEmbossSpriteScene();
		break;
	case eTST_SpotLight:
		pScene = new CSpotLightScene();
		break;
	case eTST_ActiveGrass:
		pScene = new CActiveGrassScene();
		break;
	case eTST_ScrartchBursh:
		pScene = new CScratchOffScene();
		break;
	case eTST_LaserLine:
		pScene = new CLaserLineScene();
		break;
	case eTST_SuckAction:
		pScene = new CSuckScene();
		break;
	case eTST_LabelShuffleTile:
		pScene = new CLabelShuffleTileScene();
		break;
	case eTST_BlueTexture:
		pScene = new CBlurTextureScene();
		break;
	case eTST_ImageProcess:
		pScene = new CImageProcessSpriteScene();
		break;
	case eTST_Wave1D:
		pScene = new CWave1DNodeScene();
		break;
	default:
		break;
	}

	return pScene;
}
//--------------------------------------------------------
CTestListControllerLayer::CTestListControllerLayer()
	: m_tBeginPos( CCPointZero )
{
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(s_szCloseBtnImagePath, s_szCloseBtnImagePath, this, menu_selector(CTestListControllerLayer::closeCallback) );
	CCMenu* pMenu =CCMenu::create(pCloseItem, NULL);

	pMenu->setPosition( CCPointZero );
	pCloseItem->setPosition(ccp( CVisibleRect::right().x - 50, CVisibleRect::top().y - 50));

	m_pItemMenu = CCMenu::create();
	for (int i = 0; i < static_cast<int>(eTestSceneType_Max); ++i)
	{
		CCLabelTTF* label = CCLabelTTF::create( FKStringConvert::a2u( g_aTestNames[i].c_str() ).c_str(), "Arial", 24);   
		CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(CTestListControllerLayer::menuCallback));

		m_pItemMenu->addChild(pMenuItem, i + 10000);
		pMenuItem->setPosition( ccp( CVisibleRect::center().x, (CVisibleRect::top().y - (i + 1) * TEST_MENU_LINE_SPACE) ));
	}

	m_pItemMenu->setContentSize(CCSizeMake(CVisibleRect::getVisibleRect().size.width, (static_cast<int>(eTestSceneType_Max) + 1) * (TEST_MENU_LINE_SPACE)));
	m_pItemMenu->setPosition(s_tCurPos);
	addChild(m_pItemMenu);

	setTouchEnabled(true);

	addChild(pMenu, 1);

	FKLog( "创建TestListControllerLayer成功" );
}
//--------------------------------------------------------
CTestListControllerLayer::~CTestListControllerLayer()
{

}
//--------------------------------------------------------
void CTestListControllerLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	m_tBeginPos = touch->getLocation();    
}
//--------------------------------------------------------
void CTestListControllerLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint touchLocation = touch->getLocation();    
	float nMoveY = touchLocation.y - m_tBeginPos.y;

	CCPoint curPos  = m_pItemMenu->getPosition();
	CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);

	if (nextPos.y < 0.0f)
	{
		m_pItemMenu->setPosition(CCPointZero);
		return;
	}

	if (nextPos.y > ((static_cast<int>(eTestSceneType_Max) + 1)* TEST_MENU_LINE_SPACE - CVisibleRect::getVisibleRect().size.height))
	{
		m_pItemMenu->setPosition(ccp(0, ((static_cast<int>(eTestSceneType_Max) + 1)* TEST_MENU_LINE_SPACE - CVisibleRect::getVisibleRect().size.height)));
		return;
	}

	m_pItemMenu->setPosition(nextPos);
	m_tBeginPos = touchLocation;
	s_tCurPos   = nextPos;
}
//--------------------------------------------------------
void CTestListControllerLayer::menuCallback(CCObject * pSender)
{
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int nIdx = pMenuItem->getZOrder() - 10000;

	// create the test scene and run it
	CTestScene* pScene = CreateTestScene(static_cast<ENUM_TEST_SCENE_TYPE>(nIdx));
	if (pScene)
	{
		pScene->Run();
		pScene->release();
	}
}
//--------------------------------------------------------
void CTestListControllerLayer::closeCallback(CCObject * pSender)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#else
		CCDirector::sharedDirector()->end();
	#endif
}
//--------------------------------------------------------