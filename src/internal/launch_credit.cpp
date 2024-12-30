//=============================================================
//
// 起動クレジット [launch_credit.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "launch_credit.h"
#include "manager.h"

//=============================================================
// [LaunchCredit] 初期化
//=============================================================
void LaunchCredit::Init()
{
	// クレジットロゴポリゴンを作成する
	m_creditLogo = new SingleComponent<CPolygon>();
	m_creditLogo->Init();
}

//=============================================================
// [LaunchCredit] 終了
//=============================================================
void LaunchCredit::Uninit()
{
	// クレジットロゴポリゴンを解放する
	m_creditLogo->Uninit();
	delete m_creditLogo;
}

//=============================================================
// [LaunchCredit] 更新
//=============================================================
void LaunchCredit::Update()
{
	if (m_state == STATE::MIDST)
	{
		m_alpha = 1.0f;
		if (m_time >= m_showTime - m_fadeTime && m_fadeTime > 0.0f)
		{ // フェードイン
			m_alpha = 1.0f - ((m_time - (m_showTime - m_fadeTime)) / m_fadeTime);
		}
		else if (m_time <= m_fadeTime && m_fadeTime > 0.0f)
		{ // フェードアウト
			m_alpha = m_time / m_fadeTime;
		}
		m_creditLogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

		// 表示切り替え時間になったとき
		if (m_time <= 0.0f)
		{
			// タイマーを戻す
			m_time = m_showTime;

			// 次のクレジットに変更する
			m_currentCreditID++;
			if (m_currentCreditID >= static_cast<int>(m_credits.size()))
			{ // クレジットが終了したとき
				m_state = STATE::ENDED;
				m_currentCreditID = -1;
				m_alpha = 0.0f;
				m_time = m_fadeTime;
				return;
			}

			// ポリゴンの設定を行う
			m_creditLogo->transform->SetSize(m_credits[m_currentCreditID].size * m_credits[m_currentCreditID].scale);
			m_creditLogo->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - m_creditLogo->transform->GetSize().x / (float)2.0f,
				CRenderer::SCREEN_HEIGHT / 2 - m_creditLogo->transform->GetSize().y / (float)2.0f);
			m_creditLogo->SetTexture(m_credits[m_currentCreditID].path);
		}
	}

	// 背景ポリゴンの操作
	if (m_bgPoly != nullptr)
	{
		D3DXCOLOR col = m_bgPoly->GetColor();
		if (m_state == STATE::ENDED && m_time > 0.0f && m_fadeTime > 0.0f)
		{
			col.a = m_time / m_fadeTime;
		}
		else if (m_state == STATE::ENDED)
		{
			col.a = 0.0f;
		}
		else
		{
			col.a = 1.0f;
		}
		m_bgPoly->SetColor(col);
	}

	if (m_state == STATE::MIDST || (m_state == STATE::ENDED && m_time > 0.0f))
	{
		// 計測時間を加算する
		m_time -= CManager::GetInstance()->GetDeltaTime();
	}

	// クレジットロゴポリゴンを更新する
	m_creditLogo->Update();
}

//=============================================================
// [LaunchCredit] 描画
//=============================================================
void LaunchCredit::DrawUI()
{
	// クレジットロゴポリゴンを描画する
	if (m_state == STATE::MIDST)
	{
		m_creditLogo->DrawUI();
	}
}

//=============================================================
// [LaunchCredit] 開始する
//=============================================================
void LaunchCredit::Start()
{
	m_state = STATE::MIDST;
	m_time = 0;
	m_currentCreditID = -1;
}

//=============================================================
// [LaunchCredit] クレジットを追加する
//=============================================================
int LaunchCredit::AddCredit(const std::string& path, const D3DXVECTOR2& size, const float& scale)
{
	CreditLogo credit;
	credit.path = path;
	credit.size = size;
	credit.scale = scale;
	m_credits.push_back(credit);
	return static_cast<int>(m_credits.size()) - 1;
}