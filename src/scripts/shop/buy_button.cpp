//=============================================================
//
// 購入ボタン [buy_button.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "buy_button.h"
#include "renderer.h"
#include "manager.h"
#include "shop.h"
#include "component/other/audio.h"

const float BuyButtonUI::EDGE_BOLD = 15.0f;
const float BuyButtonUI::WIDTH = 280.0f;
const float BuyButtonUI::HEIGHT = 400.0f;
const float BuyButtonUI::ITEM_SIZE = 146.0f;
const float BuyButtonUI::PRICEICON_SIZE = 60.0f;
const D3DXCOLOR BuyButtonUI::ITEM_COLOR = D3DCOLOR_RGBA(34, 130, 43, 255);
const D3DXCOLOR BuyButtonUI::ITEM_COLOR_BOUGHT = D3DCOLOR_RGBA(2, 54, 7, 255);
const D3DXCOLOR BuyButtonUI::PERK_COLOR = D3DCOLOR_RGBA(196, 0, 194, 255);
const D3DXCOLOR BuyButtonUI::PERK_COLOR_BOUGHT = D3DCOLOR_RGBA(107, 22, 106, 255);
AudioClip buySE;
//=============================================================
// [BuyButtonUI] 初期化
//=============================================================
void BuyButtonUI::Init()
{
	if (!buySE) buySE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\SHOP\\buy.mp3", FMOD_2D);
	gameObject->AddComponent<AudioSource>();

	m_progress = 0.0f;

	// イメージ画像
	m_image = new SingleComponent<CPolygon>();
	m_image->Init();
	m_image->SetTexture(m_shopItem->GetTexturePath());
	m_image->SetParent(gameObject);
	m_image->transform->SetSize(ITEM_SIZE, ITEM_SIZE);
	m_image->transform->SetPos(WIDTH / 2.0f - ITEM_SIZE / 2.0f, 30.0f, 0.0f);

	// 価格アイコン
	m_priceIcon = new SingleComponent<CPolygon>();
	m_priceIcon->Init();
	m_priceIcon->SetTexture("data\\TEXTURE\\SHOP\\point.png");
	m_priceIcon->SetParent(gameObject);
	m_priceIcon->transform->SetSize(PRICEICON_SIZE, PRICEICON_SIZE);
	m_priceIcon->transform->SetPos(50.0f, 325.0f);

	// アイテム名
	m_itemName = new SingleComponent<CText>();
	m_itemName->Init();
	m_itemName->SetParent(gameObject);
	m_itemName->transform->SetPos(WIDTH / 2.0f, 200.0f);
	m_itemName->SetFontSize(50);
	m_itemName->SetAlign(CText::CENTER);
	m_itemName->SetFont("07鉄瓶ゴシック");
	m_itemName->SetText(m_shopItem->GetName());

	// 説明
	m_description = new SingleComponent<CText>();
	m_description->Init();
	m_description->SetParent(gameObject);
	m_description->transform->SetPos(WIDTH / 2.0f, 260.0f);
	m_description->SetFontSize(26);
	m_description->SetAlign(CText::CENTER);
	m_description->SetFontColor(D3DCOLOR_RGBA(224, 224, 224, 255));
	m_description->SetOutlineColor(D3DCOLOR_RGBA(224, 224, 224, 255));
	m_description->SetFont("07鉄瓶ゴシック");
	m_description->SetText(m_shopItem->GetDescription());

	// 価格テキスト
	m_priceText = new SingleComponent<CText>();
	m_priceText->Init();
	m_priceText->SetParent(gameObject);
	m_priceText->transform->SetPos(WIDTH / 2.0f + 30.0f, 335.0f);
	m_priceText->SetFontSize(50);
	m_priceText->SetAlign(CText::CENTER);
	m_priceText->SetFont("07鉄瓶ゴシック");
	m_priceText->SetText(std::to_string(m_shopItem->GetPrice()));

	// 仕分け線
	m_splitLine = new SingleComponent<CPolygon>();
	m_splitLine->Init();
	m_splitLine->SetParent(gameObject);
	m_splitLine->transform->SetSize(WIDTH, 5.0f);
	m_splitLine->transform->SetPos(0.0f, 310.0f);

	// 縁
	m_edge = new SingleComponent<CPolygon>();
	m_edge->Init();
	m_edge->SetTexture("data\\TEXTURE\\SHOP\\buy_button_mask.png");
	m_edge->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_edge->SetParent(gameObject);
	m_edge->transform->SetSize(WIDTH, HEIGHT);

	// 購入後
	m_bought = new SingleComponent<CPolygon>();
	m_bought->Init();
	m_bought->SetTexture("data\\TEXTURE\\SHOP\\buy_button_mask.png");
	m_bought->SetParent(gameObject);
	m_bought->transform->SetSize(WIDTH - EDGE_BOLD, HEIGHT - EDGE_BOLD);
	m_bought->transform->SetPos(EDGE_BOLD/2.0f, EDGE_BOLD / 2.0f);

	// 購入前
	m_inside = new SingleComponent<CPolygon>();
	m_inside->Init();
	m_inside->SetTexture("data\\TEXTURE\\SHOP\\buy_button_mask.png");
	m_inside->SetParent(gameObject);
	m_inside->transform->SetSize(WIDTH - EDGE_BOLD, HEIGHT - EDGE_BOLD);
	m_inside->transform->SetPos(EDGE_BOLD / 2.0f, EDGE_BOLD / 2.0f);

	switch (m_shopItem->GetType())
	{
	case ShopItem::TYPE::ITEM:
		m_bought->SetColor(ITEM_COLOR_BOUGHT);
		m_inside->SetColor(ITEM_COLOR);
		break;
	case ShopItem::TYPE::PERK:
		m_bought->SetColor(PERK_COLOR_BOUGHT);
		m_inside->SetColor(PERK_COLOR);
		break;
	}

	// マスク
	m_mask = new SingleComponent<CPolygon>();
	m_mask->Init();
	m_mask->SetParent(gameObject);
	m_mask->transform->SetSize(WIDTH, HEIGHT);
}

