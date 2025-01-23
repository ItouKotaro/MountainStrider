//=============================================================
//
// 入力システム [input_system.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "input_system.h"
#include "internal/KeyAssignmentTable.h"
#include "manager.h"
#include <fstream>
using namespace std;

// 静的メンバ初期化
vector<CInputDevice*> CInputDevice::m_apInputDevice = {};
const int CKeyboardDevice::MAX_KEYBOARD_KEY;

//=============================================================
// [CInputSystem] コンストラクタ
//=============================================================
CInputSystem::CInputSystem()
{
	m_pInput = nullptr;
}

//=============================================================
// [CInputSystem] 初期化
//=============================================================
HRESULT CInputSystem::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		{
			return E_FAIL;
		}
	}

	// ファイル読み込み
	LoadFile();

	//------------------------------------------------------------------------------------------------------------------------------------------
	// 入力デバイスの生成

	// キーボード
	CKeyboardDevice* pKeyboardDevice = new CKeyboardDevice();
	pKeyboardDevice->Init(hInstance, hWnd);

	// ゲームパッド
	CGamepadDevice* pGamepadDevice = new CGamepadDevice();
	pGamepadDevice->Init(hInstance, hWnd);

	// マウス
	CMouseDevice* pMouseDevice = new CMouseDevice();
	pMouseDevice->Init(hInstance, hWnd);

	return S_OK;
}

