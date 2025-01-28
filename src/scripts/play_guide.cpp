//=============================================================
//
// プレイガイド [play_guide.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "play_guide.h"
#include "vehicle.h"

//=============================================================
// [PlayGuideManager] 初期化
//=============================================================
void PlayGuideManager::Init()
{
	SetGuide(GUIDE_ACCEL);
}

//=============================================================
// [PlayGuideManager] 終了
//=============================================================
void PlayGuideManager::Uninit()
{
	if (m_guide != nullptr)
	{
		delete m_guide;
		m_guide = nullptr;
	}

	if (m_guideUI != nullptr)
	{
		m_guideUI->gameObject->Destroy();
	}
}

//=============================================================
// [PlayGuideManager] 更新
//=============================================================
void PlayGuideManager::Update()
{
	if (m_isAllCompleted) return;

	if (m_guide != nullptr)
	{
		m_guide->Update();
		m_guide->SetAlpha(m_guideUI->GetAlpha());

		// 進捗度の更新
		m_guideUI->SetProgress(m_guide->GetProgress());

		// 完了したとき
		if (m_guide->IsCompleted())
		{
			GUIDE nextGuide = static_cast<GUIDE>(m_guideType + 1);

			if (nextGuide >= GUIDE_MAX)
			{ // ガイドがすべて完了したとき
				m_isAllCompleted = true;

				if (m_guideUI != nullptr)
				{
					m_guideUI->gameObject->Destroy();
					m_guideUI = nullptr;
				}
			}
			else
			{ // 次のガイドを設定する
				SetGuide(nextGuide);
			}
		}
	}
	
}

//=============================================================
// [PlayGuideManager] ガイドの変更
//=============================================================
void PlayGuideManager::SetGuide(const GUIDE& guide)
{
	// 新しいガイドを設定する
	m_guideType = guide;

	// 前回のガイドを破棄する
	if (m_guide != nullptr)
	{
		m_guide->Uninit();
		delete m_guide;
		m_guide = nullptr;
	}
	if (m_guideUI != nullptr)
	{
		m_guideUI->gameObject->Destroy();
		m_guideUI = nullptr;
	}

	// 新しくガイドUIを生成する
	GameObject* guideUIObj = new GameObject();
	m_guideUI = guideUIObj->AddComponent<PlayGuideUI>();
	guideUIObj->transform->SetPos(50.0f, 200.0f);

	// ガイドを生成する
	switch (m_guideType)
	{
	case PlayGuideManager::GUIDE_ACCEL:
		m_guide = new AccelGuide();
		m_guideUI->SetTitle("アクセル");
		break;
	case PlayGuideManager::GUIDE_DIRECTION:
		m_guide = new DirectionGuide();
		m_guideUI->SetTitle("方向転換");
		break;
	case PlayGuideManager::GUIDE_JUMP:
		m_guide = new JumpGuide();
		m_guideUI->SetTitle("ジャンプ");
		break;
	case PlayGuideManager::GUIDE_ACTION:
		m_guide = new ActionGuide();
		m_guideUI->SetTitle("アクション");
		break;
	}

	// 初期化処理を実行する
	m_guide->Init(guideUIObj);
}



