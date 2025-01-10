//=============================================================
//
// ���z�J�[�\�� [virtual_cursor.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _VIRTUAL_CURSOR_H_
#define _VIRTUAL_CURSOR_H_

#include "component.h"

// ���z�J�[�\��
class VirtualCursor : public Component
{
public:
	void Uninit() override;
	void Update() override;
private:
	bool m_isClick;
};

#endif // !_VIRTUAL_CURSOR_H_