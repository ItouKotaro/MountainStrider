//=============================================================
//
// �� [environmental.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ENVIRONMENTAL_H_
#define _ENVIRONMENTAL_H_

#include "environmental_type.h"

// ������
class EnvironmentalEffect
{
public:
	EnvironmentalEffect() : m_time(TIME::MORNING), m_weather(nullptr) {}
	void Init();
	void Uninit();
	void Update();

	// ���Ԃ̎��
	enum TIME
	{
		MORNING,		// ��
		DAYTIME,		// ��
		NIGHT,			// ��
		MAX
	};

	// ���Ԑݒ�
	void SetTime(const TIME& time);

	// �V��ݒ�
	void SetWeather(Weather* weather);
private:
	TIME m_time;				// ����
	Weather* m_weather;	// �V��
};

#endif // !_ENVIRONMENTAL_H_
