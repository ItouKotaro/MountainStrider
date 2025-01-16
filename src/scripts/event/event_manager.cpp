//=============================================================
//
// �C�x���g�Ǘ� [event_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "event_manager.h"
#include "manager.h"
#include "scene/game.h"

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
		MessageBox(nullptr, "�C�x���g���s", "�A���[�g", MB_OK);

		// ���Ԃ�ݒ肷��
		SetRandomTime();
	}

	// �C�x���g���X�V����
	for (auto itr = m_eventList.begin(); itr != m_eventList.end(); itr++)
	{
		if (*itr != nullptr)
		{
			(*itr)->Update();
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
		//eventTemplate = new***;
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
