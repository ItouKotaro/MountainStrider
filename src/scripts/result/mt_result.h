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
	void Reset();

	// ���ʊi�[
	struct ResultData
	{
		int time;			// �N���A����
		int highSpeed;	// �ō����x
		int action;			// �A�N�V����
	};
	static void AddResult(ResultData data);
private:
	enum PROG_STATE
	{
		P_MTTEXT,					// ���j���e�L�X�g
		P_FUEL,							// �o�C�N�̔R��
		P_ENDURANCE,				// �o�C�N�̑ϋv�l
		P_END
	};
	PROG_STATE m_progState;	// ���ʕ\���i�s�x
	int m_progCounter;				// �J�E���^�[

	GameObject* m_mtText;		// �}�E���e���e�L�X�g�i�R�̓��j���j

	GameObject* m_bg;						// �w�i�i�K�E�X�ɂ���\��j
	GameObject* m_seedText;				// �V�[�h�e�L�X�g
	GameObject* m_dataView;				// �f�[�^�\��
	GameObject* m_fuelView;				// �R���\��
	GameObject* m_enduranceView;	// �ϋv�l�\��

	GameObject* m_terrainImg;

	CGameScene* m_gameScene;


	static float m_beforeFuel;			// �O��̔R��
	static float m_beforeEndurance;	// �O��̑ϋv�l
	static UINT m_goalCount;		// ���j��
	static std::vector<ResultData> m_results;	// ����
};

#endif // !_RESULT_H_
