//=============================================================
//
// 仮想カーソル [virtual_cursor.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "virtual_cursor.h"
#include "manager.h"

//=============================================================
// [VirtualCursor] カーソルの終了
//=============================================================
void VirtualCursor::Uninit()
{
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

//=============================================================
// [VirtualCursor] カーソルの更新
//=============================================================
void VirtualCursor::Update()
{
	auto useDevice = INPUT_INSTANCE->GetLastInput();


	auto padInfo = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad;
	short stickX = padInfo.sThumbLX;
	short stickY = padInfo.sThumbLY;

	// 移動
	if (stickX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < stickX ||
		stickY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < stickY)
	{
		float angle = atan2f((float)stickX, (float)-stickY);

		POINT p;
		GetCursorPos(&p);
		p.x += static_cast<LONG>(sinf(angle) * 18.0f);
		p.y += static_cast<LONG>(cosf(angle) * 18.0f);
		SetCursorPos(p.x, p.y);
	}

	// クリック
	bool oldIsClick = m_isClick;
	m_isClick = INPUT_INSTANCE->onPress("p:a");

	if (oldIsClick != m_isClick)
	{
		mouse_event(m_isClick ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}

	CManager::CursorPos cursorPos = CManager::GetInstance()->GetCursorClientPos();
	transform->SetPos(cursorPos.x, cursorPos.y);
}