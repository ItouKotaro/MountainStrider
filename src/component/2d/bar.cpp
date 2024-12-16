//=============================================================
//
// バー [bar.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "bar.h"
#include "renderer.h"
#include "internal/data_manager.h"

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
	m_pBgObj->transform->SetParent(transform);
	m_pBarObj->transform->SetParent(transform);
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


//=============================================================
// [CAdvancedBar] 初期化
//=============================================================
void CAdvancedBar::Init()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	// デバイスの取得
	pDevice = CRenderer::GetInstance()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBar, nullptr);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBG, nullptr);

	VERTEX_2D* pVtx = nullptr; //頂点情報へのポインタ

	for (int i = 0; i < 2; i++)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		if (i == 0)
			m_pVtxBG->Lock(0, 0, (void**)&pVtx, 0);
		else if (i == 1)
			m_pVtxBar->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点バッファをアンロックする
		if (i == 0)
			m_pVtxBG->Unlock();
		else if (i == 1)
			m_pVtxBar->Unlock();
	}

	// 頂点の更新
	UpdateVertex();
}

//=============================================================
// [CAdvancedBar] 終了
//=============================================================
void CAdvancedBar::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBar != nullptr)
	{
		m_pVtxBar->Release();
		m_pVtxBar = nullptr;
	}
	if (m_pVtxBG != nullptr)
	{
		m_pVtxBG->Release();
		m_pVtxBG = nullptr;
	}
}

//=============================================================
// [CAdvancedBar] 描画
//=============================================================
void CAdvancedBar::DrawUI()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CRenderer::GetInstance()->GetDevice();

	for (int i = 0; i < 2; i++)
	{
		// 頂点バッファをデータストリームに設定
		if (i == 0)
			pDevice->SetStreamSource(0, m_pVtxBG, 0, sizeof(VERTEX_2D));
		else if (i ==1)
			pDevice->SetStreamSource(0, m_pVtxBar, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		if (i == 1)
		{
			pDevice->SetTexture(0, m_texture);
		}
		else
		{
			pDevice->SetTexture(0, nullptr);
		}
		

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
			0, //描画する最初の頂点インデックス
			2); //描画するプリミティブ数
	}
}

//=============================================================
// [CAdvancedBar] 頂点の更新
//=============================================================
void CAdvancedBar::UpdateVertex()
{
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	// 頂点座標を導き出す
	D3DXVECTOR3 vtxPos[4];
	switch (m_align)
	{
	case CAdvancedBar::LEFT:
		vtxPos[0] = { 0.0f, -m_fBold / 2, 0.0f };
		vtxPos[1] = { m_fLength, -m_fBold / 2, 0.0f };
		vtxPos[2] = { 0.0f, m_fBold / 2, 0.0f };
		vtxPos[3] = { m_fLength, m_fBold / 2, 0.0f };
		break;
	case CAdvancedBar::CENTER:
		vtxPos[0] = { -m_fLength / 2, -m_fBold / 2, 0.0f };
		vtxPos[1] = { m_fLength / 2, -m_fBold / 2, 0.0f };
		vtxPos[2] = { -m_fLength / 2, m_fBold / 2, 0.0f };
		vtxPos[3] = { m_fLength / 2, m_fBold / 2, 0.0f };
		break;
	}

	//----------------------------------------------------------------------
	// バー

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBar->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = vtxPos[0];
	pVtx[1].pos = vtxPos[1];
	pVtx[2].pos = vtxPos[2];
	pVtx[3].pos = vtxPos[3];

	// バーの長さを変更する
	pVtx[1].pos.x -= (1.0f - m_fProgress) * m_fLength;
	pVtx[3].pos.x -= (1.0f - m_fProgress) * m_fLength;

	// 指定位置に移動させる
	pVtx[0].pos += transform->GetWPos();
	pVtx[1].pos += transform->GetWPos();
	pVtx[2].pos += transform->GetWPos();
	pVtx[3].pos += transform->GetWPos();

	//頂点カラー
	pVtx[0].col = m_color[0];
	pVtx[1].col = m_color[1] + m_color[0] * (1.0f - m_fProgress);
	pVtx[2].col = m_color[2];
	pVtx[3].col = m_color[3] + m_color[2] * (1.0f - m_fProgress);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fProgress, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fProgress, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBar->Unlock();

	//----------------------------------------------------------------------
	// 背景

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBG->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = vtxPos[0];
	pVtx[1].pos = vtxPos[1];
	pVtx[2].pos = vtxPos[2];
	pVtx[3].pos = vtxPos[3];

	// 指定位置に移動させる
	pVtx[0].pos += transform->GetWPos();
	pVtx[1].pos += transform->GetWPos();
	pVtx[2].pos += transform->GetWPos();
	pVtx[3].pos += transform->GetWPos();

	//頂点カラー
	pVtx[0].col = m_bgColor;
	pVtx[1].col = m_bgColor;
	pVtx[2].col = m_bgColor;
	pVtx[3].col = m_bgColor;

	//頂点バッファをアンロックする
	m_pVtxBG->Unlock();
}

//=============================================================
// [CAdvancedBar] 色変更
//=============================================================
void CAdvancedBar::SetColor(int index, D3DXCOLOR color)
{
	if (0 <= index && index < 4)
	{
		m_color[index] = color;
	}

	// 更新
	UpdateVertex();
}

//=============================================================
// [CAdvancedBar] 色変更
//=============================================================
void CAdvancedBar::SetColor(D3DXCOLOR color)
{
	for (int i = 0; i < 4; i++)
	{
		m_color[i] = color;
	}

	// 更新
	UpdateVertex();
}

//=============================================================
// [CAdvancedBar] 背景色変更
//=============================================================
void CAdvancedBar::SetBGColor(D3DXCOLOR color)
{
	m_bgColor = color;

	// 更新
	UpdateVertex();
}

//=============================================================
// [CAdvancedBar] 進捗度変更
//=============================================================
void CAdvancedBar::SetProgress(const float& percent)
{
	m_fProgress = percent;

	// 更新
	UpdateVertex();
}

//=============================================================
// [CAdvancedBar] 長さの変更
//=============================================================
void CAdvancedBar::SetLength(const float& length)
{
	m_fLength = length;

	// 更新
	UpdateVertex();
}

//=============================================================
// [CAdvancedBar] 太さの変更
//=============================================================
void CAdvancedBar::SetBold(const float& bold)
{
	m_fBold = bold;

	// 更新
	UpdateVertex();
}

//=============================================================
// [CAdvancedBar] テクスチャの設定
//=============================================================
void CAdvancedBar::SetTexture(std::string sPath)
{
	CDataTexture* pTex = CDataManager::GetInstance()->RefTexture(sPath);

	if (pTex != nullptr)
	{
		m_texture = pTex->GetTexture();
	}
}

//=============================================================
// [CAdvancedBar] 整列の変更
//=============================================================
void CAdvancedBar::SetAlign(const ALIGN& align)
{
	m_align = align;

	// 更新
	UpdateVertex();
}