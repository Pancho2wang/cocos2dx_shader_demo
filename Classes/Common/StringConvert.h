/********************************************************************
	created:	2014/03/04
	created:	4:3:2014   15:41
	filename: 	E:\Workspace\cocos2d_x_215b_cocostudio0400\cocos2d-x_v2.1.5b\cocos2d-x\projects\FKAvgTest\Classes\StringConvert.h
	file path:	E:\Workspace\cocos2d_x_215b_cocostudio0400\cocos2d-x_v2.1.5b\cocos2d-x\projects\FKAvgTest\Classes
	file base:	StringConvert
	file ext:	h
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <iostream>
#include <cstring>
#pragma comment( lib,	"libiconv.lib" )
#include "iconv\iconv.h"
//--------------------------------------------------------------------
namespace FKStringConvert
{
	int code_convert( const char* from_charset, const char* to_charset, 
		const char* inbuf, size_t inlen, char* outbuf, size_t outlen );
	/* UTF8 - GB2312 */
	std::string u2a( const char* inbuf );
	/* GB2312 - UTF8 */
	std::string a2u( const char* inbuf );
}