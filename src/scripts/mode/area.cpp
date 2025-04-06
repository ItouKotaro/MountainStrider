//=============================================================
//
// �G���A���[�h [area.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "area.h"
#include "manager.h"

//=============================================================
// [AreaMode] ������
//=============================================================
void AreaMode::Init()
{
	m_point = 0;
	m_nextArea = 0.0f;

	// �n�`�𐶐�����
	m_game->GenerateTerrain(150, 150.0f);
}

//=============================================================
// [AreaMode] �I��
//=============================================================
void AreaMode::Uninit()
{
}

//=============================================================
// [AreaMode] �X�V
//=============================================================
void AreaMode::Update()
{
	// �G���A�̐���
	m_nextArea -= CManager::GetInstance()->GetDeltaTime();
	if (m_nextArea <= 0.0f)
	{
		// �G���A�̐���
		float distanceHalf = (m_game->GetTerrain()->GetTerrainSize() * m_game->GetTerrain()->GetTerrainScale()) / 2.0f;
		GameObject* nextArea = new GameObject("Area");
		nextArea->transform->SetPos(RandomFloat(-distanceHalf, distanceHalf), 0.0f, RandomFloat(-distanceHalf, distanceHalf));
		nextArea->AddComponent<AreaPoint>();

		// ���̃G���A
		m_nextArea = RandomFloat(10.0f, 20.0f);
	}
}



//=============================================================
// [AreaPoint] ������
//=============================================================
void AreaPoint::Init()
{
	// �o�C�N����������
	m_vehicle = GameObject::Find("Vehicle");
}

//=============================================================
// [AreaPoint] �I��
//=============================================================
void AreaPoint::Uninit()
{
}

//=============================================================
// [AreaPoint] �X�V
//=============================================================
void AreaPoint::Update()
{
	if (Benlib::PosPlaneDistance(m_vehicle->transform->GetWPos(), transform->GetWPos()) < 300.0f)
	{
		// �|�C���g�����Z����
		static_cast<AreaMode*>(ModeManager::GetInstance()->GetMode())->AddPoint(10);
	}
}
