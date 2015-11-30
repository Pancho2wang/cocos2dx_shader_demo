/*!
 * \author:	FreeKnight
 * \date:	2014-7-1
 * \comment:
 */
//--------------------------------------------------------
#include "ImageProcess.h"
#include "Resource.h"
#include "Common/StringConvert.h"
//--------------------------------------------------------
static const GLchar* s_szDefaultA8Color_VSH = 
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
	gl_Position = CC_MVPMatrix * a_position;			\n\
	v_fragmentColor = a_color;							\n\
	v_texCoord = a_texCoord;							\n\
	}													\n\
	";
//--------------------------------------------------------
// Ô­Í¼Ð§¹û
static const GLchar* s_szPassThroghh_FSH = 
	"																\n\
	precision mediump float;										\n\
	uniform sampler2D u_Texture;									\n\
	uniform vec2 u_TextureCoordOffset[25];							\n\
	varying vec2 v_texCoord; 										\n\
	varying vec4 v_fragmentColor;									\n\
																	\n\
	void main(void)													\n\
	{																\n\
	gl_FragColor = texture2D(u_Texture, v_texCoord.st);				\n\
	}																\n\
	";
//--------------------------------------------------------
// Ä£ºýÐ§¹û
static const GLchar* s_szBlurFSH =
	"																\n\
	precision mediump float;										\n\
	uniform sampler2D u_Texture;									\n\
	uniform vec2 u_TextureCoordOffset[25];							\n\
	varying vec2 v_texCoord; 										\n\
	varying vec4 v_fragmentColor;									\n\
																	\n\
	void main(void)													\n\
	{																\n\
		vec4 sample[25];												\n\
																		\n\
		for (int i = 0; i < 25; i++)									\n\
		{																\n\
		sample[i] = texture2D(u_Texture, 								\n\
		v_texCoord.st + u_TextureCoordOffset[i]);						\n\
		}																\n\
		//--------------------------------------------------------		\n\
		//   1 3 1														\n\
		//   3 1 3   / 11												\n\
		//   1 3 1														\n\
																		\n\
		//gl_FragColor = (sample[0] + (3.0*sample[1]) + sample[2] + 	\n\
		//	(3.0*sample[3]) + sample[4] + (3.0*sample[5]) + 			\n\
		//	sample[6] + (3.0*sample[7]) + sample[8]) / 11.0;			\n\
		//--------------------------------------------------------		\n\
		// Gaussian weighting:											\n\
		// 1  4  7  4 1													\n\
		// 4 16 26 16 4													\n\
		// 7 26 41 26 7 / 273 (i.e. divide by total of weightings)		\n\
		// 4 16 26 16 4													\n\
		// 1  4  7  4 1													\n\
																		\n\
		gl_FragColor = (												\n\
		(1.0  * (sample[0] + sample[4]  + sample[20] + sample[24])) +	\n\
		(4.0  * (sample[1] + sample[3]  + sample[5]  + sample[9] + 		\n\
			sample[15] + sample[19] + sample[21] + sample[23])) +		\n\
		(7.0  * (sample[2] + sample[10] + sample[14] + sample[22])) +	\n\
		(16.0 * (sample[6] + sample[8]  + sample[16] + sample[18])) +	\n\
		(26.0 * (sample[7] + sample[11] + sample[13] + sample[17])) +	\n\
		(41.0 * sample[12])												\n\
		) / 273.0;														\n\
	}";
