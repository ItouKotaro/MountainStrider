//=============================================================
//
// ステータスUI [status_ui.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _STATUS_UI_H_
#define _STATUS_UI_H_

#include "component.h"
#include "component/2d/bar.h"

class CStatusUI : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	void SetFuel(const float& percent);
	void SetEndurance(const float& percent);
	void SetVisible(const bool& visible);
private:
	CAdvancedBar* m_pFuelBar;									// 燃料バー
	CAdvancedBar* m_pEnduranceBar;							// 耐久値バー
	SingleComponent<CPolygon>* m_fuelIcon;				// 燃料アイコン
	SingleComponent<CPolygon>* m_enduranceIcon;	// 耐久値アイコン
};

#endif // !_STATUS_UI_H_
