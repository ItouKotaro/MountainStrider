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
	void OnResultEvent() override;

	// ���U���g�f�[�^���擾����
	std::vector<ResultBase::ResultData>& GetResultData() { return m_resultDatas; }
private:
	D3DXVECTOR3 CalcNearGoal();
	GameObject* m_directionObj;
	std::vector<ResultBase::ResultData> m_resultDatas;
	const float EXTENSION_DISTANCE = 150.0f;
};

#endif // !_TERMINAL_MODE_H_
