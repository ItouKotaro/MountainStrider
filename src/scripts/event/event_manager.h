//=============================================================
//
// イベント管理 [event_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _EVENT_MANAGER_H_
#define _EVENT_MANAGER_H_

// イベント登録用
class EventTemplate
{
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
};

// イベント管理クラス
class EventManager
{
public:
	void Init();
	void Uninit();
	void Update();
private:
	std::vector<EventTemplate*> m_eventList;		// 現在のイベント
};

#endif // !_EVENT_MANAGER_H_
