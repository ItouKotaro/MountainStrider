//=============================================================
//
// �J�����̓��� [camera_move.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "camera_move.h"

#include "manager.h"
#include "scripts/vehicle.h"
#include "component/3d/camera.h"

//=============================================================
// [CCameraMove] ������
//=============================================================
void CCameraMove::Init()
{
	m_cameraRot = { 0.0f, 0.0f, 0.0f };
}

//=============================================================
// [CCameraMove] �X�V
//=============================================================
void CCameraMove::Update()
{
	// �^�[�Q�b�g�����݂��邩�m�F����
	if (m_pTarget == nullptr)
	{ // �^�[�Q�b�g�����݂��Ȃ��Ƃ�
		return;
	}

	// �J�����̎擾
	CCamera* pCamera = gameObject->GetComponent<CCamera>();
	pCamera->SetCustomPosR(false);
	pCamera->SetPosR(m_pTarget->transform->GetWPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f));

	// �J�[�\��
	if (GetActiveWindow() == CManager::GetInstance()->GetHWND())
	{
		POINT setPoint;
		setPoint.x = CManager::GetInstance()->GetWindowSize().x / 2;
		setPoint.y = CManager::GetInstance()->GetWindowSize().y / 2;

		POINTS cursor = CManager::GetInstance()->GetCursorPos();
		if (cursor.x != setPoint.x || cursor.y != setPoint.y)
		{ // �O��̈ʒu�ƈقȂ�Ƃ�
			m_cameraRot.y += (cursor.x - setPoint.x) * 0.002f;
			m_cameraRot.x += (cursor.y - setPoint.y) * 0.002f;

			// ���S�ɗ��߂�
			ClientToScreen(CManager::GetInstance()->GetHWND(), &setPoint);
			SetCursorPos(setPoint.x, setPoint.y);
		}
	}

	// �R���g���[���[�̏����擾����
	auto pGamepadDev = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>();
	short stickRX = pGamepadDev->GetState().Gamepad.sThumbRX;
	short stickRY = pGamepadDev->GetState().Gamepad.sThumbRY;
	if (GetActiveWindow() == CManager::GetInstance()->GetHWND())
	{
		if (stickRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.y += -0.02f;
		}
		if (stickRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.y += 0.02f;
		}
		if (stickRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.x += -0.02f;
		}
		if (stickRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.x += 0.02f;
		}
	}


	// ���K��
	if (m_cameraRot.x < D3DX_PI * 0.5f + 0.1f)
	{
		m_cameraRot.x = D3DX_PI * 0.5f + 0.1f;
	}
	if (m_cameraRot.x > D3DX_PI)
	{
		m_cameraRot.x = D3DX_PI;
	}

	// �ݒ�
	bool flying = m_pTarget->GetComponent<CVehicle>()->GetFlying();

	// ���_�̈ʒu���v�Z����
	float distance = flying ? 250.0f : 180.0f;
	D3DXVECTOR3 posS = { 0.0f, 0.0f, -distance };
	D3DXMATRIX mtxY, mtxX, mtxS;
	D3DXMatrixRotationX(&mtxX, m_cameraRot.x);
	D3DXMatrixRotationY(&mtxY, m_cameraRot.y);
	mtxS = mtxX* mtxY ;
	D3DXVec3TransformCoord(&posS, &posS, &mtxS);

	// ���C�ŃJ�����̈ʒu�����߂�
	btVector3 Start = btVector3(m_pTarget->transform->GetWPos().x, m_pTarget->transform->GetWPos().y + 10.0f, m_pTarget->transform->GetWPos().z);
	Start += btVector3(posS.x, posS.y, posS.z) * 0.2f;
	btVector3 End = Start + btVector3(posS.x, posS.y, posS.z);
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit())
	{ // �q�b�g�����Ƃ�
		D3DXVECTOR3 minPos = { 0.0f, 0.0f, 20.0f };
		D3DXVec3TransformCoord(&minPos, &minPos, &mtxS);
		posS = { RayCallback.m_hitPointWorld.getX(), RayCallback.m_hitPointWorld.getY(), RayCallback.m_hitPointWorld.getZ() };
		posS += {minPos.x, minPos.y, minPos.z};
	}
	else
	{
		posS += m_pTarget->transform->GetWPos();
	}
	pCamera->transform->SetPos(posS);




	//// ���̎擾
	//Transform* pTargetTrans = m_pTarget->transform;
	//D3DXQUATERNION objectiveRot = pTargetTrans->GetQuaternion();
	//D3DXQUATERNION targetRot = transform->GetQuaternion();
	//D3DXVECTOR3 objectivePos;

	//bool flying = m_pTarget->GetComponent<CVehicle>()->GetFlying();
	//if (!flying)
	//{ // �n�ʂɂ���Ƃ�
	//	pCamera->SetCustomPosR(false);

	//	// �J�����̖ڕW�n�_���v�Z����
	//	D3DXMATRIX vehicleMtx = pTargetTrans->GetMatrix();
	//	objectivePos += {
	//			0.0f,
	//			40.0f,
	//			-150.0f
	//	};
	//	D3DXVec3TransformCoord(&objectivePos, &objectivePos, &vehicleMtx);
	//}
	//else
	//{ // ���ł���Ƃ�
	//	pCamera->SetCustomPosR(true);
	//	pCamera->SetPosR(pTargetTrans->GetWPos());

	//	// �J�����̖ڕW�n�_���v�Z����
	//	D3DXMATRIX vehicleMtx = pTargetTrans->GetMatrix();
	//	objectivePos += {
	//			0.0f,
	//			30.0f,
	//			-250.0f
	//	};
	//	D3DXVec3TransformCoord(&objectivePos, &objectivePos, &vehicleMtx);
	//}

	//// ��]
	//objectiveRot = Benlib::LookAt(transform->GetWPos(), pTargetTrans->GetWPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	//D3DXQuaternionSlerp(&objectiveRot, &targetRot, &objectiveRot, 0.08f);
	//transform->SetQuaternion(objectiveRot);

	//// �ڕW�n�_�Ɉړ�����
	//transform->SetPos(transform->GetWPos()+(objectivePos - transform->GetWPos()) * 0.08f);
}

//=============================================================
// [CCameraMove] �I�u�W�F�N�g���j�����ꂽ�Ƃ��̏���
//=============================================================
void CCameraMove::OnDestroyOtherObject(GameObject* other)
{
	if (other == m_pTarget)
	{ // �^�[�Q�b�g�̂Ƃ�
		m_pTarget = nullptr;
	}
}