//=============================================================
// [CInputSystem] 終了
//=============================================================
void CInputSystem::Uninit()
{
	// すべての入力デバイスを終了する
	vector<CInputDevice*>& pInputDevice = CInputDevice::GetInputDevice();
	int nNumDevice = static_cast<int>(pInputDevice.size());
	for (int i = 0; i < nNumDevice; i++)
	{
		if (pInputDevice[0] != nullptr)
		{
			pInputDevice[0]->Uninit();
			delete pInputDevice[0];
		}
	}

	// DirectInputオブジェクトの破棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//=============================================================
// [CInputSystem] 更新
//=============================================================
void CInputSystem::Update()
{
	// すべての入力デバイスを更新する
	vector<CInputDevice*>& pInputDevice = CInputDevice::GetInputDevice();
	for (unsigned int i = 0; i < pInputDevice.size(); i++)
	{
		if (pInputDevice[i] != nullptr)
		{
			pInputDevice[i]->Update();
		}
	}

	// 最後に入力されたデバイスを更新する
	UpdateLastDevice();
}

//=============================================================
// [CInputSystem] 入力判定
//=============================================================
bool CInputSystem::onInput(const std::string& sKeyName)
{
	for (unsigned int i = 0; i < m_keyData.size(); i++)
	{
		if (m_keyData[i].sKeyName == sKeyName)
		{ // キー名が合致するとき

			switch (m_keyData[i].inputType)
			{
			case INPUT_TYPE::PRESS:
				for (unsigned int nCntKey = 0; nCntKey < m_keyData[i].key.size(); nCntKey++)
				{
					if (onPress(m_keyData[i].key[nCntKey]))
					{
						return true;
					}
				}
				return false;
			case INPUT_TYPE::TRIGGER:
				for (unsigned int nCntKey = 0; nCntKey < m_keyData[i].key.size(); nCntKey++)
				{
					if (onTrigger(m_keyData[i].key[nCntKey]))
					{
						return true;
					}
				}
				return false;
			}

		}
	}
	return false;
}

//=============================================================
// [CInputSystem] プレス入力判定
//=============================================================
bool CInputSystem::onPress(const std::string& key)
{
	if (key.length() <= 0)
	{ // 文字が入力されていないとき
		return false;
	}

	// キーごとに区切る
	vector<string> sKeys = KeySplit(key);

	vector<CInputDevice*>& pInputDevice = CInputDevice::GetInputDevice();
	for (unsigned int nCntKey = 0; nCntKey < sKeys.size(); nCntKey++)
	{
		bool bInputKey = false;
		for (unsigned int i = 0; i < pInputDevice.size(); i++)
		{
			if (pInputDevice[i]->GetPress(sKeys[nCntKey]))
			{ // 入力されていたとき
				bInputKey = true;
				break;
			}
		}
		
		// 入力されていなかったとき
		if (!bInputKey)
		{
			return false;	// 1つでも入力されていない場合はfalse
		}
	}
	return true;
}

//=============================================================
// [CInputSystem] トリガー入力判定
//=============================================================
bool CInputSystem::onTrigger(const std::string& key)
{
	if (key.length() <= 0)
	{ // 文字が入力されていないとき
		return false;
	}

	// キーごとに区切る
	vector<string> sKeys = KeySplit(key);

	vector<CInputDevice*>& pInputDevice = CInputDevice::GetInputDevice();
	for (unsigned int nCntKey = 0; nCntKey < sKeys.size(); nCntKey++)
	{
		bool bInputKey = false;
		for (unsigned int i = 0; i < pInputDevice.size(); i++)
		{
			if (pInputDevice[i]->GetTrigger(sKeys[nCntKey]))
			{ // 入力されていたとき
				bInputKey = true;
				break;
			}
		}

		// 入力されていなかったとき
		if (!bInputKey)
		{
			return false;	// 1つでも入力されていない場合はfalse
		}
	}
	return true;
}

//=============================================================
// [CInputSystem] 最後のデバイスの更新
//=============================================================
void CInputSystem::UpdateLastDevice()
{
	static CManager::CursorPos oldCursorPos;
	CManager::CursorPos cursorPos = CManager::GetInstance()->GetCursorClientPos();

	// キーボード
	for (int i = 0; i < _countof(keyboardTable); i++)
	{
		if (onPress(keyboardTable[i].key))
		{
			m_lastDevice = DEVICE_KEYBOARD;
			oldCursorPos = cursorPos;
			return;
		}
	}

	// コントローラー
	for (int i = 0; i < _countof(gamepadTable); i++)
	{
		if (onPress(gamepadTable[i].key))
		{
			m_lastDevice = DEVICE_CONTROLLER;
			oldCursorPos = cursorPos;
			return;
		}
	}
	// トリガー関係
	auto padInfo = GetInputDevice<CGamepadDevice>()->GetState().Gamepad;
	if (padInfo.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		padInfo.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		padInfo.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		padInfo.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		padInfo.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		padInfo.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		padInfo.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		padInfo.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		m_lastDevice = DEVICE_CONTROLLER;
		oldCursorPos = cursorPos;
		return;
	}

	// マウス
	for (int i = 0; i < _countof(mouseTable); i++)
	{
		if (onPress(mouseTable[i].key))
		{
			m_lastDevice = DEVICE_MOUSE;
			oldCursorPos = cursorPos;
			return;
		}
	}
	
	if (oldCursorPos.x != cursorPos.x || oldCursorPos.y != cursorPos.y)
	{
		m_lastDevice = DEVICE_MOUSE;
		oldCursorPos = cursorPos;
		return;
	}
	oldCursorPos = cursorPos;
}

//=============================================================
// [CInputSystem] 入力の取得
//=============================================================
LPDIRECTINPUT8 CInputSystem::GetDirectInput()
{
	return m_pInput;
}

//=============================================================
// [CInputSystem] 入力情報ファイル読み込み
//=============================================================
HRESULT CInputSystem::LoadFile(const std::string& sPath)
{
	// jsonファイルを読み込む
	ifstream ifs(sPath.c_str());
	
	if (ifs.fail())
	{ // ファイルの読み込みに失敗
		MessageBox(CManager::GetInstance()->GetHWND() , "入力情報ファイルの読み込みに失敗しました", "エラー", MB_OK); //終了メッセージ
		return E_FAIL;
	}

	// json形式に変換
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto jInput = json::parse(sInputData);

	for (auto keyMap = jInput.items().begin(); keyMap != jInput.items().end(); keyMap++)
	{
		std::string sKeyName = keyMap.key();
		std::string sInputType = jInput[sKeyName]["input"];

		// 入力方式
		INPUT_TYPE inputType = INPUT_TYPE::PRESS;
		if (sInputType == "press")
			inputType = INPUT_TYPE::PRESS;
		else if (sInputType == "trigger")
			inputType = INPUT_TYPE::TRIGGER;

		// キーを取得
		vector<string> keys;
		for (auto key = jInput[sKeyName]["key"].begin(); key != jInput[sKeyName]["key"].end(); key++)
		{
			keys.push_back(key.value());
		}

		// キーデータを追加する
		AddKeyData(sKeyName, keys, inputType);
	}

	return S_OK;
}

//=============================================================
// [CInputSystem] &ごとに区切る
//=============================================================
std::vector<std::string> CInputSystem::KeySplit(std::string key)
{
	// 変数
	vector<string> result;
	unsigned int first = 0;
	int last = static_cast<int>(key.find_first_of("&"));
	if (last <= -1) result.push_back(key);

	// &ごとに区切る
	while (first < key.size()) 
	{
		string subStr(key, first, last - first);	// &までの範囲の文字列

		result.push_back(subStr);

		// 次の&までの範囲を取得
		first = last + 1;
		last = static_cast<int>(key.find_first_of("&", first));

		if (last == string::npos)
		{
			last = static_cast<int>(key.size());
		}
	}

	return result;
}

//=============================================================
// [CInputSystem] キーデータを追加
//=============================================================
void CInputSystem::AddKeyData(const std::string& sKeyName, const std::vector<std::string>& key, INPUT_TYPE type)
{
	// 既に同じ名前のキーデータが存在しないかを確認する
	for (unsigned int i = 0; i < m_keyData.size(); i++)
	{
		if (m_keyData[i].sKeyName == sKeyName)
		{ // 存在するとき
			return;	// キャンセルする
		}
	}

	// キーデータを作成する
	KeyData data;
	data.sKeyName = sKeyName;
	data.key = key;
	data.inputType = type;

	// キーデータを登録する
	m_keyData.push_back(data);
}

//=============================================================
// [CKeyboardDevice] コンストラクタ
//=============================================================
CInputDevice::CInputDevice()
{
	// デバイス一覧に登録する
	m_apInputDevice.push_back(this);
}

//=============================================================
// [CKeyboardDevice] デストラクタ
//=============================================================
CInputDevice::~CInputDevice()
{
	// デバイス一覧から削除する
	for (unsigned int i = 0; i < m_apInputDevice.size(); i++)
	{
		if (m_apInputDevice[i] == this)
		{
			m_apInputDevice.erase(m_apInputDevice.begin() + i);
			return;
		}
	}
}

//=============================================================
// [CKeyboardDevice] 入力デバイスリストの取得
//=============================================================
std::vector<CInputDevice*>& CInputDevice::GetInputDevice()
{
	return m_apInputDevice;
}


//=============================================================
// [CKeyboardDevice] 初期化
//=============================================================
HRESULT CKeyboardDevice::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力の取得
	LPDIRECTINPUT8 pInput = CInputSystem::GetInstance()->GetDirectInput();

	// 入力デバイス: キーボードの生成
	if (FAILED(pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================
// [CKeyboardDevice] 終了
//=============================================================
void CKeyboardDevice::Uninit()
{
	// 入力デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire(); // デバイスへのアクセス権を破棄
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

//=============================================================
// [CKeyboardDevice] 更新
//=============================================================
void CKeyboardDevice::Update()
{
	BYTE aKeyState[MAX_KEYBOARD_KEY]; // キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < MAX_KEYBOARD_KEY; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		// トリガー情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];																						// プレス情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire(); // キーボードへのアクセス権を獲得
	}
}

//=============================================================
// [CKeyboardDevice] プレス入力
//=============================================================
bool CKeyboardDevice::GetPress(const std::string& key)
{
	// キーボード用キーテーブルに含まれているか
	BYTE keycode;
	for (int i = 0; i < sizeof(keyboardTable) / sizeof(keyboardTable[0]); i++)
	{
		if (keyboardTable[i].key == key)
		{ // 名前が合致するとき
			keycode = keyboardTable[i].code;
			return (m_aKeyState[keycode] & 0x80) != 0;
		}
	}
	return false;
}

//=============================================================
// [CKeyboardDevice] トリガー入力
//=============================================================
bool CKeyboardDevice::GetTrigger(const std::string& key)
{
	// キーボード用キーテーブルに含まれているか
	BYTE keycode;
	for (int i = 0; i < sizeof(keyboardTable) / sizeof(keyboardTable[0]); i++)
	{
		if (keyboardTable[i].key == key)
		{ // 名前が合致するとき
			keycode = keyboardTable[i].code;
			return (m_aKeyStateTrigger[keycode] & 0x80) != 0;
		}
	}
	return false;
}

//=============================================================
// [CGamepadDevice] 初期化
//=============================================================
HRESULT CGamepadDevice::Init(HINSTANCE hInstance, HWND hWnd)
{
	// メモリのクリア
	memset(&m_aJoyKeyState, 0, sizeof(XINPUT_STATE));

	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	return S_OK;
}

//=============================================================
// [CGamepadDevice] 終了
//=============================================================
void CGamepadDevice::Uninit()
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);
}

//=============================================================
// [CGamepadDevice] 更新
//=============================================================
void CGamepadDevice::Update()
{
	XINPUT_STATE joykeyState;	// ジョイパッドの入力情報

	// ジョイパッドの情報を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		m_aJoyKeyStateTrigger.Gamepad.wButtons = (m_aJoyKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;
		m_aJoyKeyState = joykeyState;
	}

	// バイブレーションの終了処理
	if (m_vibrationTimer > 0.0f)
	{
		m_vibrationTimer -= CManager::GetInstance()->GetDeltaTime();

		if (m_vibrationTimer <= 0.0f)
		{
			m_vibration.wLeftMotorSpeed = 0;
			m_vibration.wRightMotorSpeed = 0;
			XInputSetState(0, &m_vibration);
		}
	}
}

