//=============================================================
//
// デリバリーモード [delivery.h]
// Author: Ito Kotaro
// 
// 配達モード
//
//=============================================================
#ifndef _DELIVERY_MODE_H_
#define _DELIVERY_MODE_H_

#include "mode_manager.h"

// 配達モード
class DeliveryMode : public ModeTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
};

#endif // !_DELIVERY_MODE_H_
