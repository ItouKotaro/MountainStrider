//=============================================================
//
// �f�[�^�\�� [result_data.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "result_data.h"

const D3DXVECTOR2 ResultDataView::TITLE_BG_SIZE = { 350.0f, 80.0f };
const float ResultDataView::SPACE_SIZE = 150.0f;

//=============================================================
// [ResultDataView] ������
//=============================================================
void ResultDataView::Init()
{
	// �N���A�^�C��
	{
		// �^�C�g���w�i
		m_timeTitleBG = new SingleComponent<CPolygon>();
		m_timeTitleBG->Init();
		m_timeTitleBG->SetParent(gameObject);
		m_timeTitleBG->SetColor(D3DCOLOR_RGBA(36, 39, 255, 255));
		m_timeTitleBG->transform->SetSize(TITLE_BG_SIZE);

		// �^�C�g���e�L�X�g
		m_timeTitleText = new SingleComponent<CText>();
		m_timeTitleText->Init();
		m_timeTitleText->SetParent(gameObject);
		m_timeTitleText->SetFont("07�S�r�S�V�b�N");
		m_timeTitleText->SetText("�N���A�^�C��");
		m_timeTitleText->SetFontSize(60);
		m_timeTitleText->transform->SetPos(20.0f, 15.0f, 0.0f);

		// �f�[�^�w�i
		m_timeDataBG = new SingleComponent<CPolygon>();
		m_timeDataBG->Init();
		m_timeDataBG->SetParent(gameObject);
		m_timeDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_timeDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_timeDataBG->SetColor(D3DCOLOR_RGBA(78, 90, 135, 255));

		// �f�[�^�e�L�X�g
		m_timeDataText = new SingleComponent<CText>();
		m_timeDataText->Init();
		m_timeDataText->SetParent(gameObject);
		m_timeDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_timeDataText->SetFont("07�S�r�S�V�b�N");
		m_timeDataText->SetAlign(CText::CENTER);
		m_timeDataText->SetFontSize(60);
	}

	// �ō����x
	{
		// �^�C�g���w�i
		m_speedTitleBG = new SingleComponent<CPolygon>();
		m_speedTitleBG->Init();
		m_speedTitleBG->SetParent(gameObject);
		m_speedTitleBG->SetColor(D3DCOLOR_RGBA(252, 135, 18, 255));
		m_speedTitleBG->transform->SetSize(TITLE_BG_SIZE);
		m_speedTitleBG->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// �^�C�g���e�L�X�g
		m_speedTitleText = new SingleComponent<CText>();
		m_speedTitleText->Init();
		m_speedTitleText->SetParent(gameObject);
		m_speedTitleText->SetFont("07�S�r�S�V�b�N");
		m_speedTitleText->SetText("�ō����x");
		m_speedTitleText->SetFontSize(60);
		m_speedTitleText->transform->SetPos(70.0f, 15.0f, 0.0f);
		m_speedTitleText->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// �f�[�^�w�i
		m_speedDataBG = new SingleComponent<CPolygon>();
		m_speedDataBG->Init();
		m_speedDataBG->SetParent(gameObject);
		m_speedDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_speedDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_speedDataBG->SetColor(D3DCOLOR_RGBA(176, 109, 81, 255));
		m_speedDataBG->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// �f�[�^�e�L�X�g
		m_speedDataText = new SingleComponent<CText>();
		m_speedDataText->Init();
		m_speedDataText->SetParent(gameObject);
		m_speedDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_speedDataText->SetFont("07�S�r�S�V�b�N");
		m_speedDataText->SetAlign(CText::CENTER);
		m_speedDataText->SetFontSize(60);
		m_speedDataText->transform->Translate(0.0f, SPACE_SIZE, 0.0f);
	}

	// �A�N�V����
	{
		// �^�C�g���w�i
		m_actionTitleBG = new SingleComponent<CPolygon>();
		m_actionTitleBG->Init();
		m_actionTitleBG->SetParent(gameObject);
		m_actionTitleBG->SetColor(D3DCOLOR_RGBA(178, 36, 255, 255));
		m_actionTitleBG->transform->SetSize(TITLE_BG_SIZE);
		m_actionTitleBG->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// �^�C�g���e�L�X�g
		m_actionTitleText = new SingleComponent<CText>();
		m_actionTitleText->Init();
		m_actionTitleText->SetParent(gameObject);
		m_actionTitleText->SetFont("07�S�r�S�V�b�N");
		m_actionTitleText->SetText("�A�N�V����");
		m_actionTitleText->SetFontSize(60);
		m_actionTitleText->transform->SetPos(50.0f, 15.0f, 0.0f);
		m_actionTitleText->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// �f�[�^�w�i
		m_actionDataBG = new SingleComponent<CPolygon>();
		m_actionDataBG->Init();
		m_actionDataBG->SetParent(gameObject);
		m_actionDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_actionDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_actionDataBG->SetColor(D3DCOLOR_RGBA(92, 72, 110, 255));
		m_actionDataBG->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// �f�[�^�e�L�X�g
		m_actionDataText = new SingleComponent<CText>();
		m_actionDataText->Init();
		m_actionDataText->SetParent(gameObject);
		m_actionDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_actionDataText->SetFont("07�S�r�S�V�b�N");
		m_actionDataText->SetAlign(CText::CENTER);
		m_actionDataText->SetFontSize(60);
		m_actionDataText->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);
	}
}