//=============================================================
// [BuyButtonUI] 終了
//=============================================================
void BuyButtonUI::Uninit()
{
	m_image->Uninit();
	delete m_image;

	m_priceIcon->Uninit();
	delete m_priceIcon;

	m_itemName->Uninit();
	delete m_itemName;

	m_description->Uninit();
	delete m_description;

	m_priceText->Uninit();
	delete m_priceText;

	m_splitLine->Uninit();
	delete m_splitLine;

	m_edge->Uninit();
	delete m_edge;

	m_bought->Uninit();
	delete m_bought;

	m_inside->Uninit();
	delete m_inside;

	m_mask->Uninit();
	delete m_mask;
}

//=============================================================
// [BuyButtonUI] 更新
//=============================================================
void BuyButtonUI::Update()
{
	// カーソルの位置を取得する
	auto cursorPos = CManager::GetInstance()->GetCursorClientPos();

	// クリックされているかを判別する
	D3DXVECTOR3 wPos = transform->GetWPos();
	bool isClicked = false;
	if (wPos.x <= cursorPos.x && cursorPos.x <= wPos.x + WIDTH &&
		wPos.y <= cursorPos.y && cursorPos.y <= wPos.y + HEIGHT)
	{ // ボタンの範囲内
		if (INPUT_INSTANCE->onPress("lclick") && 
			m_shopItem->GetShopManager()->GetPoints() >= m_shopItem->GetPrice())
		{ // 左クリックかつポイントが足りているとき
			isClicked = true;
		}
	}

	// 購入進行度
	m_progress += isClicked ? 0.03f : -0.05f;
	if (m_progress > 1.0f)
	{ // 購入確定
		m_progress = 0.0f;
		m_shopItem->onBuy();
		gameObject->GetComponent<AudioSource>()->PlayOneShot(buySE);
	}
	else if (m_progress < 0.0f)
	{
		m_progress = 0.0f;
	}

	// 進行度に応じてポリゴンを動かす
	m_mask->transform->SetPos(0.0f, -HEIGHT * m_progress, 0.0f);


	// ポリゴンの更新
	m_image->Update();
	m_priceIcon->Update();
	m_itemName->Update();
	m_description->Update();
	m_priceText->Update();
	m_splitLine->Update();
	m_edge->Update();
	m_bought->Update();
	m_inside->Update();
	m_mask->Update();
}

//=============================================================
// [BuyButtonUI] 描画
//=============================================================
void BuyButtonUI::DrawUI()
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

	m_edge->DrawUI();
	m_bought->DrawUI();

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
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x02);

	m_mask->DrawUI();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x02);

	m_inside->DrawUI();

	// もとに戻す
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, dwCurZTest);
	pDevice->SetRenderState(D3DRS_ZFUNC, dwCurZFunc);

	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x01);

	// 通常描画
	m_image->DrawUI();
	m_itemName->DrawUI();
	m_description->DrawUI();
	m_priceIcon->DrawUI();
	m_priceText->DrawUI();
	m_splitLine->DrawUI();
}