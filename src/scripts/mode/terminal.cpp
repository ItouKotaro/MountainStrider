//=============================================================
//
// �I�[���[�h [terminal.cpp]
// Author: Ito Kotaro
// 
// ���[���h�̒[�܂œ��B����ƃS�[������
//
//=============================================================
#include "terminal.h"
#include "scene/game.h"

#include "scripts/direction_arrow.h"

//=============================================================
// [TerminalMode] ������
//=============================================================
void TerminalMode::Init()
{
	// �������𐶐�����
	m_directionObj = new GameObject("DirectionArrow");
	m_directionObj->transform->SetPos(0.0f, 0.0f, 50.0f);
	m_directionObj->AddComponent<DirectionArrow>();
}

//=============================================================
// [TerminalMode] �I��
//=============================================================
void TerminalMode::Uninit()
{
	m_directionObj->Destroy();
}

//=============================================================
// [TerminalMode] �X�V
//=============================================================
void TerminalMode::Update()
{
	// �o�C�N�̈ʒu���擾����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	D3DXVECTOR3 vehiclePos = gameScene->GetBike()->transform->GetWPos();

	// �n�`�̒[�ɍs�����Ƃ�
	if (vehiclePos.x <= -Terrain::TERRAIN_DISTANCE_HALF + EXTENSION_DISTANCE || vehiclePos.x >= Terrain::TERRAIN_DISTANCE_HALF - EXTENSION_DISTANCE ||
		vehiclePos.z <= -Terrain::TERRAIN_DISTANCE_HALF + EXTENSION_DISTANCE || vehiclePos.z >= Terrain::TERRAIN_DISTANCE_HALF - EXTENSION_DISTANCE)
	{
		ModeManager::GetInstance()->SetResult<ClearResult>();
	}

	// �Œፂ�x�������ɍs�����Ƃ�
	if (vehiclePos.y < gameScene->GetTerrain()->GetMinHeight() - 5.0f)
	{
		ModeManager::GetInstance()->SetResult<ClearResult>();
	}

	// �Q�[���I�[�o�[����
	if (gameScene->GetBike()->GetComponent<CVehicle>()->GetFuel() <= 0.0f)
	{ // �R���������Ȃ����Ƃ�
		ModeManager::GetInstance()->SetResult<GameOverResult>();
	}

	// �ϋv�l�������Ȃ����Ƃ��̏���
	if (gameScene->GetBike()->GetComponent<CVehicle>()->GetEndurance() <= 0)
	{
		ModeManager::GetInstance()->SetResult<GameOverResult>();
	}

	if (INPUT_INSTANCE->onTrigger("@"))
	{
		ModeManager::GetInstance()->SetResult<ClearResult>();
	}

	// �������̖ړI�ʒu���X�V����
	m_directionObj->GetComponent<DirectionArrow>()->SetDestination(CalcNearGoal());
}

//=============================================================
// [TerminalMode] ���U���g�C�x���g
//=============================================================
void TerminalMode::OnResultEvent()
{
	// ���U���g�̃f�[�^���i�[����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	m_resultDatas.push_back(gameScene->GetResultData());
}

//=============================================================
// [TerminalMode] �ł��߂��S�[���̈ʒu���擾����
//=============================================================
D3DXVECTOR3 TerminalMode::CalcNearGoal()
{
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	D3DXVECTOR3 vehiclePos = gameScene->GetBike()->transform->GetWPos();

	// �㉺���E�̂ǂ̒n�_����ԋ߂���
	enum NEAR_PT
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};
	NEAR_PT nearPt;

	// ��ԋ߂��|�C���g�𓱂��o��
	float nearDis = Terrain::TERRAIN_DISTANCE;
	if (fabsf(vehiclePos.x - Terrain::TERRAIN_DISTANCE_HALF) < nearDis)
	{
		nearPt = LEFT;
		nearDis = fabsf(vehiclePos.x - Terrain::TERRAIN_DISTANCE_HALF);
	}
	if (fabsf(vehiclePos.x + Terrain::TERRAIN_DISTANCE_HALF) < nearDis)
	{
		nearPt = RIGHT;
		nearDis = fabsf(vehiclePos.x + Terrain::TERRAIN_DISTANCE_HALF);
	}
	if (fabsf(vehiclePos.z + Terrain::TERRAIN_DISTANCE_HALF) < nearDis)
	{
		nearPt = TOP;
		nearDis = fabsf(vehiclePos.z + Terrain::TERRAIN_DISTANCE_HALF);
	}
	if (fabsf(vehiclePos.z - Terrain::TERRAIN_DISTANCE_HALF) < nearDis)
	{
		nearPt = BOTTOM;
		nearDis = fabsf(vehiclePos.z - Terrain::TERRAIN_DISTANCE_HALF);
	}

	// ���ꂼ��̕����̖ڕW�n�_��ݒ肷��
	D3DXVECTOR3 destination = { 0.0f, 0.0f, 0.0f };
	switch (nearPt)
	{
	case TOP:
		destination = { 0.0f, 0.0f, -Terrain::TERRAIN_DISTANCE_HALF };
		break;
	case BOTTOM:
		destination = { 0.0f, 0.0f, Terrain::TERRAIN_DISTANCE_HALF };
		break;
	case LEFT:
		destination = { Terrain::TERRAIN_DISTANCE_HALF, 0.0f, 0.0f };
		break;
	case RIGHT:
		destination = { -Terrain::TERRAIN_DISTANCE_HALF, 0.0f, 0.0f };
		break;
	}

	return destination;
}