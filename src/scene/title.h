//=============================================================
//
// �^�C�g�� [title.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"
#include "internal/launch_credit.h"

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
	LaunchCredit* m_credit;
};

#endif // !_TITLE_H_
