//=============================================================
//
// メイン処理 [main.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MAIN_H_
#define _MAIN_H_

// マクロ定義
#define CLASS_NAME			"WindowClass"	// クラス名
#define WINDOW_NAME	"MountainStrider"			// ウィンドウ名
#define FIXED_FPS			(60)					// 固定FPS値

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Main
{
	bool IsActiveWindow();
	void ExitApplication();
	void SetShowCursor(const bool& show);
}

#endif