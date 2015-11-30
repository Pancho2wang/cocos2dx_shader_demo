/*!
 * \author:	FreeKnight
 * \date:	2014-6-27
 * \comment:
 */
//--------------------------------------------------------
#include "ColorSprite.h"
#include "Resource.h"
#include "Common/StringConvert.h"
//--------------------------------------------------------
static const GLchar* s_szColorSpriteVSH = 
	"													\n\
	attribute vec4 a_position;							\n\
	attribute vec2 a_texCoord;							\n\
	attribute vec4 a_color;								\n\
														\n\
	#ifdef GL_ES										\n\
	varying lowp vec4 v_fragmentColor;					\n\
	varying mediump vec2 v_texCoord;					\n\
	#else												\n\
	varying vec4 v_fragmentColor;						\n\
	varying vec2 v_texCoord;							\n\
	#endif												\n\
														\n\
	void main()											\n\
	{													\n\
		gl_Position = CC_MVPMatrix * a_position;		\n\
		v_fragmentColor = a_color;						\n\
		v_texCoord = a_texCoord;						\n\
	}";													
//--------------------------------------------------------
static const GLchar* s_szColorSpriteFSH = 
	"																													\n\
	#ifdef GL_ES 								 																		\n\
		precision mediump float; 					 																	\n\
	#endif 																												\n\
																														\n\
	uniform sampler2D u_texture; 				 																		\n\
	varying vec2 v_texCoord; 					 																		\n\
	varying vec4 v_fragmentColor; 																						\n\
																														\n\
	void main(void) 							 																		\n\
	{ 											 																		\n\
		// vec3( 0.299, 0.587, 0.114 ) 是RGB转YUV的参数值，生成灰色图														\n\
		float MixColor = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114));							\n\
		// 使用灰色图进行颜色混合																							\n\
		vec4 blendColor = vec4( 1.2, 1.0, 0.8, 1.0 ); // 调整这个值以修改最终混合色值										\n\
		gl_FragColor = vec4(MixColor * blendColor.r, MixColor * blendColor.g, MixColor * blendColor.b, blendColor.a);	\n\
	}";
//--------------------------------------------------------
// 负色
// 逐像素，使用白色减去当前像素颜色得到
static const GLchar* s_szNegativeFSH =					
	"													\n\
	precision mediump float;							\n\
	uniform sampler2D u_Texture;						\n\
	varying vec2 v_texCoord; 							\n\
	varying vec4 v_fragmentColor;						\n\
	void main()											\n\
	{													\n\
		float T = 1.0; 									\n\
		vec2 st = v_texCoord.st;						\n\
		vec3 irgb = texture2D(u_Texture, st).rgb;		\n\
		vec3 neg = vec3(1., 1., 1.)-irgb;				\n\
		gl_FragColor = vec4(mix(irgb,neg, T), 1.);		\n\
	}";
//--------------------------------------------------------
// 高光
// 使用添加 或者 减去黑色，达到调整图片光亮度的效果
static const GLchar* s_szBrightnessFSH = 
	"													\n\
	precision mediump float;							\n\
	uniform sampler2D u_Texture;						\n\
	varying vec2 v_texCoord; 							\n\
	varying vec4 v_fragmentColor;						\n\
														\n\
	void main()											\n\
	{													\n\
		float T = 2.0;									\n\
		vec2 st = v_texCoord.st;						\n\
		vec3 irgb = texture2D(u_Texture, st).rgb;		\n\
		vec3 black = vec3(0., 0., 0.);					\n\
		gl_FragColor = vec4(mix(black, irgb, T), 1.);	\n\
	}";												
//--------------------------------------------------------
// 对比效果
// 使用一个灰色图作为基础图像，和彩色图混合。
// 逐像素的将图片颜色差向灰度两边扩大，得到对比效果
static const GLchar* s_szContrastFSH = 
	"													\n\
	precision mediump float;							\n\
	uniform sampler2D u_Texture;						\n\
	varying vec2 v_texCoord; 							\n\
	varying vec4 v_fragmentColor;						\n\
														\n\
	void main()											\n\
	{													\n\
		float T = 2.0;									\n\
		vec2 st = v_texCoord.st;						\n\
		vec3 irgb = texture2D(u_Texture, st).rgb;		\n\
		vec3 target = vec3(0.5, 0.5, 0.5);				\n\
		gl_FragColor = vec4(mix(target, irgb, T), 1.);	\n\
	}";
