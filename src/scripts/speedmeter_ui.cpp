//=============================================================
//
// �X�s�[�h���[�^�[UI [speedmeter_ui.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "speedmeter_ui.h"


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
}

//=============================================================
// [SpeedMeterUI] �I��
//=============================================================
void SpeedMeterUI::Uninit()
{
	m_speedText->Uninit();
	delete m_speedText;
}

//=============================================================
// [SpeedMeterUI] �X�V
//=============================================================
void SpeedMeterUI::Update()
{
	// �X�s�[�h�e�L�X�g���X�V����
	char cSpeedText[12];
	sprintf(&cSpeedText[0], "%.1fKm/h", m_vehicle->GetSpeed());
	m_speedText->SetText(cSpeedText);


	// �X�V
	m_speedText->Update();
}

//=============================================================
// [SpeedMeterUI] UI�`��
//=============================================================
void SpeedMeterUI::DrawUI()
{
	// �`��
	m_speedText->DrawUI();
}