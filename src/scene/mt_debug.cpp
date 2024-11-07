//=============================================================
//
// マウンテンのデバッグシーン [mt_debug.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "mt_debug.h"

#include "component/3d/camera.h"
#include "component/3d/movement_space.h"
#include "component/3d/light.h"
#include "component/3d/meshfield.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scripts/terrain.h"

#include <DTL.hpp>
#include "DTL/Storage/FilePNG.hpp"

//=============================================================
// [CMountainDebug] 初期化
//=============================================================
void CMountainDebug::Init()
{
	// カメラ
	m_pCamera = new GameObject;
	m_pCamera->SetName("Camera");
	m_pCamera->SetTag("Camera");
	m_pCamera->AddComponent<CCamera>();
	m_pCamera->GetComponent<CCamera>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
	m_pCamera->GetComponent<CCamera>()->m_fClippingPlanesFar = 5000.0f;
	m_pCamera->AddComponent<CMovementSpace>();

	// ライト
	GameObject* pLight = new GameObject;
	CD3DLight::SetDefaultD3DLight(pLight);

	GameObject* pSLight = new GameObject;
	pSLight->AddComponent<CLight>()->SetIntensity(500.0f);
	pSLight->transform->Translate(0.0f, 100.0f, 0.0f);

	GameObject* pFloor = new GameObject;
	pFloor->transform->Translate(0.0f, -500.0f, 0.0f);
	pFloor->AddComponent<CBoxCollider>(D3DXVECTOR3(500.0f, 2.0f, 500.0f));
}

//=============================================================
// [CMountainDebug] 終了
//=============================================================
void CMountainDebug::Uninit()
{
}

//=============================================================
// [CMountainDebug] 更新
//=============================================================
void CMountainDebug::Update()
{
	if (INPUT_INSTANCE->onTrigger("o"))
	{
		GameObject* pTerrain = new GameObject;
		pTerrain->transform->Translate(0.0f, -200.0f, 0.0f);
		pTerrain->AddComponent<CTerrain>();
		pTerrain->GetComponent<CTerrain>()->Generate();
	}

	if (INPUT_INSTANCE->onTrigger("b"))
	{
		GameObject::LoadPrefab("data\\PREFAB\\bench.pref");
	}
}

//=============================================================
// [CMountainDebug] 描画
//=============================================================
void CMountainDebug::Draw()
{
}