//=============================================================
//
// �N���N���W�b�g [launch_credit.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "launch_credit.h"
#include "manager.h"

//=============================================================
// [LaunchCredit] ������
//=============================================================
void LaunchCredit::Init()
{
	// �N���W�b�g���S�|���S�����쐬����
	m_creditLogo = new SingleComponent<CPolygon>();
	m_creditLogo->Init();
}

//=============================================================
// [LaunchCredit] �I��
//=============================================================
void LaunchCredit::Uninit()
{
	// �N���W�b�g���S�|���S�����������
	m_creditLogo->Uninit();
	delete m_creditLogo;
}

//=============================================================
// [LaunchCredit] �X�V
//=============================================================
void LaunchCredit::Update()
{
	if (m_state == STATE::MIDST)
	{
		m_alpha = 1.0f;
		if (m_time >= m_showTime - m_fadeTime && m_fadeTime > 0.0f)
		{ // �t�F�[�h�C��
			m_alpha = 1.0f - ((m_time - (m_showTime - m_fadeTime)) / m_fadeTime);
		}
		else if (m_time <= m_fadeTime && m_fadeTime > 0.0f)
		{ // �t�F�[�h�A�E�g
			m_alpha = m_time / m_fadeTime;
		}
		m_creditLogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

		// �\���؂�ւ����ԂɂȂ����Ƃ�
		if (m_time <= 0.0f)
		{
			// �^�C�}�[��߂�
			m_time = m_showTime;

			// ���̃N���W�b�g�ɕύX����
			m_currentCreditID++;
			if (m_currentCreditID >= static_cast<int>(m_credits.size()))
			{ // �N���W�b�g���I�������Ƃ�
				m_state = STATE::ENDED;
				m_currentCreditID = -1;
				m_alpha = 0.0f;
				m_time = m_fadeTime;
				return;
			}

			// �|���S���̐ݒ���s��
			m_creditLogo->transform->SetSize(m_credits[m_currentCreditID].size * m_credits[m_currentCreditID].scale);
			m_creditLogo->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - m_creditLogo->transform->GetSize().x / (float)2.0f,
				CRenderer::SCREEN_HEIGHT / 2 - m_creditLogo->transform->GetSize().y / (float)2.0f);
			m_creditLogo->SetTexture(m_credits[m_currentCreditID].path);
		}
	}

	// �w�i�|���S���̑���
	if (m_bgPoly != nullptr)
	{
		D3DXCOLOR col = m_bgPoly->GetColor();
		if (m_state == STATE::ENDED && m_time > 0.0f && m_fadeTime > 0.0f)
		{
			col.a = m_time / m_fadeTime;
		}
		else if (m_state == STATE::ENDED)
		{
			col.a = 0.0f;
		}
		else
		{
			col.a = 1.0f;
		}
		m_bgPoly->SetColor(col);
	}

	if (m_state == STATE::MIDST || (m_state == STATE::ENDED && m_time > 0.0f))
	{
		// �v�����Ԃ����Z����
		m_time -= CManager::GetInstance()->GetDeltaTime();
	}

	// �N���W�b�g���S�|���S�����X�V����
	m_creditLogo->Update();
}

//=============================================================
// [LaunchCredit] �`��
//=============================================================
void LaunchCredit::DrawUI()
{
	// �N���W�b�g���S�|���S����`�悷��
	if (m_state == STATE::MIDST)
	{
		m_creditLogo->DrawUI();
	}
}

//=============================================================
// [LaunchCredit] �J�n����
//=============================================================
void LaunchCredit::Start()
{
	m_state = STATE::MIDST;
	m_time = 0;
	m_currentCreditID = -1;
}

//=============================================================
// [LaunchCredit] �N���W�b�g��ǉ�����
//=============================================================
int LaunchCredit::AddCredit(const std::string& path, const D3DXVECTOR2& size, const float& scale)
{
	CreditLogo credit;
	credit.path = path;
	credit.size = size;
	credit.scale = scale;
	m_credits.push_back(credit);
	return static_cast<int>(m_credits.size()) - 1;
}