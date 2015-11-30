/*!
 * \author:	FreeKnight
 * \date:	2014-6-28
 * \comment:
 */
//--------------------------------------------------------
#include "ActiveGrass.h"
#include "Resource.h"
#include "Common/StringConvert.h"
//--------------------------------------------------------
static const GLchar* szGrassShader = 
	"																									\n\
	#ifdef GL_ES																						\n\
		precision mediump float;																		\n\
	#endif																								\n\
																										\n\
	varying vec2 v_texCoord;																			\n\
	uniform sampler2D u_texture;																		\n\
	uniform float u_time;																				\n\
																										\n\
	// 1																								\n\
	const float speed = 2.0;																			\n\
	const float bendFactor = 0.2;																		\n\
	void main()																							\n\
	{																									\n\
	// ��ø߶ȣ�texCoord���µ���Ϊ0��1																	\n\
	float height = 1.0 - v_texCoord.y;																	\n\
	// ���ƫ������һ���ݺ�����ֵ���󣬵���Խ��ƫ��������													\n\
	float offset = pow(height, 2.5);																	\n\
	// ƫ������ʱ��仯�������Է��ȣ�����Ƶ��																\n\
	offset *= (sin(u_time * speed) * bendFactor);														\n\
	// ʹx����ƫ�ƣ�fractȡ����ֵ��0��1��																	\n\
	vec3 normalColor = texture2D(u_texture, fract(vec2(v_texCoord.x + offset, v_texCoord.y))).rgb;		\n\
	gl_FragColor = vec4(normalColor, 1);																\n\
	}";
//--------------------------------------------------------
bool CActiveGrassLayer::init()
{
	do{
		CC_BREAK_IF(!CCLayer::init());

		m_fTime = 0.0f;

		CCSprite* pSprite = CCSprite::create( s_szActiveGrassImg );
		if( pSprite == NULL )
		{
			break;
		}
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		pSprite->setPosition( ccp(winSize.width / 2, winSize.height / 2) );
		pSprite->setTag( 10001 );
		this->addChild( pSprite );

		// ���ض�����ɫ����ƬԪ��ɫ��
		CCGLProgram* pShader = new  CCGLProgram();
		pShader->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, szGrassShader );
		pSprite->setShaderProgram( pShader );
		pShader->release();
		CHECK_GL_ERROR_DEBUG();

		// ���ö�����ɫ����attribute���������ꡢ�������ꡢ��ɫ
		pSprite->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		pSprite->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		CHECK_GL_ERROR_DEBUG();

		// �Զ�����ɫ������
		pSprite->getShaderProgram()->link();
		CHECK_GL_ERROR_DEBUG();

		// �����ƶ������š���ת����
		pSprite->getShaderProgram()->updateUniforms();
		CHECK_GL_ERROR_DEBUG();

		m_nTimeUniformLocation = glGetUniformLocation( pSprite->getShaderProgram()->getProgram(), "u_time" );

		pSprite->getShaderProgram()->use();

		// ����֡����
		this->scheduleUpdate();
		return true;
	}while(0);
	return false;
}
//--------------------------------------------------------
void CActiveGrassLayer::update(float delta)
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
// CActiveGrassScene
//--------------------------------------------------------
CActiveGrassScene::CActiveGrassScene()
{
	CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
	layer->setColor( ccGRAY );
	addChild(layer);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CActiveGrassLayer* pActiveGrassLayer = (CActiveGrassLayer*)CActiveGrassLayer::create();
	if( pActiveGrassLayer )
	{
		this->addChild( pActiveGrassLayer );

		CCLabelTTF* pInfo = CCLabelTTF::create(FKStringConvert::a2u("���ž�̬ͼƬ��Shaderʵ�ֶ�̬��ҡ��Ч��").c_str(), "Arial", 24 );
		pInfo->setPosition( ccp( winSize.width / 2, 50));
		pInfo->setColor( ccRED );
		this->addChild(pInfo);
	}
}
//--------------------------------------------------------
void CActiveGrassScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------