//=============================================================
//
// ���R�̎Y�� [nature_produces.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "nature_produces.h"
#include "benlib.h"

//=============================================================
// [CNatureProduces] ��������
//=============================================================
void CNatureProduces::Generate(const Transform& transform)
{
	// �p�X�̓��̓`�F�b�N
	if (m_path.empty()) return;

	// �v���n�u�𐶐�����
	GameObject* pGenerateObj = GameObject::LoadPrefab(m_path, transform);
	pGenerateObj->AddComponent<CProduceTag>(this);	// �ǐՃ^�O
}

//=============================================================
// [CNatureProduces] �אڊm�����擾����
//=============================================================
float CNatureProduces::GetAdjacentRate(const D3DXVECTOR3& pos)
{
	// �{���ϐ�
	float _rate = 1.0f;

	// �t�߂ɂ���I�u�W�F�N�g���擾���A�{���𓱂��o��
	std::vector<CProduceTag*> pAdjacentObj = Component::GetComponents<CProduceTag>();
	for (unsigned int i = 0; i < pAdjacentObj.size(); i++)
	{
		if (Benlib::PosDistance(pos, pAdjacentObj[i]->transform->GetWPos()) <= m_adjacentDistance)
		{ // �͈͓��̂Ƃ�
			_rate *= GetAdjacentObjectRate(typeid(*pAdjacentObj[i]->GetNatureProduce()).name()); // <-�����Ŕ��ʂ���Ȃ���肠��
		}
	}

	return 1.0f;
}