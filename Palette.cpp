
#include "MapEditor.h"
#include "loadpng.h"


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

	//コンボボックスを作成
	hWndCombo = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"COMBOBOX",
		NULL,
		CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_VSCROLL,
		rc.left + 5,
		rc.top + 5,
		width,
		250,
		hWndPalette,
		(HMENU)IDC_COMBO1,
		hInst,
		NULL);

	if(hWndCombo == NULL)
		return FALSE;

	//コンボボックスに"ChipSets"ディレクトリ内のファイルを列挙して追加
	GetCurrentDirectory(512, text);
	strcat(text, "\\ChipSets\\*.png");

	hFind	= FindFirstFile(text, &w32fd);
	if(hFind != INVALID_HANDLE_VALUE){
		index	= 0;
		do {
			//pngファイル名をドロップダウンに追加
			SendMessage(hWndCombo, CB_ADDSTRING, -1, (LPARAM)w32fd.cFileName);
			index++;
		} while(FindNextFile(hFind, &w32fd));

		//ハンドルをクローズ
		FindClose(hFind);

		//TODO
		//ドロップダウンリストのアイテムを選択
		//SendMessage(hWndCombo, CB_SETCURSEL, 0, 0);

		//起動時にチップセットを読み込ませるためのメッセージ
		//PostMessage(hWndPalette, WM_COMMAND, (CBN_SELCHANGE << 32) | IDC_COMBO1, 0);
	}

	else{
		MessageBox(hWnd, "ChipSetsのディレクトリを開けません", "Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	//ペンを作成
	hPenSelected	= CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

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

//描画領域
LRESULT CALLBACK MAP_EDITOR::WndProcPalette_Client(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MAP_EDITOR&	Instance = MAP_EDITOR::GetInst();	//メンバにアクセスするための物

	//デバイスコンテキスト等
	HDC			hdc;
	PAINTSTRUCT	ps;

	//描画領域の矩形
	RECT		rcClient;
	RECT		rcMouse;		//マウスの位置に四角を描画

	//クリック中を判定
	static bool	bLButtonDown	= false;

	//前回描いた矩形の位置を記憶
	static RECT	rcPrevMouse;

	static int	mouse_x;
	static int	mouse_y;
	POINTS		pt;

	switch(msg)
	{
	//チップの選択
	case WM_MOUSEMOVE:
		//前回描いた矩形を削除
		rcClient.left	= mouse_x * CWIDTH - Instance.m_HSInfo.nPos;
		rcClient.top	= mouse_y * CHEIGHT - Instance.m_VSInfo.nPos;
		rcClient.right	= (mouse_x + 1) * CWIDTH - Instance.m_HSInfo.nPos;
		rcClient.bottom	= (mouse_y + 1) * CHEIGHT - Instance.m_VSInfo.nPos;

		InvalidateRect(hWnd, &rcClient, false);

		//マウス位置を取得
		pt	= MAKEPOINTS(lParam);

		pt.x	= pt.x < Instance.m_HSInfo.nMax ? pt.x : Instance.m_HSInfo.nMax;
		pt.y	= pt.y < Instance.m_VSInfo.nMax ? pt.y : Instance.m_VSInfo.nMax;

		//チップ単位の座標に変換
		mouse_x	= (pt.x + Instance.m_HSInfo.nPos) / CWIDTH;
		mouse_y	= (pt.y + Instance.m_VSInfo.nPos) / CHEIGHT;

		//ドラッグ中なら選択領域を更新
		if(bLButtonDown == true){
			//矩形を再描画
			rcClient.left	= Instance.rcSelected.left - Instance.m_HSInfo.nPos - 2;
			rcClient.top	= Instance.rcSelected.top - Instance.m_VSInfo.nPos - 2;
			rcClient.right	= Instance.rcSelected.right - Instance.m_HSInfo.nPos + 2;
			rcClient.bottom	= Instance.rcSelected.bottom - Instance.m_VSInfo.nPos + 2;
			InvalidateRect(hWnd, &rcClient, false);

			//位置を更新
			Instance.rcSelected.right	= (mouse_x + 1) * CWIDTH;
			Instance.rcSelected.bottom	= (mouse_y + 1) * CHEIGHT;

			if(Instance.rcSelected.right <= Instance.rcSelected.left)
				Instance.rcSelected.right	= Instance.rcSelected.left + CWIDTH;

			if(Instance.rcSelected.bottom <= Instance.rcSelected.top)
				Instance.rcSelected.bottom	= Instance.rcSelected.top + CHEIGHT;
		}
		break;

	case WM_LBUTTONDOWN:
		//一個前に書いた矩形を削除
		rcClient.left	= Instance.rcSelected.left - Instance.m_HSInfo.nPos - 2;
		rcClient.top	= Instance.rcSelected.top - Instance.m_VSInfo.nPos - 2;
		rcClient.right	= Instance.rcSelected.right - Instance.m_HSInfo.nPos + 2;
		rcClient.bottom	= Instance.rcSelected.bottom - Instance.m_VSInfo.nPos + 2;
		InvalidateRect(hWnd, &rcClient, false);

		//選択領域を更新
		Instance.rcSelected.left	= mouse_x * CWIDTH;
		Instance.rcSelected.top		= mouse_y * CHEIGHT;
		Instance.rcSelected.right	= (mouse_x + 1) * CWIDTH;
		Instance.rcSelected.bottom	= (mouse_y + 1) * CHEIGHT;

		bLButtonDown	= true;
		break;

	case WM_LBUTTONUP:
		bLButtonDown	= false;
		break;

	case WM_RBUTTONDOWN:
		break;

	//描画処理
	case WM_PAINT:
		//デバイスコンテキスト取得
		hdc	= BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rcClient);

		//ビットマップ描画
		BitBlt(hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, Instance.hDC_Palette, Instance.m_HSInfo.nPos, Instance.m_VSInfo.nPos, SRCCOPY);

		//今マウスがいる位置に矩形を描画
		SelectObject(hdc, GetStockObject(WHITE_PEN));
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		rcMouse.left	= mouse_x * CWIDTH - Instance.m_HSInfo.nPos;
		rcMouse.top		= mouse_y * CWIDTH - Instance.m_VSInfo.nPos;
		rcMouse.right	= rcMouse.left + CWIDTH;
		rcMouse.bottom	= rcMouse.top + CHEIGHT;
		Rectangle(hdc, rcMouse.left, rcMouse.top, rcMouse.right, rcMouse.bottom);

		//選択されている領域に矩形を描画
		SelectObject(hdc, Instance.hPenSelected);
		Rectangle(hdc, Instance.rcSelected.left - Instance.m_HSInfo.nPos, Instance.rcSelected.top - Instance.m_VSInfo.nPos, Instance.rcSelected.right - Instance.m_HSInfo.nPos, Instance.rcSelected.bottom - Instance.m_VSInfo.nPos);

		EndPaint(hWnd, &ps);
		break;

	//スクロール
	case WM_VSCROLL:
		GetScrollInfo(hWnd, SB_VERT, &Instance.m_VSInfo);
		switch(LOWORD(wParam))
		{
		case SB_TOP:
			Instance.m_VSInfo.nPos	= 0;
			break;

		case SB_BOTTOM:
			Instance.m_VSInfo.nPos	= Instance.m_VSInfo.nMax;
			break;

		case SB_LINEUP:
			Instance.m_VSInfo.nPos--;
			break;

		case SB_LINEDOWN:
			Instance.m_VSInfo.nPos++;
			break;

		case SB_PAGEUP:
			Instance.m_VSInfo.nPos	-= Instance.m_VSInfo.nPage;
			break;

		case SB_PAGEDOWN:
			Instance.m_VSInfo.nPos	+= Instance.m_VSInfo.nPage;
			break;

		case SB_THUMBTRACK:
			Instance.m_VSInfo.nPos	= Instance.m_VSInfo.nTrackPos;
			break;
		}
		SetScrollInfo(hWnd, SB_VERT, &Instance.m_VSInfo, true);
		InvalidateRect(hWnd, NULL, false);
		break;

		case WM_HSCROLL:
		GetScrollInfo(hWnd, SB_HORZ, &Instance.m_HSInfo);
		switch(LOWORD(wParam))
		{
		case SB_LINELEFT:
			Instance.m_HSInfo.nPos--;
			break;

		case SB_LINERIGHT:
			Instance.m_HSInfo.nPos++;
			break;

		case SB_PAGELEFT:
			Instance.m_HSInfo.nPos	-= Instance.m_HSInfo.nPage;
			break;

		case SB_PAGERIGHT:
			Instance.m_HSInfo.nPos	+= Instance.m_HSInfo.nPage;
			break;

		case SB_THUMBTRACK:
			Instance.m_HSInfo.nPos	= Instance.m_HSInfo.nTrackPos;
			break;
		}
		SetScrollInfo(hWnd, SB_HORZ, &Instance.m_HSInfo, true);
		InvalidateRect(hWnd, NULL, false);
		break;

	default:
		return (DefWindowProc(hWnd, msg, wParam, lParam));

	}

	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
