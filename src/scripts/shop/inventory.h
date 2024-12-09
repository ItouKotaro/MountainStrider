//=============================================================
//
// ƒCƒ“ƒxƒ“ƒgƒŠ [inventory.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _INVENTORY_UI_H_
#define _INVENTORY_UI_H_

#include "component.h"
#include "component/2d/polygon.h"
#include "component/other/button.h"

class InventoryUI : public Component
{
public:
	InventoryUI() : m_headIdx(0){}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

private:
	int m_headIdx;

	SingleComponent<CPolygon>* m_bg;
	SingleComponent<CPolygon>* m_itemFrame[4];
	SingleComponent<CPolygon>* m_itemBG[4];
	SingleComponent<CPolygon>* m_itemTexture[4];
	SingleComponent<ButtonUI>* m_backArrow;
	SingleComponent<ButtonUI>* m_nextArrow;
};

#endif // !_INVENTORY_UI_H_