//--------------------------------------------------------
// Ä¥É°
static const GLchar* s_szSharpenFSH =
	"																\n\
	precision mediump float;										\n\
	uniform sampler2D u_Texture;									\n\
	uniform vec2 u_TextureCoordOffset[25];							\n\
	varying vec2 v_texCoord; 										\n\
	varying vec4 v_fragmentColor;									\n\
																	\n\
	void main(void)													\n\
	{																\n\
		vec4 sample[25];											\n\
																	\n\
		for (int i = 0; i < 25; i++)								\n\
		{															\n\
			sample[i] = texture2D(u_Texture, 						\n\
				v_texCoord.st + u_TextureCoordOffset[i]);			\n\
		}															\n\
		//--------------------------------------------------------	\n\
		//   -1 -1 -1												\n\
		//   -1  9 -1												\n\
		//   -1 -1 -1												\n\
																	\n\
		//gl_FragColor = (sample[4] * 9.0) -						\n\
		//	(sample[0] + sample[1] + sample[2] + 					\n\
		//	sample[3] + sample[5] + 								\n\
		//	sample[6] + sample[7] + sample[8]);						\n\
		//--------------------------------------------------------	\n\
		// Sharpen weighting:										\n\
		//  0 -1 -1 -1  0											\n\
		// -1  2 -4  2 -1 											\n\
		// -1 -4 13 -4 -1 											\n\
		// -1  2 -4  2 -1											\n\
		//  0 -1 -1 -1  0											\n\
																	\n\
		//gl_FragColor = (											\n\
		//(-1.0  * ( sample[1] + sample[2]  + sample[3] + sample[5] +	\n\
		//sample[9] + sample[10] + sample[14] + sample[15] + 			\n\
		//sample[19] + sample[21] + sample[22] + sample[23]) ) +		\n\
		//(2.0 * (sample[6] + sample[8] + sample[16] + sample[18])) +	\n\
		//(-4.0 *(sample[7] + sample[11] + sample[13] + sample[17]))+	\n\
		//( 13.0 * sample[12] )										\n\
		//);															\n\
																						\n\
		// 1  1  1  1  1																\n\
		// 1  1  1  1  1																\n\
		// 1  1 -14 1  1																\n\
		// 1  1  1  1  1																\n\
		// 1  1  1  1  1																\n\
																						\n\
		gl_FragColor = -14.0 * sample[12];												\n\
																						\n\
		for (int i = 0; i < 25; i++)													\n\
		{																				\n\
		if (i != 12)																	\n\
		gl_FragColor += sample[i];														\n\
		}																				\n\
		gl_FragColor /= 14.0;															\n\
	}";
//--------------------------------------------------------		   
// ÅòÕÍ
static const GLchar* s_szDilateFSH =
	"																\n\
	precision mediump float;										\n\
	uniform sampler2D u_Texture;									\n\
	uniform vec2 u_TextureCoordOffset[25];							\n\
	varying vec2 v_texCoord; 										\n\
	varying vec4 v_fragmentColor;									\n\
																	\n\
	void main(void)													\n\
	{																\n\
	vec4 sample[25];												\n\
	vec4 maxValue = vec4(0.0);															\n\
																						\n\
	for (int i = 0; i < 25; i++)														\n\
	{																					\n\
		// Sample a grid around and including our texel									\n\
		sample[i] = texture2D(u_Texture, v_texCoord.st + u_TextureCoordOffset[i]);		\n\
		// Keep the maximum value														\n\
		maxValue = max(sample[i], maxValue);											\n\
	}																					\n\
																						\n\
	gl_FragColor = maxValue;															\n\
	}";	
//--------------------------------------------------------	
// ÇÖÊ´Ð§¹û
static const GLchar* s_szErodeFSH =
	"																\n\
	precision mediump float;										\n\
	uniform sampler2D u_Texture;									\n\
	uniform vec2 u_TextureCoordOffset[25];							\n\
	varying vec2 v_texCoord; 										\n\
	varying vec4 v_fragmentColor;									\n\
																	\n\
	void main(void)													\n\
	{																\n\
	vec4 sample[25];																\n\
	vec4 minValue = vec4(1.0);														\n\
																					\n\
	for (int i = 0; i < 25; i++)													\n\
	{																				\n\
		// Sample a grid around and including our texel								\n\
		sample[i] = texture2D(u_Texture, v_texCoord.st + u_TextureCoordOffset[i]);	\n\
		// Keep the minimum value													\n\
		minValue = min(sample[i], minValue);										\n\
	}																				\n\
																					\n\
	gl_FragColor = minValue;														\n\
	}";	
