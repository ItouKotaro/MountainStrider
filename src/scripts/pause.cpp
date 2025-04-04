//=============================================================
//
// ポーズ [pause.cpp]
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
#include "scene/game.h"

//=============================================================
// [Pause] 初期化
//=============================================================
void Pause::Init()
{
	m_pausePage = new GameObject();
	auto page = m_pausePage->AddComponent<Pages>();
	page->SetNumPage(2);

	// 背景
	GameObject* pauseBG = new GameObject();
	pauseBG->AddComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	pauseBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);
	pauseBG->SetPriority(8);
	page->AddObject(1, pauseBG);

	// ポーズテキスト
	m_pauseText = new GameObject();
	m_pauseText->AddComponent<CText>()->SetText("<size=150>PAUSE");
	m_pauseText->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_pauseText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_pauseText->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 150.0f);
	m_pauseText->SetPriority(9);
	page->AddObject(1, m_pauseText);

	// 戻るボタン
	m_sBack = new GameObject();
	m_sBack->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 300.0f, 400.0f);
	m_sBack->transform->SetSize(600.0f, 160.0f);
	m_sBack->AddComponent<CPolygon>();
	m_sBack->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\PAUSE\\back.png");
	m_sBack->SetPriority(9);
	page->AddObject(1, m_sBack);

	// 終了ボタン
	m_sExit = new GameObject();
	m_sExit->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 300.0f, 650.0f);
	m_sExit->transform->SetSize(600.0f, 160.0f);
	m_sExit->AddComponent<CPolygon>();
	m_sExit->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\PAUSE\\end.png");
	m_sExit->SetPriority(9);
	page->AddObject(1, m_sExit);

	// ページを変更する
	page->AllHideObjects();
}

//=============================================================
// [Pause] 更新
//=============================================================
void Pause::Update()
{
	// 操作の更新
	UpdateControl();

	// 色を初期値に戻す
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
// [Pause] ポーズにするか
//=============================================================
void Pause::SetPause(const bool& enabled)
{
	m_pausePage->GetComponent<Pages>()->SetPage(enabled ? 1 : 0);
	m_isPause = enabled;

	// カーソルの非表示
	if (!enabled) Main::SetShowCursor(false);

	// 物理設定
	CPhysics::GetInstance()->SetUpdatePhysics(!m_isPause);

	// カメラの有効・無効化
	if (ModeManager::GetInstance()->GetResult() == nullptr)
	{
		GameObject::Find("Camera")->GetComponent<CCameraMove>()->enabled = !enabled;
	}
}

//=============================================================
// [Pause] 操作の更新
//=============================================================
void Pause::UpdateControl()
{
	// カーソルの表示
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


	// 上
	if (INPUT_INSTANCE->onTrigger("w") || INPUT_INSTANCE->onTrigger("up") ||
		INPUT_INSTANCE->onTrigger("p:up"))
	{
		m_select = static_cast<SELECT>(m_select - 1);
	}

	// 下
	if (INPUT_INSTANCE->onTrigger("s") || INPUT_INSTANCE->onTrigger("down") ||
		INPUT_INSTANCE->onTrigger("p:down"))
	{
		m_select = static_cast<SELECT>(m_select + 1);
	}

	// 正しい数値に直す
	if (m_select < 0) m_select = SELECT_BACK;
	if (m_select >= SELECT_MAX) m_select = static_cast<SELECT>(SELECT_MAX - 1);


	// マウス用
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