/*!
 * \file:	RenderTexture.h
 * \author:	FreeKnight
 * \date:	2014-6-26
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "TestScene.h"
USING_NS_CC;
//--------------------------------------------------------
// 每秒钟纹理移动像素数
const static float MOVE_PIXELS_PER_SECOND = 100.0f;
// 渲染图片大小
const static int RENDER_TEX_WIDTH	= 512;
const static int RENDER_TEX_HEIGHT	= 512;
//--------------------------------------------------------
class CRenderTextureLayer : public CCLayer
{
public:
	CRenderTextureLayer();
public:
	virtual bool init();
	virtual void update(float dt);
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
public:
	CREATE_FUNC(CRenderTextureLayer);
public:
	CCSprite*	Create();
private:
	ccColor4F	RandomOneBrightColor();
	void		AddStripes( CCPoint* vertices, ccColor4F* colors, int nStripes,
		ccColor4F c1, ccColor4F c2 );
	void		AddGradient( CCPoint* vertices, ccColor4F* colors );
	void		AddTopLight( CCPoint* vertices, ccColor4F* colors );
	void		AddNoiseBlend( const char* p_szNoisePic );
	void		MoveTexture( float p_fDisOffset );
private:
	CCSprite*	m_pRenderTexture;
	int			m_nStep;		// 对特效的开启，关闭限制值
};
//--------------------------------------------------------
class CRenderTextureScene : public CTestScene
{
public:
	CRenderTextureScene();
public:
	void Run();
};
//--------------------------------------------------------