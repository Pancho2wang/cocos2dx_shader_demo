/*!
 * \file:	ShaderNode.h
 * \author:	FreeKnight
 * \date:	2014-6-27
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "TestScene.h"
//--------------------------------------------------------
USING_NS_CC;
//--------------------------------------------------------
class CShaderNode : public CCSprite
{
public:
	CShaderNode();
	bool initWithVertex(const char* image, const char *vert, const char *frag);
	static CShaderNode* Create( const char* image, const char *vert, const char *frag);
private:
	void initShader(const char *vert, const char *frag);
public:
	virtual void update(float delta);
	virtual void setContentSize(const CCSize& var);
	virtual void setColor(ccColor4F newColor);
	virtual void draw();
private:
	GLuint      m_unUniformResolution, m_unUniformTime, m_unUniformTex0;
	GLuint      m_unAttributeColor, m_unAttributePosition;
	float       m_fTime;
	ccVertex2F  m_tagResolutionPos,m_tagCenterPos;
	GLuint      m_pTexture;
	GLfloat     m_fColor[4];
};
//--------------------------------------------------------
class CShaderNodeScene : public CTestScene
{
public:
	CShaderNodeScene();
public:
	void Run();
};
//--------------------------------------------------------