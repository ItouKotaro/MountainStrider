//=============================================================
//
// カメラの動き [camera_move.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "camera_move.h"
#include "scripts/vehicle.h"

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

	// 情報の取得
	Transform* pTargetTrans = m_pTarget->transform;
	D3DXQUATERNION objectiveRot = pTargetTrans->GetQuaternion();
	D3DXQUATERNION targetRot = transform->GetQuaternion();
	D3DXVECTOR3 objectivePos;

	bool flying = m_pTarget->GetComponent<CVehicle>()->GetFlying();
	if (!flying)
	{
		// カメラの目標地点を計算する
		D3DXMATRIX vehicleMtx = pTargetTrans->GetMatrix();
		objectivePos += {
				0.0f,
				40.0f,
				-150.0f
		};
		D3DXVec3TransformCoord(&objectivePos, &objectivePos, &vehicleMtx);
	}
	else
	{
		// カメラの目標地点を計算する
		objectivePos += {
				0.0f,
				30.0f,
				-250.0f
		};
		D3DXMATRIX mtx;
		D3DXMATRIX mtxPos = pTargetTrans->GetTranslationMatrix();
		D3DXMatrixRotationY(&mtx, m_pTarget->GetComponent<CVehicle>()->GetStartFlyingAngle());
		D3DXMatrixMultiply(&mtx, &mtx, &mtxPos);
		D3DXVec3TransformCoord(&objectivePos, &objectivePos, &mtx);
	}

	// 回転
	objectiveRot = Benlib::LookAt(transform->GetWPos(), pTargetTrans->GetWPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	D3DXQuaternionSlerp(&objectiveRot, &targetRot, &objectiveRot, 0.08f);
	transform->SetQuaternion(objectiveRot);

	// 目標地点に移動する
	transform->SetPos(transform->GetWPos()+(objectivePos - transform->GetWPos()) * 0.08f);
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