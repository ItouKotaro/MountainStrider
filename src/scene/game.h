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

// �Q�[���V�[��
class CGameScene : public CScene
{
public:
	void Init() override;			// ������
	void Uninit() override;			// �I��
	void Update() override;		// �X�V
	void Draw() override;			// �`��
private:
	void SpawnBike();				// �o�C�N�̐���

	Terrain* m_pTerrain;

	GameObject* m_pCamera;		// �J����
	GameObject* m_pBike;			// �o�C�N
	GameObject* m_pStatusUI;	// �X�e�[�^�XUI

	GameObject* m_pFPS;
};

#endif // !_GAME_H_
