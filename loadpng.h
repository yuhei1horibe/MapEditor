#ifndef _LOADPNG_H__
#define _LOADPNG_H__

#include<string>

using namespace std;

//---------------------------------------------------------------------------
// PNG�t�@�C����ǂݍ���
HBITMAP LoadPng(const string &strFilename);
HBITMAP	LoadPngFromHandle(HANDLE hFile);

#endif //_LOADPNG_H__