//--------------------------------------------------------
// 饱和
// 混合彩色图片和其亮度图的灰阶，得到饱和图
static const GLchar* s_szSaturationFSH = 
	"													\n\
	precision mediump float;							\n\
	uniform sampler2D u_Texture;						\n\
	varying vec2 v_texCoord; 							\n\
	varying vec4 v_fragmentColor;						\n\
	const vec3 W = vec3(0.2125, 0.7154, 0.0721);		\n\
														\n\
	void main()											\n\
	{													\n\
		float T = 3.0;											\n\
		vec2 st = v_texCoord.st;								\n\
		vec3 irgb = texture2D(u_Texture, st).rgb; 				\n\
		float luminance = dot(irgb, W);							\n\
		vec3 target = vec3(luminance, luminance, luminance); 	\n\
		gl_FragColor = vec4(mix(target, irgb, T), 1.);			\n\
	}";
//--------------------------------------------------------
// 黑白
// 转为灰度图，然后根据阀值，转为黑白
static const GLchar* s_szBlackWhiteFSH = 
	"													\n\
	precision mediump float;							\n\
	uniform sampler2D u_Texture;						\n\
	varying vec2 v_texCoord; 							\n\
	varying vec4 v_fragmentColor;						\n\
	const vec4 W = vec4(0.2125, 0.7154, 0.0721, 0);		\n\
														\n\
	void main()											\n\
	{													\n\
		vec4 col = texture2D(u_Texture, v_texCoord.st);	\n\
		float lum = dot(col, W);						\n\
		if (0.5 < lum) {								\n\
		gl_FragColor = v_fragmentColor;					\n\
		} else {										\n\
		gl_FragColor = vec4(0, 0, 0, 1);}				\n\
	}";
//--------------------------------------------------------
// 边缘
static const GLchar* s_szEdgeDetectionFSH = 
	"																\n\
	#ifdef GL_ES													\n\
	precision mediump float;										\n\
	precision mediump int;											\n\
	#endif															\n\
																	\n\
	uniform sampler2D u_Texture;									\n\
	const vec2 texOffset = vec2( 0.005, 0.005);						\n\
																	\n\
	varying vec2 v_texCoord;										\n\
	varying vec4 v_fragmentColor;									\n\
																	\n\
	const vec4 lumcoeff = vec4(0.299, 0.587, 0.114, 0);				\n\
																	\n\
	void main() 													\n\
	{																\n\
	vec2 tc0 = v_texCoord.st + vec2(-texOffset.s, -texOffset.t);	\n\
	vec2 tc1 = v_texCoord.st + vec2(         0.0, -texOffset.t);	\n\
	vec2 tc2 = v_texCoord.st + vec2(+texOffset.s, -texOffset.t);	\n\
	vec2 tc3 = v_texCoord.st + vec2(-texOffset.s,          0.0);	\n\
	vec2 tc4 = v_texCoord.st + vec2(         0.0,          0.0);	\n\
	vec2 tc5 = v_texCoord.st + vec2(+texOffset.s,          0.0);	\n\
	vec2 tc6 = v_texCoord.st + vec2(-texOffset.s, +texOffset.t);	\n\
	vec2 tc7 = v_texCoord.st + vec2(         0.0, +texOffset.t);	\n\
	vec2 tc8 = v_texCoord.st + vec2(+texOffset.s, +texOffset.t);	\n\
																	\n\
	vec4 col0 = texture2D(u_Texture, tc0);							\n\
	vec4 col1 = texture2D(u_Texture, tc1);							\n\
	vec4 col2 = texture2D(u_Texture, tc2);							\n\
	vec4 col3 = texture2D(u_Texture, tc3);							\n\
	vec4 col4 = texture2D(u_Texture, tc4);							\n\
	vec4 col5 = texture2D(u_Texture, tc5);							\n\
	vec4 col6 = texture2D(u_Texture, tc6);							\n\
	vec4 col7 = texture2D(u_Texture, tc7);							\n\
	vec4 col8 = texture2D(u_Texture, tc8);							\n\
																	\n\
	vec4 sum = 8.0 * col4 - (col0 + col1 + col2 + col3 + col5 + col6 + col7 + col8);	\n\
	gl_FragColor = vec4(sum.rgb, 1.0) * v_fragmentColor;								\n\
	}";
