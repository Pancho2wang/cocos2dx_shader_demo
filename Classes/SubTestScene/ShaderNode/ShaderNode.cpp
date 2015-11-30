/*!
 * \author:	FreeKnight
 * \date:	2014-6-27
 * \comment:
 */
//--------------------------------------------------------
#include "ShaderNode.h"
#include "Resource.h"
#include "Common/FKCommon.h"
//--------------------------------------------------------
static const GLchar* s_szShaderNodeVSH = 
	"												\n\
	attribute vec4 a_color;  						\n\
	attribute vec4 a_position;  					\n\
	varying vec4 v_color;  							\n\
	void main()  									\n\
	{  												\n\
		v_color = a_color;  						\n\
		gl_Position = CC_MVPMatrix * a_position;	\n\
	}";
//--------------------------------------------------------
static const GLchar* s_szShaderNodeFSH = 
	"																\n\
	varying vec4 v_color;  											\n\
																	\n\
	uniform sampler2D tex0;  										\n\
	precision highp float;  										\n\
	uniform float time;  											\n\
	uniform vec2 resolution;  										\n\
	const float PI = 3.1415926535897932;  							\n\
																	\n\
	// 速度															\n\
	const float speed = 0.2;  										\n\
	const float speed_x = 0.3;  									\n\
	const float speed_y = 0.3;  									\n\
																	\n\
	// 折射角														\n\
	const float emboss = 0.3; 		// 凹凸强度						\n\
	const float intensity = 2.4;	// 强度  						\n\
	const int steps = 8;  			// 波纹密度						\n\
	const float frequency = 4.0;  	// 频率							\n\
	const int angle = 7; 			// 最好是素数					\n\
																	\n\
	const float delta = 30.0;  		// 增幅（越小越激烈）				\n\
	const float intence = 200.0;   	// 明暗强度						\n\
																	\n\
	// 高光															\n\
	const float reflectionCutOff = 0.012;							\n\
	const float reflectionIntence = 80000.;							\n\
																	\n\
	float col(vec2 coord)  											\n\
	{  																\n\
		float delta_theta = 2.0 * PI / float(angle);  				\n\
		float col = 0.0;  											\n\
		float theta = 0.0;  										\n\
		for (int i = 0; i < steps; i++)  							\n\
		{															\n\
			vec2 adjc = coord;										\n\
			theta = delta_theta * float(i);							\n\
			adjc.x += cos(theta)*time*speed + time * speed_x;  		\n\
			adjc.y -= sin(theta)*time*speed - time * speed_y;		\n\
			col = col + cos((adjc.x * cos(theta) - 					\n\
				adjc.y * sin(theta)) * frequency) * intensity;  	\n\
		}															\n\
		return cos(col);											\n\
	}  																\n\
																	\n\
	void main(void)  												\n\
	{  																\n\
		vec2 p = (gl_FragCoord.xy) / resolution.xy, c1 = p, c2 = p; \n\
		float cc1 = col(c1);  										\n\
																	\n\
		c2.x += resolution.x/delta;  								\n\
		float dx = emboss*(cc1-col(c2))/delta;  					\n\
																	\n\
		c2.x = p.x;  												\n\
		c2.y += resolution.y/delta;  								\n\
		float dy = emboss*(cc1-col(c2))/delta;  					\n\
																	\n\
		c1.x += dx;													\n\
		//1: 														\n\
		//c1.y = -(c1.y+dy*2.);  									\n\
		//2: 														\n\
		//c1.y += dy;												\n\
		//3:														\n\
		c1.y = -(c1.y+dy);											\n\
		c1.y = - c1.y;												\n\
		float alpha = 1.+dot(dx,dy)*intence;  						\n\
																	\n\
		float ddx = dx - reflectionCutOff;							\n\
		float ddy = dy - reflectionCutOff;							\n\
		if (ddx > 0. && ddy > 0.)									\n\
			alpha = pow(alpha, ddx*ddy*reflectionIntence);			\n\
																	\n\
		vec4 col = texture2D(tex0,c1);								\n\
		col *= alpha;												\n\
		//col *= v_color;											\n\
		//col *= alpha;												\n\
		gl_FragColor =  col;										\n\
	}";
