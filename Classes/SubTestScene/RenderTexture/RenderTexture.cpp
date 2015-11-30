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
// ��ǰ��Ч����
static const unsigned int CUR_EFFECT_NUM = 4;
//--------------------------------------------------------
CCSprite * CRenderTextureLayer::Create()
{
	ccColor4F c1 = RandomOneBrightColor();
	ccColor4F c2 = RandomOneBrightColor();
	int nStripes = ((rand() % 4) + 1) * 2;

	// ��һ�������� RenderTexture ����
	CCRenderTexture *rt = CCRenderTexture::create(RENDER_TEX_WIDTH, RENDER_TEX_HEIGHT);

	// �ڶ��������� Begin ����
	rt->beginWithClear(c1.r, c1.g, c1.b, c1.a);

	// ����������ʼ�������л���
	this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
	CC_NODE_DRAW_SETUP();

	// ��������Ͷ���ɫ
	CCPoint *vertices = new CCPoint[nStripes * 6];
	ccColor4F *colors = new ccColor4F[nStripes * 6];

	int nBoolOpenEffect = m_nStep % (CUR_EFFECT_NUM + 1);

	switch ( nBoolOpenEffect )
	{
	case 0:
		// ��Чȫ������
		AddStripes( vertices, colors, nStripes, c1, c2 );
		AddGradient( vertices, colors );
		AddTopLight( vertices, colors );
		AddNoiseBlend( s_szRenderTexNoiseSprite );
		break;
	case 1:
		// ��������
		AddStripes( vertices, colors, nStripes, c1, c2 );
		break;
	case 2:
		// ��ɫ����
		AddGradient( vertices, colors );
		break;
	case 3:
		// ���Ӷ�������
		AddTopLight( vertices, colors );
		break;
	case 4:
		// �������ͼ
		AddNoiseBlend( s_szRenderTexNoiseSprite );
		break;
	default:
		break;
	}

	// �ͷŶ���Ͷ���ɫ
	CC_SAFE_DELETE_ARRAY(vertices);
	CC_SAFE_DELETE_ARRAY(colors);

	// ���Ĳ������� end ����
	rt->end();

	// ��һ���޸���Ч��ʾ
	m_nStep++;

	// ���岽��ͨ������������
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

	FKLog("����������ʾ");
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

	FKLog("����������ʾ");
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

	FKLog("��������������ʾ");
}
//--------------------------------------------------------
void CRenderTextureLayer::AddNoiseBlend( const char* p_szNoisePic )
{
	CCSprite *noise = CCSprite::create( p_szNoisePic );
	ccBlendFunc blendFunc = {GL_DST_COLOR, GL_ZERO};
	noise->setBlendFunc(blendFunc);
	noise->setPosition(ccp(RENDER_TEX_WIDTH / 2, RENDER_TEX_HEIGHT / 2));
	noise->visit();

	FKLog("��������ͼ���");
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

		// ����������Ϣ���
		setTouchEnabled(true);
		// ����֡����
		scheduleUpdate();

		// ��ʼ����һ����Ⱦ������ʹ�ø�����
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
	// �ͷ���һ����
	if (m_pRenderTexture)
	{
		m_pRenderTexture->removeFromParentAndCleanup(true);
	}

	// ����������
	m_pRenderTexture = Create();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	m_pRenderTexture->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	m_pRenderTexture->getTexture()->setTexParameters(&tp);

	// ʹ��������
	this->addChild(m_pRenderTexture);
}
//--------------------------------------------------------
void CRenderTextureLayer::update(float dt)
{
	// ֡��������ƫ����
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