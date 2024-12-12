//=============================================================
//
// �}�E���e���̃f�o�b�O�V�[�� [mt_debug.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _MOUNTAIN_DEBUG_H_
#define _MOUNTAIN_DEBUG_H_

#include "scene.h"

// �}�E���e���f�o�b�O�V�[��
class CMountainDebug : public CScene
{
public:
	void Init() override;			// ������
	void Uninit() override;			// �I��
	void Update() override;		// �X�V
	void Draw() override;			// �`��
private:
	GameObject* m_pCamera;
	GameObject* m_pTerrain;
	GameObject* m_pRollTest;
	GameObject* m_pText;
};

#endif // !_MOUNTAIN_DEBUG_H_
