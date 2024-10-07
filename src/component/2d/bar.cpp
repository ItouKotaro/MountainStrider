//=============================================================
//
// バー [bar.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "bar.h"

// 静的メンバ変数の初期化
const float CBar::DEFAULT_BAR_LENGTH = 800.0f;
const float CBar::DEFAULT_BAR_BOLD = 80.0f;
const float CProgressBar::DEFAULT_BAR_SPACE = 8.0f;
const D3DXCOLOR CProgressBar::DEFAULT_FILL_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CProgressBar::DEFAULT_NONFILL_COLOR = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

//=============================================================
// [CBar] 初期化
//=============================================================
void CBar::Init()
{
	m_pBgObj = new GameObject();
	m_pBarObj = new GameObject();
	m_pBgObj->SetParent(gameObject);
	m_pBarObj->SetParent(gameObject);
	m_pBgObj->AddComponent<CPolygon>();
	m_pBarObj->AddComponent<CPolygon>();

	// 変数の初期化
	m_fBarLength = DEFAULT_BAR_LENGTH;
	m_fBarBold = DEFAULT_BAR_BOLD;
	m_pBarObj->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_fBarProgress = 1.0f;

	// サイズの設定
	m_pBgObj->transform->SetSize(m_fBarLength, m_fBarBold);
	m_pBarObj->transform->SetSize(m_fBarLength * m_fBarProgress, m_fBarBold);
}

//=============================================================
// [CBar] 更新
//=============================================================
void CBar::Update()
{
	m_pBarObj->transform->SetSize(m_fBarLength * m_fBarProgress, m_fBarBold);
}


//=============================================================
// [CProgressBar] コンストラクタ
//=============================================================
CProgressBar::CProgressBar(const int& nNumBar)
{
	m_nNumBar = nNumBar;
}

//=============================================================
// [CProgressBar] 初期化
//=============================================================
void CProgressBar::Init()
{
	// ゲームオブジェクトを作成する
	m_pBgObj = new GameObject();
	m_pBgObj->SetParent(gameObject);
	m_pBgObj->AddComponent<CPolygon>();

	// バーの作成
	for (int i = 0; i < m_nNumBar; i++)
	{
		GameObject* pBar = new GameObject();
		pBar->SetParent(gameObject);
		pBar->SetPriority(8);
		pBar->AddComponent<CPolygon>();
		pBar->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_pBars.push_back(pBar);
	}

	// 変数の初期化
	m_fBarLength = CBar::DEFAULT_BAR_LENGTH;
	m_fBarBold = CBar::DEFAULT_BAR_BOLD;
	m_fBarSpace = CProgressBar::DEFAULT_BAR_SPACE;
	m_fSpace = 5;
	m_fBarProgress = 1.0f;
	m_fillColor = DEFAULT_FILL_COLOR;
	m_nonFillCollor = DEFAULT_NONFILL_COLOR;
	m_fAlpha = 1.0f;

	// サイズの設定
	m_pBgObj->transform->SetSize(m_fBarLength, m_fBarBold);
}

//=============================================================
// [CProgressBar] 終了
//=============================================================
void CProgressBar::Uninit()
{
	gameObject->Destroy(true);
}

//=============================================================
// [CProgressBar] 背景の色変更
//=============================================================
void CProgressBar::SetBGColor(const D3DXCOLOR& color)
{
	m_pBgObj->GetComponent<CPolygon>()->SetColor(color);
}

//=============================================================
// [CProgressBar] 透明度
//=============================================================
void CProgressBar::SetAlpha(const float& fAlpha)
{
	m_fAlpha = fAlpha;

	// バー
	m_fillColor.a = fAlpha;
	m_nonFillCollor.a = fAlpha;

	// 背景
	D3DXCOLOR bgColor = m_pBgObj->GetComponent<CPolygon>()->GetColor();
	bgColor.a = fAlpha;
	m_pBgObj->GetComponent<CPolygon>()->SetColor(bgColor);
}

//=============================================================
// [CProgressBar] 更新
//=============================================================
void CProgressBar::Update()
{
	// バーのサイズを計算する
	D3DXVECTOR2 barSize;
	barSize.x = (m_fBarLength - m_fBarSpace * m_pBars.size() - m_fSpace * 2) / m_pBars.size();
	barSize.y = m_fBarBold - m_fSpace * 2;

	// 背景のサイズ
	m_pBgObj->transform->SetSize(m_fBarLength, m_fBarBold);

	// 埋める数（割合）
	int nFillNum = static_cast<int>(m_nNumBar * m_fBarProgress);

	for (unsigned int i = 0; i < m_pBars.size(); i++)
	{
		// 色
		if (i < static_cast<unsigned int>(nFillNum))
			m_pBars[i]->GetComponent<CPolygon>()->SetColor(m_fillColor);
		else
			m_pBars[i]->GetComponent<CPolygon>()->SetColor(m_nonFillCollor);

		// サイズを設定する
		m_pBars[i]->transform->SetSize(barSize);

		// 等間隔にバーを配置する
		m_pBars[i]->transform->SetPos(m_fSpace * 2.0f + (barSize.x + m_fBarSpace) * i, m_fSpace);
	}
}