//--------------------------------------------------------
// LaplacianÃè±ßÐ§¹û 
static const GLchar* s_szLaplacianEdgeDetectionFSH =
	"																\n\
	precision mediump float;										\n\
	uniform sampler2D u_Texture;									\n\
	uniform vec2 u_TextureCoordOffset[25];							\n\
	varying vec2 v_texCoord; 										\n\
	varying vec4 v_fragmentColor;									\n\
																	\n\
	void main(void)													\n\
	{																\n\
	vec4 sample[25];																\n\
																					\n\
	for (int i = 0; i < 25; i++)													\n\
	{																				\n\
		// Sample a grid around and including our texel								\n\
		sample[i] = texture2D(u_Texture, v_texCoord.st + u_TextureCoordOffset[i]);	\n\
	}																				\n\
																					\n\
	// Laplacian weighting:															\n\
	// -1 -1 -1 -1 -1																\n\
	// -1 -1 -1 -1 -1																\n\
	// -1 -1 24 -1 -1																\n\
	// -1 -1 -1 -1 -1																\n\
	// -1 -1 -1 -1 -1																\n\
																					\n\
	gl_FragColor = 24.0 * sample[12];												\n\
																					\n\
	for (int i = 0; i < 25; i++)													\n\
	{																				\n\
		if (i != 12)																\n\
			gl_FragColor -= sample[i];												\n\
	}																				\n\
	}";
//--------------------------------------------------------
// Sobel±ßÔµ¼ì²â
static const GLchar* s_szSobelEdgeDetectionFSH =
	"																	\n\
	precision mediump float;											\n\
	uniform sampler2D u_Texture;										\n\
	uniform vec2 u_TextureCoordOffset[25];								\n\
	varying vec2 v_texCoord; 											\n\
	varying vec4 v_fragmentColor;										\n\
																		\n\
	void main(void)														\n\
	{																	\n\
		vec4 sample[25];												\n\
																		\n\
		for (int i = 0; i < 25; i++)									\n\
		{																\n\
		sample[i] = texture2D(u_Texture, 								\n\
		v_texCoord.st + u_TextureCoordOffset[i]);						\n\
		}																\n\
		// Sobel x:														\n\
		// 1  2  0 -2 -1												\n\
		// 4  8  0 -8 -4												\n\
		// 6 12  0 -12-6	/ 12										\n\
		// 4  8  0 -8 -4												\n\
		// 1  2  0 -2 -1												\n\
		// Sobel y:														\n\
		// -1 -4 -6 -4 -1												\n\
		// -2 -8 -12-8 -2												\n\
		//  0  0  0  0  0	/ 12										\n\
		//  2  8 12  8  2												\n\
		//  1  4  6  4  1												\n\
																		\n\
		vec4 vertEdge = sample[0] + 4.0 * sample[1] +					\n\
		6.0 * sample[2] + 4.0 * sample[3] + sample[4] +					\n\
		2.0 * sample[5] + 8.0 * sample[6] + 12.0 * sample[7] +			\n\
		8.0 * sample[8] + 2.0 * sample[9] - 2.0 * sample[15] -			\n\
		8.0 * sample[16] - 12.0 * sample[17] - 8.0 * sample[18] - 		\n\
		2.0 * sample[19] - sample[20] - 4.0 * sample[21] - 				\n\
		6.0 * sample[22] - 4.0 * sample[23] - sample[24];				\n\
																		\n\
		vec4 horizEdge = - sample[0] - 2.0 * sample[1] +				\n\
		2.0 * sample[3] + sample[4] - 4.0 * sample[5] - 				\n\
		8.0 * sample[6] + 8.0 * sample[8] + 4.0 * sample[9] - 			\n\
		6.0 * sample[10] - 12.0 * sample[11] + 12.0 * sample[13] +		\n\
		6.0 * sample[14] - 4.0 * sample[15] - 8.0 * sample[16] +		\n\
		8.0 * sample[18] + 4.0 * sample[19] - sample[20] -				\n\
		2.0 * sample[21] + 2.0 * sample[23] + sample[24];				\n\
																		\n\
		//gl_FragColor.rgb = sqrt(horizEdge.rgb) + sqrt(vertEdge.rgb);	\n\
		gl_FragColor.rgb = sqrt((horizEdge.rgb * horizEdge.rgb) + 		\n\
			(vertEdge.rgb * vertEdge.rgb)) / 12.0f;						\n\
		gl_FragColor.a = 1.0;											\n\
	}";