//=============================================================
// [CGamepadDevice] プレス入力
//=============================================================
bool CGamepadDevice::GetPress(const std::string& key)
{
	// ゲームパッド用キーテーブルに含まれているか
	WORD keycode;
	for (int i = 0; i < sizeof(gamepadTable) / sizeof(gamepadTable[0]); i++)
	{
		if (gamepadTable[i].key == key)
		{ // 名前が合致するとき
			keycode = gamepadTable[i].code;
			return (m_aJoyKeyState.Gamepad.wButtons & keycode) ? true : false;
		}
	}
	return false;
}

//=============================================================
// [CGamepadDevice] トリガー入力
//=============================================================
bool CGamepadDevice::GetTrigger(const std::string& key)
{
	// ゲームパッド用キーテーブルに含まれているか
	WORD keycode;
	for (int i = 0; i < sizeof(gamepadTable) / sizeof(gamepadTable[0]); i++)
	{
		if (gamepadTable[i].key == key)
		{ // 名前が合致するとき
			keycode = gamepadTable[i].code;
			return (m_aJoyKeyStateTrigger.Gamepad.wButtons & keycode) ? true : false;
		}
	}
	return false;
}

//=============================================================
// [CGamepadDevice] バイブレーションを設定する
//=============================================================
void CGamepadDevice::SetVibration(const int& powerL, const int& powerR, const float& time)
{
	m_vibrationTimer = time;

	// バイブレーションスタート
	m_vibration.wLeftMotorSpeed = static_cast<WORD>(powerL);
	m_vibration.wRightMotorSpeed = static_cast<WORD>(powerR);

	// 適用する
	XInputSetState(0, &m_vibration);
}


