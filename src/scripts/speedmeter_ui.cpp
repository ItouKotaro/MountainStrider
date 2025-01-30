//=============================================================
//
// �X�s�[�h���[�^�[UI [speedmeter_ui.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "speedmeter_ui.h"
#include "renderer.h"

//=============================================================
// [SpeedMeterUI] ������
//=============================================================
void SpeedMeterUI::Init()
{
	// �X�s�[�h�e�L�X�g�̏�����
	m_speedText = new SingleComponent<CText>();
	m_speedText->Init();
	m_speedText->SetParent(gameObject);
	m_speedText->SetFont("07�S�r�S�V�b�N");
	m_speedText->SetFontSize(50);
	m_speedText->SetAlign(CText::CENTER);
	m_speedText->transform->SetPos(160.0f, 250.0f);

	// �X�s�[�h�e�L�X�g�̔w�i
	m_speedBG = new SingleComponent<CPolygon>();
	m_speedBG->Init();
	m_speedBG->SetParent(gameObject);
	m_speedBG->SetColor(D3DCOLOR_RGBA(42, 71, 184, 150));
	m_speedBG->transform->SetPos(70.0f, 240.0f);
	m_speedBG->transform->SetSize(162.5f, 60.0f);

	// ���[�^�[�̐���
	m_meterBG = new SingleComponent<CPolygon>();
	m_meterBG->Init();
	m_meterBG->SetParent(gameObject);
	m_meterBG->SetTexture("data\\TEXTURE\\UI\\meter.png");
	m_meterBG->transform->SetSize(300.0f, 300.0f);

	// �j�̎�
	m_showAngle = D3DX_PI * 0.27f;
	m_meter = new Transform();
	m_meter->SetParent(gameObject->transform);
	m_meter->SetPos(150.0f, 170.0f);
	m_meter->SetRot(m_showAngle);

	// �j�̐���
	m_needle = new SingleComponent<CPolygon>();
	m_needle->Init();
	m_needle->SetParent(m_meter);
	m_needle->SetTexture("data\\TEXTURE\\UI\\needle.png");
	m_needle->transform->SetPos(-10.0f, -10.0f);
	m_needle->transform->SetSize(20.0f, 150.0f);
}

//=============================================================
// [SpeedMeterUI] �I��
//=============================================================
void SpeedMeterUI::Uninit()
{
	m_speedText->Uninit();
	delete m_speedText;

	m_speedBG->Uninit();
	delete m_speedBG;

	m_meterBG->Uninit();
	delete m_meterBG;

	m_needle->Uninit();
	delete m_needle;

	delete m_meter;
}

//=============================================================
// [SpeedMeterUI] �X�V
//=============================================================
void SpeedMeterUI::Update()
{
	// �X�s�[�h�e�L�X�g���X�V����
	char cSpeedText[24];
	sprintf(&cSpeedText[0], "%.1f<size=20>Km/h", m_vehicle->GetSpeed());
	m_speedText->SetText(cSpeedText);

	// ���[�^�[�̍X�V
	UpdateMeter();

	// �X�V
	m_speedText->Update();
	m_speedBG->Update();
	m_meterBG->Update();
	m_needle->Update();
}

//=============================================================
// [SpeedMeterUI] ���[�^�[�̍X�V
//=============================================================
void SpeedMeterUI::UpdateMeter()
{
	// �X�s�[�h���牽�܂Ńo�[��]�|���邩�����߂�
	float speed = m_vehicle->GetSpeed();
	if (speed > 260.0f) { speed = 260.0f; }

	m_showAngle += ((D3DX_PI * (0.27f + 0.56f * (speed / (float)100.0f))) - m_showAngle) * 0.08f;
	m_meter->SetRot(m_showAngle);
}

//=============================================================
// [SpeedMeterUI] UI�`��
//=============================================================
void SpeedMeterUI::DrawUI()
{
	// �`��
	m_speedBG->DrawUI();
	m_meterBG->DrawUI();
	m_speedText->DrawUI();
	m_needle->DrawUI();
}