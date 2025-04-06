//=============================================================
//
// モード管理 [mode_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MODE_MANAGER_H_
#define _MODE_MANAGER_H_

#include "scripts/result/mt_result.h"
#include "scene/game.h"

// モード定義
class ModeTemplate
{
public:
	ModeTemplate();
	virtual ~ModeTemplate() {}
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void OnResultEvent() {}
protected:
	CGameScene* m_game;
};

// モード管理
class ModeManager final
{
public:
	void Init();
	void Uninit();
	void Update();
	void LastUninit();

	//@brief モードを設定する
	void SetMode(ModeTemplate* mode);

	//@brief リザルトを設定する
	void SetResult(ResultBase* result);

	//@brief モードを取得する
	ModeTemplate* GetMode() { return m_mode; }

	//@brief リザルトを取得する
	ResultBase* GetResult() { return m_result; }

	//@brief インスタンスを取得する
	static ModeManager* GetInstance()
	{
		static ModeManager instance;
		return &instance;
	}
private:
	ModeTemplate* m_mode;
	ResultBase* m_result;
};


#endif // !_MODE_MANAGER_H_
