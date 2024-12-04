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
#include "scripts/result/mt_result.h"

// �Q�[���V�[��
class CGameScene : public CScene
{
public:
	CGameScene(){}
	void Init() override;			// ������
	void Uninit() override;			// �I��
	void Update() override;		// �X�V
	void Draw() override;			// �`��

	void ResetGame();				// �Q�[���̃��Z�b�g

	void onGameOver();			// �Q�[���I�[�o�[����
	void onClear();					// �N���A����
	Terrain* GetTerrain() { return m_pTerrain; }
	GameObject* GetBike() { return m_pBike; }

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
		ENDTYPE_NONE,			// �Ȃ��i�ʏ�j
		ENDTYPE_CLEAR,			// �N���A�i�R���j�j
		ENDTYPE_GAMEOVER,	// �Q�[���I�[�o�[
	};
	ENDTYPE m_endType;		// �I�����R
private:
	void SpawnBike();				// �o�C�N�̐���
	void ClearCondition();			// �N���A����

	int m_travellingCount;
	std::vector<TravellingData> m_travellingDatas;	// ���s�f�[�^
	ResultBase* m_result;			// ���U���g

	Terrain* m_pTerrain;				// �n�`
	GameObject* m_voidField;		// �ޗ�
	GameObject* m_pCamera;		// �J����
	GameObject* m_pBike;			// �o�C�N
	GameObject* m_pStatusUI;	// �X�e�[�^�XUI

	// ���U���g�f�[�^�p
	DWORD m_startTime;			// �J�n����
	int m_highSpeed;					// �ō����x

	// FPS�\��
	GameObject* m_pFPS;
};

#endif // !_GAME_H_
