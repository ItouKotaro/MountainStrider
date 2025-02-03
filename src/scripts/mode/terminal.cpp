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

//=============================================================
// [TerminalMode] ������
//=============================================================
void TerminalMode::Init()
{

}

//=============================================================
// [TerminalMode] �I��
//=============================================================
void TerminalMode::Uninit()
{

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
		onGoal();
	}

	// �Œፂ�x�������ɍs�����Ƃ�
	if (vehiclePos.y < gameScene->GetTerrain()->GetMinHeight() - 5.0f)
	{
		onGoal();
	}
}