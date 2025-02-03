//=============================================================
//
// マネージャー [manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "manager.h"
#include "gameobject.h"
#include "scene.h"
#include "input_system.h"
#include "data_field.h"
#include "component/2d/text.h"
#include "scripts/mode/mode_manager.h"

//=============================================================
// [CManager] コンストラクタ
//=============================================================
CManager::CManager()
{
}

//=============================================================
// [CManager] デストラクタ
//=============================================================
CManager::~CManager()
{

}

//=============================================================
// [CManager] 初期化
//=============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// ハンドルの代入
	m_hwnd = hWnd;

	// レンダラーの生成
	CRenderer::GetInstance()->Init(hInstance, hWnd, bWindow);

	// フォントの読み込み
	CText::InitLoadFont();

	// 物理の生成
	CPhysics::GetInstance()->Init();

	// データマネージャーの生成
	CDataManager::GetInstance()->Init();

	// オーディオマネージャーの生成
	AudioManager::GetInstance()->Init();

	// 入力システムの生成
	CInputSystem::GetInstance()->Init(hInstance, hWnd);

	// シーンマネージャーの生成
	CSceneManager::GetInstance()->Init();
	CSceneManager::GetInstance()->SetScene(CSceneManager::START_SCENE_NAME);

	return S_OK;
}

//=============================================================
// [CManager] 終了
//=============================================================
void CManager::Uninit()
{
	// モード管理の終了
	ModeManager::GetInstance()->LastUninit();

	// シーンマネージャーの終了
	CSceneManager::GetInstance()->Uninit();

	// すべてのオブジェクトを解放する
	GameObject::DestroyAll(true);
	GameObject::DestroyDeathFlag();

	// 入力システムの終了
	CInputSystem::GetInstance()->Uninit();

	// レンダラーを破棄する
	CRenderer::GetInstance()->Uninit();

	// 物理を破棄する
	CPhysics::GetInstance()->Uninit();

	// オーディオマネージャーを破棄する
	AudioManager::GetInstance()->Uninit();

	// データ管理を破棄する
	CDataManager::GetInstance()->Uninit();
}

//=============================================================
// [CManager] 更新
//=============================================================
void CManager::Update()
{
	// 入力システムの更新
	CInputSystem::GetInstance()->Update();

	// レンダラーの更新
	CRenderer::GetInstance()->Update();

	if (!m_bPause)
	{
		// 物理の更新
		CPhysics::GetInstance()->Update();
	}

	// オーディオの更新
	AudioManager::GetInstance()->Update();

	// シーンの更新
	CSceneManager::GetInstance()->Update();

	m_mouseWheel = 0.0f;
}

//=============================================================
// [CManager] 描画
//=============================================================
void CManager::Draw()
{
	// レンダラーの描画
	CRenderer::GetInstance()->Draw();

	// シーンの描画
	CSceneManager::GetInstance()->Draw();
}

//=============================================================
// [CManager] カーソル位置を取得する
//=============================================================
CManager::CursorPos CManager::GetCursorClientPos()
{
	POINT points;
	GetCursorPos(&points);
	
	// スクリーン上で見た左上の座標を取得する
	POINT startPos;
	startPos.x = 0;
	startPos.y = 0;
	ClientToScreen(m_hwnd, &startPos);

	CursorPos cPos;
	cPos.x = static_cast<float>(points.x - startPos.x);
	cPos.y = static_cast<float>(points.y - startPos.y);


	D3DXVECTOR2 rect = GetWindowSize();
	cPos.x *= static_cast<float>(CRenderer::SCREEN_WIDTH / (float)rect.x);
	cPos.y *= static_cast<float>(CRenderer::SCREEN_HEIGHT / (float)rect.y);
	return cPos;
}

//=============================================================
// [CManager] カーソル位置を設定する
//=============================================================
void CManager::SetCursorClientPos(float x, float y)
{
	CursorPos cPos;
	cPos.x = x;
	cPos.y = y;

	// スクリーン上で見た左上の座標を取得する
	POINT startPos;
	startPos.x = 0;
	startPos.y = 0;
	ClientToScreen(m_hwnd, &startPos);

	D3DXVECTOR2 rect = GetWindowSize();
	cPos.x *= static_cast<float>(rect.x / (float)CRenderer::SCREEN_WIDTH);
	cPos.y *= static_cast<float>(rect.y / (float)CRenderer::SCREEN_HEIGHT);

	cPos.x += startPos.x;
	cPos.y += startPos.y;

	SetCursorPos(static_cast<int>(cPos.x), static_cast<int>(cPos.y));
}

//=============================================================
// [CManager] ウィンドウサイズ
//=============================================================
D3DXVECTOR2 CManager::GetWindowSize()
{
	RECT rect;
	GetWindowRect(CManager::GetInstance()->GetHWND(), &rect);
	return D3DXVECTOR2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));
}