//--------------------------------------------------------
// 浮雕
static const GLchar* s_szEmbossFSH = 
	"																\n\
	#ifdef GL_ES													\n\
	precision mediump float;										\n\
	precision mediump int;											\n\
	#endif															\n\
																	\n\
	uniform sampler2D u_Texture;									\n\
	const vec2 texOffset = vec2( 0.005, 0.005);						\n\
																	\n\
	varying vec4 v_fragmentColor;									\n\
	varying vec2 v_texCoord;										\n\
																	\n\
	const vec4 lumcoeff = vec4(0.299, 0.587, 0.114, 0);				\n\
																	\n\
	void main() 													\n\
	{																\n\
	vec2 tc0 = v_texCoord.st + vec2(-texOffset.s, -texOffset.t);	\n\
	vec2 tc1 = v_texCoord.st + vec2(         0.0, -texOffset.t);	\n\
	vec2 tc2 = v_texCoord.st + vec2(-texOffset.s,          0.0);	\n\
	vec2 tc3 = v_texCoord.st + vec2(+texOffset.s,          0.0);	\n\
	vec2 tc4 = v_texCoord.st + vec2(         0.0, +texOffset.t);	\n\
	vec2 tc5 = v_texCoord.st + vec2(+texOffset.s, +texOffset.t);			\n\
																			\n\
	vec4 col0 = texture2D(u_Texture, tc0);									\n\
	vec4 col1 = texture2D(u_Texture, tc1);									\n\
	vec4 col2 = texture2D(u_Texture, tc2);									\n\
	vec4 col3 = texture2D(u_Texture, tc3);									\n\
	vec4 col4 = texture2D(u_Texture, tc4);									\n\
	vec4 col5 = texture2D(u_Texture, tc5);									\n\
																			\n\
	vec4 sum = vec4(0.5) + (col0 + col1 + col2) - (col3 + col4 + col5);		\n\
	float lum = dot(sum, lumcoeff);											\n\
	gl_FragColor = vec4(lum, lum, lum, 1.0) * v_fragmentColor;  			\n\
	}";
