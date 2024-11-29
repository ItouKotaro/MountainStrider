//=============================================================
//
// �X�e�[�^�XUI [status_ui.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _STATUS_UI_H_
#define _STATUS_UI_H_

#include "component.h"
#include "component/2d/bar.h"

class CStatusUI : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override {};

	void SetFuel(const float& percent);
	void SetEndurance(const float& percent);
	void SetVisible(const bool& visible);
private:
	CAdvancedBar* m_pFuelBar;			// �R���o�[
	CAdvancedBar* m_pEnduranceBar;	// �ϋv�l�o�[
};

#endif // !_STATUS_UI_H_
