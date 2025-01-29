//=============================================================
//
// メイン処理 [main.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#include <thread>

// マクロ定義
#define CLASS_NAME			"WindowClass"	// クラス名
#define WINDOW_NAME	"MountainStrider"			// ウィンドウ名
#define FIXED_FPS			(60)					// 固定FPS値

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Main final
{
public:
	Main();
	~Main();
	void ThreadStart();
	void ThreadJoin();

	static bool IsActiveWindow();
	static void ExitApplication();
	static void SetShowCursor(const bool& show);
private:
	void MainLoop();

	std::thread* m_thread;
	DWORD m_dwCurrentTime;
	DWORD m_dwExecLastTime;
	DWORD m_dwFrameCount;
	DWORD m_dwFPSLastTime;
};

#endif