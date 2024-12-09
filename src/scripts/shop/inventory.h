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

class InventoryUI : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;
private:
	SingleComponent<CPolygon>* m_bg;
};

#endif // !_INVENTORY_UI_H_
