//=============================================================
//
// ショップアイテム [shop_items.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "shop_items.h"

#include "scene/game.h"
#include "scripts/vehicle.h"

float ShopItem::m_saleDiscount = 0.0f;

//=============================================================
// [ShopItem] 価格の取得
//=============================================================
int ShopItem::GetPrice()
{
	return static_cast<int>(m_price * (1.0f - m_discount) * (1.0f - m_saleDiscount));
}

//=============================================================
// [ShopItem] 支払い
//=============================================================
bool ShopItem::Pay()
{
	return m_shopManager->PayPoint(this->GetPrice());
}

//=============================================================
// [FuelItem] 購入時
//=============================================================
void FuelItem::onBuy()
{
	// ゲームシーンを取得する
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// バイクを取得する
	auto pBike = gameScene->GetBike()->GetComponent<CVehicle>();

	if (pBike->GetFuel() < CVehicle::MAX_FUEL)
	{
		if (this->Pay())
		{ // 支払えた時
			pBike->AddFuel(static_cast<float>(m_amount));
		}
	}
}

//=============================================================
// [EnduranceItem] 購入時
//=============================================================
void EnduranceItem::onBuy()
{
	// ゲームシーンを取得する
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// バイクを取得する
	auto pBike = gameScene->GetBike()->GetComponent<CVehicle>();

	if (pBike->GetEndurance() < CVehicle::MAX_ENDURANCE)
	{
		if (this->Pay())
		{ // 支払えた時
			pBike->AddEndurance(static_cast<float>(m_amount));
		}
	}
}