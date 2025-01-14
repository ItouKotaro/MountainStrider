//=============================================================
//
// �o�C�N�A�N�V���� [vehicle_action.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_action.h"
#include "scripts/vehicle.h"
#include "scene/game.h"
#include "component/other/audio.h"

AudioClip actionSE;

//=============================================================
// [VehicleAction] ������
//=============================================================
void VehicleAction::Init()
{
	if (!actionSE) actionSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\BIKE\\action00.mp3", FMOD_2D);
	m_actionSound = new GameObject();
	m_actionSound->AddComponent<AudioSource>();

	m_isMeasure = false;
	m_rolling = 0.0f;
	m_rollCount = 0;
	m_beforeRot = 0.0f;

	// ���v�|�C���g�p�e�L�X�g
	GameObject* sumObj = new GameObject("SumPoints", "UI");
	m_sumPoints = sumObj->AddComponent<SumActionPointText>();
	m_sumPoints->transform->SetPos(1500.0f, 300.0f);
	m_sumPoints->Reset();
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
		float rolling = currentRot - m_beforeRot;
		m_beforeRot = currentRot;
		if (fabsf(rolling) > D3DX_PI)
		{
			return;
		}

		// ��]�������Z����
		m_rolling += rolling;

		if (m_rollCount < (fabsf(m_rolling) - fmod(fabsf(m_rolling), D3DX_PI)) / static_cast<float>(D3DX_PI))
		{
			m_rollCount++;

			// �o�C�N���擾����
			auto pBike = gameObject->GetComponent<CVehicle>();

			// �X�R�A���v�Z����
			int addScore = 0;
			addScore += m_rollCount * 60;	// ��b�X�R�A
			addScore += static_cast<int>(pBike->GetSpeed());

			if (m_rollCount > 5)
			{
				addScore += (m_rollCount - 5) * (m_rollCount - 5) * 120;
			}

			// ���v�l�ɉ��Z����
			m_sumPoints->AddPoints(addScore);


			// �\���e�L�X�g�����߂�
			char pointText[24];
			if (m_rollCount % 2 == 0)
				sprintf(&pointText[0], "+%d��]", static_cast<int>(m_rollCount /2));
			else
				sprintf(&pointText[0], "+%.1f��]", m_rollCount * 0.5f);

			// �\��
			GameObject* addRollText = new GameObject();
			addRollText->transform->SetPos(1300.0f, 600.0f);
			auto rollText = addRollText->AddComponent<ActionPointText>();
			rollText->SetText(pointText);
			rollText->SetFade(0.02f);
			rollText->SetCounter(120);
			rollText->SetUp(2.0f);

			// ����炷
			m_actionSound->GetComponent<AudioSource>()->Play(actionSE);
			m_actionSound->GetComponent<AudioSource>()->GetChannel()->setPitch(0.5f + m_rollCount * 0.2f);
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
	m_text->SetAlign(CText::CENTER);
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

	m_points = 0;
	m_viewPoints = 0;
	m_textScale = 1.0f;
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
	// �\���|�C���g�����߂�
	m_viewPoints += static_cast<int>((m_points - m_viewPoints) * 0.08f);

	// �|�C���g��\������
	m_text->SetText(std::to_string(m_viewPoints));
	//m_text->SetFontSize(static_cast<int>(120 * m_textScale));

	// �T�C�Y��߂��Ă���
	m_textScale += (1.0f - m_textScale) * 0.08f;

	// �����ɂ��Ă���
	if (m_text->GetAlpha() > 0.05f)
	{
		m_text->SetAlpha(m_text->GetAlpha() - 0.01f);
	}
	else
	{ // �I��
		// �|�C���g�����Z����
		static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->AddScore(m_points);

		m_text->SetAlpha(0.0f);
		m_points = 0;
		m_viewPoints = 0;
	}
}

//=============================================================
// [SumActionPointText] �`��
//=============================================================
void SumActionPointText::DrawUI()
{
	m_text->DrawUI();
}

//=============================================================
// [SumActionPointText] �|�C���g���Z
//=============================================================
void SumActionPointText::AddPoints(const int& points)
{
	// �|�C���g�����Z����
	m_points += points;
	m_textScale = 1.5f;
	m_text->SetAlpha(1.0f);
}
