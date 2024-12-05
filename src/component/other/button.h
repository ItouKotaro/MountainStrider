//=============================================================
//
// ƒ{ƒ^ƒ“ [button.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "component/2d/polygon.h"

class ButtonUI : public CPolygon
{
public:
	ButtonUI() : m_onClick(nullptr){}
	virtual ~ButtonUI() {}
	virtual void Init() override { CPolygon::Init(); }
	virtual void Uninit() override { CPolygon::Uninit(); }
	virtual void Update() override;
	virtual void DrawUI() override { CPolygon::DrawUI(); }
	void setClickEvent(std::function<void(void)> onClick) { m_onClick = onClick; }
private:
	std::function<void(void)> m_onClick;
};

#endif // !_BUTTON_H_
