//=============================================================
//
// ���U���g�J���� [result_camera.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "result_camera.h"

const int ResultCamera::RECORD_COUNT = 60;		// �L�^�p�x

//=============================================================
// [ResultCamera] ������
//=============================================================
void ResultCamera::Init()
{
	m_isPlay = false;
	m_nextCounter = 20;
	m_recIdx = 0;
	m_fade = 1.0f;
	m_records.clear();

	// �J�������擾����
	m_pCamera = GameObject::Find("Camera");
}

//=============================================================
// [ResultCamera] �X�V
//=============================================================
void ResultCamera::Update()
{
	// ��O�i�L�^�������Ƃ��j
	if (m_records.size() <= 0)
	{
		return;
	}

	if (m_isPlay)
	{ // �Đ���
		Record rec = m_records[m_recIdx];

		// �ړ�������
		m_pCamera->transform->Translate(m_moving * m_fade);

		// ��]�ʂ����߂�
		D3DXQUATERNION objective_q;
		D3DXQUATERNION current_q = m_pCamera->transform->GetWQuaternion();
		D3DXQuaternionSlerp(&objective_q, &current_q, &rec.rot, (1.0f - m_fade));
		m_pCamera->transform->SetQuaternion(objective_q);

		// �t�F�[�h
		m_fade *= 0.998f;

		// �J�E���^�[����
		if (Benlib::PosDistance(m_pCamera->transform->GetWPos(), rec.pos) < 50.0f)
		{
			m_recIdx++;

			if (m_recIdx >= m_records.size())
			{ // �ő�ɓ��B�����Ƃ�
				m_recIdx = 0;	// �߂�
			}
			m_fade = 1.0f;

			// �ړ��ʂ����߂�
			m_moving = (m_records[m_recIdx].pos - m_pCamera->transform->GetWPos()) / 60;
		}
	}
}

//=============================================================
// [ResultCamera] �L�^����
//=============================================================
void ResultCamera::RecordData()
{
	m_nextCounter--;

	if (m_nextCounter <= 0)
	{
		// �J�����̏����L�^����
		Record rec;

		// �ʒu
		rec.pos = m_pCamera->transform->GetWPos();
		rec.pos.y += 150.0f;

		// ����
		rec.rot = m_pCamera->transform->GetWQuaternion();

		// �ǉ�����
		m_records.push_back(rec);

		// �J�E���^�[�����ɖ߂�
		m_nextCounter = RECORD_COUNT;
	}
}

//=============================================================
// [ResultCamera] �Đ�
//=============================================================
void ResultCamera::Play()
{
	m_isPlay = true;

	if (m_records.size() > 0)
	{
		m_moving = (m_records[0].pos - m_pCamera->transform->GetWPos()) / 60;
	}
}