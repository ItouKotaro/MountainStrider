//=============================================================
//
// スピードメーターUI [speedmeter_ui.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _SPEEDMETER_UI_H_
#define _SPEEDMETER_UI_H_

#include "component.h"
#include "vehicle.h"

#include "component/2d/text.h"

// スピードメーターUI
class SpeedMeterUI : public Component
{
public:
	SpeedMeterUI(CVehicle* vehicle) { m_vehicle = vehicle; }
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;
private:
	SingleComponent<CText>* m_speedText;
	CVehicle* m_vehicle;
};

#endif // !_SPEEDMETER_UI_H_
