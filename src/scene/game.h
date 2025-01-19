//=============================================================
//
// �Q�[���V�[�� [game.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "internal/physics.h"
#include "scripts/terrain.h"
#include "scripts/environmental.h"
#include "scripts/decoration.h"
#include "scripts/result/mt_result.h"
#include "scripts/pause.h"
#include "scripts/event/event_manager.h"
#include "scripts/lake.h"

// �Q�[���V�[��
class CGameScene : public CScene
{
public:
	CGameScene(){}
	void Init() override;			// ������
	void Uninit() override;			// �I��
	void Update() override;		// �X�V
	void Draw() override;			// �`��
	void LastUninit() override;	// �ŏI�I��

	static void ResetGame();				// �Q�[���̃��Z�b�g

	void onGameOver();			// �Q�[���I�[�o�[����
	void onClear();					// �N���A����
	Terrain* GetTerrain() { return m_pTerrain; }
	GameObject* GetBike() { return m_pBike; }

	void AddScore(const int& score) { m_score += score; }
	Pause* GetPause() { return m_pause; }

	// ���s�f�[�^
	struct TravellingData
	{
		D3DXVECTOR3 pos;
		D3DXQUATERNION rot;
	};
	std::vector<TravellingData> GetTravellingData() { return m_travellingDatas; }

	// �I���̎��
	enum ENDTYPE
	{
		ENDTYPE_NONE,											// �Ȃ��i�ʏ�j
		ENDTYPE_CLEAR,											// �N���A�i�R���j�j
		ENDTYPE_GAMEOVER,									// �Q�[���I�[�o�[
	};
	ENDTYPE m_endType;										// �I�����R
private:
	std::vector<std::string> GetTerrainFiles();			// �n�`�t�@�C���ꗗ���擾����
	void SpawnBike();												// �o�C�N�̐���
	void ClearCondition();											// �N���A����

	int m_travellingCount;										// �˔j��
	std::vector<TravellingData> m_travellingDatas;	// ���s�f�[�^
	ResultBase* m_result;										// ���U���g

	Terrain* m_pTerrain;											// �n�`
	EnvironmentalEffect* m_environmental;				// ��
	DecorationManager* m_decoration;						// ����
	EventManager* m_events;									// �C�x���g
	LakeManager* m_lake;										// ��

	GameObject* m_voidField;									// �ޗ�
	GameObject* m_pCamera;									// �J����
	GameObject* m_pBike;										// �o�C�N
	GameObject* m_pStatusUI;								// �X�e�[�^�XUI
	GameObject* m_pItemSlot;								// �A�C�e���X���b�g

	// ���U���g�f�[�^�p
	DWORD m_startTime;										// �J�n����
	int m_highSpeed;												// �ō����x
	int m_score;														// �X�R�A

	// �|�[�Y
	Pause* m_pause;
};

#endif // !_GAME_H_
