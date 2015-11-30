/*!
 * \file:	GlobalMacro.h
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
// 测试选择面板字体间隔
const static unsigned int TEST_MENU_LINE_SPACE	= 40;
//--------------------------------------------------------
// 测试场景类型
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
	"1:cocos2dx Grid3d 的波浪涟漪 [ 可点击，拖拽 ]",
	"2:shader实现波浪涟漪 [ 可点击，拖拽 ]",
	"3:cocos2dx RenderTexture 的渐变色，纹理混合，动态纹理，伪光照 [ 可点击 ]",
	"4:cocos2dx Wave3D 动作实现的水面波纹",
	"5:shader实现水面波纹",
	"6:shader实现精确去除Alpha混合图片背景色",
	"7:cocos2dx RenderTexture 的聚光灯效果[ 可拖拽 ]",
	"8:shader实现动态草效果",
	"9:cocos2dx RenderTexture 的刮奖效果 [ 可点击，拖拽 ]",
	"10:cocos2dx ProgressTimer 的激光效果 [ 可点击 ]",
	"11:cocos2dx Grid3D 的黑洞吸入效果",
	"12:cocos2dx Action 的特效字体效果",
	"13:cocos2dx Texture2D 的CPU计算获得模糊，局部模糊，模糊文字阴影效果",
	"14:shader实现图片效果:高斯模糊,运动模糊,磨砂,膨胀,侵蚀,Laplacian/Sobel/Prewitt描边",
	"15:shader实现图片效果:老照片,反色,高光,对比,饱和,黑白,边缘,简易浮雕",
	"16:shader实现图片效果:精细浮雕,底片",
	"17:CPU物理模拟1D水面效果",
};
//--------------------------------------------------------