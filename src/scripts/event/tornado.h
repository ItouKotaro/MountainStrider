//=============================================================
//
// �����C�x���g [tornado.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TORNADO_EVENT_H_
#define _TORNADO_EVENT_H_

#include "event_manager.h"

// �����C�x���g
class TornadoEvent : public EventTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

private:
	void UpdateMove();
	void ResetMoveAngle();
	void UpdateAction();

	GameObject* m_tornade;
	
	float m_maxHeight, m_minHeight;

	float m_moveAngle;		// ����������
	float m_moveTimer;		// �ړ�����
	float m_eventTimer;

	const float MOVE_ANGLE_RANGE = D3DX_PI * 0.5f;
	const float MOVE_BASE_TIME = 1.0f;
	const float MOVE_RANDOM_TIME = 3.0f;
	const float TORNADO_SPEED = 2.0f;
	const float TORNADO_RANGE = 100.0f;
	const float TORNADO_HEIGHT = 150.0f;
	const float GENERATE_MIN_DISTANCE = 200.0f;
	const float GENERATE_RADIUS = 600.0f;

	const float EVENT_BASE_TIME = 50.0f;
	const float EVENT_RANDOM_TIME = 20.0f;

	static AudioClip m_tornadeSE;
};

#endif // !_TORNADO_EVENT_H_
