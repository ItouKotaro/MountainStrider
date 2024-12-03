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
#include "component/3d/field.h"

#include "scripts/vehicle.h"
#include "scripts/terrain.h"
#include "scripts/camera_move.h"
#include "scripts/result/result_camera.h"
#include "scripts/result/mt_result.h"
#include "scripts/status_ui.h"

#include <noise/noise.h>

//=============================================================
// [CGameScene] 初期化
//=============================================================
void CGameScene::Init()
{
	// 変数の初期化
	m_endType = ENDTYPE_NONE;
	m_travellingCount = 0;
	m_travellingDatas.clear();

	// カメラの作成
	{
		m_pCamera = new GameObject("Camera", "Camera");
		m_pCamera->AddComponent<CCamera>();
		m_pCamera->GetComponent<CCamera>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
		m_pCamera->GetComponent<CCamera>()->m_fClippingPlanesFar = 5000.0f;
		m_pCamera->GetComponent<CCamera>()->GetSkybox()->LoadSkybox("data\\SKYBOX\\daylight.json");
	}

	// ライトを作成
	{
		GameObject* pLight = new GameObject("Light");
		CD3DLight::SetDefaultD3DLight(pLight);
	}

	// 地面を作成
	{
		m_pTerrain = new Terrain();
		m_pTerrain->SetSeed((unsigned int)clock());
		m_pTerrain->Init();
		m_pTerrain->Generate();
	}


	// 奈落
	{
		m_voidField = new GameObject("Void");
		m_voidField->AddComponent<CField>();
		m_voidField->GetComponent<CField>()->Set(Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE * 2, Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE * 2);
		m_voidField->transform->SetPos(0.0f, m_pTerrain->GetMinHeight() - 20.0f, 0.0f);
	}


	// リザルトマネージャーの生成
	m_resultManager = new MountainResultManager(this);

	// バイクの生成
	SpawnBike();

	// カメラの移動設定を行う
	m_pCamera->AddComponent<CCameraMove>()->SetTarget(m_pBike);
	m_pCamera->AddComponent<ResultCamera>();

	// FPS表示
	m_pFPS = new GameObject("FPS");
	m_pFPS->AddComponent<CText>();

	// 開始時間を記録する
	m_startTime = timeGetTime();

	// レンダーバッファを登録する
	CameraRenderBuffer* renderBuff = CRenderer::GetInstance()->RegisterRenderBuffer<CameraRenderBuffer>("main");
	renderBuff->SetCamera(m_pCamera->GetComponent<CCamera>());

	m_pMapDebug = new GameObject();
	m_pMapDebug->AddComponent<CText>();
	m_pMapDebug->transform->SetPos(0.0f, 200.0f);
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

	// レンダーバッファのカメラをnullにする
	static_cast<CameraRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(nullptr);
}

//=============================================================
// [CGameScene] 更新
//=============================================================
void CGameScene::Update()
{
	if (INPUT_INSTANCE->onTrigger("@"))
	{
		onGameOver();
	}

	// FPSを更新する
	m_pFPS->GetComponent<CText>()->SetText("FPS: " + std::to_string(CManager::GetInstance()->GetFPS()));

	// 地形を更新する
	m_pTerrain->Update(m_pCamera->transform->GetWPos());

	// 未ゲームオーバー時
	if (m_endType == ENDTYPE_NONE)
	{
		// カメラの情報を記録する
		m_pCamera->GetComponent<ResultCamera>()->RecordData();

		// 走行データを記録する
		m_travellingCount++;
		if (m_travellingCount >= 20)
		{
			TravellingData travelling;
			travelling.pos = m_pCamera->transform->GetWPos();
			travelling.rot = m_pCamera->transform->GetWQuaternion();
			m_travellingDatas.push_back(travelling);
			m_travellingCount = 0;

			m_pMapDebug->GetComponent<CText>()->SetText("Rec: " + std::to_string(m_travellingDatas.size()));
		}

		// 最高速度を記録する
		int bikeSpeed = static_cast<int>(m_pBike->GetComponent<CVehicle>()->GetSpeed());
		m_highSpeed = m_highSpeed < bikeSpeed ? bikeSpeed : m_highSpeed;

		// クリア条件
		ClearCondition();
	}

	// リザルトの更新処理
	if (m_endType != ENDTYPE_NONE)
	{ // ゲームオーバーのとき
		// リザルトマネージャーの更新
		m_resultManager->Update();
	}
}

//=============================================================
// [CGameScene] 描画
//=============================================================
void CGameScene::Draw()
{
}

//=============================================================
// [CGameScene] ゲームのリセット
//=============================================================
void CGameScene::ResetGame()
{
	// バイクの燃料と耐久値を回復させる
	CVehicle::ResetState();

	// リザルトデータのリセット
	MountainResultManager::Reset();
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

	// ステータスUIを生成
	m_pStatusUI = new GameObject("StatusUI", "UI");
	m_pStatusUI->AddComponent<CStatusUI>();
	m_pBike->GetComponent<CVehicle>()->SetStatusUI(m_pStatusUI->GetComponent<CStatusUI>());
}

//=============================================================
// [CGameScene] クリアの条件
//=============================================================
void CGameScene::ClearCondition()
{
	float terrainLength = static_cast<float>(Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE);
	D3DXVECTOR3 vehiclePos = m_pBike->transform->GetWPos();
	if (vehiclePos.y < m_pTerrain->GetMinHeight() - 5.0f)
	{
		onClear();
	}
}

//=============================================================
// [CGameScene] ゲームオーバー時の処理
//=============================================================
void CGameScene::onGameOver()
{
	if (m_endType == ENDTYPE_NONE)
	{ // 1回のみの処理
		// ステータスUIを非表示にする
		m_pStatusUI->GetComponent<CStatusUI>()->SetVisible(false);

		// リザルトデータの格納
		MountainResultManager::ResultData data;
		data.time = -1;
		data.highSpeed = m_highSpeed;
		data.action = 50;
		MountainResultManager::AddResult(data);

		// リザルトマネージャーの初期化
		m_resultManager->Init(MountainResultManager::TYPE_GAMEOVER);

		// リザルトカメラを起動する
		m_pCamera->GetComponent<ResultCamera>()->Play();
		m_pCamera->GetComponent<CCameraMove>()->enabled = false;

		// ゲームオーバートリガーを有効にする
		m_endType = ENDTYPE_GAMEOVER;
	}
}

//=============================================================
// [CGameScene] クリア時の処理
//=============================================================
void CGameScene::onClear()
{
	if (m_endType == ENDTYPE_NONE)
	{ // 1回のみの処理
		// ステータスUIを非表示にする
		m_pStatusUI->GetComponent<CStatusUI>()->SetVisible(false);

		// リザルトデータの格納
		MountainResultManager::ResultData data;
		data.time = (timeGetTime() - m_startTime) / 1000;
		data.highSpeed = m_highSpeed;
		data.action = 50;
		MountainResultManager::AddResult(data);

		// リザルトマネージャーの初期化
		m_resultManager->Init(MountainResultManager::TYPE_CLEAR);

		// リザルトカメラを起動する
		m_pCamera->GetComponent<ResultCamera>()->Play();
		m_pCamera->GetComponent<CCameraMove>()->enabled = false;

		// クリアトリガーを有効にする
		m_endType = ENDTYPE_CLEAR;
	}
}