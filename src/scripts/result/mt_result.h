//=============================================================
//
// ���U���g [mt_result.h]
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
		P_MTTEXT,					// ���j���e�L�X�g
		P_DATA,						// �f�[�^�̕\��
		P_VEHICLE,					// �o�C�N�̏��\��
		P_END
	};
	PROG_STATE m_progState;	// ���ʕ\���i�s�x
	int m_progCounter;				// �J�E���^�[

	GameObject* m_mtText;		// �}�E���e���e�L�X�g�i�R�̓��j���j

	GameObject* m_bg;				// �w�i�i�K�E�X�ɂ���\��j
	GameObject* m_seedText;		// �V�[�h�e�L�X�g
	GameObject* m_fuelBar;			// �R���o�[
	GameObject* m_enduranceBar;	// �ϋv�l�o�[
	GameObject* m_dataView;			// �f�[�^�\��
	GameObject* m_terrainImg;

	CGameScene* m_gameScene;




	static float m_oldFuel;			// �O��̔R��
	static float m_oldEndurance;	// �O��̑ϋv�l
	static UINT m_goalCount;		// ���j��
};

#endif // !_RESULT_H_
