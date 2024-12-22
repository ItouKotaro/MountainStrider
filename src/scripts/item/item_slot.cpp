//=============================================================
//
// アイテムスロット [item_slot.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "item_slot.h"

//=============================================================
// [ItemSlot] 初期化
//=============================================================
void ItemSlot::Init()
{
	// アイテムフレーム
	for (int i = 0; i < 2; i++)
	{
		m_itemFrame[i] = new SingleComponent<CPolygon>();
		m_itemFrame[i]->SetParent(gameObject);
		m_itemFrame[i]->Init();
		m_itemFrame[i]->SetTexture("data\\TEXTURE\\INVENTORY\\frame.png");
		m_itemFrame[i]->transform->SetSize(160.0f, 160.0f);
		m_itemFrame[i]->transform->SetPos(170.0f * i, 0.0f);
		m_itemFrame[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		m_itemBG[i] = new SingleComponent<CPolygon>();
		m_itemBG[i]->SetParent(gameObject);
		m_itemBG[i]->Init();
		m_itemBG[i]->transform->SetSize(120.0f, 120.0f);
		m_itemBG[i]->transform->SetPos(170.0f * i + 20.0f, 20.0f);
		m_itemBG[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 50));

		m_itemTexture[i] = new SingleComponent<CPolygon>();
		m_itemTexture[i]->SetParent(gameObject);
		m_itemTexture[i]->Init();
		m_itemTexture[i]->SetTexture("data\\TEXTURE\\ITEM\\fuel_tank.png");
		m_itemTexture[i]->transform->SetSize(120.0f, 120.0f);
		m_itemTexture[i]->transform->SetPos(170.0f * i + 20.0f, 20.0f);
		m_itemTexture[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}

//=============================================================
// [ItemSlot] 終了
//=============================================================
void ItemSlot::Uninit()
{
	for (int i = 0; i < 2; i++)
	{
		m_itemFrame[i]->Uninit();
		delete m_itemFrame[i];

		m_itemBG[i]->Uninit();
		delete m_itemBG[i];

		m_itemTexture[i]->Uninit();
		delete m_itemTexture[i];
	}
}

//=============================================================
// [ItemSlot] 更新
//=============================================================
void ItemSlot::Update()
{
	for (int i = 0; i < 2; i++)
	{
		m_itemFrame[i]->Update();
		m_itemBG[i]->Update();
		m_itemTexture[i]->Update();
	}
}

//=============================================================
// [ItemSlot] 描画
//=============================================================
void ItemSlot::DrawUI()
{
	for (int i = 0; i < 2; i++)
	{
		m_itemFrame[i]->DrawUI();
		m_itemBG[i]->DrawUI();
		m_itemTexture[i]->DrawUI();
	}
}