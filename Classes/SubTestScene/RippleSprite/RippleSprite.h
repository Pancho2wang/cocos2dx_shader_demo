/*!
 * \file:	RippleSprite.h
 * \author:	FreeKnight
 * \date:	2014-6-26
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include <iostream>
#include <list>
#include "cocos2d.h"
#include "ccTypes.h"
#include "TestScene.h"
//--------------------------------------------------------
USING_NS_CC;
using std::list;
//--------------------------------------------------------
// �ָ�Ƹ�����������ֵԽ����Խϸ��
#define RIPPLE_DEFAULT_QUAD_COUNT_X             120
#define RIPPLE_DEFAULT_QUAD_COUNT_Y				80

#define RIPPLE_DEFAULT_RADIUS                   300			// ����Ӱ�����뾶
#define RIPPLE_DEFAULT_LIFESPAN                 4.0f		// ÿ�����Ƶ���������
#define RIPPLE_CHILD_MODIFIER                   10.0f		// ���ȵ�����
//--------------------------------------------------------
// ���Ʋ�������
typedef enum {
	RIPPLE_TYPE_RUBBER,										// �𽺿���ʣ����𽺣�
	RIPPLE_TYPE_GEL,										// ��ճ���Һ�����Ͳ���
	RIPPLE_TYPE_WATER,										// ��ճ���Һ�����Ͳ��ʣ���ˮ�ȣ�
} RIPPLE_TYPE;
//--------------------------------------------------------
// �Ӳ��Ƶĳ�������
typedef enum {
	RIPPLE_CHILD_LEFT,
	RIPPLE_CHILD_TOP,
	RIPPLE_CHILD_RIGHT,
	RIPPLE_CHILD_BOTTOM,
	RIPPLE_CHILD_COUNT
} RIPPLE_CHILD;
//--------------------------------------------------------
typedef struct _rippleData {
	bool                    parent;							// �����Ƿ����Ӳ���
	bool                    childCreated[ 4 ];				// �����򣩲����Ƿ������ഴ��
	RIPPLE_TYPE             rippleType;						// ���Ʋ�������
	cocos2d::CCPoint        center;							// ������������
	cocos2d::CCPoint        centerCoordinate;				// �������Ķ�Ӧ����������
	float                   radius;							// �������뾶���յ�������СӰ�죩
	float                   strength;						// ��������
	float                   runtime;						// ���Ƶ�ǰִ��ʱ��
	float                   currentRadius;					// ���Ƶ�ǰ�뾶
	float                   lifespan;						// ���Ƶ���������
} rippleData;
//--------------------------------------------------------
typedef std::list<rippleData*>::iterator					RIPPLE_DATA_LIST;
typedef std::list<rippleData*>::reverse_iterator			REVERSE_RIPPLE_DATA_LIST;
//--------------------------------------------------------
class CCRippleSpriteMgr :public cocos2d::CCNode
{
private:
	ccV3F_C4B_T2F_Quad	m_sQuad;
	int					m_nTextureMaxIndex;
	int					m_nRippleNumIndex;
	bool				m_bIsRippleDirty;
	CCPoint				m_obOffsetPosition;
private:
	void setTextureRect(const CCRect& rect, bool rotated,const CCSize& untrimmedSize);
	void setTextureCoords(CCRect rect);
	void setMyTexture(CCTexture2D *texture);
	void setBatchNode(CCSpriteBatchNode *pobSpriteBatchNode);
	bool initShader();
	void tesselate();
	void addRippleChild(rippleData* parent,RIPPLE_CHILD type);
public:
	CCRippleSpriteMgr();
	~CCRippleSpriteMgr();
public:
	CC_SYNTHESIZE(cocos2d::CCTexture2D*,m_texture, Texture)
	CC_SYNTHESIZE(int,m_quadCountX, QuadCountX)
	CC_SYNTHESIZE(int,m_quadCountY, QuadCountY)
	CC_SYNTHESIZE(int,m_VerticesPrStrip, VerticesPrStrip)
	CC_SYNTHESIZE(int,m_bufferSize, BuffSize)
	CC_SYNTHESIZE(cocos2d::CCPoint*,m_vertice, Vertice)
	CC_SYNTHESIZE(cocos2d::CCPoint*,m_textureCoordinate, TextureCoordinate)
	CC_SYNTHESIZE_READONLY(float*,m_edgeVertice, EdgeVertice)
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::list<rippleData*>,m_rippleList, RippleList)
public:
	static CCRippleSpriteMgr* CreateSpriteWithFile(const char* filename);
public:
	void CreateRipple(cocos2d::CCPoint &pos,RIPPLE_TYPE type, float strength);
public:
	virtual bool initWithFile(const char* filename);
	virtual void draw();
	virtual void update(float dt);
};
//--------------------------------------------------------
class CCRippleSpriteLayer : public CCLayer
{
public:
	CREATE_FUNC(CCRippleSpriteLayer);
public:
	virtual bool	init();

	virtual void	ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	virtual void	update(float dt);
private:
	CCRippleSpriteMgr*	m_pRippleSprite;
};
//--------------------------------------------------------
class CRippleSpriteScene : public CTestScene
{
public:
	CRippleSpriteScene();
public:
	void Run();
};
//--------------------------------------------------------