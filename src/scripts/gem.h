//=============================================================
//
// ÉWÉFÉÄ [gem.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _GEM_H_
#define _GEM_H_

#include "component.h"
class Gem : public Component
{
public:
	void Init() override;
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;
private:
	GameObject* m_particle;
};

#endif // !_GEM_H_
