/** Windowsプログラム雛形
* @Author Yuhei
*/

#include "MapEditor.h"
#include <stdio.h>

const char			MAP_EDITOR::szClassName[]			= "MapEditor";
const char			MAP_EDITOR::szClassNamePalette[]	= "Palette";
bool				MAP_EDITOR::once;


//新規作成ダイアログ
bool CALLBACK CreateNewDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);


//インスタンス取得
MAP_EDITOR& MAP_EDITOR::GetInst()
{
	static MAP_EDITOR MapEdit;

	return MapEdit;
}

//コンストラクタ
MAP_EDITOR::MAP_EDITOR()
{
	//エディタ
	//マップサイズ
	map_width	= 0;
	map_height	= 0;

	//パレット
	//スクロールバー
	ZeroMemory(&m_VSInfo, sizeof(m_VSInfo));
	m_VSInfo.cbSize		= sizeof(m_VSInfo);			//構造体のサイズ
	m_VSInfo.fMask		= SIF_ALL;					//スクロールのレンジ設定
	m_VSInfo.nMin		= 0;						//スクロール最小値
	m_VSInfo.nMax		= 1;						//高さの上限
	m_VSInfo.nPage		= 1;

	ZeroMemory(&m_HSInfo, sizeof(m_HSInfo));
	m_HSInfo.cbSize		= sizeof(m_HSInfo);			//構造体のサイズ
	m_HSInfo.fMask		= SIF_ALL;					//スクロールのレンジ設定
	m_HSInfo.nMin		= 0;						//スクロール最小値
	m_HSInfo.nMax		= 11;						//幅の上限
	m_HSInfo.nPage		= 1;

	//選択されている矩形
	rcSelected.left		= 0;
	rcSelected.top		= 0;
	rcSelected.right	= CWIDTH;
	rcSelected.bottom	= CHEIGHT;
}

//デストラクタ
MAP_EDITOR::~MAP_EDITOR()
{
	//メモリデバイスコンテキスト解放
	DeleteDC(hDC_Palette);

	//ペンを削除
	DeleteObject(hPenSelected);
}

