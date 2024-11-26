//=============================================================
//
// ���U���g [result.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _MT_RESULT_H_
#define _MT_RESULT_H_

#include "gameobject.h"

class CGameScene;
class MountainResultManager
{
public:
	MountainResultManager(CGameScene* scene) { m_gameScene = scene; }
	void Init();
	void Uninit();
	void Update();
private:
	enum PROG_STATE
	{
		P_MT_TEXT,					// ���j���e�L�X�g
		P_FUEL_TEXT,				// �R�������e�L�X�g
		P_FUEL_BAR,					// �R���o�[����
		P_ENDURANCE_TEXT,	// �ϋv�l�����e�L�X�g
		P_ENDURANCE_BAR,		// �ϋv�l�o�[����
		P_END
	};
	PROG_STATE m_progState;
	int m_progCounter;

	GameObject* m_mtText;		// �}�E���e���e�L�X�g�i�R�̓��j���j

	GameObject* m_bg;				// �w�i�i�K�E�X�ɂ���\��j
	GameObject* m_seedText;		// �V�[�h�e�L�X�g
	GameObject* m_fuelBar;			// �R���o�[
	GameObject* m_enduranceBar;	// �ϋv�l�o�[

	CGameScene* m_gameScene;




	static float m_oldFuel;			// �O��̔R��
	static float m_oldEndurance;	// �O��̑ϋv�l
	static UINT m_goalCount;		// ���j��
};

#endif // !_RESULT_H_
