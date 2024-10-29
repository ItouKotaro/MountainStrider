//=============================================================
//
// 空間移動コンポーネント [movement_space.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "movement_space.h"

//=============================================================
// [CMovementSpace] コンストラクタ
//=============================================================
CMovementSpace::CMovementSpace(float fSpeed, float fMouseSpeed)
{
	m_ptBeforeLMouse = {0, 0};
	m_ptBeforeRMouse = { 0, 0 };
	m_fSpeed = fSpeed;
	m_fMouseSpeed = fMouseSpeed;
}

//=============================================================
// [CMovementSpace] 更新
//=============================================================
void CMovementSpace::Update()
{
	D3DXVECTOR2 LeftClickMove = GetLeftClickingMouseMove();
	D3DXVECTOR2 RightClickMove = GetRightClickingMouseMove();

	// 移動処理
	if (INPUT_INSTANCE->onPress("w") ||
		INPUT_INSTANCE->onPress("a") ||
		INPUT_INSTANCE->onPress("s") ||
		INPUT_INSTANCE->onPress("d"))
	{
		float fAngle = -GetMoveAngle() + transform->GetRotY() + D3DX_PI * 0.5f;  // 角度取得
		transform->Translate(sinf(fAngle) * m_fSpeed, 0.0f, cosf(fAngle) * m_fSpeed);
	}

	//--------------------------------------------------------
	// 位置を変更（左右クリック）
	if (GetKeyState(VK_LBUTTON) & 0x80 && GetKeyState(VK_RBUTTON) & 0x80)
	{
		transform->Translate(
			cosf(transform->GetRotY()) * LeftClickMove.y * m_fSpeed,
			0.0f,
			sinf(transform->GetRotY()) * LeftClickMove.y * m_fSpeed
		);

		transform->Translate(
			cosf(D3DX_PI / 2 + transform->GetRotY()) * LeftClickMove.x * m_fSpeed,
			0.0f,
			sinf(D3DX_PI / 2 + transform->GetRotY()) * LeftClickMove.x * m_fSpeed
		);

		return;
	}

	if (INPUT_INSTANCE->onPress("space"))
	{
		transform->Translate(0.0f, 10.0f, 0.0f);
	}
	if (INPUT_INSTANCE->onPress("lshift"))
	{
		transform->Translate(0.0f, -10.0f, 0.0f);
	}

	//--------------------------------------------------------
	// 視点の位置を変更（左クリック）
	transform->Rotate(LeftClickMove.y * m_fMouseSpeed, -LeftClickMove.x * m_fMouseSpeed, 0.0f);
}

//=============================================================
// [CMovementSpace] 左クリック時のマウス移動量の取得
//=============================================================
D3DXVECTOR2 CMovementSpace::GetLeftClickingMouseMove()
{
	// マウス移動量用の変数
	D3DXVECTOR2 mouseMove = D3DXVECTOR2(0.0f, 0.0f);

	if (GetKeyState(VK_LBUTTON) & 0x80 && GetActiveWindow() != nullptr)
	{ // 左クリック時
		// マウスの座標を取得する
		POINT point;
		GetCursorPos(&point);

		// 前回のマウス座標と比較し、移動量を取得する
		mouseMove.x = (float)(m_ptBeforeLMouse.x - point.x);
		mouseMove.y = (float)(m_ptBeforeLMouse.y - point.y);

		// 今回のマウス座標を保存する
		GetCursorPos(&m_ptBeforeLMouse);
	}
	else
	{ // 左クリックされていないとき
		// 現在のマウス位置を保存
		GetCursorPos(&m_ptBeforeLMouse);
	}

	return mouseMove;	// 移動量を返す
}

//=============================================================
// [CMovementSpace] 右クリック時のマウス移動量の取得
//=============================================================
D3DXVECTOR2 CMovementSpace::GetRightClickingMouseMove()
{
	// マウス移動量用の変数
	D3DXVECTOR2 mouseMove = D3DXVECTOR2(0.0f, 0.0f);

	if (GetKeyState(VK_RBUTTON) & 0x80 && GetActiveWindow() != nullptr)
	{ // 右クリック時
		// マウスの座標を取得する
		POINT point;
		GetCursorPos(&point);

		// 前回のマウス座標と比較し、移動量を取得する
		mouseMove.x = (float)(m_ptBeforeRMouse.x - point.x);
		mouseMove.y = (float)(m_ptBeforeRMouse.y - point.y);

		// 今回のマウス座標を保存する
		GetCursorPos(&m_ptBeforeRMouse);
	}
	else
	{ // 右クリックされていないとき
		// 現在のマウス位置を保存
		GetCursorPos(&m_ptBeforeRMouse);
	}

	return mouseMove;	// 移動量を返す
}

//=============================================================
// [CMovementSpace] 移動角度を取得
//=============================================================
float CMovementSpace::GetMoveAngle()
{
	float fMoveX = 0.0f;
	float fMoveY = 0.0f;

	if (INPUT_INSTANCE->onPress("w"))
	{
		fMoveY += 1.0f;
	}
	if (INPUT_INSTANCE->onPress("s"))
	{
		fMoveY += -1.0f;
	}
	if (INPUT_INSTANCE->onPress("a"))
	{
		fMoveX += -1.0f;
	}
	if (INPUT_INSTANCE->onPress("d"))
	{
		fMoveX += 1.0f;
	}

	return atan2f(fMoveY, fMoveX);
}