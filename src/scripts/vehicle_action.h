//=============================================================
//
// バイクアクション [vehicle_action.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_ACTION_H_
#define _VEHICLE_ACTION_H_

#include "component.h"
#include "component/2d/text.h"

// バイクアクション
class VehicleAction : public Component
{
public:
	void Init() override;
	void Update() override;

	void onFlyBegin();
	void onFlyEnd();
private:
	float m_rolling;
	float m_beforeRot;
	bool m_isMeasure;
	int m_rollCount;

	
};

// バイク回転ポイント表示
class ActionPointText : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	void SetText(const std::string& text) { m_text->SetText(text); }
	void SetUp(const float& upValue) { m_upValue = upValue; }
	void SetFade(const float& fade) { m_fade = fade; }
	void SetCounter(const int& count) { m_counter = count; }
private:
	SingleComponent<CText>* m_text;	// ポリゴン
	int m_counter;											// フェードカウンター
	float m_fade;												// フェード
	float m_upValue;										// 上昇値
};

// 合計ポイントテキスト表示
class SumActionPointText : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

private:
	SingleComponent<CText>* m_text;
};

#endif // !_VEHICLE_ACTION_H_
