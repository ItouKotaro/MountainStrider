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
	GameObject* m_pCamera;	// �J����
	GameObject* m_pBike;
	GameObject* m_pField;		// �t�B�[���h
};

#endif // !_GAME_H_
