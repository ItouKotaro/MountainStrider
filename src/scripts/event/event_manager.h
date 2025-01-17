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
	EventTemplate() : m_endEvent(false) {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;

	//@brief イベントを終了する
	void EndEvent() { m_endEvent = true; }

	//@brief イベントが終了しているかを取得する
	bool GetEndEvent() { return m_endEvent; }

private:
	bool m_endEvent;
};

// イベント管理クラス
class EventManager
{
public:
	void Init();
	void Uninit();
	void Update();

	// イベント
	enum EVENTID
	{
		EVENTID_METEO = 0,		// 隕石
		EVENTID_TORNADO,			// トルネード
		EVENTID_MAX
	};

	//@brief イベントを起こす
	void AddEvent(EVENTID eventID);
private:
	//@brief ランダムな時間を設定する
	void SetRandomTime();

	float m_eventTimer;
	std::vector<EventTemplate*> m_eventList;		// 現在のイベント

	const int BASE_TIME = 10;
	const int RANDOM_TIME = 10;
};

#endif // !_EVENT_MANAGER_H_
