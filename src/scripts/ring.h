//=============================================================
//
// ƒŠƒ“ƒO [ring.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RING_H_
#define _RING_H_

#include "component.h"

// ƒŠƒ“ƒO
class PointRing : public Component
{
public:
	PointRing() : m_isUsed(false){}
	void Update() override;
	void OnTriggerExit(GameObject* other) override;
private:
	static AudioClip m_throughSE;

	bool m_isUsed;
	const int POINT = 5;
	const float ROLL_ANGLE = 0.01f;
};

#endif // !_RING_H_
