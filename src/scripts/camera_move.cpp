//=============================================================
//
// カメラの動き [camera_move.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "camera_move.h"

#include "manager.h"
#include "scripts/vehicle.h"
#include "component/3d/camera.h"
#include "component/3d/meshfield.h"

//=============================================================
// [CCameraMove] 初期化
//=============================================================
void CCameraMove::Init()
{
	m_cameraRot = { 0.0f, 0.0f, 0.0f };
	m_moveCounter = 0;
}

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

	// カーソル
	if (Main::IsActiveWindow())
	{
		// 現在のカーソル位置を取得する
		CManager::CursorPos cursor = CManager::GetInstance()->GetCursorClientPos();

		// 前回のカーソル位置と異なるとき
		if (cursor.x != m_oldCursor.x || cursor.y != m_oldCursor.y)
		{ // 前回の位置と異なるとき
			m_cameraRot.y += (cursor.x - m_oldCursor.x) * 0.002f;
			m_cameraRot.x += (cursor.y - m_oldCursor.y) * 0.002f;
		}

		// ウィンドウ外にマウスが行ったとき
		CManager::GetInstance()->SetCursorClientPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), static_cast<float>(CRenderer::SCREEN_HEIGHT / 2));
		m_oldCursor = CManager::GetInstance()->GetCursorClientPos();
	}

	// コントローラーの情報を取得する
	auto pGamepadDev = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>();
	short stickRX = pGamepadDev->GetState().Gamepad.sThumbRX;
	short stickRY = pGamepadDev->GetState().Gamepad.sThumbRY;
	if (Main::IsActiveWindow())
	{
		if (stickRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.y += -0.05f;
		}
		if (stickRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.y += 0.05f;
		}
		if (stickRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.x += 0.02f;
		}
		if (stickRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.x += -0.02f;
		}
	}

	// TPS視点の更新
	UpdateTPS();

	if (m_moveCounter > 0)
	{
		m_moveCounter--;
		transform->Translate((m_posS - transform->GetPos()) * 0.5f);
	}
	else
	{
		transform->SetPos(m_posS);
	}
}

//=============================================================
// [CCameraMove] 更新
//=============================================================
void CCameraMove::UpdateFPS()
{
	// カメラの取得
	CCamera* pCamera = gameObject->GetComponent<CCamera>();
	pCamera->SetCustomPosR(true);

	// 補正
	if (m_cameraRot.x < -D3DX_PI * 0.5f)
	{
		m_cameraRot.x = -D3DX_PI * 0.5f;
	}
	else if (m_cameraRot.x > D3DX_PI * 0.5f)
	{
		m_cameraRot.x = D3DX_PI * 0.5f;
	}
	if (m_cameraRot.y < -D3DX_PI * 0.5f)
	{
		m_cameraRot.y = -D3DX_PI * 0.5f;
	}
	else if (m_cameraRot.y > D3DX_PI * 0.5f)
	{
		m_cameraRot.y = D3DX_PI * 0.5f;
	}

	// 視点
	D3DXVECTOR3 fpsPos = D3DXVECTOR3(0.0f, 20.0f, -10.0f);
	D3DXMATRIX fpsMtx = m_pTarget->transform->GetMatrix();
	D3DXVec3TransformCoord(&fpsPos, &fpsPos, &fpsMtx);
	m_posS = fpsPos;

	// 注視点
	fpsPos = D3DXVECTOR3(0.0f, 20.0f, -10.0f);
	fpsPos += {sinf(m_cameraRot.y + D3DX_PI) * 50.0f, sinf(m_cameraRot.x) * -50.0f, cosf(m_cameraRot.y + D3DX_PI) * 50.0f};
	D3DXVec3TransformCoord(&fpsPos, &fpsPos, &fpsMtx);
	pCamera->SetPosR(fpsPos);
}

//=============================================================
// [CCameraMove] 更新
//=============================================================
void CCameraMove::UpdateTPS()
{
	// カメラの取得
	CCamera* pCamera = gameObject->GetComponent<CCamera>();
	pCamera->SetCustomPosR(true);
	pCamera->SetPosR(m_pTarget->transform->GetWPos() + D3DXVECTOR3(0.0f, 9.0f, 0.0f));

	if (m_moveCounter <= 0)
	{ // 最初
		m_cameraRot = m_pTarget->transform->GetRot();
		m_cameraRot.y += D3DX_PI;
	}

	// 正規化
	if (m_cameraRot.x > D3DX_PI * 0.5f - UP_LIMIT)
	{
		m_cameraRot.x = D3DX_PI * 0.5f - UP_LIMIT;
	}
	if (m_cameraRot.x < -D3DX_PI * 0.5f + BOTTOM_LIMIT)
	{
		m_cameraRot.x = -D3DX_PI * 0.5f + BOTTOM_LIMIT;
	}

	// 視点の位置を計算する
	float distance = 100.0f;
	D3DXVECTOR3 posS = { 0.0f, 0.0f, -distance };
	D3DXMATRIX mtxY, mtxX, mtxS;
	D3DXMatrixRotationX(&mtxX, m_cameraRot.x);
	D3DXMatrixRotationY(&mtxY, m_cameraRot.y);
	mtxS = mtxX * mtxY;
	D3DXVec3TransformCoord(&posS, &posS, &mtxS);

	// レイでカメラの位置を決める
	btVector3 Start = btVector3(m_pTarget->transform->GetWPos().x, m_pTarget->transform->GetWPos().y + 10.0f, m_pTarget->transform->GetWPos().z);
	Start += btVector3(posS.x, posS.y, posS.z) * 0.2f;
	btVector3 End = Start + btVector3(posS.x, posS.y, posS.z);
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit() && CCollision::GetGameObjectFromCollisionObj(RayCallback.m_collisionObject)->GetComponent<CMeshField>() != nullptr)
	{ // ヒットしたとき
		D3DXVECTOR3 minPos = { 0.0f, 0.0f, 20.0f };
		D3DXVec3TransformCoord(&minPos, &minPos, &mtxS);
		posS = { RayCallback.m_hitPointWorld.getX(), RayCallback.m_hitPointWorld.getY(), RayCallback.m_hitPointWorld.getZ() };
		posS += {minPos.x, minPos.y, minPos.z};
	}
	else
	{
		posS += m_pTarget->transform->GetWPos();
	}
	m_posS = posS;

	m_moveCounter = 60;
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