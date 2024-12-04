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
	m_pAudioManager = new CAudioManager();
	m_pAudioManager->Init();

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
	if (m_pAudioManager != nullptr)
	{
		m_pAudioManager->Uninit();
		delete m_pAudioManager;
		m_pAudioManager = nullptr;
	}

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

	// オーディオの更新
	m_pAudioManager->Update();

	// レンダラーの更新
	CRenderer::GetInstance()->Update();

	if (!m_bPause)
	{
		// 物理の更新
		CPhysics::GetInstance()->Update();
	}

	// シーンの更新
	CSceneManager::GetInstance()->Update();
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
POINT CManager::GetCursorClientPos()
{
	POINT points;
	GetCursorPos(&points);
	ScreenToClient(m_hwnd, &points);
	D3DXVECTOR2 rect = GetWindowSize();
	points.y *= static_cast<LONG>(CRenderer::SCREEN_HEIGHT / (float)rect.y);
	points.x *= static_cast<LONG>(CRenderer::SCREEN_WIDTH / (float)rect.x);
	return points;
}

//=============================================================
// [CManager] カーソル位置を設定する
//=============================================================
void CManager::SetCursorClientPos(LONG x, LONG y)
{
	POINT point;
	point.x = x;
	point.y = y;

	ClientToScreen(m_hwnd, &point);
	D3DXVECTOR2 rect = GetWindowSize();
	point.x *= static_cast<LONG>(rect.x / (float)CRenderer::SCREEN_WIDTH);
	point.y *= static_cast<LONG>(rect.y / (float)CRenderer::SCREEN_HEIGHT);
	SetCursorPos(point.x, point.y);
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