//=============================================================
//
// プレイガイド [play_guide.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "play_guide.h"

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

		// 進捗度の更新
		m_guideUI->SetProgress(m_guide->GetProgress());

		// 完了したとき
		if (m_guide->IsCompleted())
		{
			GUIDE nextGuide = static_cast<GUIDE>(m_guideType + 1);

			if (nextGuide >= GUIDE_MAX)
			{ // ガイドがすべて完了したとき
				m_isAllCompleted = true;
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
		break;
	case PlayGuideManager::GUIDE_DIRECTION:
		m_guide = new AccelGuide();
		break;
	case PlayGuideManager::GUIDE_JUMP:
		m_guide = new AccelGuide();
		break;
	case PlayGuideManager::GUIDE_ACTION:
		m_guide = new AccelGuide();
		break;
	}

	// 初期化処理を実行する
	m_guide->Init();
}


//=============================================================
// [AccelGuide] 更新
//=============================================================
void AccelGuide::Update()
{
	if (INPUT_INSTANCE->onInput("accel"))
	{
		m_progress += 0.005f;
	}
}





//=============================================================
// [PlayGuideUI] 初期化
//=============================================================
void PlayGuideUI::Init()
{
	// 変数の初期化
	m_progress = PROGRESS_SEGMENT;
	m_progressCounter = 30;

	// セグメントの作成
	m_segment = new SingleComponent<CPolygon>();
	m_segment->Init();
	m_segment->SetParent(gameObject);
	m_segment->transform->SetSize(SEGMENT_WIDTH, 0.0f);
	m_segment->SetColor(D3DCOLOR_RGBA(106, 187, 252, 255));

	// 背景の作成
	m_bg = new SingleComponent<CPolygon>();
	m_bg->Init();
	m_bg->SetParent(gameObject);
	m_bg->transform->SetPos(SEGMENT_WIDTH, 0.0f);
	m_bg->transform->SetSize(0.0f, SIZE.y);

	// プログレスバーの作成
	m_progressBar = new SingleComponent<CPolygon>();
	m_progressBar->Init();
	m_progressBar->SetParent(gameObject);
	m_progressBar->transform->SetPos(SEGMENT_WIDTH, SIZE.y - PROGRESSBAR_HEIGHT);
	m_progressBar->transform->SetSize(0.0f, PROGRESSBAR_HEIGHT);
	m_progressBar->SetColor(D3DCOLOR_RGBA(255, 111, 33, 255));
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
				m_progressCounter = 50;
				break;
			}
		}
	}


	// 更新
	m_segment->Update();
	m_bg->Update();
	m_progressBar->Update();
}

//=============================================================
// [PlayGuideUI] 描画
//=============================================================
void PlayGuideUI::DrawUI()
{
	m_segment->DrawUI();
	m_bg->DrawUI();
	m_progressBar->DrawUI();
}