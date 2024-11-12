//=============================================================
//
// �Q�[���}�l�[�W���[ [game_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "component.h"

class CGameManager : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	static const int MAX_FUEL;
	static const int MAX_ENDURANCE;
private:
	GameObject* m_pStatusUI;	// �X�e�[�^�XUI

	int m_nFuel;				// �R��
	int m_nEndurance;		// �ϋv�l
};

#endif // !_GAME_MANAGER_H_
