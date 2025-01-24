//=============================================================
//
// �� [environmental.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ENVIRONMENTAL_H_
#define _ENVIRONMENTAL_H_

#include "environmental_type.h"
#include "component/3d/camera.h"

// ������
class EnvironmentalEffect
{
public:
	EnvironmentalEffect() : m_time(TIME::MORNING), m_weather(nullptr) {}
	void Init(const std::string path);
	void Uninit();
	void Update();
	CCamera* GetLightCamera() { return m_light->GetComponent<CCamera>(); }

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
	// ���C�g�ʒu�̍X�V
	void UpdateLight();

	TIME m_time;				// ����
	Weather* m_weather;	// �V��
	GameObject* m_light;	// ���C�g�i�e�p�j
	GameObject* m_vehicle;// �o�C�N�i�ǐ՗p�j

	AudioClip m_bgmClip;
	GameObject* m_bgm;	// BGM
};

#endif // !_ENVIRONMENTAL_H_
