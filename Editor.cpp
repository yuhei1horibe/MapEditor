
#include "MapEditor.h"

bool MAP_EDITOR::SetMapSize(int x, int y)
{
	if((x < 20) || (x > 512) || (y < 20) || (y > 512)){
		MessageBox(NULL, "�}�b�v�̃T�C�Y��20�`512Chip�~20�`512Chip�̊ԂŎw�肵�Ă�������", "Caution", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	map_width	= x;
	map_height	= y;

	return true;
}

/*
//�p���b�g�̃E�B���h�E�쐬
bool MAP_EDITOR::InitPalette()
{
	//�E�B���h�E�N���X
	WNDCLASSEX	wc;			//�e�E�B���h�E
	RECT		rc;			//�e�E�B���h�E�̗̈�
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
		MessageBox(NULL, "�E�B���h�E�N���X�̓o�^�Ɏ��s(Palette)�B", "RegisterClass Fault", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	//�e�E�B���h�E�̃T�C�Y�ɍ��킹��
	GetClientRect(hWnd, &rc);

	x	= rc.right - Palette_width - 10;
	y	= rc.bottom - Palette_height - 10;

	//�E�B���h�E�쐬
	hWndPalette = CreateWindow(szClassNamePalette,	//�N���X��
						"�p���b�g",					//�E�B���h�E���i�^�C�g���o�[�ɕ\���j
						WS_OVERLAPPEDWINDOW | \
						WS_CHILD,					//�E�B���h�E�X�^�C��
						x >= 0 ? x : 0,				//x���W
						y >= 0 ? y : 0,				//y���W
						Palette_width,				//�E�B���h�E�̕�
						Palette_height,				//�E�B���h�E�̍���
						hWnd,						//�e�E�B���h�E�n���h��
						NULL,						//���j���[�n���h���A�������͎q�E�B���h�EID
						hInst,						//�C���X�^���X�n���h��
						NULL						//�E�B���h�E�쐬�f�[�^
						);

	if(!hWndPalette)
		return FALSE;

	ShowWindow(hWndPalette, SW_SHOW);
	UpdateWindow(hWndPalette);

	//�`��̈�
	return InitPalette_Client();
}

//�`��̈�
bool MAP_EDITOR::InitPalette_Client()
{
	//�E�B���h�E�N���X
	WNDCLASSEX	wc;			//�e�E�B���h�E
	RECT		rc;			//�e�E�B���h�E�̗̈�
	int			width;
	int			height;

	//�R���{�{�b�N�X����
	static int		index;
	char			text[512];
	HANDLE			hFind;		//�f�B���N�g���̃t�@�C�����
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
		MessageBox(NULL, "�E�B���h�E�N���X�̓o�^�Ɏ��s(Palette�`��̈�)�B", "RegisterClass Fault", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	//�e�E�B���h�E�̃T�C�Y�ɍ��킹��
	GetClientRect(hWndPalette, &rc);

	width	= rc.right - rc.left - 10;
	height	= rc.bottom - rc.top - 40;

	//�E�B���h�E�쐬
	hWndPaletteClient = CreateWindowEx(
						WS_EX_CLIENTEDGE,			//�g���X�^�C��
						"PaletteClient",			//�N���X��
						"�`��̈�",					//�E�B���h�E���i�^�C�g���o�[�ɕ\���j
						WS_CHILD | \
						WS_VISIBLE | \
						WS_VSCROLL | \
						WS_HSCROLL,					//�E�B���h�E�X�^�C��
						5,							//x���W
						35,							//y���W
						width,						//�E�B���h�E�̕�
						height,						//�E�B���h�E�̍���
						hWndPalette,				//�e�E�B���h�E�n���h��
						NULL,						//���j���[�n���h���A�������͎q�E�B���h�EID
						hInst,						//�C���X�^���X�n���h��
						NULL						//�E�B���h�E�쐬�f�[�^
						);

	if(!hWndPaletteClient){
		MessageBox(NULL, "�E�B���h�E�쐬�Ɏ��s(Palette�`��̈�)�B", "RegisterClass Fault", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	ShowWindow(hWndPaletteClient, SW_SHOW);
	UpdateWindow(hWndPaletteClient);

	return TRUE;
}






//�E�B���h�E�v���V�[�W��
//�p���b�g�{��
LRESULT CALLBACK MAP_EDITOR::WndProcPalette(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MAP_EDITOR&	Instance = MAP_EDITOR::GetInst();	//�����o�ɃA�N�Z�X���邽�߂̕�
	RECT		rcParent;
	RECT		rcPalette;
	RECT		rcClient;
	int			width;
	int			height;

	//�I�����ꂽ���X�g�ԍ�
	int			index;

	//�t�@�C����
	char		CurrentDir[512];
	char		FileName[512];

	//�r�b�g�}�b�v�n���h��
	HBITMAP		hBmp;
	BITMAP		bmpInfo;

	//�E�B���h�E�̃f�o�C�X�R���e�L�X�g
	HDC			hdc;

	DWORD		msg1;

	switch(msg)
	{
	case WM_SIZE:
		GetClientRect(hWnd, &rcPalette);

		width				= rcPalette.right - rcPalette.left - 10;
		height				= rcPalette.bottom - rcPalette.top - 40;

		//�`��̈�����T�C�Y
		MoveWindow(Instance.hWndPaletteClient, rcPalette.left + 5, rcPalette.top + 35, width, height, false);

		//�R���{�{�b�N�X�����T�C�Y
		MoveWindow(Instance.hWndCombo, rcPalette.left + 5, rcPalette.top + 5, width, 250, false);

		//�X�N���[���o�[��ݒ�
		GetClientRect(Instance.hWndPaletteClient, &rcClient);
		Instance.m_VSInfo.nPage		= rcClient.bottom - rcClient.top;			//�y�[�W�̍���
		SetScrollInfo(Instance.hWndPaletteClient, SB_VERT, &Instance.m_VSInfo, TRUE);

		Instance.m_HSInfo.nPage		= rcClient.right - rcClient.left;			//�y�[�W�̕�
		SetScrollInfo(Instance.hWndPaletteClient, SB_HORZ, &Instance.m_HSInfo, TRUE);

		break;

	//�R���{�{�b�N�X�̏���
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		//�R���{�{�b�N�X
		case IDC_COMBO1:
			switch(HIWORD(wParam)){

			//�t�H�[�J�X���ڂ������ɕ`�悷��r�b�g�}�b�v��؂�ւ���
			case CBN_SELCHANGE:
				GetCurrentDirectory(512, CurrentDir);

				index	= SendMessage(Instance.hWndCombo, CB_GETCURSEL, 0, 0);
				SendMessage(Instance.hWndCombo, CB_GETLBTEXT, index, (LPARAM)FileName);

				strcat(CurrentDir, "\\ChipSets\\");
				strcat(CurrentDir, FileName);

				//MessageBox(hWnd, CurrentDir, "test", MB_OK);

				//�p���b�g�Ƀr�b�g�}�b�v��\�������邽�߂̃f�o�C�X�R���e�L�X�g���쐬
				hdc		= GetDC(hWnd);
				DeleteDC(Instance.hDC_Palette);

				Instance.hDC_Palette	= CreateCompatibleDC(hdc);

				ReleaseDC(hWnd, hdc);

				//png�ǂݍ���
				hBmp	= LoadPng(CurrentDir);

				GetClientRect(Instance.hWndPaletteClient, &rcClient);

				//�X�N���[���o�[��ݒ�
				GetObject(hBmp, (int)sizeof(BITMAP), &bmpInfo);
				Instance.m_VSInfo.nPos		= 0;
				Instance.m_VSInfo.nMax		= bmpInfo.bmHeight - 1;						//�����̏��
				Instance.m_VSInfo.nPage		= rcClient.bottom - rcClient.top;			//�y�[�W�̍���
				SetScrollInfo(Instance.hWndPaletteClient, SB_VERT, &Instance.m_VSInfo, TRUE);

				Instance.m_HSInfo.nPos		= 0;
				Instance.m_HSInfo.nMax		= bmpInfo.bmWidth - 1;						//���̏��
				Instance.m_HSInfo.nPage		= rcClient.right - rcClient.left;			//�y�[�W�̕�
				SetScrollInfo(Instance.hWndPaletteClient, SB_HORZ, &Instance.m_HSInfo, TRUE);

				//�I��̈悪�ǂݍ��񂾃r�b�g�}�b�v�̊O�ɏo�Ă��܂����̓��Z�b�g
				if((Instance.rcSelected.right > Instance.m_HSInfo.nMax) || (Instance.rcSelected.bottom > Instance.m_VSInfo.nMax)){
					Instance.rcSelected.left	= 0;
					Instance.rcSelected.top		= 0;
					Instance.rcSelected.right	= CWIDTH;
					Instance.rcSelected.bottom	= CHEIGHT;
				}

				//�������f�o�C�X�R���e�L�X�g�Ƀr�b�g�}�b�v��I��
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

