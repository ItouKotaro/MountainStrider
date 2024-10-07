//=============================================================
//
// �o�[ [bar.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "bar.h"

// �ÓI�����o�ϐ��̏�����
const float CBar::DEFAULT_BAR_LENGTH = 800.0f;
const float CBar::DEFAULT_BAR_BOLD = 80.0f;
const float CProgressBar::DEFAULT_BAR_SPACE = 8.0f;
const D3DXCOLOR CProgressBar::DEFAULT_FILL_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CProgressBar::DEFAULT_NONFILL_COLOR = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

//=============================================================
// [CBar] ������
//=============================================================
void CBar::Init()
{
	m_pBgObj = new GameObject();
	m_pBarObj = new GameObject();
	m_pBgObj->SetParent(gameObject);
	m_pBarObj->SetParent(gameObject);
	m_pBgObj->AddComponent<CPolygon>();
	m_pBarObj->AddComponent<CPolygon>();

	// �ϐ��̏�����
	m_fBarLength = DEFAULT_BAR_LENGTH;
	m_fBarBold = DEFAULT_BAR_BOLD;
	m_pBarObj->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_fBarProgress = 1.0f;

	// �T�C�Y�̐ݒ�
	m_pBgObj->transform->SetSize(m_fBarLength, m_fBarBold);
	m_pBarObj->transform->SetSize(m_fBarLength * m_fBarProgress, m_fBarBold);
}

//=============================================================
// [CBar] �X�V
//=============================================================
void CBar::Update()
{
	m_pBarObj->transform->SetSize(m_fBarLength * m_fBarProgress, m_fBarBold);
}


//=============================================================
// [CProgressBar] �R���X�g���N�^
//=============================================================
CProgressBar::CProgressBar(const int& nNumBar)
{
	m_nNumBar = nNumBar;
}

//=============================================================
// [CProgressBar] ������
//=============================================================
void CProgressBar::Init()
{
	// �Q�[���I�u�W�F�N�g���쐬����
	m_pBgObj = new GameObject();
	m_pBgObj->SetParent(gameObject);
	m_pBgObj->AddComponent<CPolygon>();

	// �o�[�̍쐬
	for (int i = 0; i < m_nNumBar; i++)
	{
		GameObject* pBar = new GameObject();
		pBar->SetParent(gameObject);
		pBar->SetPriority(8);
		pBar->AddComponent<CPolygon>();
		pBar->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_pBars.push_back(pBar);
	}

	// �ϐ��̏�����
	m_fBarLength = CBar::DEFAULT_BAR_LENGTH;
	m_fBarBold = CBar::DEFAULT_BAR_BOLD;
	m_fBarSpace = CProgressBar::DEFAULT_BAR_SPACE;
	m_fSpace = 5;
	m_fBarProgress = 1.0f;
	m_fillColor = DEFAULT_FILL_COLOR;
	m_nonFillCollor = DEFAULT_NONFILL_COLOR;
	m_fAlpha = 1.0f;

	// �T�C�Y�̐ݒ�
	m_pBgObj->transform->SetSize(m_fBarLength, m_fBarBold);
}

//=============================================================
// [CProgressBar] �I��
//=============================================================
void CProgressBar::Uninit()
{
	gameObject->Destroy(true);
}

//=============================================================
// [CProgressBar] �w�i�̐F�ύX
//=============================================================
void CProgressBar::SetBGColor(const D3DXCOLOR& color)
{
	m_pBgObj->GetComponent<CPolygon>()->SetColor(color);
}

//=============================================================
// [CProgressBar] �����x
//=============================================================
void CProgressBar::SetAlpha(const float& fAlpha)
{
	m_fAlpha = fAlpha;

	// �o�[
	m_fillColor.a = fAlpha;
	m_nonFillCollor.a = fAlpha;

	// �w�i
	D3DXCOLOR bgColor = m_pBgObj->GetComponent<CPolygon>()->GetColor();
	bgColor.a = fAlpha;
	m_pBgObj->GetComponent<CPolygon>()->SetColor(bgColor);
}

//=============================================================
// [CProgressBar] �X�V
//=============================================================
void CProgressBar::Update()
{
	// �o�[�̃T�C�Y���v�Z����
	D3DXVECTOR2 barSize;
	barSize.x = (m_fBarLength - m_fBarSpace * m_pBars.size() - m_fSpace * 2) / m_pBars.size();
	barSize.y = m_fBarBold - m_fSpace * 2;

	// �w�i�̃T�C�Y
	m_pBgObj->transform->SetSize(m_fBarLength, m_fBarBold);

	// ���߂鐔�i�����j
	int nFillNum = static_cast<int>(m_nNumBar * m_fBarProgress);

	for (unsigned int i = 0; i < m_pBars.size(); i++)
	{
		// �F
		if (i < static_cast<unsigned int>(nFillNum))
			m_pBars[i]->GetComponent<CPolygon>()->SetColor(m_fillColor);
		else
			m_pBars[i]->GetComponent<CPolygon>()->SetColor(m_nonFillCollor);

		// �T�C�Y��ݒ肷��
		m_pBars[i]->transform->SetSize(barSize);

		// ���Ԋu�Ƀo�[��z�u����
		m_pBars[i]->transform->SetPos(m_fSpace * 2.0f + (barSize.x + m_fBarSpace) * i, m_fSpace);
	}
}