//--------------------------------------------------------
// Prewitt±ßÔµ¼ì²â
static const GLchar* s_szPrewittEdgeDetectionFSH =
	"																	\n\
	precision mediump float;											\n\
	uniform sampler2D u_Texture;										\n\
	uniform vec2 u_TextureCoordOffset[25];								\n\
	varying vec2 v_texCoord; 											\n\
	varying vec4 v_fragmentColor;										\n\
																		\n\
	void main(void)														\n\
	{																	\n\
		vec4 sample[25];												\n\
																		\n\
		for (int i = 0; i < 25; i++)									\n\
		{																\n\
		sample[i] = texture2D(u_Texture, 								\n\
		v_texCoord.st + u_TextureCoordOffset[i]);						\n\
		}																\n\
		// Prewitt x:													\n\
		// -5 -4  0  4  5												\n\
		// -8-10  0 10  8												\n\
		//-10-20  0 20 10	/ 20										\n\
		// -8-10  0 10  8												\n\
		// -5 -4  0  4  5												\n\
		// Prewitt y:													\n\
		//  5  8 10  8  5												\n\
		//  4 10 20 10  4												\n\
		//  0  0  0  0  0	/ 20										\n\
		// -4-10-20-10 -4												\n\
		// -5 -8-10 -8 -5												\n\
																		\n\
		vec4 vertEdge = - 5.0 * sample[0] - 8.0 * sample[1] -			\n\
		10.0 * sample[2] - 8.0 * sample[3] - 5.0 * sample[4] -			\n\
		4.0 * sample[5] - 10.0 * sample[6] - 20.0 * sample[7] -			\n\
		10.0 * sample[8] - 4.0 * sample[9] + 4.0 * sample[15] +			\n\
		10.0 * sample[16] + 20.0 * sample[17] + 10.0 * sample[18] + 	\n\
		4.0 * sample[19] + 5.0 * sample[20] + 8.0 * sample[21] + 		\n\
		10.0 * sample[22] + 8.0 * sample[23] + 5.0 * sample[24];		\n\
																		\n\
		vec4 horizEdge = 5.0 * sample[0] + 4.0 * sample[1] -			\n\
		4.0 * sample[3] - 5.0 * sample[4] + 8.0 * sample[5] + 			\n\
		10.0 * sample[6] - 10.0 * sample[8] - 8.0 * sample[9] + 		\n\
		10.0 * sample[10] + 20.0 * sample[11] - 20.0 * sample[13] -		\n\
		10.0 * sample[14] + 8.0 * sample[15] + 10.0 * sample[16] -		\n\
		10.0 * sample[18] - 8.0 * sample[19] + 5.0 * sample[20] +		\n\
		4.0 * sample[21] - 4.0 * sample[23] - 5.0 * sample[24];			\n\
																		\n\
		gl_FragColor.rgb = sqrt((horizEdge.rgb * horizEdge.rgb) + 		\n\
			(vertEdge.rgb * vertEdge.rgb)) / 20.0f;						\n\
		gl_FragColor.a = 1.0;											\n\
	}";
