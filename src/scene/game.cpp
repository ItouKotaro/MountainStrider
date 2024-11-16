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

#include <noise/noise.h>

//=============================================================
// [CGameScene] 初期化
//=============================================================
void CGameScene::Init()
{
	// カメラの作成
	m_pCamera = new GameObject("Camera", "Camera");
	m_pCamera->AddComponent<CCamera>();
	m_pCamera->GetComponent<CCamera>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
	m_pCamera->GetComponent<CCamera>()->m_fClippingPlanesFar = 5000.0f;

	// ライトを作成
	GameObject* pLight = new GameObject("Light");
	CD3DLight::SetDefaultD3DLight(pLight);

	// 地面を作成
	m_pTerrain = new Terrain();
	m_pTerrain->Init();
	m_pTerrain->Generate();

	// バイクの生成
	SpawnBike();

	// カメラの移動設定を行う
	m_pCamera->AddComponent<CCameraMove>()->SetTarget(m_pBike);


	m_pFPS = new GameObject("FPS");
	m_pFPS->AddComponent<CText>();
}

//=============================================================
// [CGameScene] 終了
//=============================================================
void CGameScene::Uninit()
{
	if (m_pTerrain != nullptr)
	{
		m_pTerrain->Uninit();
		delete m_pTerrain;
		m_pTerrain = nullptr;
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
	m_pTerrain->Update(m_pBike->transform->GetWPos());
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
	m_pBike = new GameObject;
	m_pBike->transform->Rotate(0.0f, D3DX_PI, 0.0f);
	m_pBike->AddComponent<CVehicle>();

	m_pBike->GetComponent<CVehicle>()->SetPos({ 0.0f, hitY + 50.0f, 0.0f });
}