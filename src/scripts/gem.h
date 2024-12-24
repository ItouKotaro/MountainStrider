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
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;
};

#endif // !_GEM_H_