//--------------------------------------------------------				  
//--------------------------------------------------------
CColorSprite::CColorSprite()
{  

}  
//--------------------------------------------------------
CColorSprite::~CColorSprite()
{  

}  
//--------------------------------------------------------
CColorSprite* CColorSprite::create( const char* pszFileName, const GLchar* pszVSH, const GLchar* pszFSH )
{  
	CColorSprite* pColorSprite = new CColorSprite;  
	if (pColorSprite && pColorSprite->initWithFile(pszFileName, pszVSH, pszFSH))
	{  
		pColorSprite->autorelease();  
		return pColorSprite;  
	}	
	CC_SAFE_RELEASE(pColorSprite);  
	return NULL;  
}  
//--------------------------------------------------------
bool CColorSprite::initWithFile(const char *pszFilename,
				  const GLchar* pszVSH, const GLchar* pszFSH )
{
	CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszFilename);
	if (pTexture)
	{
		CCRect rect = CCRectZero;
		rect.size = pTexture->getContentSize();
		return initWithTexture(pTexture, rect, pszVSH, pszFSH);
	}

	// don't release here.
	// when load texture failed, it's better to get a "transparent" sprite then a crashed program
	// this->release(); 
	return false;
}
//--------------------------------------------------------
bool CColorSprite::initWithTexture(CCTexture2D* pTexture, const CCRect& tRect,
								   const GLchar* pszVSH, const GLchar* pszFSH )
{  
	do{  
		CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));  

		// 执行shader
		CCGLProgram* pProgram = new CCGLProgram();  
		pProgram->initWithVertexShaderByteArray( pszVSH, pszFSH );  
		this->setShaderProgram(pProgram);  
		pProgram->release();  
		CHECK_GL_ERROR_DEBUG();  

		this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);  
		this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);  
		this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);  
		CHECK_GL_ERROR_DEBUG();  

		this->getShaderProgram()->link();  
		CHECK_GL_ERROR_DEBUG();  

		this->getShaderProgram()->updateUniforms();  
		CHECK_GL_ERROR_DEBUG();  

		return true;  
	} while (0);  
	return false;  
}  
//--------------------------------------------------------
void CColorSprite::draw()
{  
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );  
	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );  

	this->getShaderProgram()->use();  
	this->getShaderProgram()->setUniformsForBuiltins();  

	ccGLBindTexture2D( this->getTexture()->getName() );  

	long offset = (long)&m_sQuad;  
	// 顶点
	int diff = offsetof( ccV3F_C4B_T2F, vertices);  
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(m_sQuad.bl) , (void*) (offset + diff));  

	// 纹理  
	diff = offsetof( ccV3F_C4B_T2F, texCoords);  
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(m_sQuad.bl) , (void*)(offset + diff));  

	// 颜色
	diff = offsetof( ccV3F_C4B_T2F, colors);  
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(m_sQuad.bl) , (void*)(offset + diff));  

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  
	CC_INCREMENT_GL_DRAWS(1);  
}  
//--------------------------------------------------------
// CColorSpriteScene
//--------------------------------------------------------
CColorSpriteScene::CColorSpriteScene()
{
	CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	layer->setColor( ccGRAY );
	layer->setTag( 9999 );
	addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// 原图
	CCSprite* p = CCSprite::create( s_szCommonDogImgPath );
	if( p )
	{
		p->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		layer->addChild( p );
		CCLabelTTF* pInfo = CCLabelTTF::create(FKStringConvert::a2u("原图").c_str(), "Arial", 36 );
		pInfo->setPosition( p->getPosition() + ccp(50, 50));
		pInfo->setColor( ccRED );
		layer->addChild(pInfo);
	}

	CreateSprite( s_szNegativeFSH, "反色", ccp(winSize.width / 2 + p->getContentSize().width + 10,
		winSize.height / 2) );

	CreateSprite( s_szBrightnessFSH, "高光", ccp(winSize.width / 2, 
		winSize.height / 2 - p->getContentSize().height - 10 ) );

	CreateSprite( s_szColorSpriteFSH, "老照片", ccp(winSize.width / 2 + p->getContentSize().width + 10, 
		winSize.height / 2 - p->getContentSize().height - 10 ) );

	CreateSprite( s_szContrastFSH, "对比", ccp(winSize.width / 2 + p->getContentSize().width + 10, 
		winSize.height / 2 + p->getContentSize().height + 10 ) );

	CreateSprite( s_szSaturationFSH, "饱和", ccp(winSize.width / 2, 
		winSize.height / 2 + p->getContentSize().height + 10 ) );

	CreateSprite( s_szBlackWhiteFSH, "黑白", ccp(winSize.width / 2 - p->getContentSize().width - 10 , 
		winSize.height / 2 + p->getContentSize().height + 10 ) );

	CreateSprite( s_szEdgeDetectionFSH, "边缘", ccp(winSize.width / 2 - p->getContentSize().width - 10 , 
		winSize.height / 2 ) );

	CreateSprite( s_szEmbossFSH, "浮雕", ccp(winSize.width / 2 - p->getContentSize().width - 10 , 
		winSize.height / 2 - p->getContentSize().height - 10 ) );
}
//--------------------------------------------------------
bool CColorSpriteScene::CreateSprite( const GLchar* szFSH, std::string szInfo, CCPoint tagPos )
{
	CColorSprite* p = CColorSprite::create( s_szCommonDogImgPath, s_szColorSpriteVSH, szFSH );
	CCLayerColor* pLayer = ( CCLayerColor* )getChildByTag( 9999 );
	if( p )
	{
		p->setPosition( tagPos );
		pLayer->addChild( p );
		CCLabelTTF* pInfo = CCLabelTTF::create(FKStringConvert::a2u( szInfo.c_str() ).c_str(), "Arial", 36 );
		pInfo->setPosition( p->getPosition() + ccp(50, 50));
		pInfo->setColor( ccRED );
		pLayer->addChild(pInfo);
		return true;
	}
	return false;
}
//--------------------------------------------------------
void CColorSpriteScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------