//--------------------------------------------------------
// CShaderNode
//--------------------------------------------------------
CShaderNode::CShaderNode()
{

}
//--------------------------------------------------------
CShaderNode* CShaderNode::Create(const char* image, const char *vert, const char *frag)
{
	CShaderNode* shader = new CShaderNode();
	if(shader && shader->initWithVertex(image,vert,frag))
	{
		shader->autorelease();
		return shader;
	}

	CC_SAFE_DELETE(shader);
	return NULL;
}
//--------------------------------------------------------
void CShaderNode::initShader( const char *vert, const char *frag)
{
	CCGLProgram* shader = new CCGLProgram();  
	shader->initWithVertexShaderByteArray(vert, frag);   //载入着色器程序  
	//使用着色器程序  
	this->setShaderProgram(shader);  
	shader->release();  
	CHECK_GL_ERROR_DEBUG(); 

	//绑定attribute变量  
	this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);  
	this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);  
	this->getShaderProgram()->link();  
	CHECK_GL_ERROR_DEBUG(); 

	//获取attribute变量标识  
	m_unAttributeColor = glGetAttribLocation(this->getShaderProgram()->getProgram(), kCCAttributeNameColor);  
	m_unAttributePosition = glGetAttribLocation(this->getShaderProgram()->getProgram(), kCCAttributeNamePosition);  
	this->getShaderProgram()->updateUniforms();  
	CHECK_GL_ERROR_DEBUG(); 

	//获取uniform变量标识  
	m_unUniformResolution = glGetUniformLocation(this->getShaderProgram()->getProgram(), "resolution");  
	m_unUniformTime = glGetUniformLocation(this->getShaderProgram()->getProgram(), "time");  
	m_unUniformTex0 = glGetUniformLocation(this->getShaderProgram()->getProgram(), "tex0");  

}
//--------------------------------------------------------
void CShaderNode::setColor(ccColor4F newColor)
{
	m_fColor[0] = newColor.r;
	m_fColor[1] = newColor.g;
	m_fColor[2] = newColor.b;
	m_fColor[3] = newColor.a;
}
//--------------------------------------------------------
bool CShaderNode::initWithVertex( const char* image,const char *vert, const char *frag)
{
	initShader(vert,frag);
	CCTexture2D* pTex = CCTextureCache::sharedTextureCache()->addImage( image );
	if( pTex == NULL )
	{
		return false;
	}
	m_pTexture = pTex->getName();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize( winSize );//pTex->getContentSize() );
	setPosition(ccp(winSize.width / 2 - getContentSize().width / 2,
		winSize.height / 2 - getContentSize().height / 2 ));

	m_fTime = 0;

	// 开启自更新
	scheduleUpdate();
	return true;
}
//--------------------------------------------------------
void CShaderNode::update(float dt)
{
	m_fTime += dt;
}
//--------------------------------------------------------
void CShaderNode::setContentSize(const CCSize& var)
{
	CCNode::setContentSize(var);
	m_tagResolutionPos = vertex2(getContentSize().width,getContentSize().height);
	m_tagCenterPos.x = m_tagResolutionPos.x/2;
	m_tagCenterPos.y = m_tagResolutionPos.y/2;
}
//--------------------------------------------------------
void CShaderNode::draw()
{
	CC_NODE_DRAW_SETUP();  

	//传递uniform变量  
	CCGLProgram* shader = getShaderProgram();  
	shader->setUniformLocationWith2f(m_unUniformResolution, m_tagResolutionPos.x, m_tagResolutionPos.y);
	shader->setUniformLocationWith1i(m_unUniformTex0, 0); 
	glUniform1f(m_unUniformTime, m_fTime);

	//获取attribute变量  
	CCSize size = this->getContentSize();  
	float w = size.width;  
	float h = size.height;  

	ccGLBindTexture2D(m_pTexture);								//绑定纹理到槽位  
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0); //截取屏幕数据到纹理  
	glEnableVertexAttribArray(m_unAttributePosition); 

	//传递attribute变量  
	GLfloat vertices[12] = {  
		0, 0, //左下0
		w, 0, //右下1
		w, h, //右上2
		0, 0, //左下0
		0, h, //左上3
		w, h, //右上2
	};  
	glVertexAttribPointer(m_unAttributePosition, 2, GL_FLOAT, GL_FALSE, 0, vertices);  
	glVertexAttrib4fv(m_unAttributeColor, m_fColor);  
	//绘制  
	glDrawArrays(GL_TRIANGLES, 0, 6);  
}
//--------------------------------------------------------
// CShaderNodeScene
//--------------------------------------------------------
CShaderNodeScene::CShaderNodeScene()
{
	CCLayerColor *layer = CCLayerColor::create();
	layer->setColor( ccGRAY );
	addChild(layer);

	CCSprite* pBK = CCSprite::create( s_szCommonBackgroundImgPath );
	if( pBK )
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		pBK->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		layer->addChild( pBK );
	}

	CShaderNode* pShaderNode = CShaderNode::Create( s_szShaderNodeShaderAreaMask, 
		s_szShaderNodeVSH, s_szShaderNodeFSH );

	pShaderNode->setColor(ccc4f(0.0f,0.0f,0.0f,0.5f));
	this->addChild(pShaderNode,2);
}
//--------------------------------------------------------
void CShaderNodeScene::Run()
{
	CCDirector::sharedDirector()->replaceScene(this);
}
//--------------------------------------------------------