//=============================================================
// [ResultDataView] �I��
//=============================================================
void ResultDataView::Uninit()
{
	m_timeTitleBG->Uninit();
	delete m_timeTitleBG;

	m_timeTitleText->Uninit();
	delete m_timeTitleText;

	m_timeDataBG->Uninit();
	delete m_timeDataBG;

	m_timeDataText->Uninit();
	delete m_timeDataText;

	m_speedTitleBG->Uninit();
	delete m_speedTitleBG;

	m_speedTitleText->Uninit();
	delete  m_speedTitleText;

	m_speedDataBG->Uninit();
	delete m_speedDataBG;

	m_speedDataText->Uninit();
	delete m_speedDataText;

	m_actionTitleBG->Uninit();
	delete m_actionTitleBG;

	m_actionTitleText->Uninit();
	delete m_actionTitleText;

	m_actionDataBG->Uninit();
	delete m_actionDataBG;

	m_actionDataText->Uninit();
	delete m_actionDataText;
}

//=============================================================
// [ResultDataView] �X�V
//=============================================================
void ResultDataView::Update()
{
	m_timeTitleBG->Update();
	m_timeTitleText->Update();
	m_timeDataBG->Update();
	m_timeDataText->Update();

	m_speedTitleBG->Update();
	m_speedTitleText->Update();
	m_speedDataBG->Update();
	m_speedDataText->Update();

	m_actionTitleBG->Update();
	m_actionTitleText->Update();
	m_actionDataBG->Update();
	m_actionDataText->Update();
}

//=============================================================
// [ResultDataView] �`��
//=============================================================
void ResultDataView::DrawUI()
{
	m_timeTitleBG->DrawUI();
	m_timeTitleText->DrawUI();
	m_timeDataBG->DrawUI();
	m_timeDataText->DrawUI();

	m_speedTitleBG->DrawUI();
	m_speedTitleText->DrawUI();
	m_speedDataBG->DrawUI();
	m_speedDataText->DrawUI();

	m_actionTitleBG->DrawUI();
	m_actionTitleText->DrawUI();
	m_actionDataBG->DrawUI();
	m_actionDataText->DrawUI();
}

//=============================================================
// [ResultDataView] ���Ԃ̐ݒ�
//=============================================================
void ResultDataView::SetTime(const int& time)
{
	if (time == -1)
	{
		m_timeDataText->SetText("�Ȃ�");
		return;
	}

	// ���ƕb
	int min = (time - time % 60) / 60;
	int sec = time % 60;

	// ���Ԃ̕\���`����ύX����
	char* timeText = new char[128];
	sprintf(timeText, "%d:%02d", min, sec);

	// �e�L�X�g���X�V����
	m_timeDataText->SetText(timeText);

	// �j��
	delete[] timeText;
	timeText = nullptr;
}

//=============================================================
// [ResultDataView] �ō����x�̐ݒ�
//=============================================================
void ResultDataView::SetHighSpeed(const int& kmh)
{
	// �e�L�X�g���X�V����
	m_speedDataText->SetText(std::to_string(kmh) + "<size=40>km/h");
}

//=============================================================
// [ResultDataView] �A�N�V�����̐ݒ�
//=============================================================
void ResultDataView::SetAction(const int& score)
{
	// �e�L�X�g���X�V����
	m_actionDataText->SetText(std::to_string(score));
}