//=============================================================
//
// メイン処理 [main.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "main.h"
#include "manager.h"
#include <windowsx.h>
#include <thread>

// メモリーリーク検出
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define FIND_MEM_LEAKS
#endif // _DEBUG

#ifdef FIND_MEM_LEAKS
#define _NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define _NEW new
#endif // FIND_MEM_LEAKS

// グローバル変数
bool g_isActiveWindow;
bool g_isEnded = false;
bool g_showCursor = true;
bool g_beforeShowCursor = true;
//=============================================================
// メイン関数
//=============================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	// メモリーリーク検知
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1424590);
#endif

	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						// メモリサイズ
		CS_CLASSDC,									// ウィンドウのスタイル
		WindowProc,										// ウィンドウのプロシージャ
		0,
		0,
		hInstance,											// インスタンスハンドル
		LoadIcon(nullptr, IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(nullptr,IDC_ARROW),			// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		// クライアント領域の背景色
		nullptr,												// メニューバー
		CLASS_NAME,									// ウィンドウクラスの名前
		LoadIcon(nullptr, IDI_APPLICATION)		// ファイルのアイコン
	};

	HWND hWnd;	// ウィンドウハンドル（識別子）
	MSG msg;			// メッセージを格納する変数

	// ウィンドウの座標を格納
	RECT rect = { 0, 0, CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT };

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定サイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウを生成
	hWnd = CreateWindowEx(
		0,														// 拡張ウィンドウスタイル
		CLASS_NAME,									// ウィンドウクラスの名前
		WINDOW_NAME,								// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,				// ウィンドウスタイル
		CW_USEDEFAULT,								// ウィンドウの左上のX座標
		CW_USEDEFAULT,								// ウィンドウの左上のY座標
		(rect.right - rect.left),							// ウィンドウの幅
		(rect.bottom - rect.top),						// ウィンドウの高さ
		nullptr,												// 親ウィンドウのハンドル
		nullptr,												// メニューハンドルまたは子ウィンドウID
		hInstance,											// インスタンスバンドル
		nullptr												// ウィンドウ作成データ
	);

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);		// ウィンドウの表示状態を設定
	UpdateWindow(hWnd);						// クライアント領域を更新

	// マネージャーの生成・初期化
	CManager::GetInstance()->Init(hInstance, hWnd, TRUE);

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;							// 初期化する
	dwExecLastTime = timeGetTime();		// 現在時刻を取得

	// FPS計測の初期化
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	std::thread mainLoopThread([&]() {
		while (!g_isEnded)
		{
			dwCurrentTime = timeGetTime();		// 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ // 0.5秒経過毎
				// FPSを計測
				CManager::GetInstance()->SetFPS((dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime));
				dwFPSLastTime = dwCurrentTime;							// 計測した時刻を記録
				dwFrameCount = 0;												// フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FIXED_FPS))
			{ // 60分の1秒経過

				// デルタタイムを設定する
				CManager::GetInstance()->SetDeltaTime((dwCurrentTime - dwExecLastTime) * 0.001f);

				//処理開始時刻
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				CManager::GetInstance()->Update();

				// 描画処理
				CManager::GetInstance()->Draw();

				// シーンの変更処理
				CSceneManager::GetInstance()->ChangingScene();

				// 死亡フラグのついたオブジェクトを破棄する
				GameObject::DestroyDeathFlag();

				dwFrameCount++; // フレームカウントを加算
			}
		}
		});

	while (!g_isEnded)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{ // Windowsの処理
			if (msg.message == WM_QUIT)
			{ // WM_QUITメッセージ
				g_isEnded = true;
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// アクティブ情報の更新
		g_isActiveWindow = GetActiveWindow() != NULL;

		// カーソルの表示設定の更新
		if (g_showCursor != g_beforeShowCursor)
		{
			ShowCursor(g_showCursor ? TRUE : FALSE);
			g_beforeShowCursor = g_showCursor;
		}
	}

	// メインループのスレッドが終わるまで待つ
	mainLoopThread.join();

	// 分解能を戻す
	timeEndPeriod(1);

	// マネージャーの終了・解放
	CManager::GetInstance()->Uninit();

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================
// ウィンドウプロシージャ
//=============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
	auto hdc = GetDC(hWnd);

	switch (uMsg)
	{
	case WM_MOUSEWHEEL:
		// マウスホイールの移動量を取得
		CManager::GetInstance()->SetMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		return 0;

	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); //既定の処理を返す
}

//=============================================================
// アクティブウィンドウか
//=============================================================
bool Main::IsActiveWindow()
{
	return g_isActiveWindow;
}

//=============================================================
// アプリケーション終了命令
//=============================================================
void Main::ExitApplication()
{
	g_isEnded = true;
}

//=============================================================
// カーソルの表示
//=============================================================
void Main::SetShowCursor(const bool& show)
{
	g_showCursor = show;
}
