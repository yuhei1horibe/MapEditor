/** �G���g���|�C���g
* @Author Yuhei
*/

#include "MapEditor.h"

//�G���g���|�C���g
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
	MAP_EDITOR&	WinInst	= MAP_EDITOR::GetInst();

	if(WinInst.Initialize(hCurInst) == FALSE)
		return -1;

	return WinInst.MessLoop();
}
