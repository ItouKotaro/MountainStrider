//=============================================================
//
// シーン [scene.h]
// Author: Ito Kotaro
// 
// 説明: シーンの管理を行います
//
//=============================================================
#include "scene.h"
#include "scene/start_scene.h"
#include "gameobject.h"

// 静的メンバの初期化
const std::string CSceneManager::START_SCENE_NAME = "start";


//=============================================================
// [CSceneManager] コンストラクタ
//=============================================================
CSceneManager::CSceneManager()
{
	m_pCurrentScene = nullptr;
	m_pNextScene = nullptr;
	m_bReload = false;
	m_apScene = {};
}

//=============================================================
// [CSceneManager] シーンの取得
//=============================================================
CSceneManager::SceneData* CSceneManager::GetScene(std::string sName)
{
	for (unsigned int i = 0; i < m_apScene.size(); i++)
	{
		if (m_apScene[i]->sName == sName)
		{ // 名前が一致したとき
			return m_apScene[i];		// シーンデータを返す
		}
	}
	return nullptr;	// 見つからなかったとき
}

//=============================================================
// [CSceneManager] 初期化
//=============================================================
void CSceneManager::Init()
{
	// 最初のシーンを登録する
	RegistScene<CStartScene>(START_SCENE_NAME);
}

//=============================================================
// [CSceneManager] 終了
//=============================================================
void CSceneManager::Uninit()
{
	// 現在シーンの終了処理
	if (m_pCurrentScene != nullptr)
	{
		m_pCurrentScene->pScene->Uninit();
		m_pCurrentScene->pScene->LastUninit();
		m_pCurrentScene = nullptr;
	}

	// すべてのシーンデータの破棄
	for (unsigned int i = 0; i < m_apScene.size(); i++)
	{
		if (m_apScene[i] != nullptr)
		{
			// シーンの破棄
			if (m_apScene[i]->pScene != nullptr)
			{
				delete m_apScene[i]->pScene;
				m_apScene[i]->pScene = nullptr;
			}

			// 名前の初期化
			m_apScene[i]->sName = "";

			// シーンデータの破棄
			delete m_apScene[i];
			m_apScene[i] = nullptr;
		}
	}
	m_apScene.clear();	// 空にする
}

//=============================================================
// [CSceneManager] 更新
//=============================================================
void CSceneManager::Update()
{
	// 現在シーンの更新処理
	if (m_pCurrentScene != nullptr)
	{
		m_pCurrentScene->pScene->Update();
	}
}

//=============================================================
// [CSceneManager] 描画
//=============================================================
void CSceneManager::Draw()
{
	// 現在シーンの描画処理
	if (m_pCurrentScene != nullptr)
	{
		m_pCurrentScene->pScene->Draw();
	}
}

//=============================================================
// [CSceneManager] シーンの変更
//=============================================================
HRESULT CSceneManager::SetScene(std::string sName, bool bIsDestroy)
{
	// シーンが存在するかを確認する
	SceneData* pNewScene = GetScene(sName);
	if (pNewScene == nullptr)
	{ // 存在しないとき
		return E_FAIL;	// 失敗
	}

	// 次のシーンに代入する
	m_pNextScene = pNewScene;
	m_bNoDestroy = bIsDestroy;

	return S_OK;		// 成功
}

//=============================================================
// [CSceneManager] 現在シーンの再読み込み
//=============================================================
void CSceneManager::ReloadScene()
{
	m_bReload = true;
}

//=============================================================
// [CSceneManager] 前シーンの終了、次シーンの初期化（安全性を守るため、実行しないでください）
//=============================================================
void CSceneManager::ChangingScene()
{
	// 再読み込み
	if (m_bReload)
	{
		// 現在シーンの終了処理
		if (m_pCurrentScene != nullptr)
		{
			m_pCurrentScene->pScene->Uninit();
		}

		// すべてのオブジェクトの削除
		GameObject::DestroyAll();
		GameObject::DestroyDeathFlag();

		// シーンの初期化
		m_pCurrentScene->pScene->Init();
		m_bReload = false;
		return;
	}

	if (m_pNextScene == nullptr)
	{ // 次のシーンに何も入っていないとき
		return;
	}

	// 現在シーンの終了処理
	if (m_pCurrentScene != nullptr)
	{
		m_pCurrentScene->pScene->Uninit();
		m_pCurrentScene = nullptr;
	}

	// すべてのオブジェクトの削除
	if (m_bNoDestroy)
	{
		GameObject::DestroyAll();
	}

	// 新しいシーンを現在のシーンに代入
	m_pCurrentScene = m_pNextScene;
	m_pNextScene = nullptr;

	// 新しいシーンの初期化処理
	m_pCurrentScene->pScene->Init();
}





////=============================================================
//// シーンの初期化処理
////=============================================================
//void InitScene()
//{
//	// スタートアップをTrueにする
//	g_bIsStartupScene = true;
//
//	// 最初のシーンの読み込み
//	SetScene(FIRST_SCENE);
//}
//
////=============================================================
//// シーンの終了処理
////=============================================================
//void UninitScene()
//{
//	switch (g_currentScene)
//	{
//	case SCENE_GAME:
//		UninitGame();
//		break;
//	}
//}
//
////=============================================================
//// シーンの更新処理
////=============================================================
//void UpdateScene()
//{
//	switch (g_currentScene)
//	{
//	case SCENE_GAME:
//		UpdateGame();
//		break;
//	}
//}
//
////=============================================================
//// シーンの描画処理
////=============================================================
//void DrawScene()
//{
//	switch (g_currentScene)
//	{
//	case SCENE_GAME:
//		DrawGame();
//		break;
//	}
//}
//
////=============================================================
//// シーンの設定処理
////=============================================================
//void SetScene(SCENE scene)
//{
//	// 現在のシーンを終了する
//	if (!g_bIsStartupScene)
//		switch (g_currentScene)
//		{
//		case SCENE_GAME:
//			UninitGame();
//			break;
//		}
//	else g_bIsStartupScene = false;
//
//	// 新しいシーンを初期化する
//	switch (scene)
//	{
//	case SCENE_GAME:
//		InitGame();
//		break;
//	}
//
//	// 現在のシーンを新しいシーンに置き換える
//	g_currentScene = scene;
//}