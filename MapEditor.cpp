/** Windows�v���O�������`
* @Author Yuhei
*/

#include "MapEditor.h"
#include <stdio.h>

const char			MAP_EDITOR::szClassName[]			= "MapEditor";
const char			MAP_EDITOR::szClassNamePalette[]	= "Palette";
bool				MAP_EDITOR::once;


//�V�K�쐬�_�C�A���O
bool CALLBACK CreateNewDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);


//�C���X�^���X�擾
MAP_EDITOR& MAP_EDITOR::GetInst()
{
	static MAP_EDITOR MapEdit;

	return MapEdit;
}

//�R���X�g���N�^
MAP_EDITOR::MAP_EDITOR()
{
	//�G�f�B�^
	//�}�b�v�T�C�Y
	map_width	= 0;
	map_height	= 0;

	//�p���b�g
	//�X�N���[���o�[
	ZeroMemory(&m_VSInfo, sizeof(m_VSInfo));
	m_VSInfo.cbSize		= sizeof(m_VSInfo);			//�\���̂̃T�C�Y
	m_VSInfo.fMask		= SIF_ALL;					//�X�N���[���̃����W�ݒ�
	m_VSInfo.nMin		= 0;						//�X�N���[���ŏ��l
	m_VSInfo.nMax		= 1;						//�����̏��
	m_VSInfo.nPage		= 1;

	ZeroMemory(&m_HSInfo, sizeof(m_HSInfo));
	m_HSInfo.cbSize		= sizeof(m_HSInfo);			//�\���̂̃T�C�Y
	m_HSInfo.fMask		= SIF_ALL;					//�X�N���[���̃����W�ݒ�
	m_HSInfo.nMin		= 0;						//�X�N���[���ŏ��l
	m_HSInfo.nMax		= 11;						//���̏��
	m_HSInfo.nPage		= 1;

	//�I������Ă����`
	rcSelected.left		= 0;
	rcSelected.top		= 0;
	rcSelected.right	= CWIDTH;
	rcSelected.bottom	= CHEIGHT;
}

//�f�X�g���N�^
MAP_EDITOR::~MAP_EDITOR()
{
	//�������f�o�C�X�R���e�L�X�g���
	DeleteDC(hDC_Palette);

	//�y�����폜
	DeleteObject(hPenSelected);
}

