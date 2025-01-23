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
#include "component/2d/bar.h"

// 合計ポイントテキスト表示
class SumActionPointText : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	// ポイント加算
	void AddPoints(const int& points);

	// ポイント加算終了
	void Reset() 
	{ 
		m_points = 0;
		m_viewPoints = 0;
		m_text->SetAlpha(0.0f);
	}
private:
	SingleComponent<CText>* m_text;
	SingleComponent<CPolygon>* m_bg;
	int m_points;
	int m_viewPoints;
	float m_textScale;
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
	int m_counter;									// フェードカウンター
	float m_fade;										// フェード
	float m_upValue;								// 上昇値
};


// バイクアクション
class VehicleAction : public Component
{
public:
	void Init() override;
	void Update() override;

	void onFlyBegin();
	void onFlyEnd();
private:
	float m_rolling;			// 合計回転
	float m_beforeRot;		// 前回の回転
	bool m_isMeasure;		// 計測
	int m_rollCount;			// 回転カウンター

	SumActionPointText* m_sumPoints;
	GameObject* m_actionSound;
};

#endif // !_VEHICLE_ACTION_H_