//--------------------------------------------------------
// ÔË¶¯Ä£ºý
static const GLchar* s_szMotionBlurFSH =
	"																	\n\
	precision mediump float;											\n\
	uniform sampler2D u_Texture;										\n\
	uniform vec2 u_TextureCoordOffset[25];								\n\
	varying vec2 v_texCoord; 											\n\
	varying vec4 v_fragmentColor;										\n\
	uniform sampler2D tex;												\n\
																		\n\
	const float contrast = 1.6;											\n\
	const float brightness = 0.3;										\n\
	const float factor = -0.5 * contrast + brightness;					\n\
																		\n\
	void main()															\n\
    {																	\n\
		gl_FragColor = vec4(0.0);										\n\
		//vec4 c = texture2D(u_Texture,v_texCoord.st);					\n\
		//gl_FragColor = clamp(c * contrast + factor, 0.0, 1.);			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.028))*0.0044299121055113265;		\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.024))*0.00895781211794;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.020))*0.0215963866053;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.016))*0.0443683338718;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.012))*0.0776744219933;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.008))*0.115876621105;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.004))*0.147308056121;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord         )*0.159576912161;						\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.004))*0.147308056121;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.008))*0.115876621105;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.012))*0.0776744219933;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.016))*0.0443683338718;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.020))*0.0215963866053;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.024))*0.00895781211794;			\n\
		gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.028))*0.0044299121055113265;		\n\
	}";
