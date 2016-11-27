/** エントリポイント
* @Author Yuhei
*/

#include "MapEditor.h"

//エントリポイント
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
	MAP_EDITOR&	WinInst	= MAP_EDITOR::GetInst();

	if(WinInst.Initialize(hCurInst) == FALSE)
		return -1;

	return WinInst.MessLoop();
}
