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
#include "scripts/vehicle.h"
#include "component/2d/text.h"
#include "component/3d/field.h"
#include "component/3d/particle.h"

#include "render/shadow_rb.h"

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

	//GameObject* pSLight = new GameObject;
	//pSLight->AddComponent<CLight>()->SetIntensity(500.0f);
	//pSLight->transform->Translate(0.0f, 100.0f, 0.0f);

	GameObject* pFloor = new GameObject;
	pFloor->transform->Translate(0.0f, -51.0f, 0.0f);
	pFloor->AddComponent<CField>()->Set(500.0f, 500.0f);

	pFloor = new GameObject;
	pFloor->transform->Translate(0.0f, -50.0f, 0.0f);
	pFloor->AddComponent<CBoxCollider>(D3DXVECTOR3(500.0f, 2.0f, 500.0f));
	pFloor->AddComponent<CField>()->Set(500.0f, 500.0f);

	m_pRollTest = new GameObject;
	m_pRollTest->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\fence.x");

	m_pText = new GameObject();
	m_pText->AddComponent<CText>();
	//m_pGhostTest = new GameObject;
	//m_pGhostTest->transform->Translate(0.0f, -50.0f, 0.0f);
	//m_pGhostTest->AddComponent<CBoxCollider>(D3DXVECTOR3(30.0f, 10.0f, 30.0f));
	//CCollision::GetCollision(m_pGhostTest)->IsTrigger(true);

	// ライトカメラ
	GameObject* m_pLightCamera = new GameObject;
	m_pLightCamera->AddComponent<CCamera>();
	m_pLightCamera->transform->Translate(0.0f, 10.0f, -100.0f);
	m_pLightCamera->transform->Rotate(0.8f, 0.0f, 0.0f);

	// レンダーバッファを登録する
	CRenderer::GetInstance()->RegisterRenderBuffer<ShadowRenderBuffer>("main");
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(m_pCamera->GetComponent<CCamera>());
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetLightCamera(m_pLightCamera->GetComponent<CCamera>());
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

	//if (INPUT_INSTANCE->onTrigger("o"))
	//{
	//	GameObject* pBox = new GameObject;
	//	pBox->SetName("BOX");
	//	pBox->transform->Translate(0.0f, 20.0f, 0.0f);
	//	pBox->AddComponent<CBoxCollider>(D3DXVECTOR3(5, 5, 5));
	//	pBox->AddComponent<CRigidBody>();
	//	CCollision::GetCollision(pBox)->SetMass(0.0f);
	//}

	D3DXVECTOR3 angularVelocity = { 0.0f, 0.0f, 0.0f };
	if (INPUT_INSTANCE->onPress("up"))
	{
		angularVelocity += {sinf(0.0f+D3DX_PI*0.5f) * 1.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * 1.5f};
	}
	if (INPUT_INSTANCE->onPress("down"))
	{
		angularVelocity += {sinf(0.0f + D3DX_PI * 0.5f) * -1.5f, 0.0f, cosf(0.0f + D3DX_PI * 0.5f) * -1.5f};
	}
	//m_pRollTest->transform->Rotate(angularVelocity * 0.08f);

	angularVelocity *= 0.08f;
	m_pRollTest->transform->Rotate(angularVelocity);

	m_pText->GetComponent<CText>()->SetText(std::to_string(5));

	//if (INPUT_INSTANCE->onTrigger("o"))
	//{
	//	m_pTerrain = new GameObject;
	//	m_pTerrain->transform->Translate(0.0f, -200.0f, 0.0f);
	//	m_pTerrain->AddComponent<Terrain>();
	//	m_pTerrain->GetComponent<Terrain>()->Generate();
	//}

	// バイクを生成する
	if (INPUT_INSTANCE->onTrigger("b"))
	{
		GameObject* pBike = new GameObject();
		pBike->transform->Rotate(0.0f, D3DX_PI, 0.0f);
		pBike->AddComponent<CVehicle>();
	}

}

//=============================================================
// [CMountainDebug] 描画
//=============================================================
void CMountainDebug::Draw()
{
}