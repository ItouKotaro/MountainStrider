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
	void Update() override;
	void OnTriggerExit(GameObject* other) override;
private:
	static AudioClip m_throughSE;

	const int POINT = 10;
	const float ROLL_ANGLE = 0.02f;
};

#endif // !_RING_H_