//ウィンドウ作成
bool MAP_EDITOR::Initialize(HINSTANCE hInstance)
{
	hInst	= hInstance;
	bool	bResult;

	//ウィンドウクラス
	WNDCLASSEX	wc;			//親ウィンドウ

	//ウィンドウクラス設定
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= &MAP_EDITOR::MainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInst;
	wc.hIcon			= (HICON)LoadImage(NULL,
										   MAKEINTRESOURCE(IDI_APPLICATION),
										   IMAGE_ICON, 
										   0, 
										   0, 
										   LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor			= (HCURSOR)LoadImage(NULL,
											 MAKEINTRESOURCE(IDC_ARROW),
											 IMAGE_CURSOR,
											 0,
											 0,
											 LR_DEFAULTSIZE | LR_SHARED);
	wc.hbrBackground	= (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszMenuName		= MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName	= (LPCSTR)szClassName;
	wc.hIconSm			= (HICON)LoadImage(NULL,
										   MAKEINTRESOURCE(IDI_APPLICATION),
										   IMAGE_ICON,
										   0,
										   0,
										   LR_DEFAULTSIZE | LR_SHARED);

	if(RegisterClassEx(&wc) == NULL)
	{
		MessageBox(NULL, "ウィンドウクラスの登録に失敗(MainWindow)。", "RegisterClass Fault", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	//ウィンドウ作成
	hWnd = CreateWindow(szClassName,				//クラス名
						"MapEditor β版",			//ウィンドウ名（タイトルバーに表示）
						WS_OVERLAPPEDWINDOW,		//ウィンドウスタイル
						CW_USEDEFAULT,				//x座標
						CW_USEDEFAULT,				//y座標
						CW_USEDEFAULT,				//ウィンドウの幅
						CW_USEDEFAULT,				//ウィンドウの高さ
						NULL,						//親ウィンドウハンドル（親ウィンドウの作成なのでNULL）
						NULL,						//メニューハンドル、もしくは子ウィンドウID
						hInst,						//インスタンスハンドル
						NULL						//ウィンドウ作成データ
						);

	if(!hWnd)
		return FALSE;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	bResult	= InitPalette();

	return bResult;
}



//メッセージループ
int MAP_EDITOR::MessLoop()
{
	MSG msg;
	BOOL bRet;


	while((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if(bRet == -1)
		{
			MessageBox(NULL, "GetMessage error", "Error", MB_OK);
			break;
		}

		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	return (int)msg.wParam;
}




//ウィンドウプロシージャ
//メインウィンドウ
LRESULT CALLBACK MAP_EDITOR::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MAP_EDITOR&	Instance = MAP_EDITOR::GetInst();	//メンバにアクセスするための物
	RECT	rcParent;			//このウィンドウのクライアント領域を取得
	RECT	rcPalette;			//パレットのウィンドウ領域

	//ウィンドウのリサイズ用
	int		width;
	int		height;

	//描画用
	HDC			hdc;
	PAINTSTRUCT	ps;
	char		str[128];	//デバッグ用テキスト
	static RECT	rcText;		//デバッグ用テキスト表示領域
	RECT		rc1, rc2;

	switch(msg)
	{
	//メニュー操作
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		//新規作成ダイアログ
		case IDM_NEW:
			DialogBox(Instance.hInst, 
				MAKEINTRESOURCE(IDD_DIALOG1),
				hWnd, 
				(DLGPROC)CreateNewDlgProc);
			break;

		//パレットを表示
		case IDM_VIEW_PALETTE:
			ShowWindow(Instance.hWndPalette, SW_SHOW);
			
			//パレットを初期位置に移動
			GetClientRect(hWnd, &rcParent);
			GetWindowRect(Instance.hWndPalette, &rcPalette);
			width	= rcPalette.right - rcPalette.left;
			height	= rcPalette.bottom - rcPalette.top;

			MoveWindow(Instance.hWndPalette, rcParent.right - width - 10, rcParent.bottom - height - 10, width, height, true);
			break;

		case IDM_HELP:
			ShellAbout(hWnd, "MapEditor", "MapEditor Ver.β001", NULL);
			break;

		default:
			return (DefWindowProc(hWnd, msg, wParam, lParam));
		};
		break;

	case WM_PAINT:
		hdc	= BeginPaint(hWnd, &ps);

		GetWindowRect(hWnd, &rc1);
		GetWindowRect(Instance.hWndPalette, &rc2);

		sprintf(str, "(%d, %d), (%d, %d)", rc1.left, rc1.top, rc2.left, rc2.top);
		rcText.left		= 0;
		rcText.top		= 0;
		rcText.right	= strlen(str) * 10;
		rcText.bottom	= 15;

		DrawText(hdc, str, strlen(str), &rcText, DT_CENTER | DT_WORDBREAK);

		EndPaint(hWnd, &ps);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return (DefWindowProc(hWnd, msg, wParam, lParam));

	}

	return 0;
}


//新規作成ダイアログ用
bool CALLBACK CreateNewDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//子ウィンドウ
	static HWND		hRadio[4];	//ラジオボックス
	static HWND		hEdit[2];	//エディットボックス

	//マップエディターのインスタンス
	MAP_EDITOR&	Instance = MAP_EDITOR::GetInst();	//メンバにアクセスするための物

	int				i;

	switch(msg){
	//ダイアログ初期化
	case WM_INITDIALOG:
		ShowWindow(hDlg, SW_SHOW);

		//ラジオボタン
		for(i = 0; i < 4; i++)
			hRadio[i]	= GetDlgItem(hDlg, IDC_RADIO1 + i);

		//エディットコントロール
		for(i = 0; i < 2; i++)
			hEdit[i]	= GetDlgItem(hDlg, IDC_EDIT1 + i);

		//ラジオコントロールの初期値
		SendMessage(hRadio[1], BM_SETCHECK, (WPARAM)BST_CHECKED, 0);

		//エディットコントロールに初期値を設定
		SendMessage(hEdit[0], WM_SETTEXT, 0, (LPARAM)"40");
		SendMessage(hEdit[1], WM_SETTEXT, 0, (LPARAM)"100");
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam)){
		//ラジオボタン("カスタム"が選択された時だけエディットコントロールを有効)
		case IDC_RADIO1:
		case IDC_RADIO2:
		case IDC_RADIO3:
			EnableWindow(hEdit[0], false);
			EnableWindow(hEdit[1], false);
			break;

		case IDC_RADIO4:
			EnableWindow(hEdit[0], true);
			EnableWindow(hEdit[1], true);
			break;

		//OK、キャンセル
		case IDOK:
			//マップサイズを設定
			for(i = 0; i < 3; i++){
				if(SendMessage(hRadio[i], BM_GETCHECK, 0, 0) == BST_CHECKED){
					switch(i){
					//マップサイズ：小
					case 0:
						Instance.SetMapSize(40, 30);
						break;

					//マップサイズ：中
					case 1:
						Instance.SetMapSize(80, 60);
						break;

					//マップサイズ：大
					case 2:
						Instance.SetMapSize(160, 120);
						break;

					default:
						break;
					}
					break;
				}
			}
			EndDialog(hDlg, IDOK);
			return true;

		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return true;
		}
		return false;
		break;
	}
	return false;
}

