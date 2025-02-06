//=============================================================
//
// ゲームオブジェクト [direction_arrow.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "direction_arrow.h"
#include "scene/game.h"

//=============================================================
// [DirectionArrow] 初期化
//=============================================================
void DirectionArrow::Init()
{
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// バイクを取得する
	m_vehicle = gameScene->GetBike();

	// 矢印のモデルの初期化
	m_arrowModel = new SingleComponent<CMesh>();
	m_arrowModel->Init();
	m_arrowModel->LoadMeshX("data\\MODEL\\direction_arrow.x");
	m_arrowModel->SetParent(gameObject);
}

//=============================================================
// [DirectionArrow] 終了
//=============================================================
void DirectionArrow::Uninit()
{
	// 矢印のモデルの破棄
	if (m_arrowModel != nullptr)
	{
		m_arrowModel->Uninit();
		delete m_arrowModel;
		m_arrowModel = nullptr;
	}
}

//=============================================================
// [DirectionArrow] 更新
//=============================================================
void DirectionArrow::Update()
{
	// バイクの位置に基づいて矢印の位置を決める
	D3DXVECTOR3 pos;
	pos = m_vehicle->transform->GetWPos();
	pos.y = m_fadeY;
	gameObject->transform->SetPos(pos);

	// 回転方向を決める
	m_angle += (Benlib::PosAngle(m_vehicle->transform->GetWPos(), m_destination) - m_angle) * 0.08f;
	gameObject->transform->Translate(sinf(m_angle) * 50.0f, 0.0f, cosf(m_angle) * 50.0f);
	gameObject->transform->SetRot(0.0f, m_angle, 0.0f);

	// 例を放って高さを確かめる
	btVector3 Start = btVector3(gameObject->transform->GetWPos().x, m_vehicle->transform->GetWPos().y + 20.0f, gameObject->transform->GetWPos().z);
	btVector3 End = btVector3(gameObject->transform->GetWPos().x, m_vehicle->transform->GetWPos().y - 50.0f, gameObject->transform->GetWPos().z);

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit())
	{ // ヒットしたとき
		m_fadeY += ((RayCallback.m_hitPointWorld.getY() + 5.0f) - m_fadeY) * 0.08f;
	}

	// 矢印のモデルの更新
	m_arrowModel->Update();
}

//=============================================================
// [DirectionArrow] 描画
//=============================================================
void DirectionArrow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得

	// 矢印のモデルの描画
	m_arrowModel->SetShader(GetShaderInfo());
	m_arrowModel->Draw();
}