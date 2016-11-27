/** Windowsプログラムの雛形
*  @Auther Yuhei
*/

#ifndef	__MAP_EDITOR__
#define	__MAP_EDITOR__


#include<windows.h>
#include "MapEditor/resource.h"

//チップセットのサイズ
const int	CWIDTH	= 16;
const int	CHEIGHT	= 16;

//親Windowを取り扱う基本クラス
class MAP_EDITOR{
private:
	HINSTANCE	hInst;			//インスタンスハンドル

	//ウィンドウハンドル
	HWND		hWnd;				//ウィンドウハンドル
	HWND		hWndPalette;		//パレット（子ウィンドウ）
	HWND		hWndPaletteClient;	//パレットの描画領域（子ウィンドウ）
	HWND		hWndCombo;			//パレットのコンボボックス


	//オープンファイルネーム
	OPENFILENAME	ofn;


	//エディタ
	//マップのサイズ
	int		map_width;
	int		map_height;



	//パレット
	//ウィンドウサイズ
	const static int	Palette_width	= 240;
	const static int	Palette_height	= 320;

	//描画領域のメモリデバイスコンテキスト
	HDC			hDC_Palette;		//パレット

	//スクロールインフォ
	SCROLLINFO		m_VSInfo;		//スクロール情報
	SCROLLINFO		m_HSInfo;		//横スクロール情報

	//描画オブジェクト
	HPEN			hPenSelected;	//選択された領域を描画するためのペン

	//今選択されているパレットの矩形
	RECT			rcSelected;


	const static char	szClassName[];
	const static char	szClassNamePalette[];
	static bool			once;

	//パレット
	bool InitPalette();
	bool InitPalette_Client();

	//オープンファイルネーム構造体初期化
	bool InitOpenFileNameStr(HWND hWnd, HINSTANCE hInst);

public:
	//インスタンス取得
	static MAP_EDITOR&	GetInst();

	//コンストラクタ
	MAP_EDITOR();

	//デストラクタ
	~MAP_EDITOR();

	//初期化、ウィンドウ作成
	//メインウィンドウ
	bool Initialize(HINSTANCE hInst);

	//マップサイズ設定
	bool SetMapSize(int x, int y);

	//メッセージループ（メイン処理）
	int MessLoop();

	//ウィンドウプロシージャ
	//親ウィンドウ
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//パレット
	static LRESULT CALLBACK WndProcPalette(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);			//本体
	static LRESULT CALLBACK WndProcPalette_Client(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	//描画領域
};


#endif
