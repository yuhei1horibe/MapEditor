
#include "MapEditor.h"
#include "loadpng.h"


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

	//�R���{�{�b�N�X���쐬
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

	//�R���{�{�b�N�X��"ChipSets"�f�B���N�g�����̃t�@�C����񋓂��Ēǉ�
	GetCurrentDirectory(512, text);
	strcat(text, "\\ChipSets\\*.png");

	hFind	= FindFirstFile(text, &w32fd);
	if(hFind != INVALID_HANDLE_VALUE){
		index	= 0;
		do {
			//png�t�@�C�������h���b�v�_�E���ɒǉ�
			SendMessage(hWndCombo, CB_ADDSTRING, -1, (LPARAM)w32fd.cFileName);
			index++;
		} while(FindNextFile(hFind, &w32fd));

		//�n���h�����N���[�Y
		FindClose(hFind);

		//TODO
		//�h���b�v�_�E�����X�g�̃A�C�e����I��
		//SendMessage(hWndCombo, CB_SETCURSEL, 0, 0);

		//�N�����Ƀ`�b�v�Z�b�g��ǂݍ��܂��邽�߂̃��b�Z�[�W
		//PostMessage(hWndPalette, WM_COMMAND, (CBN_SELCHANGE << 32) | IDC_COMBO1, 0);
	}

	else{
		MessageBox(hWnd, "ChipSets�̃f�B���N�g�����J���܂���", "Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	//�y�����쐬
	hPenSelected	= CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

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

//�`��̈�
LRESULT CALLBACK MAP_EDITOR::WndProcPalette_Client(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MAP_EDITOR&	Instance = MAP_EDITOR::GetInst();	//�����o�ɃA�N�Z�X���邽�߂̕�

	//�f�o�C�X�R���e�L�X�g��
	HDC			hdc;
	PAINTSTRUCT	ps;

	//�`��̈�̋�`
	RECT		rcClient;
	RECT		rcMouse;		//�}�E�X�̈ʒu�Ɏl�p��`��

	//�N���b�N���𔻒�
	static bool	bLButtonDown	= false;

	//�O��`������`�̈ʒu���L��
	static RECT	rcPrevMouse;

	static int	mouse_x;
	static int	mouse_y;
	POINTS		pt;

	switch(msg)
	{
	//�`�b�v�̑I��
	case WM_MOUSEMOVE:
		//�O��`������`���폜
		rcClient.left	= mouse_x * CWIDTH - Instance.m_HSInfo.nPos;
		rcClient.top	= mouse_y * CHEIGHT - Instance.m_VSInfo.nPos;
		rcClient.right	= (mouse_x + 1) * CWIDTH - Instance.m_HSInfo.nPos;
		rcClient.bottom	= (mouse_y + 1) * CHEIGHT - Instance.m_VSInfo.nPos;

		InvalidateRect(hWnd, &rcClient, false);

		//�}�E�X�ʒu���擾
		pt	= MAKEPOINTS(lParam);

		pt.x	= pt.x < Instance.m_HSInfo.nMax ? pt.x : Instance.m_HSInfo.nMax;
		pt.y	= pt.y < Instance.m_VSInfo.nMax ? pt.y : Instance.m_VSInfo.nMax;

		//�`�b�v�P�ʂ̍��W�ɕϊ�
		mouse_x	= (pt.x + Instance.m_HSInfo.nPos) / CWIDTH;
		mouse_y	= (pt.y + Instance.m_VSInfo.nPos) / CHEIGHT;

		//�h���b�O���Ȃ�I��̈���X�V
		if(bLButtonDown == true){
			//��`���ĕ`��
			rcClient.left	= Instance.rcSelected.left - Instance.m_HSInfo.nPos - 2;
			rcClient.top	= Instance.rcSelected.top - Instance.m_VSInfo.nPos - 2;
			rcClient.right	= Instance.rcSelected.right - Instance.m_HSInfo.nPos + 2;
			rcClient.bottom	= Instance.rcSelected.bottom - Instance.m_VSInfo.nPos + 2;
			InvalidateRect(hWnd, &rcClient, false);

			//�ʒu���X�V
			Instance.rcSelected.right	= (mouse_x + 1) * CWIDTH;
			Instance.rcSelected.bottom	= (mouse_y + 1) * CHEIGHT;

			if(Instance.rcSelected.right <= Instance.rcSelected.left)
				Instance.rcSelected.right	= Instance.rcSelected.left + CWIDTH;

			if(Instance.rcSelected.bottom <= Instance.rcSelected.top)
				Instance.rcSelected.bottom	= Instance.rcSelected.top + CHEIGHT;
		}
		break;

	case WM_LBUTTONDOWN:
		//��O�ɏ�������`���폜
		rcClient.left	= Instance.rcSelected.left - Instance.m_HSInfo.nPos - 2;
		rcClient.top	= Instance.rcSelected.top - Instance.m_VSInfo.nPos - 2;
		rcClient.right	= Instance.rcSelected.right - Instance.m_HSInfo.nPos + 2;
		rcClient.bottom	= Instance.rcSelected.bottom - Instance.m_VSInfo.nPos + 2;
		InvalidateRect(hWnd, &rcClient, false);

		//�I��̈���X�V
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

	//�`�揈��
	case WM_PAINT:
		//�f�o�C�X�R���e�L�X�g�擾
		hdc	= BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rcClient);

		//�r�b�g�}�b�v�`��
		BitBlt(hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, Instance.hDC_Palette, Instance.m_HSInfo.nPos, Instance.m_VSInfo.nPos, SRCCOPY);

		//���}�E�X������ʒu�ɋ�`��`��
		SelectObject(hdc, GetStockObject(WHITE_PEN));
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		rcMouse.left	= mouse_x * CWIDTH - Instance.m_HSInfo.nPos;
		rcMouse.top		= mouse_y * CWIDTH - Instance.m_VSInfo.nPos;
		rcMouse.right	= rcMouse.left + CWIDTH;
		rcMouse.bottom	= rcMouse.top + CHEIGHT;
		Rectangle(hdc, rcMouse.left, rcMouse.top, rcMouse.right, rcMouse.bottom);

		//�I������Ă���̈�ɋ�`��`��
		SelectObject(hdc, Instance.hPenSelected);
		Rectangle(hdc, Instance.rcSelected.left - Instance.m_HSInfo.nPos, Instance.rcSelected.top - Instance.m_VSInfo.nPos, Instance.rcSelected.right - Instance.m_HSInfo.nPos, Instance.rcSelected.bottom - Instance.m_VSInfo.nPos);

		EndPaint(hWnd, &ps);
		break;

	//�X�N���[��
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
