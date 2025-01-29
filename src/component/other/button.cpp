//=============================================================
//
// ボタン [button.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "button.h"
#include "manager.h"

//=============================================================
// [ButtonUI] 更新
//=============================================================
void ButtonUI::Update()
{
	CPolygon::Update();

	// マウス位置の取得
	CManager::CursorPos mousePos = CManager::GetInstance()->GetCursorClientPos();
	D3DXVECTOR3 wPos = transform->GetWPos();

	if (wPos.x <= mousePos.x && mousePos.x <= wPos.x + transform->GetSize().x &&
		wPos.y <= mousePos.y && mousePos.y <= wPos.y + transform->GetSize().y)
	{ // クリック範囲内にあるとき
		if (INPUT_INSTANCE->onTrigger("lclick"))
		{
			// クリックイベント
			if (m_onClick != nullptr)
			{
				m_onClick();
			}
		}

		// ボタンの色を変更
		SetColor(D3DCOLOR_RGBA(190, 190, 190, 255));

		// 状態を変更する
		m_isOnCursor = true;
	}
	else
	{
		// ボタンの色を変更
		SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		// 状態を変更する
		m_isOnCursor = false;
	}
}