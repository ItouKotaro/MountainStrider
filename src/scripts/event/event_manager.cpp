//=============================================================
//
// イベント管理 [event_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "event_manager.h"
#include "manager.h"
#include "scene/game.h"

#include "meteo.h"
#include "tornado.h"
#include "strong_wind.h"

//=============================================================
// [EventManager] 初期化
//=============================================================
void EventManager::Init()
{
	// タイマーを設定する
	SetRandomTime();


}

//=============================================================
// [EventManager] 終了
//=============================================================
void EventManager::Uninit()
{
	// イベントを破棄する
	for (auto itr = m_eventList.begin(); itr != m_eventList.end(); itr++)
	{
		if (*itr != nullptr)
		{
			(*itr)->Uninit();
			delete* itr;
			*itr = nullptr;
		}
	}
	m_eventList.clear();
}

//=============================================================
// [EventManager] 更新
//=============================================================
void EventManager::Update()
{
	// ポーズ時は更新しない
	if (static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetPause()->GetPause())
		return;

	// タイマーを進める
	m_eventTimer -= CManager::GetInstance()->GetDeltaTime();
	if (m_eventTimer <= 0.0f)
	{
		// イベントを起こす
		AddEvent(static_cast<EVENTID>(rand() % EVENTID_MAX));

		// 時間を設定する
		SetRandomTime();
	}

	// イベントを更新する
	for (auto itr = m_eventList.begin(); itr != m_eventList.end(); itr++)
	{
		if (*itr != nullptr)
		{
			(*itr)->Update();

			// イベントが終了しているか
			if ((*itr)->GetEndEvent())
			{
				// イベントを破棄する
				(*itr)->Uninit();
				delete* itr;
				itr = m_eventList.erase(itr);

				// 最後のイベントの時
				if (itr == m_eventList.end())
				{
					break;
				}
			}
		}
	}
}

//=============================================================
// [EventManager] イベントを起こす
//=============================================================
void EventManager::AddEvent(EVENTID eventID)
{
	EventTemplate* eventTemplate = nullptr;


	// イベントIDに合わせて生成する
	switch (eventID)
	{
	case EventManager::EVENTID_METEO:
		eventTemplate = new MeteoEvent();
		break;
	case EventManager::EVENTID_TORNADO:
		eventTemplate = new TornadoEvent();
		break;
	case EventManager::EVENTID_STRONGWIND:
		eventTemplate = new StrongWindEvent();
		break;
	}


	if (eventTemplate != nullptr)
	{
		// イベントの初期化
		eventTemplate->Init();

		// イベントリストに追加する
		m_eventList.push_back(eventTemplate);
	}
}

//=============================================================
// [EventManager] ランダムな時間を設定する
//=============================================================
void EventManager::SetRandomTime()
{
	m_eventTimer = static_cast<float>(BASE_TIME + rand() % RANDOM_TIME);
}
