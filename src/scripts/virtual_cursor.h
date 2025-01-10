//=============================================================
//
// 仮想カーソル [virtual_cursor.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _VIRTUAL_CURSOR_H_
#define _VIRTUAL_CURSOR_H_

#include "component.h"

// 仮想カーソル
class VirtualCursor : public Component
{
public:
	void Uninit() override;
	void Update() override;
private:
	bool m_isClick;
};

#endif // !_VIRTUAL_CURSOR_H_