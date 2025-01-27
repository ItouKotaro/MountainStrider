//=============================================================
//
// スピードメーターUI [speedmeter_ui.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "speedmeter_ui.h"
#include "renderer.h"

//=============================================================
// [SpeedMeterUI] 初期化
//=============================================================
void SpeedMeterUI::Init()
{
	// スピードテキストの初期化
	m_speedText = new SingleComponent<CText>();
	m_speedText->Init();
	m_speedText->SetParent(gameObject);
	m_speedText->SetFont("07鉄瓶ゴシック");

	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 device = CRenderer::GetInstance()->GetDevice();

	// 頂点バッファの生成
	device->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * BAR_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_vtxBuff, nullptr);
	VERTEX_2D* vtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	for (int i = 0; i < BAR_NUM; i++)
	{
		// バーの高さを算出する
		float height = i * i * 10.0f + 1.0f;

		// 頂点座標の設定
		vtx[0].pos = D3DXVECTOR3(0.0f, height, 0.0f);
		vtx[1].pos = D3DXVECTOR3(BAR_WIDTH, height, 0.0f);
		vtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vtx[3].pos = D3DXVECTOR3(BAR_WIDTH, 0.0f, 0.0f);

		// スペース
		vtx[0].pos.x += (BAR_WIDTH + BAR_SPACE) * i + BAR_RIGHT;
		vtx[1].pos.x += (BAR_WIDTH + BAR_SPACE) * i + BAR_RIGHT;
		vtx[2].pos.x += (BAR_WIDTH + BAR_SPACE) * i + BAR_RIGHT;
		vtx[3].pos.x += (BAR_WIDTH + BAR_SPACE) * i + BAR_RIGHT;

		vtx[0].pos.y += BAR_TOP;
		vtx[1].pos.y += BAR_TOP;
		vtx[2].pos.y += BAR_TOP;
		vtx[3].pos.y += BAR_TOP;

		// トランスフォームの基づく位置に移動する
		vtx[0].pos += transform->GetWPos();
		vtx[1].pos += transform->GetWPos();
		vtx[2].pos += transform->GetWPos();
		vtx[3].pos += transform->GetWPos();

		// rhwの設定
		vtx[0].rhw = 1.0f;
		vtx[1].rhw = 1.0f;
		vtx[2].rhw = 1.0f;
		vtx[3].rhw = 1.0f;

		// 頂点カラー
		vtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		vtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		vtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		vtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		// テクスチャ座標の設定
		vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 次のポインターへ
		vtx += 4;
	}

	// 頂点バッファをアンロックする
	m_vtxBuff->Unlock();
}

//=============================================================
// [SpeedMeterUI] 終了
//=============================================================
void SpeedMeterUI::Uninit()
{
	m_speedText->Uninit();
	delete m_speedText;

	// 頂点バッファの破棄
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// [SpeedMeterUI] 更新
//=============================================================
void SpeedMeterUI::Update()
{
	// スピードテキストを更新する
	char cSpeedText[12];
	sprintf(&cSpeedText[0], "%.1fKm/h", m_vehicle->GetSpeed());
	m_speedText->SetText(cSpeedText);


	// 更新
	m_speedText->Update();
}

//=============================================================
// [SpeedMeterUI] UI描画
//=============================================================
void SpeedMeterUI::DrawUI()
{
	// 描画
	m_speedText->DrawUI();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CRenderer::GetInstance()->GetDevice();

	for (int i = 0; i < BAR_NUM; i++)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, nullptr);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
			i * 4, //描画する最初の頂点インデックス
			2); //描画するプリミティブ数
	}
}