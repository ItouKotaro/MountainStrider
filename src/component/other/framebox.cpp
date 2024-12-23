//=============================================================
//
// フレームボックス [framebox.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "framebox.h"
#include "renderer.h"

//=============================================================
// [FrameBox] 初期化
//=============================================================
void FrameBox::Init()
{
	m_mainImg = new SingleComponent<CPolygon>();
	m_mainImg->Init();
	m_mainImg->SetParent(gameObject);

	m_maskImg = new SingleComponent<CPolygon>();
	m_maskImg->Init();
	m_maskImg->transform->SetParent(m_mainImg->transform);
}

//=============================================================
// [FrameBox] 終了
//=============================================================
void FrameBox::Uninit()
{
	if (m_mainImg != nullptr)
	{
		m_mainImg->Uninit();
		delete m_mainImg;
		m_mainImg = nullptr;
	}

	if (m_maskImg != nullptr)
	{
		m_maskImg->Uninit();
		delete m_maskImg;
		m_maskImg = nullptr;
	}
}

//=============================================================
// [FrameBox] 更新
//=============================================================
void FrameBox::Update()
{
	// サイズ調整
	m_mainImg->transform->SetSize(transform->GetSize());
	m_maskImg->transform->SetSize(transform->GetSize().x - m_frameSize * 2, transform->GetSize().y - m_frameSize * 2);
	m_maskImg->transform->SetPos(m_frameSize, m_frameSize);

	m_mainImg->Update();
	m_maskImg->Update();
}

//=============================================================
// [FrameBox] 描画
//=============================================================
void FrameBox::DrawUI()
{
	// デバイスの取得
	auto pDevice = CRenderer::GetInstance()->GetDevice();

	pDevice->Clear(
		0,
		nullptr,
		D3DCLEAR_STENCIL,	// 初期化するバッファの種類
		D3DCOLOR_ARGB(255, 255, 255, 0),				// フレームバッファの初期化色
		1.0f,							// Zバッファの初期値
		0);							// ステンシルバッファの初期値

	// Zバッファの設定を変更するので現在の状態を保存しておく
	DWORD dwCurZTest, dwCurZFunc;
	pDevice->GetRenderState(D3DRS_ZENABLE, &dwCurZTest);
	pDevice->GetRenderState(D3DRS_ZFUNC, &dwCurZFunc);

	// Zバッファに書き込まないようにします
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

	// ステンシルバッファの設定です
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);  // ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); // ステンシルテストは常に行う

	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x02);

	m_maskImg->DrawUI();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x02);

	m_mainImg->DrawUI();

	// もとに戻す
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, dwCurZTest);
	pDevice->SetRenderState(D3DRS_ZFUNC, dwCurZFunc);

	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x01);
}

//=============================================================
// [FrameBox] テクスチャの設定
//=============================================================
void FrameBox::SetTexture(const std::string& path)
{
	m_mainImg->SetTexture(path);
	m_maskImg->SetTexture(path);
}