//=============================================================
//
// �|�[�Y [pause.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "pause.h"
#include "renderer.h"
#include "component/other/page.h"
#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "scripts/camera_move.h"
#include "internal/physics.h"

//=============================================================
// [Pause] ������
//=============================================================
void Pause::Init()
{
	m_pausePage = new GameObject();
	auto page = m_pausePage->AddComponent<Pages>();
	page->SetNumPage(2);

	// �w�i
	GameObject* pauseBG = new GameObject();
	pauseBG->AddComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	pauseBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);
	pauseBG->SetPriority(8);
	page->AddObject(1, pauseBG);

	// �|�[�Y�e�L�X�g
	m_pauseText = new GameObject();
	m_pauseText->AddComponent<CText>()->SetText("<size=150>PAUSE");
	m_pauseText->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_pauseText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_pauseText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 150.0f);
	m_pauseText->SetPriority(9);
	page->AddObject(1, m_pauseText);

	// �߂�{�^��
	m_sBack = new GameObject();
	m_sBack->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 300.0f, 400.0f);
	m_sBack->transform->SetSize(600.0f, 160.0f);
	m_sBack->AddComponent<CPolygon>();
	m_sBack->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\PAUSE\\back.png");
	m_sBack->SetPriority(9);
	page->AddObject(1, m_sBack);

	// �I���{�^��
	m_sExit = new GameObject();
	m_sExit->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 300.0f, 650.0f);
	m_sExit->transform->SetSize(600.0f, 160.0f);
	m_sExit->AddComponent<CPolygon>();
	m_sExit->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\PAUSE\\end.png");
	m_sExit->SetPriority(9);
	page->AddObject(1, m_sExit);

	// �y�[�W��ύX����
	page->AllHideObjects();
}

//=============================================================
// [Pause] �X�V
//=============================================================
void Pause::Update()
{
	// ����̍X�V
	UpdateControl();

	// �F�������l�ɖ߂�
	m_sBack->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
	m_sExit->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));

	switch (m_select)
	{
	case Pause::SELECT_BACK:
		m_sBack->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case Pause::SELECT_EXIT:
		m_sExit->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	}

	if (INPUT_INSTANCE->onTrigger("space") || INPUT_INSTANCE->onTrigger("enter") || INPUT_INSTANCE->onTrigger("p:a") ||
		(INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_MOUSE && INPUT_INSTANCE->onTrigger("lclick") && m_onCursor))
	{
		switch (m_select)
		{
		case Pause::SELECT_BACK:
			SetPause(false);
			break;
		case Pause::SELECT_EXIT:
			SetPause(false);
			CSceneManager::GetInstance()->SetScene("title"); 
			break;
		}
	}
}

//=============================================================
// [Pause] �|�[�Y�ɂ��邩
//=============================================================
void Pause::SetPause(const bool& enabled)
{
	m_pausePage->GetComponent<Pages>()->SetPage(enabled ? 1 : 0);
	m_isPause = enabled;

	if (!enabled) Main::SetShowCursor(false);

	// �����ݒ�
	CPhysics::GetInstance()->SetUpdatePhysics(!m_isPause);
	GameObject::Find("Camera")->GetComponent<CCameraMove>()->enabled = !enabled;
}

//=============================================================
// [Pause] ����̍X�V
//=============================================================
void Pause::UpdateControl()
{
	Main::SetShowCursor(INPUT_INSTANCE->GetLastInput() == CInputSystem::DEVICE_MOUSE);

	auto padInfo = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad;
	short stickX = padInfo.sThumbLX;
	short stickY = padInfo.sThumbLY;
	static int stickCounter = 0;

	if (stickCounter > 0) stickCounter--;
	if (stickCounter <= 0 && stickY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_select = static_cast<SELECT>(m_select - 1);
		stickCounter = 10;
	}
	if (stickCounter <= 0 && stickY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_select = static_cast<SELECT>(m_select + 1);
		stickCounter = 10;
	}


	// ��
	if (INPUT_INSTANCE->onTrigger("w") || INPUT_INSTANCE->onTrigger("up") ||
		INPUT_INSTANCE->onTrigger("p:up"))
	{
		m_select = static_cast<SELECT>(m_select - 1);
	}

	// ��
	if (INPUT_INSTANCE->onTrigger("s") || INPUT_INSTANCE->onTrigger("down") ||
		INPUT_INSTANCE->onTrigger("p:down"))
	{
		m_select = static_cast<SELECT>(m_select + 1);
	}

	// ���������l�ɒ���
	if (m_select < 0) m_select = SELECT_BACK;
	if (m_select >= SELECT_MAX) m_select = static_cast<SELECT>(SELECT_MAX - 1);


	// �}�E�X�p
	CManager::CursorPos cursorPos = CManager::GetInstance()->GetCursorClientPos();
	if (INPUT_INSTANCE->GetLastInput() == CInputSystem::DEVICE_MOUSE &&
		m_sBack->transform->GetWPos().x <= cursorPos.x && cursorPos.x <= m_sBack->transform->GetWPos().x + m_sBack->transform->GetSize().x &&
		m_sBack->transform->GetWPos().y <= cursorPos.y && cursorPos.y <= m_sBack->transform->GetWPos().y + m_sBack->transform->GetSize().y)
	{
		m_select = SELECT_BACK;
		m_onCursor = true;
	}
	else if (INPUT_INSTANCE->GetLastInput() == CInputSystem::DEVICE_MOUSE &&
		m_sExit->transform->GetWPos().x <= cursorPos.x && cursorPos.x <= m_sExit->transform->GetWPos().x + m_sExit->transform->GetSize().x &&
		m_sExit->transform->GetWPos().y <= cursorPos.y && cursorPos.y <= m_sExit->transform->GetWPos().y + m_sExit->transform->GetSize().y)
	{
		m_select = SELECT_EXIT;
		m_onCursor = true;
	}
	else
	{
		m_onCursor = false;
	}
}