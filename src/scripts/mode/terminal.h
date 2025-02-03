//=============================================================
//
// �I�[���[�h [terminal.h]
// Author: Ito Kotaro
// 
// ���[���h�̒[�܂œ��B����ƃS�[������
//
//=============================================================
#ifndef _TERMINAL_MODE_H_
#define _TERMINAL_MODE_H_

#include "mode_manager.h"

// �I�[���[�h
class TerminalMode : public ModeTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	const float EXTENSION_DISTANCE = 150.0f;
};

#endif // !_TERMINAL_MODE_H_
