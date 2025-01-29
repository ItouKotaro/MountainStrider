//=============================================================
//
// ���C������ [main.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#include <thread>

// �}�N����`
#define CLASS_NAME			"WindowClass"	// �N���X��
#define WINDOW_NAME	"MountainStrider"			// �E�B���h�E��
#define FIXED_FPS			(60)					// �Œ�FPS�l

// �v���g�^�C�v�錾
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