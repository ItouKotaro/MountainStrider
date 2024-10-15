//=============================================================
//
// ゲームシーン [game.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "game.h"
#include "component/3d/camera.h"
#include "component/3d/light.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"

//=============================================================
// [CGameScene] 初期化
//=============================================================
void CGameScene::Init()
{
	m_pCamera = new GameObject;
	m_pCamera->SetName("Camera");
	m_pCamera->SetTag("Camera");
	m_pCamera->AddComponent<CCamera>();
	m_pCamera->GetComponent<CCamera>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
	m_pCamera->transform->Translate(0.0f, 100.0f, -200.0f);
	m_pCamera->transform->LookAt({ 0.0f, 0.0f, 0.0f });

	// ライトを作成
	GameObject* pLight = new GameObject();
	CD3DLight::SetDefaultD3DLight(pLight);

	// ベンチを作成
	GameObject* pBench = GameObject::LoadPrefab("data\\PREFAB\\motor_bike.pref", Transform({ 0.0f, 20.0f, 200.0f }));

	// 地面を作成
	GameObject* pFloor = new GameObject;
	pFloor->transform->Translate(0.0f, -30.0f, 100.0f);
	pFloor->AddComponent<CBoxCollider>(D3DXVECTOR3(200.0f, 1.0f, 200.0f));
}

//=============================================================
// [CGameScene] 終了
//=============================================================
void CGameScene::Uninit()
{
}

//=============================================================
// [CGameScene] 更新
//=============================================================
void CGameScene::Update()
{
}

//=============================================================
// [CGameScene] 描画
//=============================================================
void CGameScene::Draw()
{
}