//=============================================================
// [PlayGuideUI] 初期化
//=============================================================
void PlayGuideUI::Init()
{
	// 変数の初期化
	m_progress = PROGRESS_SEGMENT;
	m_progressCounter = 30;
	m_alpha = 0.0f;

	// セグメントの作成
	m_segment = new SingleComponent<CPolygon>();
	m_segment->Init();
	m_segment->SetParent(gameObject);
	m_segment->transform->SetSize(SEGMENT_WIDTH, 0.0f);
	m_segment->SetColor(D3DCOLOR_RGBA(189, 255, 253, 255));

	// 背景の作成
	m_bg = new SingleComponent<CPolygon>();
	m_bg->Init();
	m_bg->SetParent(gameObject);
	m_bg->transform->SetPos(SEGMENT_WIDTH, 0.0f);
	m_bg->transform->SetSize(0.0f, SIZE.y);
	m_bg->SetColor(D3DCOLOR_RGBA(14, 92, 89, 255));

	// プログレスバーの作成
	m_progressBar = new SingleComponent<CPolygon>();
	m_progressBar->Init();
	m_progressBar->SetParent(gameObject);
	m_progressBar->transform->SetPos(SEGMENT_WIDTH, SIZE.y - PROGRESSBAR_HEIGHT);
	m_progressBar->transform->SetSize(0.0f, PROGRESSBAR_HEIGHT);
	m_progressBar->SetColor(D3DCOLOR_RGBA(228, 92, 255, 255));

	// ガイドブックアイコンの作成
	m_bookIcon = new SingleComponent<CPolygon>();
	m_bookIcon->Init();
	m_bookIcon->SetParent(gameObject);
	m_bookIcon->transform->SetPos(SEGMENT_WIDTH + 10.0f, 12.0f);
	m_bookIcon->transform->SetSize(BOOKICON_SIZE, BOOKICON_SIZE);
	m_bookIcon->SetTexture("data\\TEXTURE\\UI\\guide_book.png");
	m_bookIcon->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f));

	// テキストの作成
	m_text = new SingleComponent<CText>();
	m_text->Init();
	m_text->SetParent(gameObject);
	m_text->SetFont("07鉄瓶ゴシック");
	m_text->SetFontSize(60);
	m_text->transform->SetPos(BOOKICON_SIZE + 50.0f, 18.0f);
	m_text->SetAlpha(0.0f);
}

//=============================================================
// [PlayGuideUI] 終了
//=============================================================
void PlayGuideUI::Uninit()
{
	if (m_segment != nullptr)
	{
		m_segment->Uninit();
		delete m_segment;
	}

	if (m_bg != nullptr)
	{
		m_bg->Uninit();
		delete m_bg;
	}

	if (m_progressBar != nullptr)
	{
		m_progressBar->Uninit();
		delete m_progressBar;
	}

	if (m_bookIcon != nullptr)
	{
		m_bookIcon->Uninit();
		delete m_bookIcon;
	}

	if (m_text != nullptr)
	{
		m_text->Uninit();
		delete m_text;
	}
}

//=============================================================
// [PlayGuideUI] 更新
//=============================================================
void PlayGuideUI::Update()
{
	// 更新処理
	if (m_progress <= PlayGuideUI::PROGRESS_SEGMENT)
	{
		float height = m_segment->transform->GetSize().y;
		height += (SIZE.y - height) * 0.08f;
		m_segment->transform->SetSize(SEGMENT_WIDTH, height);
	}
	else if (m_progress <= PlayGuideUI::PROGRESS_BG)
	{
		float width = m_bg->transform->GetSize().x;
		width += ((SIZE.x - SEGMENT_WIDTH) - width) * 0.08f;
		m_bg->transform->SetSize(width, m_segment->transform->GetSize().y);
	}
	else if (m_progress <= PlayGuideUI::PROGRESS_TEXT)
	{
		m_alpha = m_text->GetAlpha();
		m_alpha += (1.0f - m_alpha) * 0.08f;
		m_text->SetAlpha(m_alpha);
		m_bookIcon->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_alpha));
	}

	// プログレスバーの更新
	float progressWidth = m_progressBar->transform->GetSize().x;
	progressWidth += (m_bg->transform->GetSize().x * m_progressValue - m_progressBar->transform->GetSize().x) * 0.08f;
	m_progressBar->transform->SetPos(SEGMENT_WIDTH, m_segment->transform->GetSize().y - PROGRESSBAR_HEIGHT);
	m_progressBar->transform->SetSize(progressWidth, PROGRESSBAR_HEIGHT);

	// プログレスカウント
	if (m_progress != PlayGuideUI::PROGRESS_END)
	{
		// カウンターをデクリメント
		m_progressCounter--;

		// カウントが終了したとき
		if (m_progressCounter <= 0)
		{
			// 次の進捗
			m_progress = static_cast<PROGRESS>(m_progress + 1);

			// カウントを設定する
			switch (m_progress)
			{
			case PlayGuideUI::PROGRESS_BG:
				m_progressCounter = 30;
				break;
			case PlayGuideUI::PROGRESS_TEXT:
				m_progressCounter = 50;
				break;
			}
		}
	}


	// 更新
	m_segment->Update();
	m_bg->Update();
	m_progressBar->Update();
	m_bookIcon->Update();
	m_text->Update();
}

