//=============================================================
//
// カメラの動き [camera_move.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "camera_move.h"

//=============================================================
// [CCameraMove] 更新
//=============================================================
void CCameraMove::Update()
{
	// ターゲットが存在するか確認する
	if (m_pTarget == nullptr)
	{ // ターゲットが存在しないとき
		return;
	}

	Transform* pTargetTrans = m_pTarget->transform;

	// 回転
	transform->SetRot(/*pTargetTrans->GetRotX() * 0.8f +*/ 0.3f, pTargetTrans->GetWRotY(), 0.0f);

	// カメラの目標地点を計算する
	D3DXVECTOR3 objectivePos;
	objectivePos = pTargetTrans->GetWPos();
	//objectivePos += {
	//	0.0f,
	//	40.0f,
	//	0.0f
	//};
	objectivePos += {
		sinf(pTargetTrans->GetWRotY()) * -150.0f,
		100.0f,
		cosf(pTargetTrans->GetWRotY()) * -150.0f
	};
	//objectivePos -= {
	//		sinf(pTargetTrans->GetWRotY() + D3DX_PI * 0.5f) * pTargetTrans->GetRotZ() * 20.0f,
	//		fabsf(pTargetTrans->GetRotZ()) * 10.0f,
	//		cosf(pTargetTrans->GetWRotY() + D3DX_PI * 0.5f) * pTargetTrans->GetRotZ() * 20.0f
	//};

	// 目標地点に移動する
	transform->SetPos(objectivePos);
}

//=============================================================
// [CCameraMove] オブジェクトが破棄されたときの処理
//=============================================================
void CCameraMove::OnDestroyOtherObject(GameObject* other)
{
	if (other == m_pTarget)
	{ // ターゲットのとき
		m_pTarget = nullptr;
	}
}