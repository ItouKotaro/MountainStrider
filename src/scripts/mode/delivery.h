//=============================================================
//
// �f���o���[���[�h [delivery.h]
// Author: Ito Kotaro
// 
// �z�B���[�h
//
//=============================================================
#ifndef _DELIVERY_MODE_H_
#define _DELIVERY_MODE_H_

#include "mode_manager.h"

// �z�B���[�h
class DeliveryMode : public ModeTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
};

#endif // !_DELIVERY_MODE_H_
