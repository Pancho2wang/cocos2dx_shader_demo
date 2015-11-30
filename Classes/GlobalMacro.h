/*!
 * \file:	GlobalMacro.h
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
// ����ѡ�����������
const static unsigned int TEST_MENU_LINE_SPACE	= 40;
//--------------------------------------------------------
// ���Գ�������
enum ENUM_TEST_SCENE_TYPE
{
	// to do add...
	eTST_Ripple_UseGrid3D	= 0,
	eTST_RippleSprite_UseShader,
	eTST_RenderTexture,
	eTST_Wave3DAction,
	eTST_ShaderNode,
	eTST_RemoveBKSprite,
	eTST_SpotLight,
	eTST_ActiveGrass,
	eTST_ScrartchBursh,
	eTST_LaserLine,
	eTST_SuckAction,
	eTST_LabelShuffleTile,
	eTST_BlueTexture,
	eTST_ImageProcess,
	eTST_ColorSprite_UseShader,
	eTST_EmbossSprite,
	eTST_Wave1D,

	eTestSceneType_Max,
};
const std::string g_aTestNames[eTestSceneType_Max] = {
	"1:cocos2dx Grid3d �Ĳ������� [ �ɵ������ק ]",
	"2:shaderʵ�ֲ������� [ �ɵ������ק ]",
	"3:cocos2dx RenderTexture �Ľ���ɫ�������ϣ���̬����α���� [ �ɵ�� ]",
	"4:cocos2dx Wave3D ����ʵ�ֵ�ˮ�沨��",
	"5:shaderʵ��ˮ�沨��",
	"6:shaderʵ�־�ȷȥ��Alpha���ͼƬ����ɫ",
	"7:cocos2dx RenderTexture �ľ۹��Ч��[ ����ק ]",
	"8:shaderʵ�ֶ�̬��Ч��",
	"9:cocos2dx RenderTexture �Ĺν�Ч�� [ �ɵ������ק ]",
	"10:cocos2dx ProgressTimer �ļ���Ч�� [ �ɵ�� ]",
	"11:cocos2dx Grid3D �ĺڶ�����Ч��",
	"12:cocos2dx Action ����Ч����Ч��",
	"13:cocos2dx Texture2D ��CPU������ģ�����ֲ�ģ����ģ��������ӰЧ��",
	"14:shaderʵ��ͼƬЧ��:��˹ģ��,�˶�ģ��,ĥɰ,����,��ʴ,Laplacian/Sobel/Prewitt���",
	"15:shaderʵ��ͼƬЧ��:����Ƭ,��ɫ,�߹�,�Ա�,����,�ڰ�,��Ե,���׸���",
	"16:shaderʵ��ͼƬЧ��:��ϸ����,��Ƭ",
	"17:CPU����ģ��1Dˮ��Ч��",
};
//--------------------------------------------------------