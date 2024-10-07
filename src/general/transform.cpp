//=============================================================
//
// トランスフォーム [transform.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "transform.h"
#include "internal/physics.h"

//=============================================================
// [Transform] 回転の設定
//=============================================================
void Transform::SetRot(float x, float y, float z)
{
	m_rotation = { x, y, z };
}

//=============================================================
// [Transform] 親を設定する
//=============================================================
void Transform::SetParent(Transform* pParent)
{
	// nullptrが設定されたとき
	if (pParent == nullptr)
	{
		m_pParent = nullptr;
		return;
	}

	// 引数に自分のポインタが入れられたとき
	if (pParent == this)
		return;

	// 指定したトランスフォームの親が自分以外の時のみ、設定する
	if (pParent->GetParent() != this)
		m_pParent = pParent;
}

//=============================================================
// [Transform] ワールド座標での位置を取得
//=============================================================
D3DXVECTOR3 Transform::GetWPos()
{
	D3DXMATRIX mtx = GetMatrix();
	D3DXVECTOR3 pos = { mtx._41, mtx._42, mtx._43 };
	return pos;
}

//=============================================================
// [Transform] ワールド座標での回転を取得
//=============================================================
D3DXVECTOR3 Transform::GetWRot()
{
	//rot.z = atan2f(mtx._12, mtx._22);
	//rot.x = asinf(-mtx._32);
	//rot.y = atan2f(mtx._31, mtx._33);

	//if (fabsf(cosf(rot.x)) < 1.0e-6f)
	//{
	//	rot.z += mtx._12 > 0.0f ? D3DX_PI : -D3DX_PI;
	//	rot.y += mtx._31 > 0.0f ? D3DX_PI : -D3DX_PI;
	//}

	// 親のワールド座標をローカル座標に足す
	D3DXVECTOR3 worldRot = { 0.0f, 0.0f, 0.0f };
	Transform* worldTransform = this;
	do
	{
		// 座標を足す
		worldRot += worldTransform->GetRot();

		// 次の親に進む
		worldTransform = worldTransform->GetParent();

	} while (worldTransform != nullptr);

	// 正規化
	worldRot = NormalizeRotation(worldRot);

	return worldRot;
}

//=============================================================
// [Transform] ワールド座標でのスケールを取得
//=============================================================
D3DXVECTOR3 Transform::GetWScale()
{
	// 親のワールド座標をローカル座標に掛ける
	D3DXVECTOR3 worldScale = { 1.0f, 1.0f, 1.0f };
	Transform* worldTransform = this;
	do
	{
		// 座標を掛ける
		worldScale.x *= worldTransform->GetScale().x;
		worldScale.y *= worldTransform->GetScale().y;
		worldScale.z *= worldTransform->GetScale().z;

		// 次の親に進む
		worldTransform = worldTransform->GetParent();

	} while (worldTransform != nullptr);

	return worldScale;
}

//=============================================================
// [Transform] マトリックスを取得する
//=============================================================
D3DXMATRIX& Transform::GetMatrix()
{
	// 変数
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtx);

	// スケールを反映
	D3DXVECTOR3 scale = GetScale();
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxScale);

	// 向きを反映
	D3DXVECTOR3 rotation = GetRot();
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotation.y, rotation.x, rotation.z);
	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxRot);

	// 位置を反映
	D3DXVECTOR3 position = GetPos();
	D3DXMatrixTranslation(&mtxTrans, position.x, position.y, position.z);
	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxTrans);

	if (GetParent() != nullptr)
	{
		D3DXMATRIX mtxParent;
		mtxParent = GetParent()->GetMatrix();
		D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxParent);
	}

	return m_mtx;
}

//=============================================================
// [Transform] 演算子 ==
//=============================================================
bool Transform::operator ==(Transform& transform)
{
	if (GetWPos() == transform.GetWPos() &&
		GetWRot() == transform.GetWRot() &&
		GetWScale() == transform.GetWScale() &&
		m_pParent == transform.m_pParent)
	{
		return true;
	}
	return false;
}

//=============================================================
// [Transform] 指定位置の方向を向く
//=============================================================
void Transform::LookAt(D3DXVECTOR3 target)
{
	// 現在の位置を取得する
	D3DXVECTOR3 pos = GetWPos();

	// 向くべき方向を算出する
	D3DXVECTOR3 rot;
	float fTargetAngle = -atan2f(target.x - pos.x, target.z - pos.z);
	float fTargetHeightAngle = atan2f(sqrtf(fabsf(target.x - pos.x) * fabsf(target.x - pos.x) +
	fabsf(target.z - pos.z) * fabsf(target.z - pos.z)), (target.y - pos.y));

	// 向かせる
	rot = D3DXVECTOR3(-fTargetHeightAngle + D3DX_PI * 0.5f, fTargetAngle, 0.0f);
	SetRot(rot);
}

//=============================================================
// [Transform] 回転の正規化
//=============================================================
D3DXVECTOR3 Transform::NormalizeRotation(const D3DXVECTOR3& rot)
{
	D3DXVECTOR3 normalized = rot;
	if (normalized.x > D3DX_PI)
	{
		normalized.x -= D3DX_PI * 2;
	}
	if (normalized.y > D3DX_PI)
	{
		normalized.y -= D3DX_PI * 2;
	}
	if (normalized.z > D3DX_PI)
	{
		normalized.z -= D3DX_PI * 2;
	}

	if (normalized.x < -D3DX_PI)
	{
		normalized.x += D3DX_PI * 2;
	}
	if (normalized.y < -D3DX_PI)
	{
		normalized.y += D3DX_PI * 2;
	}
	if (normalized.z < -D3DX_PI)
	{
		normalized.z += D3DX_PI * 2;
	}
	return normalized;
}