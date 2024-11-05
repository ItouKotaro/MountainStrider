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

	// カメラの目標地点を計算する
	D3DXVECTOR3 objectivePos;
	objectivePos = pTargetTrans->GetWPos();
	objectivePos += {sinf(pTargetTrans->GetWRotY()) * -150.0f, 50.0f, cosf(pTargetTrans->GetWRotY()) * -150.0f};

	// 目標地点に移動する
	transform->SetPos(objectivePos);

	
	// 回転
	transform->SetRot(0.4f, pTargetTrans->GetWRotY(), 0.0f);
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