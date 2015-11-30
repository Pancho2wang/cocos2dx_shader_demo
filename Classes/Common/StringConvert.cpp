/********************************************************************
	created:	2014/03/05
	created:	5:3:2014   18:22
	filename: 	E:\Workspace\cocos2d_x_215b_cocostudio0400\cocos2d-x_v2.1.5b\cocos2d-x\projects\FKAvgTest\Classes\StringConvert.cpp
	file path:	E:\Workspace\cocos2d_x_215b_cocostudio0400\cocos2d-x_v2.1.5b\cocos2d-x\projects\FKAvgTest\Classes
	file base:	StringConvert
	file ext:	cpp
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#include "StringConvert.h"

//--------------------------------------------------------------------
namespace FKStringConvert
{
	//--------------------------------------------------------------------
	int code_convert( const char* from_charset, const char* to_charset, const char* inbuf, size_t inlen, char* outbuf, size_t outlen )
	{
		iconv_t cd;
		const char* temp = inbuf;
		const char** pin = &temp;
		char** pout = &outbuf;
		memset( outbuf, 0, outlen );
		cd = iconv_open( to_charset, from_charset );
		if( cd == 0 ) return -1;
		if( iconv( cd, pin, &inlen, pout, &outlen ) == -1 ) 
		{
			return -1;
		}
		iconv_close( cd );
		return 0;
	}
	//--------------------------------------------------------------------
	/* UTF8 -> GB2312 */
	std::string u2a( const char* inbuf )
	{
		size_t inlen = strlen( inbuf );
		char* outbuf = new char[inlen * 2 + 2 ];
		std::string strRet;
		if( code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, inlen * 2 + 2 ) == 0 )
		{
			strRet = outbuf;
		}
		delete [] outbuf;
		return strRet;
	}
	//--------------------------------------------------------------------
	/* GB2312 -> UTF8 */
	std::string a2u( const char* inbuf )
	{
		size_t inlen = strlen( inbuf );
		char* outbuf = new char[inlen * 2 + 2];
		std::string strRet;
		if( code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2 ) == 0 )
		{
			strRet = outbuf;
		}
		delete [] outbuf;
		return strRet;
	}
	//--------------------------------------------------------------------
}