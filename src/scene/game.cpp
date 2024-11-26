//=============================================================
//
// ゲームシーン [game.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "game.h"
#include "manager.h"

#include "component/3d/camera.h"
#include "component/3d/light.h"
#include "component/2d/text.h"

#include "scripts/vehicle.h"
#include "scripts/terrain.h"
#include "scripts/camera_move.h"
#include "scripts/result/result_camera.h"

#include <noise/noise.h>

//=============================================================
// [CGameScene] 初期化
//=============================================================
void CGameScene::Init()
{
	// 変数の初期化
	m_isGameOvered = false;

	// カメラの作成
	m_pCamera = new GameObject("Camera", "Camera");
	m_pCamera->AddComponent<CCamera>();
	m_pCamera->GetComponent<CCamera>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
	m_pCamera->GetComponent<CCamera>()->m_fClippingPlanesFar = 5000.0f;
	m_pCamera->GetComponent<CCamera>()->GetSkybox()->LoadSkybox("data\\SKYBOX\\daylight.json");

	// ライトを作成
	GameObject* pLight = new GameObject("Light");
	CD3DLight::SetDefaultD3DLight(pLight);

	// 地面を作成
	m_pTerrain = new Terrain();
	m_pTerrain->SetSeed((unsigned int)clock());
	m_pTerrain->Init();
	m_pTerrain->Generate();

	// リザルトマネージャーの生成
	m_resultManager = new MountainResultManager(this);

	// バイクの生成
	SpawnBike();

	// カメラの移動設定を行う
	m_pCamera->AddComponent<CCameraMove>()->SetTarget(m_pBike);
	m_pCamera->AddComponent<ResultCamera>();

	m_pFPS = new GameObject("FPS");
	m_pFPS->AddComponent<CText>();

	CameraRenderBuffer* renderBuff = CRenderer::GetInstance()->RegisterRenderBuffer<CameraRenderBuffer>("main");
	renderBuff->SetCamera(m_pCamera->GetComponent<CCamera>());
}

//=============================================================
// [CGameScene] 終了
//=============================================================
void CGameScene::Uninit()
{
	// 地形の破棄
	if (m_pTerrain != nullptr)
	{
		m_pTerrain->Uninit();
		delete m_pTerrain;
		m_pTerrain = nullptr;
	}

	// リザルトマネージャーの破棄
	if (m_resultManager != nullptr)
	{
		m_resultManager->Uninit();
		delete m_resultManager;
		m_resultManager = nullptr;
	}
}

//=============================================================
// [CGameScene] 更新
//=============================================================
void CGameScene::Update()
{
	// FPSを更新する
	m_pFPS->GetComponent<CText>()->SetText("FPS: " + std::to_string(CManager::GetInstance()->GetFPS()));

	// 地形を更新する
	m_pTerrain->Update(m_pCamera->transform->GetWPos());

	// リザルトカメラ
	if (!m_isGameOvered)
	{ // 未ゲームオーバー時
		// カメラの情報を記録する
		m_pCamera->GetComponent<ResultCamera>()->RecordData();
	}

	// リザルトの更新処理
	if (m_isGameOvered)
	{ // ゲームオーバーのとき
		// リザルトマネージャーの更新
		m_resultManager->Update();

		if (INPUT_INSTANCE->onTrigger("enter"))
		{
			CSceneManager::GetInstance()->ReloadScene();
		}
	}
}

//=============================================================
// [CGameScene] 描画
//=============================================================
void CGameScene::Draw()
{
}

//=============================================================
// [CGameScene] バイクの生成
//=============================================================
void CGameScene::SpawnBike()
{
	// 中心からレイを打ち、高さを取得する
	btVector3 Start = btVector3(0.0f, 3000.0f, 0.0f);
	btVector3 End = btVector3(0.0f, -3000.0f, 0.0f);
	float hitY = 0.0f;

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit())
	{ // ヒットしたとき
		hitY = RayCallback.m_hitPointWorld.getY();
	}

	// バイクを生成する
	m_pBike = new GameObject("Vehicle");
	m_pBike->transform->Rotate(0.0f, D3DX_PI, 0.0f);
	m_pBike->AddComponent<CVehicle>();

	m_pBike->GetComponent<CVehicle>()->SetPos({ 0.0f, hitY + 50.0f, 0.0f });
}

//=============================================================
// [CGameScene] ゲームオーバー時の処理
//=============================================================
void CGameScene::onGameOver()
{
	if (!m_isGameOvered)
	{ // 1回のみの処理

		// リザルトマネージャーの初期化
		m_resultManager->Init();

		// リザルトカメラを起動する
		m_pCamera->GetComponent<ResultCamera>()->Play();
		m_pCamera->GetComponent<CCameraMove>()->enabled = false;

		m_isGameOvered = true;
	}
}