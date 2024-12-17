//=============================================================
//
// 環境タイプ [environmental_type.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ENVIRONMENTAL_TYPE_H_
#define _ENVIRONMENTAL_TYPE_H_

// 天候（基底）
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

	// 影響データ
	struct AffectData
	{
		float friction;		// 摩擦
	};
	AffectData GetAffectData() { return m_affectData; }
protected:
	AffectData m_affectData;	// 影響データ
};


// 晴れ
class WeatherClear : public Weather
{
public:
	void Init() override { m_affectData.friction = 1.0f; }
};

// 霧
class WeatherFog : public Weather
{
public:
	void Init() override;
	void Uninit() override;
private:
	float m_fogDensity;
};

// 雨
//class WeatherRain : public Weather
//{
//public:
//	void Init() override;
//};


#endif // !_ENVIRONMENTAL_TYPE_H_
