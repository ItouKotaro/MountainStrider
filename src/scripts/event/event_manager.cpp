//=============================================================
//
// �C�x���g�Ǘ� [event_manager.cpp]
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
// [EventManager] ������
//=============================================================
void EventManager::Init()
{
	// �^�C�}�[��ݒ肷��
	SetRandomTime();


}

//=============================================================
// [EventManager] �I��
//=============================================================
void EventManager::Uninit()
{
	// �C�x���g��j������
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
// [EventManager] �X�V
//=============================================================
void EventManager::Update()
{
	// �|�[�Y���͍X�V���Ȃ�
	if (static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetPause()->GetPause())
		return;

	// �^�C�}�[��i�߂�
	m_eventTimer -= CManager::GetInstance()->GetDeltaTime();
	if (m_eventTimer <= 0.0f)
	{
		// �C�x���g���N����
		AddEvent(static_cast<EVENTID>(rand() % EVENTID_MAX));
		//AddEvent(EVENTID_STRONGWIND);

		// ���Ԃ�ݒ肷��
		SetRandomTime();
	}

	// �C�x���g���X�V����
	for (auto itr = m_eventList.begin(); itr != m_eventList.end(); itr++)
	{
		if (*itr != nullptr)
		{
			(*itr)->Update();

			// �C�x���g���I�����Ă��邩
			if ((*itr)->GetEndEvent())
			{
				// �C�x���g��j������
				(*itr)->Uninit();
				delete* itr;
				itr = m_eventList.erase(itr);

				// �Ō�̃C�x���g�̎�
				if (itr == m_eventList.end())
				{
					break;
				}
			}
		}
	}
}

//=============================================================
// [EventManager] �C�x���g���N����
//=============================================================
void EventManager::AddEvent(EVENTID eventID)
{
	EventTemplate* eventTemplate = nullptr;


	// �C�x���gID�ɍ��킹�Đ�������
	switch (eventID)
	{
	case EventManager::EVENTID_METEO:
		//eventTemplate = new MeteoEvent();
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
		// �C�x���g�̏�����
		eventTemplate->Init();

		// �C�x���g���X�g�ɒǉ�����
		m_eventList.push_back(eventTemplate);
	}
}

//=============================================================
// [EventManager] �����_���Ȏ��Ԃ�ݒ肷��
//=============================================================
void EventManager::SetRandomTime()
{
	m_eventTimer = static_cast<float>(BASE_TIME + rand() % RANDOM_TIME);
}
