
#include "MapEditor.h"

bool MAP_EDITOR::SetMapSize(int x, int y)
{
	if((x < 20) || (x > 512) || (y < 20) || (y > 512)){
		MessageBox(NULL, "マップのサイズは20～512Chip×20～512Chipの間で指定してください", "Caution", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	map_width	= x;
	map_height	= y;

	return true;
}

/*
//パレットのウィンドウ作成
bool MAP_EDITOR::InitPalette()
{
	//ウィンドウクラス
	WNDCLASSEX	wc;			//親ウィンドウ
	RECT		rc;			//親ウィンドウの領域
	int			x;
	int			y;


	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.style		= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	= &MAP_EDITOR::WndProcPalette;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	= hInst;
	wc.hIcon		= (HICON)LoadImage(NULL,
							MAKEINTRESOURCE(IDI_APPLICATION),
							IMAGE_ICON, 
							0, 
							0, 
							LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor		= (HCURSOR)LoadImage(NULL,
							MAKEINTRESOURCE(IDC_ARROW),
							IMAGE_CURSOR,
							0,
							0,
							LR_DEFAULTSIZE | LR_SHARED);
	wc.hbrBackground	= (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= (LPCSTR)szClassNamePalette;
	wc.hIconSm			= (HICON)LoadImage(NULL,
							MAKEINTRESOURCE(IDI_APPLICATION),
							IMAGE_ICON,
							0,
							0,
							LR_DEFAULTSIZE | LR_SHARED);

	if(RegisterClassEx(&wc) == NULL)
	{
		MessageBox(NULL, "ウィンドウクラスの登録に失敗(Palette)。", "RegisterClass Fault", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	//親ウィンドウのサイズに合わせる
	GetClientRect(hWnd, &rc);

	x	= rc.right - Palette_width - 10;
	y	= rc.bottom - Palette_height - 10;

	//ウィンドウ作成
	hWndPalette = CreateWindow(szClassNamePalette,	//クラス名
						"パレット",					//ウィンドウ名（タイトルバーに表示）
						WS_OVERLAPPEDWINDOW | \
						WS_CHILD,					//ウィンドウスタイル
						x >= 0 ? x : 0,				//x座標
						y >= 0 ? y : 0,				//y座標
						Palette_width,				//ウィンドウの幅
						Palette_height,				//ウィンドウの高さ
						hWnd,						//親ウィンドウハンドル
						NULL,						//メニューハンドル、もしくは子ウィンドウID
						hInst,						//インスタンスハンドル
						NULL						//ウィンドウ作成データ
						);

	if(!hWndPalette)
		return FALSE;

	ShowWindow(hWndPalette, SW_SHOW);
	UpdateWindow(hWndPalette);

	//描画領域
	return InitPalette_Client();
}

//描画領域
bool MAP_EDITOR::InitPalette_Client()
{
	//ウィンドウクラス
	WNDCLASSEX	wc;			//親ウィンドウ
	RECT		rc;			//親ウィンドウの領域
	int			width;
	int			height;

	//コンボボックス処理
	static int		index;
	char			text[512];
	HANDLE			hFind;		//ディレクトリのファイルを列挙
	WIN32_FIND_DATA	w32fd;


	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.style		= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	= &MAP_EDITOR::WndProcPalette_Client;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	= hInst;
	wc.hIcon		= (HICON)LoadImage(NULL,
							MAKEINTRESOURCE(IDI_APPLICATION),
							IMAGE_ICON, 
							0, 
							0, 
							LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor		= (HCURSOR)LoadImage(NULL,
							MAKEINTRESOURCE(IDC_ARROW),
							IMAGE_CURSOR,
							0,
							0,
							LR_DEFAULTSIZE | LR_SHARED);
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "PaletteClient";
	wc.hIconSm			= (HICON)LoadImage(NULL,
							MAKEINTRESOURCE(IDI_APPLICATION),
							IMAGE_ICON,
							0,
							0,
							LR_DEFAULTSIZE | LR_SHARED);

	if(RegisterClassEx(&wc) == NULL)
	{
		MessageBox(NULL, "ウィンドウクラスの登録に失敗(Palette描画領域)。", "RegisterClass Fault", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	//親ウィンドウのサイズに合わせる
	GetClientRect(hWndPalette, &rc);

	width	= rc.right - rc.left - 10;
	height	= rc.bottom - rc.top - 40;

	//ウィンドウ作成
	hWndPaletteClient = CreateWindowEx(
						WS_EX_CLIENTEDGE,			//拡張スタイル
						"PaletteClient",			//クラス名
						"描画領域",					//ウィンドウ名（タイトルバーに表示）
						WS_CHILD | \
						WS_VISIBLE | \
						WS_VSCROLL | \
						WS_HSCROLL,					//ウィンドウスタイル
						5,							//x座標
						35,							//y座標
						width,						//ウィンドウの幅
						height,						//ウィンドウの高さ
						hWndPalette,				//親ウィンドウハンドル
						NULL,						//メニューハンドル、もしくは子ウィンドウID
						hInst,						//インスタンスハンドル
						NULL						//ウィンドウ作成データ
						);

	if(!hWndPaletteClient){
		MessageBox(NULL, "ウィンドウ作成に失敗(Palette描画領域)。", "RegisterClass Fault", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	ShowWindow(hWndPaletteClient, SW_SHOW);
	UpdateWindow(hWndPaletteClient);

	return TRUE;
}






//ウィンドウプロシージャ
//パレット本体
LRESULT CALLBACK MAP_EDITOR::WndProcPalette(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MAP_EDITOR&	Instance = MAP_EDITOR::GetInst();	//メンバにアクセスするための物
	RECT		rcParent;
	RECT		rcPalette;
	RECT		rcClient;
	int			width;
	int			height;

	//選択されたリスト番号
	int			index;

	//ファイル名
	char		CurrentDir[512];
	char		FileName[512];

	//ビットマップハンドル
	HBITMAP		hBmp;
	BITMAP		bmpInfo;

	//ウィンドウのデバイスコンテキスト
	HDC			hdc;

	DWORD		msg1;

	switch(msg)
	{
	case WM_SIZE:
		GetClientRect(hWnd, &rcPalette);

		width				= rcPalette.right - rcPalette.left - 10;
		height				= rcPalette.bottom - rcPalette.top - 40;

		//描画領域をリサイズ
		MoveWindow(Instance.hWndPaletteClient, rcPalette.left + 5, rcPalette.top + 35, width, height, false);

		//コンボボックスをリサイズ
		MoveWindow(Instance.hWndCombo, rcPalette.left + 5, rcPalette.top + 5, width, 250, false);

		//スクロールバーを設定
		GetClientRect(Instance.hWndPaletteClient, &rcClient);
		Instance.m_VSInfo.nPage		= rcClient.bottom - rcClient.top;			//ページの高さ
		SetScrollInfo(Instance.hWndPaletteClient, SB_VERT, &Instance.m_VSInfo, TRUE);

		Instance.m_HSInfo.nPage		= rcClient.right - rcClient.left;			//ページの幅
		SetScrollInfo(Instance.hWndPaletteClient, SB_HORZ, &Instance.m_HSInfo, TRUE);

		break;

	//コンボボックスの処理
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		//コンボボックス
		case IDC_COMBO1:
			switch(HIWORD(wParam)){

			//フォーカスが移った時に描画するビットマップを切り替える
			case CBN_SELCHANGE:
				GetCurrentDirectory(512, CurrentDir);

				index	= SendMessage(Instance.hWndCombo, CB_GETCURSEL, 0, 0);
				SendMessage(Instance.hWndCombo, CB_GETLBTEXT, index, (LPARAM)FileName);

				strcat(CurrentDir, "\\ChipSets\\");
				strcat(CurrentDir, FileName);

				//MessageBox(hWnd, CurrentDir, "test", MB_OK);

				//パレットにビットマップを表示させるためのデバイスコンテキストを作成
				hdc		= GetDC(hWnd);
				DeleteDC(Instance.hDC_Palette);

				Instance.hDC_Palette	= CreateCompatibleDC(hdc);

				ReleaseDC(hWnd, hdc);

				//png読み込み
				hBmp	= LoadPng(CurrentDir);

				GetClientRect(Instance.hWndPaletteClient, &rcClient);

				//スクロールバーを設定
				GetObject(hBmp, (int)sizeof(BITMAP), &bmpInfo);
				Instance.m_VSInfo.nPos		= 0;
				Instance.m_VSInfo.nMax		= bmpInfo.bmHeight - 1;						//高さの上限
				Instance.m_VSInfo.nPage		= rcClient.bottom - rcClient.top;			//ページの高さ
				SetScrollInfo(Instance.hWndPaletteClient, SB_VERT, &Instance.m_VSInfo, TRUE);

				Instance.m_HSInfo.nPos		= 0;
				Instance.m_HSInfo.nMax		= bmpInfo.bmWidth - 1;						//幅の上限
				Instance.m_HSInfo.nPage		= rcClient.right - rcClient.left;			//ページの幅
				SetScrollInfo(Instance.hWndPaletteClient, SB_HORZ, &Instance.m_HSInfo, TRUE);

				//選択領域が読み込んだビットマップの外に出てしまう時はリセット
				if((Instance.rcSelected.right > Instance.m_HSInfo.nMax) || (Instance.rcSelected.bottom > Instance.m_VSInfo.nMax)){
					Instance.rcSelected.left	= 0;
					Instance.rcSelected.top		= 0;
					Instance.rcSelected.right	= CWIDTH;
					Instance.rcSelected.bottom	= CHEIGHT;
				}

				//メモリデバイスコンテキストにビットマップを選択
				SelectObject(Instance.hDC_Palette, hBmp);
				InvalidateRect(Instance.hWndPaletteClient, NULL, true);

				DeleteObject(hBmp);
			}
			break;

		default:
			return (DefWindowProc(hWnd, msg, wParam, lParam));
		}
		break;

	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return (DefWindowProc(hWnd, msg, wParam, lParam));

	}

	return 0;
}
*/

