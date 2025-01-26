//=============================================================
//
// ショップアイテム [shop_items.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "shop_items.h"

#include "scene/game.h"
#include "scripts/vehicle.h"

#include "scripts/item/items.h"
#include "scripts/item/item_manager.h"

float ShopItem::m_saleDiscount = 0.0f;
AudioClip ShopItem::m_buySE = nullptr;

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
	// 音
	if (m_buySE == nullptr) m_buySE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\SHOP\\buy.mp3", FMOD_2D);
	if (m_sePlayer == nullptr)
	{
		m_sePlayer = new GameObject();
		m_sePlayer->AddComponent<AudioSource>();
	}
	m_sePlayer->GetComponent<AudioSource>()->PlayOneShot(m_buySE);

	return m_shopManager->PayPoint(this->GetPrice());
}

//=============================================================
// [FuelItem] 購入時
//=============================================================
bool FuelItem::onBuy()
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
			return true;
		}
	}
	return false;
}

//=============================================================
// [EnduranceItem] 購入時
//=============================================================
bool EnduranceItem::onBuy()
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
			return true;
		}
	}
	return false;
}

//=============================================================
// [ShopFuelTank] 購入時
//=============================================================
bool ShopFuelTank::onBuy()
{
	if (this->Pay())
	{ // 支払えた時
		// 燃料タンクをインベントリに追加
		ItemManager::GetInstance()->AddItem(new FuelTankItem());
		return true;
	}
	return false;
}

//=============================================================
// [ShopToolBox] 購入時
//=============================================================
bool ShopToolBox::onBuy()
{
	if (this->Pay())
	{ // 支払えた時
		// 工具箱をインベントリに追加
		ItemManager::GetInstance()->AddItem(new ToolBoxItem());
		return true;
	}
	return false;
}

//=============================================================
// [ShopExplosionBoost] 購入時
//=============================================================
bool ShopExplosionBoost::onBuy()
{
	if (this->Pay())
	{ // 支払えた時
		// 爆発ブーストをインベントリに追加
		ItemManager::GetInstance()->AddItem(new ExplosionBoostItem());
		return true;
	}
	return false;
}

//=============================================================
// [ShopTeruTeruBouzu] 購入時
//=============================================================
bool ShopTeruTeruBouzu::onBuy()
{
	if (this->Pay())
	{ // 支払えた時
		// てるてる坊主をインベントリに追加
		ItemManager::GetInstance()->AddItem(new TeruTezuBouzuItem());
		return true;
	}
	return false;
}

//=============================================================
// [ShopMyRoad] 購入時
//=============================================================
bool ShopMyRoad::onBuy()
{
	if (this->Pay())
	{ // 支払えた時
		// マイロードをインベントリに追加
		ItemManager::GetInstance()->AddItem(new MyRoadItem());
		return true;
	}
	return false;
}