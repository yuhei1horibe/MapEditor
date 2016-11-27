#ifndef _LOADPNG_H__
#define _LOADPNG_H__

#include<string>

using namespace std;

//---------------------------------------------------------------------------
// PNGÉtÉ@ÉCÉãÇì«Ç›çûÇﬁ
HBITMAP LoadPng(const string &strFilename);
HBITMAP	LoadPngFromHandle(HANDLE hFile);

#endif //_LOADPNG_H__
