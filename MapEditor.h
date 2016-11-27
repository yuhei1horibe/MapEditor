/** Windows�v���O�����̐��`
*  @Auther Yuhei
*/

#ifndef	__MAP_EDITOR__
#define	__MAP_EDITOR__


#include<windows.h>
#include "MapEditor/resource.h"

//�`�b�v�Z�b�g�̃T�C�Y
const int	CWIDTH	= 16;
const int	CHEIGHT	= 16;

//�eWindow����舵����{�N���X
class MAP_EDITOR{
private:
	HINSTANCE	hInst;			//�C���X�^���X�n���h��

	//�E�B���h�E�n���h��
	HWND		hWnd;				//�E�B���h�E�n���h��
	HWND		hWndPalette;		//�p���b�g�i�q�E�B���h�E�j
	HWND		hWndPaletteClient;	//�p���b�g�̕`��̈�i�q�E�B���h�E�j
	HWND		hWndCombo;			//�p���b�g�̃R���{�{�b�N�X


	//�I�[�v���t�@�C���l�[��
	OPENFILENAME	ofn;


	//�G�f�B�^
	//�}�b�v�̃T�C�Y
	int		map_width;
	int		map_height;



	//�p���b�g
	//�E�B���h�E�T�C�Y
	const static int	Palette_width	= 240;
	const static int	Palette_height	= 320;

	//�`��̈�̃������f�o�C�X�R���e�L�X�g
	HDC			hDC_Palette;		//�p���b�g

	//�X�N���[���C���t�H
	SCROLLINFO		m_VSInfo;		//�X�N���[�����
	SCROLLINFO		m_HSInfo;		//���X�N���[�����

	//�`��I�u�W�F�N�g
	HPEN			hPenSelected;	//�I�����ꂽ�̈��`�悷�邽�߂̃y��

	//���I������Ă���p���b�g�̋�`
	RECT			rcSelected;


	const static char	szClassName[];
	const static char	szClassNamePalette[];
	static bool			once;

	//�p���b�g
	bool InitPalette();
	bool InitPalette_Client();

	//�I�[�v���t�@�C���l�[���\���̏�����
	bool InitOpenFileNameStr(HWND hWnd, HINSTANCE hInst);

public:
	//�C���X�^���X�擾
	static MAP_EDITOR&	GetInst();

	//�R���X�g���N�^
	MAP_EDITOR();

	//�f�X�g���N�^
	~MAP_EDITOR();

	//�������A�E�B���h�E�쐬
	//���C���E�B���h�E
	bool Initialize(HINSTANCE hInst);

	//�}�b�v�T�C�Y�ݒ�
	bool SetMapSize(int x, int y);

	//���b�Z�[�W���[�v�i���C�������j
	int MessLoop();

	//�E�B���h�E�v���V�[�W��
	//�e�E�B���h�E
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//�p���b�g
	static LRESULT CALLBACK WndProcPalette(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);			//�{��
	static LRESULT CALLBACK WndProcPalette_Client(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	//�`��̈�
};


#endif
