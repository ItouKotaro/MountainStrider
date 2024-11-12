//=============================================================
//
// ゲームマネージャー [game_manager.h]
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
	GameObject* m_pStatusUI;	// ステータスUI

	int m_nFuel;				// 燃料
	int m_nEndurance;		// 耐久値
};

#endif // !_GAME_MANAGER_H_
