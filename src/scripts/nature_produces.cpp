//=============================================================
//
// ���R�̎Y�� [nature_produces.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "nature_produces.h"
#include "benlib.h"
#include "scene/game.h"

//=============================================================
// [CNatureProduces] ��������
//=============================================================
GameObject* CNatureProduces::Generate(const Transform& transform)
{
	// �p�X�̓��̓`�F�b�N
	if (m_path.empty()) return nullptr;

	// �v���n�u�𐶐�����
	GameObject* pGenerateObj = GameObject::LoadPrefab(m_path, transform);

	return pGenerateObj;
}

//=============================================================
// [CNatureProduces] �אڊm�����擾����
//=============================================================
float CNatureProduces::GetAdjacentRate(const D3DXVECTOR3& pos)
{
	// �n�`�̎擾
	auto pProducesManager = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain()->GetProducesManager();
	
	float rate = 1.0f;

#ifndef _DEBUG
	rate = pProducesManager->GetNearProducesRate(m_produceName, pos, m_adjacentDistance);
#endif // _DEBUG
			
	return rate;
}