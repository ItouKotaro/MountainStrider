//=============================================================
//
// ���C������ [main.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MAIN_H_
#define _MAIN_H_

// �}�N����`
#define CLASS_NAME			"WindowClass"	// �N���X��
#define WINDOW_NAME	"MountainStrider"			// �E�B���h�E��
#define FIXED_FPS			(60)					// �Œ�FPS�l

// �v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Main
{
	bool IsActiveWindow();
	void ExitApplication();
	void SetShowCursor(const bool& show);
}

#endif