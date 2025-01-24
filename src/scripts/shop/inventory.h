//=============================================================
//
// インベントリ [inventory.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _INVENTORY_UI_H_
#define _INVENTORY_UI_H_

#include "component.h"
#include "component/2d/polygon.h"
#include "component/other/button.h"
#include "component/2d/text.h"

class Item;
class InventoryUI : public Component
{
public:
	InventoryUI() : m_headIdx(0), m_infoItem(nullptr){}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

private:
	int m_headIdx;

	SingleComponent<CPolygon>* m_itemFrame[4];
	SingleComponent<CPolygon>* m_itemBG[4];
	SingleComponent<CPolygon>* m_itemTexture[4];
	SingleComponent<ButtonUI>* m_backArrow;
	SingleComponent<ButtonUI>* m_nextArrow;

	// 音
	SingleComponent<AudioSource>* m_sePlayer;
	AudioClip m_clickSE;

	// 情報パネル
	Item* m_infoItem;
	SingleComponent<CPolygon>* m_infoEdge;
	SingleComponent<CPolygon>* m_infoBG;
	SingleComponent<CText>* m_infoName;
	SingleComponent<CText>* m_infoDescription;
};

#endif // !_INVENTORY_UI_H_
