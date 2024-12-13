//=============================================================
//
// �o�C�N�A�N�V���� [vehicle_action.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_action.h"

//=============================================================
// [VehicleAction] ������
//=============================================================
void VehicleAction::Init()
{
	m_isMeasure = false;
	m_rolling = 0.0f;
	m_rollCount = 0;
	m_beforeRot = 0.0f;
}

//=============================================================
// [VehicleAction] �X�V
//=============================================================
void VehicleAction::Update()
{
	// �v�����̏���
	if (m_isMeasure)
	{
		// ���݂̊p�x���擾����
		float currentRot = transform->GetRot().y;

		// ���炩�ɉ�]�������������Ƃ�
		float rolling = fabsf(currentRot - m_beforeRot);
		m_beforeRot = currentRot;
		if (fabsf(rolling) > D3DX_PI)
		{
			return;
		}

		// ��]�������Z����
		m_rolling += rolling;

		if (m_rollCount < (m_rolling - fmod(m_rolling, D3DX_PI)) / static_cast<float>(D3DX_PI))
		{
			m_rollCount++;

			// �\���e�L�X�g�����߂�
			char pointText[24];
			if (m_rollCount % 2 == 0)
				sprintf(&pointText[0], "+%d��]", static_cast<int>(m_rollCount /2));
			else
				sprintf(&pointText[0], "+%.1f��]", m_rollCount * 0.5f);

			// �\��
			GameObject* addRollText = new GameObject();
			addRollText->transform->SetPos(1300.0f, 600.0f);
			addRollText->AddComponent<ActionPointText>();
			auto rollText = addRollText->GetComponent<ActionPointText>();
			rollText->SetText(pointText);
			rollText->SetFade(0.02f);
			rollText->SetCounter(120);
			rollText->SetUp(2.0f);
		}
	}
}

//=============================================================
// [VehicleAction] ��юn��
//=============================================================
void VehicleAction::onFlyBegin()
{
	m_rolling = 0.0f;
	m_rollCount = 0;
	m_beforeRot = transform->GetRot().y;
	m_isMeasure = true;
}

//=============================================================
// [VehicleAction] ��яI���
//=============================================================
void VehicleAction::onFlyEnd()
{
	m_rolling = 0.0f;
	m_isMeasure = false;
}


//=============================================================
// [ActionPointText] ������
//=============================================================
void ActionPointText::Init()
{
	m_text = new SingleComponent<CText>();
	m_text->Init();
	m_text->SetParent(gameObject);
	m_text->SetFont("07�S�r�S�V�b�N");
}

//=============================================================
// [ActionPointText] �I��
//=============================================================
void ActionPointText::Uninit()
{
	m_text->Uninit();
	delete m_text;
}

//=============================================================
// [ActionPointText] �X�V
//=============================================================
void ActionPointText::Update()
{
	// �㏸�ƃt�F�[�h
	m_text->transform->Translate(0.0f, -m_upValue, 0.0f);
	m_text->SetAlpha(m_text->GetAlpha() - m_fade);

	// �J�E���^�[���f�N�������g
	m_counter--;
	if (m_counter <= 0)
	{ // �����Ȃ����Ƃ�
		gameObject->Destroy();
	}

	m_text->Update();
}

//=============================================================
// [ActionPointText] �`��
//=============================================================
void ActionPointText::DrawUI()
{
	m_text->DrawUI();
}



//=============================================================
// [SumActionPointText] ������
//=============================================================
void SumActionPointText::Init()
{
	m_text = new SingleComponent<CText>();
	m_text->Init();
	m_text->SetParent(gameObject);
	m_text->SetFont("07�S�r�S�V�b�N");
}

//=============================================================
// [SumActionPointText] �I��
//=============================================================
void SumActionPointText::Uninit()
{
	m_text->Uninit();
	delete m_text;
}

//=============================================================
// [SumActionPointText] �X�V
//=============================================================
void SumActionPointText::Update()
{
	m_text->Update();
}

//=============================================================
// [SumActionPointText] �`��
//=============================================================
void SumActionPointText::DrawUI()
{
	m_text->DrawUI();
}