//=============================================================
//
// ���U���g�r���[ [result_view.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _RESULT_VIEW_H_
#define _RESULT_VIEW_H_

#include "component.h"
#include "component/2d/bar.h"
#include "component/2d/text.h"

class ResultViewBar : public Component
{
public:
	ResultViewBar(const std::string& title, const D3DXCOLOR& titleColor, const D3DXCOLOR& barColor, const D3DXCOLOR& percentColor);
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	// �l�̐ݒ�
	void SetValue(const float& before, const float& objective);	
	// �A�j���[�V���������s
	void StartAnim() 
	{
		m_isStartAnim = true;
		m_animCounter = 0;
	}


	static const int ANIM_FRAME;
private:
	bool m_isStartAnim;
	int m_animCounter;

	// ���
	std::string m_titleName;
	D3DXCOLOR m_titleColor;
	D3DXCOLOR m_barColor;
	D3DXCOLOR m_percentColor;
	float m_viewBefore;
	float m_viewObjective;
	float m_viewProgress;

	// �\���I�u�W�F�N�g
	SingleComponent<CText>* m_viewText;
	SingleComponent<CAdvancedBar>* m_viewBar;
	SingleComponent<CText>* m_viewValue;
	SingleComponent<CText>* m_viewCompare;
};

#endif // !_RESULT_VIEW_H_