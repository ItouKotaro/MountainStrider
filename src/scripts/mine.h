//=============================================================
//
// ’n—‹ [mine.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _LAND_MINE_H_
#define _LAND_MINE_H_

#include "component.h"

// ’n—‹
class LandMine : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;
private:
	GameObject* m_particle;
	const float DAMAGE = 50.0f;
};

#endif // !_LAND_MINE_H_
