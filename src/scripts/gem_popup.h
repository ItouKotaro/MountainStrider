//=============================================================
//
// ジェムのポップアップ表示UI [gem_popup.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _GEM_POPUP_UI_H_
#define _GEM_POPUP_UI_H_

#include "component.h"
#include "component/2d/polygon.h"
#include "component/2d/text.h"

class GemPopupUI : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	static const D3DXVECTOR2 BG_SIZE;
private:
	SingleComponent<CPolygon>* m_bg;
	SingleComponent<CText>* m_text;
	float m_showTime;
	int m_oldPoint;

	const float SHOW_TIME = 3.5f;	// UI表示時間
	const float DAMPING = 0.08f;		// 減衰
};

#endif // !_GEM_POPUP_UI_H_
