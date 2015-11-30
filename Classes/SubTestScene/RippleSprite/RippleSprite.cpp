/*!
 * \author:	FreeKnight
 * \date:	2014-6-26
 * \comment:
 */
//--------------------------------------------------------
#include "RippleSprite.h"
#include "CCGL.h"
#include "Resource.h"
#include "Common\FKCommon.h"
//--------------------------------------------------------
#define RIPPLE_EDGE_ATTR						4
//--------------------------------------------------------
static const GLchar* s_szRippleSpriteFSH = 
	"															\n\
	#ifdef GL_ES												\n\
	precision mediump float;										\n\
	#endif														\n\
	varying vec2 v_texCoord;									\n\
	uniform sampler2D CC_Texture0;                              \n\
	void main()                                                 \n\
	{															\n\
		gl_FragColor =  texture2D(CC_Texture0, v_texCoord);    	\n\
	}";             										
//--------------------------------------------------------
static const GLchar* s_szRippleSpriteVSH = 
"																									\n\
attribute vec4 a_position;																			\n\
attribute vec2 a_texCoord;																			\n\
attribute float a_edge;																				\n\
																									\n\
struct RippleData																					\n\
{																									\n\
	vec2  center;																					\n\
	vec2  coor_center;																				\n\
	int   ripple_type;																				\n\
	float run_time;																					\n\
	float current_radius;																			\n\
};																									\n\
uniform vec2  texture_max;																			\n\
uniform int   ripple_num;																			\n\
// 这里预分配了512个，一般来说，建议进行控制，在128左右通常足够RPG类型使用了								\n\
uniform RippleData ripples[512];																	\n\
																									\n\
#ifdef GL_ES																						\n\
varying mediump vec2 v_texCoord;																	\n\
#else																								\n\
varying vec2 v_texCoord;																			\n\
#endif																								\n\
																									\n\
																									\n\
void main()																							\n\
{																									\n\
	float PI=3.1415927;																				\n\
	float ripple_cycle = 0.4;																		\n\
	float ripple_radius = 400.0;																	\n\
	float life_span = 4.0;																			\n\
	gl_Position = CC_MVPMatrix * a_position;														\n\
	vec2 vertex_pos = a_position.xy;																\n\
	if (ripple_num == 0 || a_edge == 1.0)															\n\
	{																								\n\
		v_texCoord = a_texCoord;																	\n\
	}																								\n\
	else																							\n\
	{																								\n\
		v_texCoord = a_texCoord;																	\n\
		for (int i = 0; i < ripple_num; i++)														\n\
		{																							\n\
			float ripple_distance = distance(ripples[i].center, vertex_pos);						\n\
			float correction = 0.0;																	\n\
			if (ripple_distance < ripples[i].current_radius)										\n\
			{																						\n\
				if (ripples[i].ripple_type == 0)													\n\
				{																					\n\
					correction = sin(2.0 * PI * ripples[i].run_time / ripple_cycle);				\n\
				}																					\n\
				else if (ripples[i].ripple_type == 1)												\n\
				{																					\n\
					correction = sin(2.0 * PI * (ripples[i].current_radius - ripple_distance)/ ripple_radius * life_span / ripple_cycle);				\n\
				}																																		\n\
				else																																	\n\
				{																																		\n\
					correction = (ripple_radius * ripple_cycle / life_span)/(ripples[i].current_radius - ripple_distance);								\n\
					if (correction > 1.0) correction = 1.0;																								\n\
					correction = correction * correction;																								\n\
					correction = sin(2.0 * PI * (ripples[i].current_radius - ripple_distance) / ripple_radius * life_span / ripple_cycle) * correction;	\n\
				}																																		\n\
				correction = correction * (1.0 - ripple_distance / ripples[i].current_radius);															\n\
				correction = correction * (1.0 - ripples[i].run_time / life_span);																		\n\
				correction = correction * 0.1;            																								\n\
				correction = correction / distance(ripples[i].coor_center, v_texCoord);																	\n\
				v_texCoord = v_texCoord + (v_texCoord - ripples[i].coor_center) * correction;															\n\
				v_texCoord = clamp(v_texCoord, vec2(0.0, 0.0), texture_max); 																			\n\
			}																																			\n\
		}																																				\n\
	}																																					\n\
}";
//--------------------------------------------------------
CCRippleSpriteMgr* CCRippleSpriteMgr::CreateSpriteWithFile(const char* filename)
{
	CCRippleSpriteMgr* pgeRippleSprite = new CCRippleSpriteMgr();
	if(pgeRippleSprite && pgeRippleSprite->initWithFile(filename))
	{
		pgeRippleSprite->autorelease();
		return pgeRippleSprite;
	}
	CC_SAFE_DELETE(pgeRippleSprite);
	return NULL;
}
//--------------------------------------------------------
CCRippleSpriteMgr::CCRippleSpriteMgr()
	:m_texture(NULL),
	m_vertice(NULL),
	m_textureCoordinate(NULL),
	m_edgeVertice(NULL),
	m_nTextureMaxIndex(0),
	m_nRippleNumIndex(0),
	m_bIsRippleDirty(false)
{
}
//--------------------------------------------------------
CCRippleSpriteMgr::~CCRippleSpriteMgr()
{
	CC_SAFE_RELEASE(m_texture);
	CC_SAFE_DELETE_ARRAY(m_vertice);
	CC_SAFE_DELETE_ARRAY(m_textureCoordinate);
	CC_SAFE_DELETE_ARRAY(m_edgeVertice);

	// 水纹列表
	RIPPLE_DATA_LIST iterBegin =m_rippleList.begin();
	while (iterBegin != m_rippleList.end())
	{
		rippleData* date = *iterBegin;
		CC_SAFE_DELETE(date);
		iterBegin++;
	}
	m_rippleList.clear();
}
//--------------------------------------------------------
bool CCRippleSpriteMgr::initWithFile(const char* filename)
{
	if (!initShader())
	{
		return false;
	}

	m_texture =CCTextureCache::sharedTextureCache()->addImage(filename);
	if (!m_texture)
	{
		return false;
	}
	m_texture->retain();

	m_vertice = NULL;
	m_textureCoordinate =NULL;
	CC_SAFE_DELETE_ARRAY(m_vertice);
	CC_SAFE_DELETE_ARRAY(m_textureCoordinate);
	CC_SAFE_DELETE_ARRAY(m_edgeVertice);
	m_quadCountX = RIPPLE_DEFAULT_QUAD_COUNT_X;
	m_quadCountY = RIPPLE_DEFAULT_QUAD_COUNT_Y;

	tesselate();

	m_pShaderProgram->use();

	GLfloat texture_max[] = {m_texture->getMaxS(),m_texture->getMaxT()};
	m_pShaderProgram->setUniformLocationWith2fv(m_nTextureMaxIndex, texture_max,1);
	CHECK_GL_ERROR_DEBUG();

	return true;
}
//--------------------------------------------------------
bool CCRippleSpriteMgr::initShader()
{
	CCGLProgram* glProgram = new CCGLProgram();
	if (!glProgram->initWithVertexShaderByteArray( s_szRippleSpriteVSH, s_szRippleSpriteFSH ))
	{
		return false;
	}

	glProgram->addAttribute(kCCAttributeNamePosition,kCCVertexAttrib_Position);
	glProgram->addAttribute(kCCAttributeNameTexCoord,kCCVertexAttrib_TexCoords);
	glProgram->addAttribute("a_edge",RIPPLE_EDGE_ATTR);

	glProgram->link();
	glProgram->updateUniforms();

	m_nTextureMaxIndex = glProgram->getUniformLocationForName("texture_max");
	m_nRippleNumIndex = glProgram->getUniformLocationForName("ripple_num");
	CHECK_GL_ERROR_DEBUG();

	setShaderProgram(glProgram);

	return true;
}
//--------------------------------------------------------
void CCRippleSpriteMgr::draw()
{
	CC_NODE_DRAW_SETUP();

	if (m_texture !=NULL)
	{
		ccGLBindTexture2D(m_texture->getName() );
		ccGLEnableVertexAttribs (kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords  );
		glEnableVertexAttribArray(RIPPLE_EDGE_ATTR);
	}
	
	// 顶点坐标
	glVertexAttribPointer(kCCVertexAttrib_Position,2, GL_FLOAT,GL_FALSE, 0, m_vertice);
	// 纹理坐标
	if (m_texture !=NULL)
	{
		glVertexAttribPointer(kCCVertexAttrib_TexCoords,2, GL_FLOAT,GL_FALSE, 0, m_textureCoordinate);
		glVertexAttribPointer(RIPPLE_EDGE_ATTR,1, GL_FLOAT,GL_FALSE, 0,m_edgeVertice);
	}

	for ( int strip =0; strip < m_quadCountY; strip ++ ) 
	{
		glDrawArrays(GL_TRIANGLE_STRIP, strip *m_VerticesPrStrip, m_VerticesPrStrip );
	}
	CHECK_GL_ERROR_DEBUG();

	CC_INCREMENT_GL_DRAWS(1);
	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite,"CCSprite - draw");
}
//--------------------------------------------------------
void CCRippleSpriteMgr::tesselate()
{
	int vertexPos = 0;
	CCPoint normalized;

	CC_SAFE_DELETE_ARRAY(m_vertice);
	CC_SAFE_DELETE_ARRAY(m_textureCoordinate);
	CC_SAFE_DELETE_ARRAY(m_edgeVertice);

	m_VerticesPrStrip =2 * (m_quadCountX +1);
	m_bufferSize =m_VerticesPrStrip * m_quadCountY;

	// 分配缓冲区
	m_vertice = new CCPoint[m_bufferSize];
	m_textureCoordinate =new CCPoint[m_bufferSize];
	m_edgeVertice =new float[m_bufferSize];
	memset(m_edgeVertice,0, sizeof(int) *m_bufferSize);

	vertexPos =0;

	FKLog("CCRippleSprite::循环开始");
	for (int y =0; y < m_quadCountY; y++)
	{
		for (int x =0; x < (m_quadCountX +1); x++)
		{
			for ( int yy =0; yy < 2; yy ++ ) 
			{
				// first simply calculate a normalized position into rectangle
				normalized.x = (float )x / ( float )m_quadCountX;
				normalized.y = (float )( y + yy ) / ( float )m_quadCountY;


				// calculate vertex by multiplying rectangle ( texture ) size
				CCSize contentSize = m_texture->getContentSize();
				m_vertice[ vertexPos ] = ccp( normalized.x * contentSize.width, normalized.y * contentSize.height );


				// adjust texture coordinates according to texture size
				// as a texture is always in the power of 2, maxS and maxT are the fragment of the size actually used
				// invert y on texture coordinates
				m_textureCoordinate[ vertexPos ] = ccp( normalized.x * m_texture->getMaxS(), m_texture->getMaxT()- ( normalized.y *m_texture->getMaxT() )  );


				// check if vertice is an edge vertice, because edge vertices are never modified to keep outline consistent
				if (( x == 0 ) ||
					( x ==m_quadCountX ) ||
					( ( y ==0 ) && ( yy == 0 ) ) ||
					( ( y == (m_quadCountY - 1 ) ) && ( yy >0 ) ))
				{
					m_edgeVertice[vertexPos] = 1.0f;
				}

				// next buffer pos
				vertexPos ++;
			}
		}
	}
	FKLog("CCRippleSprite::循环结束");
}
//--------------------------------------------------------
void CCRippleSpriteMgr::CreateRipple(cocos2d::CCPoint &pos,RIPPLE_TYPE type, float strength)
{
	rippleData* newRipple;
	// 创建新波纹信息
	newRipple =new rippleData;

	// 默认初始化波纹
	newRipple->parent =true;
	for ( int count =0; count < 4; count ++ ) 
	{
		newRipple->childCreated[ count ] =false;
	}
	newRipple->rippleType = type;
	newRipple->center = pos;

	CCSize contentSize = m_texture->getContentSize();
	newRipple->centerCoordinate =ccp( pos.x / contentSize.width *m_texture->getMaxS(),m_texture->getMaxT() - ( pos.y / contentSize.height *m_texture->getMaxT()) );
	newRipple->radius = RIPPLE_DEFAULT_RADIUS * strength;
	newRipple->strength = strength;
	newRipple->runtime =0;
	newRipple->currentRadius =0;
	newRipple->lifespan =RIPPLE_DEFAULT_LIFESPAN;

	// 将创建好的波纹对象放到运行列表
	m_rippleList.push_back(newRipple);
	m_bIsRippleDirty =true;
}
//--------------------------------------------------------
void CCRippleSpriteMgr::addRippleChild(rippleData* parent,RIPPLE_CHILD type)
{
	rippleData* newRipple;
	CCPoint pos;

	// 创建一个波纹信息
	newRipple =new rippleData;
	// 基本是复制父类的波纹信息
	memcpy( newRipple, parent, sizeof( rippleData ) );

	// 对父子部分不同的信息进行修改
	newRipple->parent =false;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	switch ( type ) 
	{
		// 根据子波纹类型，设置其中心信息
	case RIPPLE_CHILD_LEFT:
		pos =ccp( -parent->center.x, parent->center.y );
		break;
	case RIPPLE_CHILD_TOP:
		pos =ccp( parent->center.x, winSize.height + ( winSize.height - parent->center.y ) );
		break;
	case RIPPLE_CHILD_RIGHT:
		pos =ccp( winSize.width + ( winSize.width - parent->center.x ), parent->center.y );
		break;
	case RIPPLE_CHILD_BOTTOM:
		pos =ccp( parent->center.x, -parent->center.y );
		break;
	default:
		pos =ccp( parent->center.x, -parent->center.y );
		break;
	}
	newRipple->center = pos;
	CCSize contentSize = m_texture->getContentSize();
	newRipple->centerCoordinate =ccp( pos.x / contentSize.width *m_texture->getMaxS(),m_texture->getMaxT()- ( pos.y / contentSize.height *m_texture->getMaxT()) );
	newRipple->strength *= RIPPLE_CHILD_MODIFIER;

	// 通知父类，该子类创建成功
	parent->childCreated[ type ] =true;

	// 将创建好的波纹对象放到运行列表
	m_rippleList.push_back(newRipple);
	m_bIsRippleDirty =true;
}
//--------------------------------------------------------
void CCRippleSpriteMgr::update(float dt)
{
	rippleData* ripple = NULL;
	if ( m_rippleList.size() ==0 ) 
		return; 
	//FKLog("CCRippleSprite::RippleSize = %d", m_rippleList.size() );

	// scan through running ripples
	// the scan is backwards, so that ripples can be removed on the fly
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	REVERSE_RIPPLE_DATA_LIST iterRipple =m_rippleList.rbegin();
	while (  iterRipple != m_rippleList.rend())
	{
		// get ripple data
		ripple = *iterRipple;

		// calculate radius
		ripple->currentRadius = ripple->radius * ripple->runtime / ripple->lifespan;


		// check if ripple should expire
		ripple->runtime += dt;
		if ( ripple->runtime >= ripple->lifespan )
		{
			// 生命周期达到，开始释放
			CC_SAFE_DELETE( ripple );

			RIPPLE_DATA_LIST it = --iterRipple.base() ;
			RIPPLE_DATA_LIST it_after_del = m_rippleList.erase(it);
			iterRipple =std::list<rippleData*>::reverse_iterator(it_after_del);
			m_bIsRippleDirty = true;
		}
		else
		{
			// 检查父类的波纹是否需要创建子类波纹
			if ( ripple->parent ==true ) 
			{
				if ( ( ripple->childCreated[RIPPLE_CHILD_LEFT ] == false ) && ( ripple->currentRadius > ripple->center.x ) ) {
					addRippleChild(ripple, RIPPLE_CHILD_LEFT);
				}
				if ( ( ripple->childCreated[RIPPLE_CHILD_TOP ] == false ) && ( ripple->currentRadius > winSize.height - ripple->center.y ) ) {
					addRippleChild(ripple, RIPPLE_CHILD_TOP);
				}
				if ( ( ripple->childCreated[RIPPLE_CHILD_RIGHT ] == false ) && ( ripple->currentRadius > winSize.width - ripple->center.x ) ) {
					addRippleChild(ripple,RIPPLE_CHILD_RIGHT);
				}
				if ( ( ripple->childCreated[RIPPLE_CHILD_BOTTOM ] == false ) && ( ripple->currentRadius > ripple->center.y ) ) {
					addRippleChild(ripple,RIPPLE_CHILD_BOTTOM);
				}
			}
			iterRipple++;
		}
	}

	iterRipple =m_rippleList.rbegin();

	int ripple_index = 0;
	m_pShaderProgram->use();
	m_pShaderProgram->setUniformLocationWith1i(m_nRippleNumIndex, (int)(m_rippleList.size()));
	CHECK_GL_ERROR_DEBUG();

	while ( iterRipple != m_rippleList.rend() )
	{
		// get ripple data
		ripple = *iterRipple;

		char ripple_attr_name[128] = {0};
		sprintf(ripple_attr_name, "ripples[%d].center", ripple_index);

		GLint ripple_center = m_pShaderProgram->getUniformLocationForName(ripple_attr_name);
		m_pShaderProgram->setUniformLocationWith2fv(ripple_center, (GLfloat*)(&ripple->center),1);
		CHECK_GL_ERROR_DEBUG();

		sprintf(ripple_attr_name, "ripples[%d].coor_center", ripple_index);
		GLint ripple_coor_center = m_pShaderProgram->getUniformLocationForName(ripple_attr_name);
		m_pShaderProgram->setUniformLocationWith2fv(ripple_coor_center, (GLfloat*)(&ripple->centerCoordinate),1);
		CHECK_GL_ERROR_DEBUG();

		sprintf(ripple_attr_name, "ripples[%d].ripple_type", ripple_index);
		GLint rippe_type = m_pShaderProgram->getUniformLocationForName(ripple_attr_name);
		m_pShaderProgram->setUniformLocationWith1i(rippe_type, (GLint)ripple->rippleType);
		CHECK_GL_ERROR_DEBUG();

		sprintf(ripple_attr_name, "ripples[%d].run_time", ripple_index);
		GLint ripple_run_time = m_pShaderProgram->getUniformLocationForName(ripple_attr_name);
		m_pShaderProgram->setUniformLocationWith1f(ripple_run_time, (GLfloat)ripple->runtime);
		CHECK_GL_ERROR_DEBUG();

		sprintf(ripple_attr_name, "ripples[%d].current_radius", ripple_index);
		GLint ripple_radius = m_pShaderProgram->getUniformLocationForName(ripple_attr_name);
		m_pShaderProgram->setUniformLocationWith1f(ripple_radius, (GLfloat)ripple->currentRadius);
		CHECK_GL_ERROR_DEBUG();

		iterRipple++;
		ripple_index++;
	}
}
//--------------------------------------------------------
// CCRippleSpriteLayer
//--------------------------------------------------------
bool CCRippleSpriteLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	FKLog( "当前OpenGL版本为：%s", glGetString(GL_VERSION));

	// 开启Input消息
	this->setTouchEnabled(true);
	// 开启update
	this->scheduleUpdate();

	// 创建RippleSprite对象
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_pRippleSprite = CCRippleSpriteMgr::CreateSpriteWithFile( s_szCommonBackgroundImgPath );
	this->addChild(m_pRippleSprite, 0);
	
	return true;
}
//--------------------------------------------------------
void CCRippleSpriteLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
	cocos2d::CCPoint touchLocation = pTouch->getLocation();

	touchLocation = m_pRippleSprite->convertToNodeSpace(touchLocation);
	m_pRippleSprite->CreateRipple(touchLocation,RIPPLE_TYPE_GEL, 1.2f);
}
//--------------------------------------------------------
void CCRippleSpriteLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
	cocos2d::CCPoint touchLocation = pTouch->getLocation();

	touchLocation = m_pRippleSprite->convertToNodeSpace(touchLocation);
	m_pRippleSprite->CreateRipple(touchLocation,RIPPLE_TYPE_WATER, 0.8f);
}
//--------------------------------------------------------
void CCRippleSpriteLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

}
//--------------------------------------------------------
void CCRippleSpriteLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}
//--------------------------------------------------------
void CCRippleSpriteLayer::update(float dt)
{
	m_pRippleSprite->update(dt);
}
//--------------------------------------------------------
// CRippleSpriteScene
//--------------------------------------------------------
CRippleSpriteScene::CRippleSpriteScene()
{
	CCRippleSpriteLayer *layer = CCRippleSpriteLayer::create();
	this->addChild(layer);
}
//--------------------------------------------------------
void CRippleSpriteScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------