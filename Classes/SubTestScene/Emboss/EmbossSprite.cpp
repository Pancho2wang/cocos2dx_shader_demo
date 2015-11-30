/*!
 * \author:	FreeKnight
 * \date:	2014-6-28
 * \comment:
 */
//--------------------------------------------------------
#include "EmbossSprite.h"
#include "Resource.h"
#include "Common/StringConvert.h"
//--------------------------------------------------------
static const GLchar* szColorRampShader = 
	"																						\n\
	#ifdef GL_ES																			\n\
	precision mediump float;																\n\
	#endif																					\n\
	varying vec2 v_texCoord;																\n\
	uniform sampler2D u_texture;															\n\
	uniform sampler2D u_colorRampTexture;													\n\
	void main()																				\n\
	{																						\n\
	vec3 normalColor = texture2D( u_texture, v_texCoord ).rgb;								\n\
	float rampedR = texture2D( u_colorRampTexture, vec2( normalColor.r, 0 )).g;				\n\
	float rampedG = texture2D( u_colorRampTexture, vec2( normalColor.r, 0 )).g;				\n\
	float rampedB = texture2D( u_colorRampTexture, vec2( normalColor.r, 0 )).b;				\n\
	gl_FragColor = vec4( rampedR, rampedG, rampedB, 1 );									\n\
	}";

//--------------------------------------------------------
static const GLchar* szEmbossShader = 
	"																						\n\
	#ifdef GL_ES																			\n\
		precision mediump float;															\n\
	#endif																					\n\
	varying vec2 v_texCoord;																\n\
	uniform sampler2D u_texture;															\n\
	uniform float u_time;																	\n\
	void main()																				\n\
	{																						\n\
		vec2 onePixel = vec2(1.0 / 480.0, 1.0 / 320.0);										\n\
		vec2 texCoord = v_texCoord;															\n\
		texCoord.x += sin(u_time) * (onePixel.x * 6.0);										\n\
		texCoord.y += cos(u_time) * (onePixel.y * 6.0);										\n\
		vec4 color;																			\n\
		color.rgb = vec3(0.5);																\n\
		color -= texture2D(u_texture, texCoord - onePixel) * 5.0;							\n\
		color += texture2D(u_texture, texCoord + onePixel) * 5.0;							\n\
		color.rgb = vec3((color.r + color.g + color.b) / 3.0);								\n\
		gl_FragColor = vec4(color.rgb, 1);													\n\
	}";
//--------------------------------------------------------
bool CEmbossLayer::init()
{
	do{
		CC_BREAK_IF(!CCLayer::init());

		CreateEmbossSprite();
		CreateColorRampSprite();

		this->scheduleUpdate();
		return true;
	}while(0);
	return false;
}
//--------------------------------------------------------
void CEmbossLayer::update(float delta)
{
	m_fTime += delta;
	CCSprite* p = (CCSprite*)getChildByTag( 10001 );
	if( p )
	{
		p->getShaderProgram()->use();
	}
	glUniform1f( m_nTimeUniformLocation, m_fTime);
}
//--------------------------------------------------------
bool CEmbossLayer::CreateEmbossSprite()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* pSprite = CCSprite::create( s_szCommonDogImgPath );
	pSprite->setTag( 10001 );
	pSprite->setPosition( ccp( 0.0f, - pSprite->getContentSize().height - 10) );
	addChild( pSprite );

	CCLabelTTF* pInfo = CCLabelTTF::create(FKStringConvert::a2u("精细浮雕").c_str(), "Arial", 36 );
	pInfo->setPosition( pSprite->getPosition() + ccp(50, 50));
	pInfo->setColor( ccRED );
	addChild(pInfo);

	// 加载顶点着色器和片元着色器
	CCGLProgram* pShader = new  CCGLProgram();
	pShader->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, szEmbossShader );
	pSprite->setShaderProgram( pShader );
	pShader->release();
	CHECK_GL_ERROR_DEBUG();

	// 启用顶点着色器的attribute变量，坐标、纹理坐标、颜色
	pSprite->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	pSprite->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	CHECK_GL_ERROR_DEBUG();

	// 自定义着色器链接
	pSprite->getShaderProgram()->link();
	CHECK_GL_ERROR_DEBUG();

	// 设置移动、缩放、旋转矩阵
	pSprite->getShaderProgram()->updateUniforms();
	CHECK_GL_ERROR_DEBUG();

	m_nTimeUniformLocation = glGetUniformLocation( pSprite->getShaderProgram()->getProgram(), "u_time" );

	pSprite->getShaderProgram()->use();

	return true;
}
//--------------------------------------------------------
bool CEmbossLayer::CreateColorRampSprite()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* pSprite = CCSprite::create( s_szCommonDogImgPath );
	pSprite->setPosition( ccp( 0.0f, pSprite->getContentSize().height + 10 ) );
	addChild( pSprite );

	CCLabelTTF* pInfo = CCLabelTTF::create(FKStringConvert::a2u("底片").c_str(), "Arial", 36 );
	pInfo->setPosition( pSprite->getPosition() + ccp(50, 50));
	pInfo->setColor( ccRED );
	addChild(pInfo);

	// 加载顶点着色器和片元着色器
	CCGLProgram* pShader = new  CCGLProgram();
	pShader->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, szColorRampShader );
	pSprite->setShaderProgram( pShader );
	pShader->release();
	CHECK_GL_ERROR_DEBUG();

	// 启用顶点着色器的attribute变量，坐标、纹理坐标、颜色
	pSprite->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	pSprite->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	CHECK_GL_ERROR_DEBUG();

	// 自定义着色器链接
	pSprite->getShaderProgram()->link();
	CHECK_GL_ERROR_DEBUG();

	// 设置移动、缩放、旋转矩阵
	pSprite->getShaderProgram()->updateUniforms();
	CHECK_GL_ERROR_DEBUG();

	int nColorRampUniformLocation = glGetUniformLocation(pSprite->getShaderProgram()->getProgram(), "u_colorRampTexture");
	glUniform1i( nColorRampUniformLocation, 1 );

	CCTexture2D* pTex = CCTextureCache::sharedTextureCache()->addImage( s_szColorDumpSprite );
	pTex->setAliasTexParameters();

	pSprite->getShaderProgram()->use();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pTex->getName() );
	glActiveTexture(GL_TEXTURE0);

	return true;
}
//--------------------------------------------------------
// CEmbossSpriteScene
//--------------------------------------------------------
CEmbossSpriteScene::CEmbossSpriteScene()
{
	CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	layer->setColor( ccGRAY );
	addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CEmbossLayer* pEmbossLayer = (CEmbossLayer*)CEmbossLayer::create();
	if( pEmbossLayer )
	{
		pEmbossLayer->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		layer->addChild( pEmbossLayer );
	}

	CCSprite* pNormalSprite = CCSprite::create( s_szCommonDogImgPath );
	if( pNormalSprite )
	{
		pNormalSprite->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		layer->addChild( pNormalSprite );

		CCLabelTTF* pInfo = CCLabelTTF::create(FKStringConvert::a2u("原图").c_str(), "Arial", 36 );
		pInfo->setPosition( pNormalSprite->getPosition() + ccp(50, 50));
		pInfo->setColor( ccRED );
		layer->addChild(pInfo);
	}
}
//--------------------------------------------------------
void CEmbossSpriteScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------