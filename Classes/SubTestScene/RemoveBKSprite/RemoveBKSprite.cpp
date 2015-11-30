/*!
 * \author:	FreeKnight
 * \date:	2014-6-27
 * \comment:
 */
//--------------------------------------------------------
#include "RemoveBKSprite.h"
#include "Resource.h"
//--------------------------------------------------------
static const GLchar *szRemoveBKShader =
	"																						\n\
	#ifdef GL_ES																			\n\
	precision lowp float;																	\n\
	#endif																					\n\
	varying vec4 v_fragmentColor;															\n\
	varying vec2 v_texCoord;																\n\
	uniform sampler2D u_texture;															\n\
	void main()																				\n\
	{																						\n\
		float ratio=0.0;																	\n\
		vec4 texColor = texture2D(u_texture, v_texCoord);									\n\
		ratio = texColor[0] > texColor[1]?(texColor[0] > texColor[2] ?						\n\
		texColor[0] : texColor[2]) :(texColor[1] > texColor[2]? texColor[1] : texColor[2]);	\n\
		if (ratio != 0.0)																	\n\
		{																					\n\
			texColor[0] = texColor[0] /  ratio;												\n\
			texColor[1] = texColor[1] /  ratio;												\n\
			texColor[2] = texColor[2] /  ratio;												\n\
			texColor[3] = ratio;															\n\
		}																					\n\
		else																				\n\
		{																					\n\
			texColor[3] = 0.0;																\n\
		}																					\n\
		gl_FragColor = v_fragmentColor*texColor;											\n\
	}";
//--------------------------------------------------------
CRemoveBKSprite* CRemoveBKSprite::Create(const char *pszFileName)
{
	CRemoveBKSprite *pRet = new CRemoveBKSprite();
	if (pRet && pRet->initWithFile(pszFileName)) 
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE( pRet );
	return NULL;
}
//--------------------------------------------------------
bool CRemoveBKSprite::initWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
	do{
		CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, rect));

		// 加载顶点着色器和片元着色器
		m_pShaderProgram = new  CCGLProgram();
		m_pShaderProgram ->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, szRemoveBKShader );
		CHECK_GL_ERROR_DEBUG();

		// 启用顶点着色器的attribute变量，坐标、纹理坐标、颜色
		m_pShaderProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		m_pShaderProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		m_pShaderProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		CHECK_GL_ERROR_DEBUG();

		// 自定义着色器链接
		m_pShaderProgram->link();
		CHECK_GL_ERROR_DEBUG();

		// 设置移动、缩放、旋转矩阵
		m_pShaderProgram->updateUniforms();
		CHECK_GL_ERROR_DEBUG();

		return true;

	}while(0);
	return false;
}
//--------------------------------------------------------
void CRemoveBKSprite::draw(void)
{
	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
	CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");

	CC_NODE_DRAW_SETUP();

	// 启用attributes变量输入，顶点坐标，纹理坐标，颜色
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
	ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);

	m_pShaderProgram->use();
	m_pShaderProgram->setUniformsForBuiltins();

	// 绑定纹理到纹理槽0
	ccGLBindTexture2D(m_pobTexture->getName());

	long offset = (long)&m_sQuad;
	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(m_sQuad.bl), (void*) (offset + diff));
	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(m_sQuad.bl), (void*)(offset + diff));
	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(m_sQuad.bl), (void*)(offset + diff));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CHECK_GL_ERROR_DEBUG();

	CC_INCREMENT_GL_DRAWS(1);
	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
}
//--------------------------------------------------------
// CColorSpriteScene
//--------------------------------------------------------
CRemoveBKSpriteScene::CRemoveBKSpriteScene()
{
	CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	layer->setColor( ccGRAY );
	addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CRemoveBKSprite* pRemoveSprite = CRemoveBKSprite::Create( s_szRemoveBKSprite );
	if( pRemoveSprite )
	{
		pRemoveSprite->setPosition(ccp(winSize.width / 2 + pRemoveSprite->getContentSize().width / 2 , 
			winSize.height / 2));
		layer->addChild( pRemoveSprite );
	}
	CCSprite* pNormalSprite = CCSprite::create( s_szRemoveBKSprite );
	if( pNormalSprite )
	{
		pNormalSprite->setPosition(ccp(winSize.width / 2 - pNormalSprite->getContentSize().width / 2 , 
			winSize.height / 2));
		layer->addChild( pNormalSprite );
	}
}
//--------------------------------------------------------
void CRemoveBKSpriteScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------