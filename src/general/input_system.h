//=============================================================
//
// 入力システム [input_system.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

// 入力デバイス（基底）
class CInputDevice
{
public:
	CInputDevice();																				// コンストラクタ
	virtual ~CInputDevice();																	// デストラクタ
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd) = 0;		// 初期化
	virtual void Uninit() = 0;																	// 終了
	virtual void Update() = 0;																// 更新
	virtual bool GetPress(const std::string& key) = 0;								// プレス入力の取得
	virtual bool GetTrigger(const std::string& key) = 0;							// トリガー入力の取得
	static std::vector<CInputDevice*>& GetInputDevice();						// 入力デバイス一覧の取得

protected:
	LPDIRECTINPUTDEVICE8 m_pDevice;
	static std::vector<CInputDevice*> m_apInputDevice;
};

// キーボードデバイス
class CKeyboardDevice : public CInputDevice
{
public:
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(const std::string& key);
	bool GetTrigger(const std::string& key);
	static const int MAX_KEYBOARD_KEY = 256;
private:
	BYTE m_aKeyState[MAX_KEYBOARD_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEYBOARD_KEY];
};

// ゲームパッドデバイス
class CGamepadDevice : public CInputDevice
{
public:
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(const std::string& key);
	bool GetTrigger(const std::string& key);
	void SetVibration(const int& powerL, const int& powerR, const float& time);
	XINPUT_STATE& GetState() { return m_aJoyKeyState; }
private:
	XINPUT_STATE m_aJoyKeyState;
	XINPUT_STATE m_aJoyKeyStateTrigger;

	// バイブレーション
	float m_vibrationTimer;
	XINPUT_VIBRATION m_vibration;
};

// マウスデバイス
class CMouseDevice : public CInputDevice
{
public:
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(const std::string& key);
	bool GetTrigger(const std::string& key);

	static const int MAX_MOUSE_BUTTON = 3;
private:
	bool m_aMouseState[MAX_MOUSE_BUTTON];
	bool m_aMouseStateTrigger[MAX_MOUSE_BUTTON];
};

// 入力システム
class CInputSystem
{
public:
	// @brief コンストラクタ
	CInputSystem();

	/*
	@brief 入力システムの初期化処理
	@return 実行結果
	*/
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);

	/*
	@brief 入力システムの終了処理
	*/
	void Uninit();

	/*
	@brief 入力システムの更新処理
	*/
	void Update();

	/*
	@brief 指定キーが入力されているかの判定
	@param[in] sKeyName : 登録したキー名
	@return bool : 入力情報
	*/
	bool onInput(const std::string& sKeyName);

	/*
	@brief キー単体のプレス判定
	@param[in] key : キー名
	@return bool : 入力情報
	*/
	bool onPress(const std::string& key);

	/*
	@brief キー単体のトリガー判定
	@param[in] key : キー名
	@return bool : 入力情報
	*/
	bool onTrigger(const std::string& key);

	// デバイス
	enum DEVICE
	{
		DEVICE_KEYBOARD,
		DEVICE_MOUSE,
		DEVICE_CONTROLLER,
	};
	//@brief 最後に入力したデバイス
	DEVICE GetLastInput() { return m_lastDevice; }

	/*
	@brief 登録されている入力デバイスの取得
	@return 入力デバイスのポインタ（失敗: nullptr）
	*/
	template<class T> T* GetInputDevice() {
		std::vector<CInputDevice*>& pDeviceList = CInputDevice::GetInputDevice();
		for (int i = 0; i < pDeviceList.size(); i++)
		{
			if (typeid(T) == typeid(*pDeviceList[i]))
			{ // 型が一致するとき
				T* pCasted = (T*)pDeviceList[i];
				return pCasted;
			}
		}
		return nullptr;	// 見つからなかったとき
	}

	/*
	@brief 入力の取得
	@return LPDIRECTINPUT8
	*/
	LPDIRECTINPUT8 GetDirectInput();

	/*
	@brief インスタンスの取得
	@return インスタンス
	*/
	static CInputSystem* GetInstance()
	{
		static CInputSystem instance;
		return &instance;
	}

	// 入力方式
	enum INPUT_TYPE
	{
		PRESS,
		TRIGGER
	};

	// キーデータ
	struct KeyData
	{
		std::string sKeyName;				// キー名
		std::vector<std::string> key;		// 入力キー（入力判別）
		INPUT_TYPE inputType;				// 入力方式
	};
private:
	/*
	@brief 入力情報ファイル読み込み
	@param[in] sPath : 入力情報ファイルのパス
	@return bool : 実行結果
	*/
	HRESULT LoadFile(const std::string& sPath = "data\\input.json");

	/*
	@brief 同時押しに対応するために&区切りに分ける
	*/
	std::vector<std::string> KeySplit(std::string key);

	/*
	@brief キーデータの作成
	@param[in] sKeyName : キー名
	@param[in] key : 入力キー
	@param[in] type : 入力方式（PRESS, TRIGGER）
	*/
	void AddKeyData(const std::string& sKeyName, const std::vector<std::string>& key, INPUT_TYPE type);

	//@brief 最後のデバイスを取得する
	void UpdateLastDevice();

	std::vector<KeyData> m_keyData;
	LPDIRECTINPUT8 m_pInput;
	DEVICE m_lastDevice;
};

#endif // !_INPUT_SYSTEM_H_
