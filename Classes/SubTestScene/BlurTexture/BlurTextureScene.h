/*!
 * \file:	BlurTextureScene.h
 * \author:	FreeKnight
 * \date:	2014-6-30
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "AWTextureFilter.h"
#include "Texture2DMutable.h"
#include "TestScene.h"
//--------------------------------------------------------
class CFullFilterLayer : public CCLayer
{
public:
	virtual bool init();
public:
	CREATE_FUNC( CFullFilterLayer );
};
//--------------------------------------------------------
class CPartialFilterLayer : public CCLayer
{
public:
	virtual bool init();
public:
	CREATE_FUNC( CPartialFilterLayer );
};
//--------------------------------------------------------
class CTextShadow : public CCLayer
{
public:
	virtual bool init();
public:
	CREATE_FUNC( CTextShadow );
public:
	virtual void update( float delta );
private:
	void setText( CCString* p_Text );
private:
	int			m_nNum;
};
//--------------------------------------------------------
class CBlurTextureLayer : public CCLayer
{
public:
	virtual bool init();
public:
	CREATE_FUNC( CBlurTextureLayer );
private:
	int				m_nCurSubLayerIndex;
};
//--------------------------------------------------------
class CBlurTextureScene : public CTestScene
{
public:
	CBlurTextureScene();
public:
	void Run();
};
//--------------------------------------------------------