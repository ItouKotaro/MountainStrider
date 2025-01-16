//=============================================================
//
// ”j‰ó‰Â”\ [destructible.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _DESTRUCTIBLE_H_
#define _DESTRUCTIBLE_H_

#include "component.h"
#include "decoration.h"

// ”j‰ó‰Â”\
class Destructible : public Component
{
public:
	Destructible(DecorationManager* decoManager) : m_startTimer(0.0f), m_fade(1.0f), m_isHit(false){ m_decoManager = decoManager; }
	void Init() override;
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;
	void SetDecoData(DecorationManager::DecorationData* data) { m_decoData = data; }
	void ForceHit(bool playSound = true);

	static const float FADE_VALUE;
	static const float START_TIME;
private:
	float m_startTimer;
	float m_fade;
	bool m_isHit;

	DecorationManager* m_decoManager;
	DecorationManager::DecorationData* m_decoData;
};

#endif // !_DESTRUCTIBLE_H_