//--------------------------------------------------------
CImageProcessSprite::CImageProcessSprite()												
{  																						
																						
}  																						
//--------------------------------------------------------
CImageProcessSprite::~CImageProcessSprite()
{  

}  
//--------------------------------------------------------
CImageProcessSprite* CImageProcessSprite::create( const char* pszFileName, const GLchar* pszVSH, const GLchar* pszFSH )
{  
	CImageProcessSprite* pSprite = new CImageProcessSprite;  
	if (pSprite && pSprite->initWithFile(pszFileName, pszVSH, pszFSH))
	{  
		pSprite->autorelease();  
		return pSprite;  
	}	
	CC_SAFE_RELEASE(pSprite);  
	return NULL;  
}  
//--------------------------------------------------------
bool CImageProcessSprite::initWithFile(const char *pszFilename,
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
bool CImageProcessSprite::initWithTexture(CCTexture2D* pTexture, const CCRect& tRect,
								   const GLchar* pszVSH, const GLchar* pszFSH )
{  
	do{  
		CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));  

		// Ö´ÐÐshader
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

		// ¼ÆËãÎÆÀíÆ«ÒÆ
		GLfloat	fTexCoordOffsets[tcOffsetRows * tcOffsetColumns * 2];
		genTexCoordOffsets( tRect.size.width, tRect.size.height, fTexCoordOffsets );
		int nLoc = glGetUniformLocation( this->getShaderProgram()->getProgram(), "u_TextureCoordOffset" );
		glUniform2fv(nLoc, tcOffsetRows * tcOffsetColumns, fTexCoordOffsets);

		return true;  
	} while (0);  
	return false;  
}  
//--------------------------------------------------------
void CImageProcessSprite::draw()
{  
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );  
	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );  

	this->getShaderProgram()->use();  
	this->getShaderProgram()->setUniformsForBuiltins();  

	ccGLBindTexture2D( this->getTexture()->getName() );  

	long offset = (long)&m_sQuad;  
	// ¶¥µã
	int diff = offsetof( ccV3F_C4B_T2F, vertices);  
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(m_sQuad.bl) , (void*) (offset + diff));  

	// ÎÆÀí  
	diff = offsetof( ccV3F_C4B_T2F, texCoords);  
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(m_sQuad.bl) , (void*)(offset + diff));  

	// ÑÕÉ«
	diff = offsetof( ccV3F_C4B_T2F, colors);  
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(m_sQuad.bl) , (void*)(offset + diff));  

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  
	CC_INCREMENT_GL_DRAWS(1);  
}  
//--------------------------------------------------------
// Note: Change this step value to increase the number of pixels we sample across...
void CImageProcessSprite::genTexCoordOffsets(GLuint width, GLuint height, GLfloat* pOutTexCoordOff, GLfloat step )
{
	// Note: You can multiply the step to displace the samples further. Do this with diff values horiz and vert and you have directional blur of a sort...
	float xInc = step / (GLfloat)(width);
	float yInc = step / (GLfloat)(height);

	for (int i = 0; i < tcOffsetColumns; i++)
	{
		for (int j = 0; j < tcOffsetRows; j++)
		{
			pOutTexCoordOff[(((i*5)+j)*2)+0] = (-2.0f * xInc) + ((GLfloat)i * xInc);
			pOutTexCoordOff[(((i*5)+j)*2)+1] = (-2.0f * yInc) + ((GLfloat)j * yInc);
		}
	}
}
//--------------------------------------------------------
// CImageProcessSpriteScene
//--------------------------------------------------------
CImageProcessSpriteScene::CImageProcessSpriteScene()
{
	CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	layer->setColor( ccGRAY );
	layer->setTag( 9999 );
	addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	// Ô­Í¼
	CImageProcessSprite* p = CImageProcessSprite::create( s_szCommonDogImgPath, s_szDefaultA8Color_VSH, s_szPassThroghh_FSH );
	if( p )
	{
		p->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		layer->addChild( p );
		CCLabelTTF* pInfo = CCLabelTTF::create(FKStringConvert::a2u("Ô­Í¼").c_str(), "Arial", 36 );
		pInfo->setPosition( p->getPosition() + ccp(50, 50));
		pInfo->setColor( ccRED );
		layer->addChild(pInfo);
	}

	CreateSprite( s_szBlurFSH, "Ä£ºý", ccp(winSize.width / 2 + p->getContentSize().width + 10,
		winSize.height / 2) );

	CreateSprite( s_szSharpenFSH, "Ä¥É°", ccp(winSize.width / 2, 
		winSize.height / 2 - p->getContentSize().height - 10 ) );

	CreateSprite( s_szDilateFSH, "ÅòÕÍ", ccp(winSize.width / 2 + p->getContentSize().width + 10, 
		winSize.height / 2 - p->getContentSize().height - 10 ) );

	CreateSprite( s_szErodeFSH, "ÇÖÊ´", ccp(winSize.width / 2 + p->getContentSize().width + 10, 
		winSize.height / 2 + p->getContentSize().height + 10 ) );

	CreateSprite( s_szLaplacianEdgeDetectionFSH, "Laplacian±ßÔµ", ccp(winSize.width / 2, 
		winSize.height / 2 + p->getContentSize().height + 10 ) );

	CreateSprite( s_szSobelEdgeDetectionFSH, "Sobel±ßÔµ", ccp(winSize.width / 2 - p->getContentSize().width - 10 , 
		winSize.height / 2 + p->getContentSize().height + 10 ) );

	CreateSprite( s_szPrewittEdgeDetectionFSH, "Prewitt±ßÔµ", ccp(winSize.width / 2 - p->getContentSize().width - 10 , 
		winSize.height / 2 ) );

	CreateSprite( s_szMotionBlurFSH, "ÔË¶¯Ä£ºý", ccp(winSize.width / 2 - p->getContentSize().width - 10 , 
		winSize.height / 2 - p->getContentSize().height - 10 ) );
}
//--------------------------------------------------------
bool CImageProcessSpriteScene::CreateSprite( const GLchar* szFSH, std::string szInfo, CCPoint tagPos )
{
	CImageProcessSprite* p = CImageProcessSprite::create( s_szCommonDogImgPath, s_szDefaultA8Color_VSH, szFSH );
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
void CImageProcessSpriteScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------