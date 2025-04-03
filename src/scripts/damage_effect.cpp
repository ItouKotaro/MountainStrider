//=============================================================
//
// �_���[�W�G�t�F�N�g [damage_effect.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "damage_effect.h"
#include "manager.h"

//=============================================================
// [DamageEffect] ������
//=============================================================
void DamageEffect::Init()
{
	// �J�������擾����
	m_camera = GameObject::Find("Camera");
}

//=============================================================
// [DamageEffect] �I��
//=============================================================
void DamageEffect::Uninit()
{
	for (auto itr = m_billboards.begin(); itr != m_billboards.end(); itr++)
	{
		(*itr)->Destroy();
	}

	// ���X�g���폜����
	m_billboards.clear();
}

//=============================================================
// [DamageEffect] �X�V
//=============================================================
void DamageEffect::Update()
{
	// ��������
	if (m_camera != nullptr)
	{
		transform->SetRot(0.0f, Benlib::PosAngle(transform->GetWPos(), m_camera->transform->GetWPos()), 0.0f);
	}

	// ��ɏ㏸������
	transform->Translate(0.0f, 0.5f, 0.0f);

	// ����
	m_time -= CManager::GetInstance()->GetDeltaTime();
	if (m_time <= 0.0f)
	{
		gameObject->Destroy();
	}
}

//=============================================================
// [DamageEffect] �`��
//=============================================================
void DamageEffect::Draw()
{

}

//=============================================================
// [DamageEffect] ����
//=============================================================
void DamageEffect::Generate(const float& damage, const float& time)
{
	// ���Ԃ�ݒ肷��
	m_time = time;

	// �����̊���o���ɕK�v�Ȍv�Z
	int numInt = static_cast<int>(damage);
	int numFloat = static_cast<int>((damage - static_cast<float>(numInt)) / 0.1f);
	int digit = static_cast<int>(log10f(numInt)) + 2;	// 2��log10�̌����⊮�Ə������ʂ̍��v
	float widthLength = digit * SIZE / 2.0f;	// �r���{�[�h�̈ʒu

	// �K�v�ȃr���{�[�h��p�ӂ���
	for (int i = 0; i < digit; i++)
	{
		// ����
		GameObject* billboard = new GameObject();
		billboard->AddComponent<CBillboard>()->Set(SIZE, SIZE);

		// �Ώۂ̐������擾����
		int targetNum = numInt / powf(10, digit - i - 2);
		numInt -= targetNum * powf(10, digit - i - 2);

		// �Ō�̃��[�v�i�����_�̐��ɒu��������j
		if (i == digit - 1)
		{
			// �����_�̐��Ƀ^�[�Q�b�g��u��������
			targetNum = numFloat;

			// �s���I�h�r���{�[�h�𐶐�����
			GameObject* periodBillboard = new GameObject();
			periodBillboard->AddComponent<CBillboard>()->Set(SIZE, SIZE);
			periodBillboard->GetComponent<CBillboard>()->SetTexture("data\\TEXTURE\\UI\\NUMBER\\period.png");
			periodBillboard->transform->SetPos(widthLength + SIZE / 2.0f, 0.0f, 0.0f);
			periodBillboard->SetParent(gameObject);
			m_billboards.emplace_back(periodBillboard);
		}

		// �e�N�X�`���p�X
		char texturePath[MAX_PATH];
		sprintf(&texturePath[0], "data\\TEXTURE\\UI\\NUMBER\\%d.png", targetNum);

		// �r���{�[�h�̐ݒ���s��
		billboard->GetComponent<CBillboard>()->SetTexture(texturePath);

		// �r���{�[�h�̈ʒu
		billboard->transform->SetPos(widthLength, 0.0f, 0.0f);
		widthLength -= SIZE;
		billboard->SetParent(gameObject);
		
		// ���X�g�ɒǉ�����
		m_billboards.emplace_back(billboard);
	}

	
}
