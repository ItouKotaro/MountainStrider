//=============================================================
//
// ���^�C�v [environmental_type.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ENVIRONMENTAL_TYPE_H_
#define _ENVIRONMENTAL_TYPE_H_

#include "component/other/audio.h"

// �V��i���j
class Weather
{
public:
	Weather()
	{
		m_affectData.friction = 1.0f;
	}
	virtual ~Weather() {}

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}

	// �e���f�[�^
	struct AffectData
	{
		float friction;		// ���C
	};
	AffectData GetAffectData() { return m_affectData; }
protected:
	AffectData m_affectData;	// �e���f�[�^
};


// ����
class WeatherClear : public Weather
{
public:
	void Init() override { m_affectData.friction = 1.0f; }
};

// �J
class WeatherRain : public Weather
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	GameObject* m_rainParticle;
	AudioClip m_rainSE;
};

//// ��
//class WeatherFog : public Weather
//{
//public:
//	void Init() override;
//	void Uninit() override;
//private:
//	float m_fogDensity;
//};

// �J
//class WeatherRain : public Weather
//{
//public:
//	void Init() override;
//};


#endif // !_ENVIRONMENTAL_TYPE_H_