//=============================================================
// [PlayGuideUI] 描画
//=============================================================
void PlayGuideUI::DrawUI()
{
	m_segment->DrawUI();
	m_bg->DrawUI();
	m_progressBar->DrawUI();
	m_bookIcon->DrawUI();
	m_text->DrawUI();
}

//=============================================================
// [PlayGuideUI] タイトルの設定
//=============================================================
void PlayGuideUI::SetTitle(const std::string& title)
{
	m_text->SetText(title);
}


//=============================================================
// [AccelGuide] 初期化
//=============================================================
void AccelGuide::Init(GameObject* parent)
{
	m_controlImg = new GameObject();
	m_controlImg->AddComponent<CPolygon>();
	m_controlImg->SetParent(parent);
	m_controlImg->transform->SetPos(35.0f, 90.0f);
	m_controlImg->transform->SetSize(70.0f, 70.0f);
	m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\ltrigger.png");
	m_controlImg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_pressText = new GameObject();
	m_pressText->SetParent(parent);
	m_pressText->AddComponent<CText>()->SetText("<color=225, 233, 245>長押し");
	m_pressText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_pressText->GetComponent<CText>()->SetFontSize(55);
}

//=============================================================
// [AccelGuide] 更新
//=============================================================
void AccelGuide::Update()
{
	// アクセルを踏んでいるか
	auto pGamepadDev = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>();
	bool isAccel = (INPUT_INSTANCE->onInput("accel") ||
		pGamepadDev->GetState().Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ||
		pGamepadDev->GetState().Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

	if (isAccel)
	{
		m_progress += 0.005f;
	}

	if (INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER)
	{
		m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\ltrt.png");
		m_controlImg->transform->SetSize(240.0f, 80.0f);
		m_pressText->transform->SetPos(300.0f, 110.0f);
	}
	else
	{
		m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_shift.png");
		m_controlImg->transform->SetSize(225.0f, 80.0f);
		m_pressText->transform->SetPos(280.0f, 105.0f);
	}
	
	// 透明度
	m_controlImg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_pressText->GetComponent<CText>()->SetAlpha(m_alpha);
}


//=============================================================
// [DirectionGuide] 初期化
//=============================================================
void DirectionGuide::Init(GameObject* parent)
{
	m_controlL = new GameObject();
	m_controlL->AddComponent<CPolygon>();
	m_controlL->SetParent(parent);
	m_controlL->transform->SetPos(135.0f, 90.0f);
	m_controlL->transform->SetSize(70.0f, 70.0f);
	m_controlL->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_a.png");
	m_controlL->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_controlR = new GameObject();
	m_controlR->AddComponent<CPolygon>();
	m_controlR->SetParent(parent);
	m_controlR->transform->SetPos(260.0f, 90.0f);
	m_controlR->transform->SetSize(70.0f, 70.0f);
	m_controlR->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_d.png");
	m_controlR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_controlLR = new GameObject();
	m_controlLR->AddComponent<CPolygon>();
	m_controlLR->SetParent(parent);
	m_controlLR->transform->SetPos(170.0f, 70.0f);
	m_controlLR->transform->SetSize(130.0f, 110.0f);
	m_controlLR->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\lstick-lr.png");
	m_controlLR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_descText = new GameObject();
	m_descText->SetParent(parent);
	m_descText->AddComponent<CText>()->SetText("<color=225, 233, 245>←     →");
	m_descText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_descText->GetComponent<CText>()->SetFontSize(100);
	m_descText->transform->SetPos(40.0f, 105.0f);

	// バイクを取得する
	m_vehicle = GameObject::Find("Vehicle");
}

//=============================================================
// [DirectionGuide] 更新
//=============================================================
void DirectionGuide::Update()
{
	// コントローラーの情報を取得する
	auto pGamepadDev = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>();
	short stickLX = pGamepadDev->GetState().Gamepad.sThumbLX;

	if (!m_vehicle->GetComponent<CVehicle>()->GetFlying() && (fabsf(stickLX) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		INPUT_INSTANCE->onInput("left") || INPUT_INSTANCE->onInput("right")))
	{
		m_progress += 0.004f;
	}

	if (INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER)
	{
		m_controlL->SetVisible(false);
		m_controlR->SetVisible(false);
		m_controlLR->SetVisible(true);
	}
	else
	{
		m_controlL->SetVisible(true);
		m_controlR->SetVisible(true);
		m_controlLR->SetVisible(false);
	}

	// 透明度
	m_controlL->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_controlR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_controlLR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_descText->GetComponent<CText>()->SetAlpha(m_alpha);
}


//=============================================================
// [JumpGuide] 初期化
//=============================================================
void JumpGuide::Init(GameObject* parent)
{
	m_controlImg = new GameObject();
	m_controlImg->AddComponent<CPolygon>();
	m_controlImg->SetParent(parent);
	m_controlImg->transform->SetPos(35.0f, 90.0f);
	m_controlImg->transform->SetSize(70.0f, 70.0f);
	m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\ax.png");
	m_controlImg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	// バイクを取得する
	m_vehicle = GameObject::Find("Vehicle");
}

//=============================================================
// [JumpGuide] 更新
//=============================================================
void JumpGuide::Update()
{
	if (m_vehicle->GetComponent<CVehicle>()->GetGroundDistance() <= CVehicle::GROUND_DISTANCE && INPUT_INSTANCE->onInput("jump"))
	{
		m_progress += 0.35f;
	}

	if (INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER)
	{
		m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\ax.png");
		m_controlImg->transform->SetSize(180.0f, 70.0f);
	}
	else
	{
		m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_space.png");
		m_controlImg->transform->SetSize(225.0f, 80.0f);
	}

	// 透明度
	m_controlImg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
}


//=============================================================
// [ActionGuide] 初期化
//=============================================================
void ActionGuide::Init(GameObject* parent)
{
	m_controlL = new GameObject();
	m_controlL->AddComponent<CPolygon>();
	m_controlL->SetParent(parent);
	m_controlL->transform->SetPos(290.0f, 95.0f);
	m_controlL->transform->SetSize(60.0f, 60.0f);
	m_controlL->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_a.png");
	m_controlL->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_controlR = new GameObject();
	m_controlR->AddComponent<CPolygon>();
	m_controlR->SetParent(parent);
	m_controlR->transform->SetPos(365.0f, 95.0f);
	m_controlR->transform->SetSize(60.0f, 60.0f);
	m_controlR->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_d.png");
	m_controlR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_controlLR = new GameObject();
	m_controlLR->AddComponent<CPolygon>();
	m_controlLR->SetParent(parent);
	m_controlLR->transform->SetPos(305.0f, 80.0f);
	m_controlLR->transform->SetSize(110.0f, 90.0f);
	m_controlLR->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\lstick-lr.png");
	m_controlLR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_descText = new GameObject();
	m_descText->SetParent(parent);
	m_descText->AddComponent<CText>()->SetText("<color=225, 233, 245>空中で回転 ←       →");
	m_descText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_descText->GetComponent<CText>()->SetFontSize(45);
	m_descText->transform->SetPos(40.0f, 105.0f);

	// バイクを取得する
	m_vehicle = GameObject::Find("Vehicle");
}

//=============================================================
// [ActionGuide] 更新
//=============================================================
void ActionGuide::Update()
{
	// コントローラーの情報を取得する
	auto pGamepadDev = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>();
	short stickLX = pGamepadDev->GetState().Gamepad.sThumbLX;

	if (m_vehicle->GetComponent<CVehicle>()->GetFlying() && (fabsf(stickLX) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		INPUT_INSTANCE->onInput("left") || INPUT_INSTANCE->onInput("right")))
	{
		m_progress += 0.01f;
	}

	if (INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER)
	{
		m_controlL->SetVisible(false);
		m_controlR->SetVisible(false);
		m_controlLR->SetVisible(true);
	}
	else
	{
		m_controlL->SetVisible(true);
		m_controlR->SetVisible(true);
		m_controlLR->SetVisible(false);
	}

	// 透明度
	m_controlL->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_controlR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_controlLR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_descText->GetComponent<CText>()->SetAlpha(m_alpha);
}