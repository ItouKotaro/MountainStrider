//=============================================================
//
// ���U���g�r���[ [result_view.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "result_view.h"

const int ResultViewBar::ANIM_FRAME = 60;

//=============================================================
// [ResultViewBar] �R���X�g���N�^
//=============================================================
ResultViewBar::ResultViewBar(const std::string& title, const D3DXCOLOR& titleColor, const D3DXCOLOR& barColor, const D3DXCOLOR& percentColor)
{
	m_titleName = title;
	m_titleColor = titleColor;
	m_barColor = barColor;
	m_percentColor = percentColor;
}

//=============================================================
// [ResultViewBar] ������
//=============================================================
void ResultViewBar::Init()
{
	// �A�j���[�V�����̏�����
	m_isStartAnim = false;
	m_animCounter = 0;

	// �^�C�g���e�L�X�g
	m_viewText = new SingleComponent<CText>();
	m_viewText->Init();
	m_viewText->SetParent(gameObject);
	m_viewText->SetFontSize(60);
	m_viewText->SetOutlineSize(2);
	m_viewText->SetFontColor(m_titleColor);
	m_viewText->SetOutlineColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_viewText->SetFont("07�S�r�S�V�b�N");
	m_viewText->SetText(m_titleName);

	// �o�[
	m_viewBar = new SingleComponent<CAdvancedBar>();
	m_viewBar->Init();
	m_viewBar->SetParent(gameObject);
	m_viewBar->transform->SetPos(0.0f, 80.0f);
	m_viewBar->SetTexture("data\\TEXTURE\\RESULT\\hand_bar.png");
	m_viewBar->SetProgress(1.0f);
	m_viewBar->SetBGColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_viewBar->SetLength(500.0f);
	m_viewBar->SetBold(35.0f);
	m_viewBar->SetColor(0, m_barColor);
	m_viewBar->SetColor(2, m_barColor);
	m_viewBar->SetColor(1, m_barColor);
	m_viewBar->SetColor(3, m_barColor);

	// �����e�L�X�g
	m_viewValue = new SingleComponent<CText>();
	m_viewValue->Init();
	m_viewValue->SetParent(gameObject);
	m_viewValue->transform->SetPos(500.0f, 10.0f);
	m_viewValue->SetAlign(CText::RIGHT);
	m_viewValue->SetFontSize(60);
	m_viewValue->SetOutlineSize(2);
	m_viewValue->SetFontColor(m_percentColor);
	m_viewValue->SetOutlineColor(D3DCOLOR_RGBA(230, 230, 230, 255));
	m_viewValue->SetFont("07�S�r�S�V�b�N");
	m_viewValue->SetText("100%");

	// ��r�e�L�X�g
	m_viewCompare = new SingleComponent<CText>();
	m_viewCompare->Init();
	m_viewCompare->SetParent(gameObject);
	m_viewCompare->transform->SetPos(500.0f, 110.0f);
	m_viewCompare->SetAlign(CText::RIGHT);
	m_viewCompare->SetFontSize(30);
	m_viewCompare->SetFont("07�S�r�S�V�b�N");
	m_viewCompare->SetText("<color=230,230,230>-0.0%");
}

//=============================================================
// [ResultViewBar] �I��
//=============================================================
void ResultViewBar::Uninit()
{
	m_viewText->Uninit();
	delete m_viewText;

	m_viewBar->Uninit();
	delete m_viewBar;

	m_viewValue->Uninit();
	delete m_viewValue;

	m_viewCompare->Uninit();
	delete m_viewCompare;
}

//=============================================================
// [ResultViewBar] �X�V
//=============================================================
void ResultViewBar::Update()
{
	if (!m_isStartAnim)
	{
		return;	// �X�g�b�v�̎��͏������΂�
	}

	// ���݂̐i�s�x���v�Z����
	m_viewProgress = m_viewBefore + ((m_viewObjective - m_viewBefore) / ANIM_FRAME) * (m_animCounter+1);

	// �i�s�x���X�V����
	m_viewBar->SetProgress(m_viewProgress);
	m_viewValue->SetText(std::to_string(static_cast<int>(m_viewProgress * 100.0f))+"%");

	// �O��Ɣ�r���Ăǂ���
	float compareView = (m_viewProgress - m_viewBefore) * 100;
	char* compareText = new char[24];
	sprintf(compareText, "%.1f", fabs(compareView));

	std::string mark = "<color=230,230,230>-";
	if (compareView <= -0.1f)
	{ // �������Ă���
		mark = "<color=31,120,255>��";
	}
	else if (compareView >= 0.1f)
	{ // �オ���Ă���
		mark = "<color=255,121,25>��";
	}
	m_viewCompare->SetText(mark + compareText + "%");
	
	// ���
	delete[] compareText;

	// �J�E���^�[��i�߂�
	m_animCounter++;
	if (m_animCounter >= ANIM_FRAME)
	{
		m_animCounter = 0;
		m_isStartAnim = false;
	}

	// �X�V
	m_viewText->Update();
	m_viewBar->Update();
	m_viewValue->Update();
	m_viewCompare->Update();
}

//=============================================================
// [ResultViewBar] �`��
//=============================================================
void ResultViewBar::DrawUI()
{
	m_viewText->DrawUI();
	m_viewBar->DrawUI();
	m_viewValue->DrawUI();
	m_viewCompare->DrawUI();
}

//=============================================================
// [ResultViewBar] �l�̐ݒ�
//=============================================================
void ResultViewBar::SetValue(const float& before, const float& objective)
{
	m_viewBefore = before;
	m_viewObjective = objective;
	m_viewProgress = before;

	m_viewBar->SetProgress(m_viewBefore);
	m_viewValue->SetText(std::to_string(static_cast<int>(m_viewProgress * 100.0f)) + "%");
}