//=============================================================
// [CMouseDevice] 初期化
//=============================================================
HRESULT CMouseDevice::Init(HINSTANCE hInstance, HWND hWnd)
{
	return S_OK;
}

//=============================================================
// [CMouseDevice] 終了
//=============================================================
void CMouseDevice::Uninit()
{
}

//=============================================================
// [CMouseDevice] 更新
//=============================================================
void CMouseDevice::Update()
{
	bool aMouseState[MAX_MOUSE_BUTTON]; // マウスの入力情報

	// 左クリック
	aMouseState[0] = GetKeyState(VK_LBUTTON) & 0x80;
	m_aMouseStateTrigger[0] = (m_aMouseState[0] ^ aMouseState[0]) & aMouseState[0];
	m_aMouseState[0] = aMouseState[0];

	// 右クリック
	aMouseState[1] = GetKeyState(VK_RBUTTON) & 0x80;
	m_aMouseStateTrigger[1] = (m_aMouseState[1] ^ aMouseState[1]) & aMouseState[1];
	m_aMouseState[1] = aMouseState[1];

	// 中央クリック
	aMouseState[2] = GetKeyState(VK_MBUTTON) & 0x80;
	m_aMouseStateTrigger[2] = (m_aMouseState[2] ^ aMouseState[2]) & aMouseState[2];
	m_aMouseState[2] = aMouseState[2];
}

//=============================================================
// [CMouseDevice] プレス入力
//=============================================================
bool CMouseDevice::GetPress(const std::string& key)
{
	// マウス用キーテーブルに含まれているか
	for (int i = 0; i < sizeof(mouseTable) / sizeof(mouseTable[0]); i++)
	{
		if (mouseTable[i].key == key)
		{ // 名前が合致するとき
			return m_aMouseState[mouseTable[i].code];
		}
	}
	return false;
}

//=============================================================
// [CMouseDevice] トリガー入力
//=============================================================
bool CMouseDevice::GetTrigger(const std::string& key)
{
	// マウス用キーテーブルに含まれているか
	for (int i = 0; i < sizeof(mouseTable) / sizeof(mouseTable[0]); i++)
	{
		if (mouseTable[i].key == key)
		{ // 名前が合致するとき
			return m_aMouseStateTrigger[mouseTable[i].code];
		}
	}
	return false;
}