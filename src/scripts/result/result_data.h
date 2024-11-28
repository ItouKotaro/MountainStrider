//=============================================================
//
// �f�[�^�\�� [result_data.h]
// Author: Ito Kotaro
// 
// �N���A�^�C���A�ō����x�A�A�N�V����
// 
//=============================================================
#ifndef _RESULT_DATA_H_
#define _RESULT_DATA_H_

#include "component.h"

// �f�[�^�\���Ǘ��R���|�[�l���g
class ResultDataView : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	void SetTime(const int& time);
	void SetHighSpeed(const int& kmh);
	void SetAction(const int& score);

	static const D3DXVECTOR2 TITLE_BG_SIZE;
	static const float SPACE_SIZE;
private:
	GameObject* m_timeTitleBG;
	GameObject* m_timeTitleText;
	GameObject* m_timeDataBG;
	GameObject* m_timeDataText;

	GameObject* m_speedTitleBG;
	GameObject* m_speedTitleText;
	GameObject* m_speedDataBG;
	GameObject* m_speedDataText;

	GameObject* m_actionTitleBG;
	GameObject* m_actionTitleText;
	GameObject* m_actionDataBG;
	GameObject* m_actionDataText;
};



#endif // !_RESULT_DATA_H_
