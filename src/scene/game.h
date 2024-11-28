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

	void onGameOver();			// �Q�[���I�[�o�[����
	Terrain* GetTerrain() { return m_pTerrain; }
private:
	void SpawnBike();				// �o�C�N�̐���

	bool m_isGameOvered;									// �Q�[���I�[�o�[�ς݂�
	MountainResultManager* m_resultManager;		// �}�E���e�����U���g�}�l�[�W���[

	Terrain* m_pTerrain;				// �n�`
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
