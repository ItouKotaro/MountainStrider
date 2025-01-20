//=============================================================
//
// マネージャー [manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "renderer.h"
#include "scene.h"
#include "internal/physics.h"
#include "internal/data_manager.h"
#include "internal/audio_manager.h"
#include "gameobject.h"

// マネージャークラスの定義
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// 初期化
	void Uninit();																						// 終了
	void Update();																					// 更新
	void Draw();																						// 描画

	static CManager* GetInstance() {
		static CManager manager;
		return &manager;
	}

	void SetFPS(const int& nFPS) { m_nFPS = nFPS; }														// FPSの設定（ユーザーの使用禁止）
	const int GetFPS() { return m_nFPS; }																			// FPSの取得
	void SetDeltaTime(const float& fDeltaTime) { m_fDeltaTime = fDeltaTime; }					// デルタタイムの設定（ユーザーの使用禁止）
	const float GetDeltaTime() { return m_fDeltaTime; }													// デルタタイムの取得
	void SetPause(const bool& bPause) { m_bPause = bPause; }											// ポーズの設定
	const bool GetPause() { return m_bPause; }																// ポーズの取得
	void SetMouseWheel(const float& mouseWheel) { m_mouseWheel = mouseWheel; }		// マウスホイールの設定（ユーザーの使用禁止）
	float GetMouseWheel() { return m_mouseWheel; }														// マウスホイールの取得

	struct CursorPos
	{
		float x;
		float y;
	};

	CursorPos GetCursorClientPos();																						// カーソル位置の取得
	void SetCursorClientPos(float x, float y);																	// カーソル位置の設定
	D3DXVECTOR2 GetWindowSize();																				// ウィンドウサイズの取得

	HWND GetHWND() { return m_hwnd; }												// ハンドルの取得
private:
	int m_nFPS;												// FPS値
	float m_fDeltaTime;									// デルタタイム
	float m_mouseWheel;									// マウスホイール値
	POINT m_cursorPos;									// マウス位置
	bool m_bPause;											// ポーズ
	HWND m_hwnd;										// ハンドル
};

#endif // !_MANAGER_H_
