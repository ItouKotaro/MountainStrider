//=============================================================
//
// モード管理 [mode_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MODE_MANAGER_H_
#define _MODE_MANAGER_H_

#include "scripts/result/mt_result.h"

// モード定義
class ModeTemplate
{
public:
	ModeTemplate() {}
	virtual ~ModeTemplate() {}
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void OnResultEvent() {}
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
	template<class T> void SetResult()
	{
		// リザルトが設定されていない場合のみ
		if (m_result == nullptr)
		{
			// 新しいモードを設定する
			m_result = new T();
			m_result->Init();
		}
	}

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
