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
#include "component/3d/field.h"
#include "scripts/vehicle.h"

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
	m_pCamera->transform->Translate(100.0f, 0.0f, 0.0f);
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
	pFloor->AddComponent<CField>();
	pFloor->GetComponent<CField>()->Set(400.0f, 400.0f);

	// バイクの生成
	CreateBike();
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
	if (INPUT_INSTANCE->onTrigger("space"))
	{
		GameObject* pObj = new GameObject();
		pObj->transform->Translate(0.0f, 10.0f, 0.0f);
		pObj->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
		pObj->AddComponent<CSphereCollider>(10.0f);
		pObj->AddComponent<CRigidBody>();
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
void CGameScene::CreateBike()
{
	// バイクを生成する
	GameObject* pBikeBody = new GameObject;
	pBikeBody->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 10.0f, 30.0f));
	pBikeBody->AddComponent<CRigidBody>()->EnableAlwayActive();
	pBikeBody->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");

	GameObject* pBikeHandle = new GameObject;
	pBikeHandle->SetParent(pBikeBody);
	pBikeHandle->transform->Translate(0.0f, 8.0f, -25.0f);
	pBikeHandle->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\handle.x");

	GameObject* pBikeFrontTire = new GameObject;
	pBikeFrontTire->transform->Translate(0.0f, -30.0f, -45.0f);
	pBikeFrontTire->AddComponent<CCylinderCollider>(15.0f, 3.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	pBikeFrontTire->AddComponent<CRigidBody>()->EnableAlwayActive();
	pBikeFrontTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");

	GameObject* pBikeBehindTire = new GameObject;
	pBikeBehindTire->transform->Translate(0.0f, -30.0f, 23.0f);
	pBikeBehindTire->AddComponent<CCylinderCollider>(15.0f, 3.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	pBikeBehindTire->AddComponent<CRigidBody>()->EnableAlwayActive();
	pBikeBehindTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");

	GameObject* pBikeHinge2 = new GameObject;
	pBikeHinge2->AddComponent<CHinge2Constraint>()->SetConstraint(
		CCollision::GetCollision(pBikeBody)->GetRigidBody(),
		CCollision::GetCollision(pBikeFrontTire)->GetRigidBody(),
		pBikeFrontTire->transform->GetPos(),
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	);
	pBikeHinge2->AddComponent<CHinge2Constraint>()->SetConstraint(
		CCollision::GetCollision(pBikeBody)->GetRigidBody(),
		CCollision::GetCollision(pBikeBehindTire)->GetRigidBody(),
		pBikeBehindTire->transform->GetPos(),
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	);
}