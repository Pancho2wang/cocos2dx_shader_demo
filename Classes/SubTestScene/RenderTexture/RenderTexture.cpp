/*!
 * \author:	FreeKnight
 * \date:	2014-6-26
 * \comment:
 */
//--------------------------------------------------------
#include "RenderTexture.h"
#include "Resource.h"
#include "Common/FKCommon.h"
//--------------------------------------------------------
// 当前特效个数
static const unsigned int CUR_EFFECT_NUM = 4;
//--------------------------------------------------------
CCSprite * CRenderTextureLayer::Create()
{
	ccColor4F c1 = RandomOneBrightColor();
	ccColor4F c2 = RandomOneBrightColor();
	int nStripes = ((rand() % 4) + 1) * 2;

	// 第一步：创建 RenderTexture 对象
	CCRenderTexture *rt = CCRenderTexture::create(RENDER_TEX_WIDTH, RENDER_TEX_HEIGHT);

	// 第二步：调用 Begin 函数
	rt->beginWithClear(c1.r, c1.g, c1.b, c1.a);

	// 第三步：开始向纹理中绘制
	this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
	CC_NODE_DRAW_SETUP();

	// 创建顶点和顶点色
	CCPoint *vertices = new CCPoint[nStripes * 6];
	ccColor4F *colors = new ccColor4F[nStripes * 6];

	int nBoolOpenEffect = m_nStep % (CUR_EFFECT_NUM + 1);

	switch ( nBoolOpenEffect )
	{
	case 0:
		// 特效全部开启
		AddStripes( vertices, colors, nStripes, c1, c2 );
		AddGradient( vertices, colors );
		AddTopLight( vertices, colors );
		AddNoiseBlend( s_szRenderTexNoiseSprite );
		break;
	case 1:
		// 增加条纹
		AddStripes( vertices, colors, nStripes, c1, c2 );
		break;
	case 2:
		// 颜色渐变
		AddGradient( vertices, colors );
		break;
	case 3:
		// 增加顶部光照
		AddTopLight( vertices, colors );
		break;
	case 4:
		// 混合噪声图
		AddNoiseBlend( s_szRenderTexNoiseSprite );
		break;
	default:
		break;
	}

	// 释放顶点和定点色
	CC_SAFE_DELETE_ARRAY(vertices);
	CC_SAFE_DELETE_ARRAY(colors);

	// 第四步：调用 end 函数
	rt->end();

	// 下一次修改特效显示
	m_nStep++;

	// 第五步：通过纹理创建精灵
	return CCSprite::createWithTexture(rt->getSprite()->getTexture());
}
//--------------------------------------------------------
ccColor4F CRenderTextureLayer::RandomOneBrightColor()
{
	while (true)
	{
		float requiredBrightness = 192;
		ccColor4B randomColor = ccc4(rand() % 255, rand() % 255, rand() % 255, 255);
		if (randomColor.r > requiredBrightness || 
			randomColor.g > requiredBrightness ||
			randomColor.b > requiredBrightness)
		{
			return ccc4FFromccc4B(randomColor);
		}
	}
}
//--------------------------------------------------------
void CRenderTextureLayer::AddStripes( CCPoint* vertices, ccColor4F* colors,
									 int nStripes, ccColor4F c1, ccColor4F c2  )
{
	int nVertices = 0;
	float x1 = -RENDER_TEX_HEIGHT;
	float x2 = 0.0f;
	float y1 = RENDER_TEX_HEIGHT;
	float y2 = 0.0f;
	float dx = RENDER_TEX_WIDTH / nStripes * 2;
	float fStripeWidth = dx / 2;

	for (int i = 0; i < nStripes; ++i)
	{
		x2  = x1 + RENDER_TEX_HEIGHT;

		vertices[nVertices] = ccp(x1, y1);
		colors[nVertices++] = ccc4f(c2.r, c2.g, c2.b, c2.a);

		vertices[nVertices] = ccp(x1 + fStripeWidth, y1);
		colors[nVertices++] = ccc4f(c2.r, c2.g, c2.b, c2.a);

		vertices[nVertices] = ccp(x2, y2);
		colors[nVertices++] = ccc4f(c2.r, c2.g, c2.b, c2.a);

		vertices[nVertices] = vertices[nVertices - 2];
		colors[nVertices++] = ccc4f(c2.r, c2.g, c2.b, c2.a);

		vertices[nVertices] = vertices[nVertices - 2];
		colors[nVertices++] = ccc4f(c2.r, c2.g, c2.b, c2.a);

		vertices[nVertices] = ccp(x2 + fStripeWidth, y2);
		colors[nVertices++] = ccc4f(c2.r, c2.g, c2.b, c2.a);
		x1 += dx;
	}

	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)nVertices);

	FKLog("开启条纹显示");
}
//--------------------------------------------------------
void CRenderTextureLayer::AddGradient( CCPoint* vertices, ccColor4F* colors )
{
	float gradientAlpha = 1.2f;
	int nVertices = 0;

	vertices[nVertices] = ccp(0, 0);
	colors[nVertices++] = ccc4f(0, 0, 0, 0);

	vertices[nVertices] = ccp(RENDER_TEX_WIDTH, 0);
	colors[nVertices++] = ccc4f(0, 0, 0, 0);

	vertices[nVertices] = ccp(0, RENDER_TEX_HEIGHT);
	colors[nVertices++] = ccc4f(0, 0, 0, gradientAlpha);

	vertices[nVertices] = ccp(RENDER_TEX_WIDTH, RENDER_TEX_HEIGHT);
	colors[nVertices++] = ccc4f(0, 0, 0, gradientAlpha);

	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);

	FKLog("开启渐变显示");
}
//--------------------------------------------------------
void CRenderTextureLayer::AddTopLight( CCPoint* vertices, ccColor4F* colors )
{
	int nVertices = 0;
	float borderHeight = RENDER_TEX_HEIGHT / 16;
	float borderAlpha = 0.7f;
	nVertices = 0;

	vertices[nVertices] = ccp(0, 0);
	colors[nVertices++] = ccc4f(1, 1, 1, borderAlpha);

	vertices[nVertices] = ccp(RENDER_TEX_WIDTH, 0);
	colors[nVertices++] = ccc4f(1, 1, 1, borderAlpha);

	vertices[nVertices] = ccp(0, borderHeight);
	colors[nVertices++] = ccc4f(0, 0, 0, 0);

	vertices[nVertices] = ccp(RENDER_TEX_WIDTH, borderHeight);
	colors[nVertices++] = ccc4f(0, 0, 0, 0);

	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);

	FKLog("开启顶部光照显示");
}
//--------------------------------------------------------
void CRenderTextureLayer::AddNoiseBlend( const char* p_szNoisePic )
{
	CCSprite *noise = CCSprite::create( p_szNoisePic );
	ccBlendFunc blendFunc = {GL_DST_COLOR, GL_ZERO};
	noise->setBlendFunc(blendFunc);
	noise->setPosition(ccp(RENDER_TEX_WIDTH / 2, RENDER_TEX_HEIGHT / 2));
	noise->visit();

	FKLog("开启噪音图混合");
}
//--------------------------------------------------------
void CRenderTextureLayer::MoveTexture( float p_fDisOffset )
{
	CCSize textureSize = m_pRenderTexture->getTextureRect().size;
	m_pRenderTexture->setTextureRect(CCRectMake(p_fDisOffset, 0, textureSize.width, textureSize.height));
}
//--------------------------------------------------------
bool CRenderTextureLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());

		m_nStep = 0;

		// 开启输入消息检查
		setTouchEnabled(true);
		// 开启帧更新
		scheduleUpdate();

		// 初始创建一个渲染纹理，并使用该纹理
		m_pRenderTexture = Create();
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		m_pRenderTexture->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
		m_pRenderTexture->getTexture()->setTexParameters(&tp);
		this->addChild(m_pRenderTexture);

		bRet = true;
	} while (0);

	return bRet;
}
//--------------------------------------------------------
CRenderTextureLayer::CRenderTextureLayer()
{
	m_pRenderTexture = NULL;
}
//--------------------------------------------------------
void CRenderTextureLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	// 释放上一纹理
	if (m_pRenderTexture)
	{
		m_pRenderTexture->removeFromParentAndCleanup(true);
	}

	// 创建新纹理
	m_pRenderTexture = Create();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	m_pRenderTexture->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	m_pRenderTexture->getTexture()->setTexParameters(&tp);

	// 使用新纹理
	this->addChild(m_pRenderTexture);
}
//--------------------------------------------------------
void CRenderTextureLayer::update(float dt)
{
	// 帧更新纹理偏移量
	static float fOffsetLen = 0.0f;
	fOffsetLen += MOVE_PIXELS_PER_SECOND * dt;
	MoveTexture( fOffsetLen );
}
//--------------------------------------------------------
// CRenderTextureScene
//--------------------------------------------------------
CRenderTextureScene::CRenderTextureScene()
{
	CRenderTextureLayer *layer = CRenderTextureLayer::create();
	this->addChild(layer);
}
//--------------------------------------------------------
void CRenderTextureScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------