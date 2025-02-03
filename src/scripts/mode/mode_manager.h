//=============================================================
//
// モード管理 [mode_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MODE_MANAGER_H_
#define _MODE_MANAGER_H_

// モード定義
class ModeTemplate
{
public:
	// 状態
	enum STATE
	{
		STATE_GAME,	// ゲーム中（通常時）
		STATE_GOAL,		// ゴール時
		STATE_FAIL,		// 失敗時
	};

	ModeTemplate() : m_state(STATE_GAME) {}
	virtual ~ModeTemplate() {}
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}

	//@brief 状態を取得する
	STATE GetState() { return m_state; }

	//@brief 状態をリセットする
	void ResetState() { m_state = STATE::STATE_GAME; }
protected:
	//@brief ゴールイベントを起こす
	void onGoal() { m_state = STATE::STATE_GOAL; }

	//@brief 失敗イベントを起こす
	void onFail() { m_state = STATE::STATE_FAIL; }

private:
	STATE m_state;		// 状態
};

// モード管理
class ModeManager final
{
public:
	void Init();
	void Uninit();
	void Update();
	void LastUninit();

	//@brief 状態を取得する
	ModeTemplate::STATE GetState() { return m_mode->GetState(); }

	//@brief モードを設定する
	void SetMode(ModeTemplate* mode);

	//@brief インスタンスを取得する
	static ModeManager* GetInstance()
	{
		static ModeManager instance;
		return &instance;
	}
private:
	ModeTemplate* m_mode;
};


#endif // !_MODE_MANAGER_H_