//�E�B���h�E�쐬
bool MAP_EDITOR::Initialize(HINSTANCE hInstance)
{
	hInst	= hInstance;
	bool	bResult;

	//�E�B���h�E�N���X
	WNDCLASSEX	wc;			//�e�E�B���h�E

	//�E�B���h�E�N���X�ݒ�
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
		MessageBox(NULL, "�E�B���h�E�N���X�̓o�^�Ɏ��s(MainWindow)�B", "RegisterClass Fault", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	//�E�B���h�E�쐬
	hWnd = CreateWindow(szClassName,				//�N���X��
						"MapEditor ����",			//�E�B���h�E���i�^�C�g���o�[�ɕ\���j
						WS_OVERLAPPEDWINDOW,		//�E�B���h�E�X�^�C��
						CW_USEDEFAULT,				//x���W
						CW_USEDEFAULT,				//y���W
						CW_USEDEFAULT,				//�E�B���h�E�̕�
						CW_USEDEFAULT,				//�E�B���h�E�̍���
						NULL,						//�e�E�B���h�E�n���h���i�e�E�B���h�E�̍쐬�Ȃ̂�NULL�j
						NULL,						//���j���[�n���h���A�������͎q�E�B���h�EID
						hInst,						//�C���X�^���X�n���h��
						NULL						//�E�B���h�E�쐬�f�[�^
						);

	if(!hWnd)
		return FALSE;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	bResult	= InitPalette();

	return bResult;
}



//���b�Z�[�W���[�v
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




//�E�B���h�E�v���V�[�W��
//���C���E�B���h�E
LRESULT CALLBACK MAP_EDITOR::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MAP_EDITOR&	Instance = MAP_EDITOR::GetInst();	//�����o�ɃA�N�Z�X���邽�߂̕�
	RECT	rcParent;			//���̃E�B���h�E�̃N���C�A���g�̈���擾
	RECT	rcPalette;			//�p���b�g�̃E�B���h�E�̈�

	//�E�B���h�E�̃��T�C�Y�p
	int		width;
	int		height;

	//�`��p
	HDC			hdc;
	PAINTSTRUCT	ps;
	char		str[128];	//�f�o�b�O�p�e�L�X�g
	static RECT	rcText;		//�f�o�b�O�p�e�L�X�g�\���̈�
	RECT		rc1, rc2;

	switch(msg)
	{
	//���j���[����
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		//�V�K�쐬�_�C�A���O
		case IDM_NEW:
			DialogBox(Instance.hInst, 
				MAKEINTRESOURCE(IDD_DIALOG1),
				hWnd, 
				(DLGPROC)CreateNewDlgProc);
			break;

		//�p���b�g��\��
		case IDM_VIEW_PALETTE:
			ShowWindow(Instance.hWndPalette, SW_SHOW);
			
			//�p���b�g�������ʒu�Ɉړ�
			GetClientRect(hWnd, &rcParent);
			GetWindowRect(Instance.hWndPalette, &rcPalette);
			width	= rcPalette.right - rcPalette.left;
			height	= rcPalette.bottom - rcPalette.top;

			MoveWindow(Instance.hWndPalette, rcParent.right - width - 10, rcParent.bottom - height - 10, width, height, true);
			break;

		case IDM_HELP:
			ShellAbout(hWnd, "MapEditor", "MapEditor Ver.��001", NULL);
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


//�V�K�쐬�_�C�A���O�p
bool CALLBACK CreateNewDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//�q�E�B���h�E
	static HWND		hRadio[4];	//���W�I�{�b�N�X
	static HWND		hEdit[2];	//�G�f�B�b�g�{�b�N�X

	//�}�b�v�G�f�B�^�[�̃C���X�^���X
	MAP_EDITOR&	Instance = MAP_EDITOR::GetInst();	//�����o�ɃA�N�Z�X���邽�߂̕�

	int				i;

	switch(msg){
	//�_�C�A���O������
	case WM_INITDIALOG:
		ShowWindow(hDlg, SW_SHOW);

		//���W�I�{�^��
		for(i = 0; i < 4; i++)
			hRadio[i]	= GetDlgItem(hDlg, IDC_RADIO1 + i);

		//�G�f�B�b�g�R���g���[��
		for(i = 0; i < 2; i++)
			hEdit[i]	= GetDlgItem(hDlg, IDC_EDIT1 + i);

		//���W�I�R���g���[���̏����l
		SendMessage(hRadio[1], BM_SETCHECK, (WPARAM)BST_CHECKED, 0);

		//�G�f�B�b�g�R���g���[���ɏ����l��ݒ�
		SendMessage(hEdit[0], WM_SETTEXT, 0, (LPARAM)"40");
		SendMessage(hEdit[1], WM_SETTEXT, 0, (LPARAM)"100");
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam)){
		//���W�I�{�^��("�J�X�^��"���I�����ꂽ�������G�f�B�b�g�R���g���[����L��)
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

		//OK�A�L�����Z��
		case IDOK:
			//�}�b�v�T�C�Y��ݒ�
			for(i = 0; i < 3; i++){
				if(SendMessage(hRadio[i], BM_GETCHECK, 0, 0) == BST_CHECKED){
					switch(i){
					//�}�b�v�T�C�Y�F��
					case 0:
						Instance.SetMapSize(40, 30);
						break;

					//�}�b�v�T�C�Y�F��
					case 1:
						Instance.SetMapSize(80, 60);
						break;

					//�}�b�v�T�C�Y�F��
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

