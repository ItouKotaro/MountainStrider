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
	m_pCamera->GetComponent<CCamera>()->m_fClippingPlanesFar = 5000.0f;
	m_pCamera->transform->Translate(100.0f, 10.0f, -50.0f);
	m_pCamera->transform->Rotate(0.0f, -0.8f, 0.0f);
	//m_pCamera->transform->Translate(0.0f, 10.0f, -10.0f);

	// ライトを作成
	GameObject* pLight = new GameObject();
	CD3DLight::SetDefaultD3DLight(pLight);

	// 地面を作成
	GameObject* pFloor = new GameObject;
	pFloor->transform->Translate(0.0f, 0.0f, -2000.0f);
	pFloor->AddComponent<CBoxCollider>(D3DXVECTOR3(5000.0f, 50.0f, 5000.0f), D3DXVECTOR3(0.0f, -50.0f, 0.0f));
	pFloor->AddComponent<CRigidBody>()->GetCollision()->SetMass(0.0f);
	pFloor->AddComponent<CField>();
	pFloor->GetComponent<CField>()->Set(10000.0f, 10000.0f);

	// バイクの生成
	m_pBike = new GameObject;
	m_pBike->transform->Rotate(0.0f, D3DX_PI, 0.0f);

	m_pBike->AddComponent<CVehicle>();

	m_pCamera->SetParent(m_pBike);

	for (int i = 0; i < 1000; i++)
	{
		GameObject* pBench = new GameObject;
		pBench->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\player.x");
		pBench->transform->SetPos(rand() % 10000 - 5000, -30.0f, rand() % 10000 - 5000);
	}
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
	//m_pCamera->transform->SetQuaternion(m_pBike->transform->GetQuaternion());
	//m_pCamera->transform->SetPos(m_pBike->transform->GetWPos() + D3DXVECTOR3(0.0f, 20.0f, 30.0f));

	//m_pCamera->GetComponent<CCamera>()->SetPosR(m_pBike->transform->GetWPos());

	//if (INPUT_INSTANCE->onTrigger("space"))
	//{
	//	GameObject* pObj = new GameObject();
	//	pObj->transform->Translate(0.0f, 10.0f, 0.0f);
	//	pObj->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	//	pObj->AddComponent<CSphereCollider>(10.0f);
	//	pObj->AddComponent<CRigidBody>();
	//}
}

//=============================================================
// [CGameScene] 描画
//=============================================================
void CGameScene::Draw()
{
}