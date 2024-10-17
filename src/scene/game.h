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

// �Q�[���V�[��
class CGameScene : public CScene
{
public:
	void Init() override;			// ������
	void Uninit() override;			// �I��
	void Update() override;		// �X�V
	void Draw() override;			// �`��
private:
	void CreateBike();				// �o�C�N�𐶐�����

	GameObject* m_pCamera;	// �J����
	btHinge2Constraint* m_hinge2;
};

#endif // !_GAME_H_
