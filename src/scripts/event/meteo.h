//=============================================================
//
// 覐΃C�x���g [meteo.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _METEO_EVENT_H_
#define _METEO_EVENT_H_

#include "event_manager.h"
#include "component.h"

// 覐΃C�x���g
class MeteoEvent : public EventTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	void Generate();

	float m_generateTimer;
	float m_eventTimer;

	const float METEO_RANGE = 600.0f;
	const float EVENT_BASE_TIME = 10.0f;
	const float EVENT_RANDOM_TIME = 10.0f;
	const float GENERATE_BASE_TIME = 2.5f;
	const float GENERATE_RANDOM_TIME = 3.0f;
};

// 覐΃R���|�[�l���g
class MeteoCollision : public Component
{
public:
	void Update();
	void HitAction();
private:
	const float RANGE = 150.0f;
	const float VEHICLE_DAMAGE = 20.0f;
};

#endif // !_METEO_EVENT_H_
