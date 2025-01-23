//=============================================================
//
// �o�C�N�A�N�V���� [vehicle_action.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_ACTION_H_
#define _VEHICLE_ACTION_H_

#include "component.h"
#include "component/2d/text.h"
#include "component/2d/bar.h"

// ���v�|�C���g�e�L�X�g�\��
class SumActionPointText : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	// �|�C���g���Z
	void AddPoints(const int& points);

	// �|�C���g���Z�I��
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

// �o�C�N��]�|�C���g�\��
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
	SingleComponent<CText>* m_text;	// �|���S��
	int m_counter;									// �t�F�[�h�J�E���^�[
	float m_fade;										// �t�F�[�h
	float m_upValue;								// �㏸�l
};


// �o�C�N�A�N�V����
class VehicleAction : public Component
{
public:
	void Init() override;
	void Update() override;

	void onFlyBegin();
	void onFlyEnd();
private:
	float m_rolling;			// ���v��]
	float m_beforeRot;		// �O��̉�]
	bool m_isMeasure;		// �v��
	int m_rollCount;			// ��]�J�E���^�[

	SumActionPointText* m_sumPoints;
	GameObject* m_actionSound;
};

#endif // !_VEHICLE_ACTION_H_
