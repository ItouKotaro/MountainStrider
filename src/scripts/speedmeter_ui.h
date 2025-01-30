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
	void UpdateMeter();

	SingleComponent<CText>* m_speedText;
	SingleComponent<CPolygon>* m_speedBG;
	SingleComponent<CPolygon>* m_meterBG;
	SingleComponent<CPolygon>* m_needle;
	
	Transform* m_meter;
	float m_showAngle;

	CVehicle* m_vehicle;

	// 定数
	const int BAR_NUM = 15;
	const float BAR_WIDTH = 20.0f;
	const float BAR_SPACE = 10.0f;
	const float BAR_TOP = 100.0f;
	const float BAR_RIGHT = 10.0f;
};

#endif // !_SPEEDMETER_UI_H_
