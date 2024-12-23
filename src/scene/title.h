//=============================================================
//
// �^�C�g�� [title.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"

// �^�C�g���V�[��
class CTitleScene : public CScene
{
public:
	void Init() override;						// ������
	void Uninit() override;						// �I��
	void Update() override;					// �X�V
	void Draw() override;						// �`��

private:
	GameObject* m_titleScene;
};

#endif // !_TITLE_H_
