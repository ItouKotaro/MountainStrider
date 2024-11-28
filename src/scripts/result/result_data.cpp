//=============================================================
//
// �f�[�^�\�� [result_data.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "result_data.h"

#include "component/2d/polygon.h"
#include "component/2d/bar.h"
#include "component/2d/text.h"

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
		m_timeTitleBG = new GameObject("TimeTitleBG", "ResultData");
		m_timeTitleBG->SetParent(gameObject);
		m_timeTitleBG->AddComponent<CPolygon>();
		m_timeTitleBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(36, 39, 255, 255));
		m_timeTitleBG->transform->SetSize(TITLE_BG_SIZE);

		// �^�C�g���e�L�X�g
		m_timeTitleText = new GameObject("TimeTitleText", "ResultData");
		m_timeTitleText->SetParent(gameObject);
		m_timeTitleText->AddComponent<CText>();
		m_timeTitleText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		m_timeTitleText->GetComponent<CText>()->SetText("�N���A�^�C��");
		m_timeTitleText->GetComponent<CText>()->SetFontSize(60);
		m_timeTitleText->transform->SetPos(20.0f, 15.0f, 0.0f);

		// �f�[�^�w�i
		m_timeDataBG = new GameObject("TimeDataBG", "ResultData");
		m_timeDataBG->SetParent(gameObject);
		m_timeDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_timeDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_timeDataBG->AddComponent<CPolygon>();
		m_timeDataBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(78, 90, 135, 255));

		// �f�[�^�e�L�X�g
		m_timeDataText = new GameObject("TimeDataText", "ResultData");
		m_timeDataText->SetParent(gameObject);
		m_timeDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_timeDataText->AddComponent<CText>();
		m_timeDataText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		m_timeDataText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_timeDataText->GetComponent<CText>()->SetFontSize(60);
	}

	// �ō����x
	{
		// �^�C�g���w�i
		m_speedTitleBG = new GameObject("SpeedTitleBG", "ResultData");
		m_speedTitleBG->SetParent(gameObject);
		m_speedTitleBG->AddComponent<CPolygon>();
		m_speedTitleBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(252, 135, 18, 255));
		m_speedTitleBG->transform->SetSize(TITLE_BG_SIZE);
		m_speedTitleBG->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// �^�C�g���e�L�X�g
		m_speedTitleText = new GameObject("SpeedTitleText", "ResultData");
		m_speedTitleText->SetParent(gameObject);
		m_speedTitleText->AddComponent<CText>();
		m_speedTitleText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		m_speedTitleText->GetComponent<CText>()->SetText("�ō����x");
		m_speedTitleText->GetComponent<CText>()->SetFontSize(60);
		m_speedTitleText->transform->SetPos(70.0f, 15.0f, 0.0f);
		m_speedTitleText->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// �f�[�^�w�i
		m_speedDataBG = new GameObject("SpeedDataBG", "ResultData");
		m_speedDataBG->SetParent(gameObject);
		m_speedDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_speedDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_speedDataBG->AddComponent<CPolygon>();
		m_speedDataBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(176, 109, 81, 255));
		m_speedDataBG->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// �f�[�^�e�L�X�g
		m_speedDataText = new GameObject("SpeedDataText", "ResultData");
		m_speedDataText->SetParent(gameObject);
		m_speedDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_speedDataText->AddComponent<CText>();
		m_speedDataText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		m_speedDataText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_speedDataText->GetComponent<CText>()->SetFontSize(60);
		m_speedDataText->transform->Translate(0.0f, SPACE_SIZE, 0.0f);
	}

	// �A�N�V����
	{
		// �^�C�g���w�i
		m_actionTitleBG = new GameObject("ActionTitleBG", "ResultData");
		m_actionTitleBG->SetParent(gameObject);
		m_actionTitleBG->AddComponent<CPolygon>();
		m_actionTitleBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(178, 36, 255, 255));
		m_actionTitleBG->transform->SetSize(TITLE_BG_SIZE);
		m_actionTitleBG->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// �^�C�g���e�L�X�g
		m_actionTitleText = new GameObject("ActionTitleText", "ResultData");
		m_actionTitleText->SetParent(gameObject);
		m_actionTitleText->AddComponent<CText>();
		m_actionTitleText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		m_actionTitleText->GetComponent<CText>()->SetText("�A�N�V����");
		m_actionTitleText->GetComponent<CText>()->SetFontSize(60);
		m_actionTitleText->transform->SetPos(50.0f, 15.0f, 0.0f);
		m_actionTitleText->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// �f�[�^�w�i
		m_actionDataBG = new GameObject("ActionDataBG", "ResultData");
		m_actionDataBG->SetParent(gameObject);
		m_actionDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_actionDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_actionDataBG->AddComponent<CPolygon>();
		m_actionDataBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(92, 72, 110, 255));
		m_actionDataBG->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// �f�[�^�e�L�X�g
		m_actionDataText = new GameObject("ActionDataText", "ResultData");
		m_actionDataText->SetParent(gameObject);
		m_actionDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_actionDataText->AddComponent<CText>();
		m_actionDataText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		m_actionDataText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_actionDataText->GetComponent<CText>()->SetFontSize(60);
		m_actionDataText->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);
	}
}

//=============================================================
// [ResultDataView] �I��
//=============================================================
void ResultDataView::Uninit()
{

}

//=============================================================
// [ResultDataView] �X�V
//=============================================================
void ResultDataView::Update()
{

}

//=============================================================
// [ResultDataView] ���Ԃ̐ݒ�
//=============================================================
void ResultDataView::SetTime(const int& time)
{
	// ���ƕb
	int min = (time - time % 60) / 60;
	int sec = time % 60;

	// ���Ԃ̕\���`����ύX����
	char* timeText = new char[128];
	sprintf(timeText, "%d:%02d", min, sec);

	// �e�L�X�g���X�V����
	m_timeDataText->GetComponent<CText>()->SetText(timeText);

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
	m_speedDataText->GetComponent<CText>()->SetText(std::to_string(kmh) + "<size=40>km/h");
}

//=============================================================
// [ResultDataView] �A�N�V�����̐ݒ�
//=============================================================
void ResultDataView::SetAction(const int& score)
{
	// �e�L�X�g���X�V����
	m_actionDataText->GetComponent<CText>()->SetText(std::to_string(score));
}