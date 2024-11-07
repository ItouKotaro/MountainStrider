//=============================================================
//
// ���R�̎Y�� [nature_produces.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "nature_produces.h"

//=============================================================
// [CNatureProduces] ��������
//=============================================================
void CNatureProduces::Generate(const Transform& transform)
{
	// �p�X�̓��̓`�F�b�N
	if (m_path.empty()) return;

	// �v���n�u�𐶐�����
	GameObject* pGenerateObj = GameObject::LoadPrefab(m_path, transform);
	pGenerateObj->AddComponent<CProduceTag>(typeid(*this).name());	// �ǐՃ^�O
}

//=============================================================
// [CNatureProduces] �אڊm�����擾����
//=============================================================
float CNatureProduces::GetAdjacentRate(const D3DXVECTOR3& pos)
{
	// 
	return 1.0f;
}