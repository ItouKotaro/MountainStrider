//=============================================================
//
// アイテムスロット [item_slot.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ITEM_SLOT_H_
#define _ITEM_SLOT_H_

#include "component.h"
#include "component/2d/polygon.h"
class ItemSlot : public Component
{
public:
	ItemSlot() : m_selectIdx(0), m_scroll(0){}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;
private:
	int m_scroll;
	int m_selectIdx;
	SingleComponent<CPolygon>* m_itemFrame[2];
	SingleComponent<CPolygon>* m_itemBG[2];
	SingleComponent<CPolygon>* m_itemTexture[2];
};

#endif // !_ITEM_SLOT_H_
