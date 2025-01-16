//=============================================================
//
// �C�x���g�Ǘ� [event_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _EVENT_MANAGER_H_
#define _EVENT_MANAGER_H_

// �C�x���g�o�^�p
class EventTemplate
{
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
};

// �C�x���g�Ǘ��N���X
class EventManager
{
public:
	void Init();
	void Uninit();
	void Update();

	// �C�x���g
	enum EVENTID
	{
		EVENTID_METEO = 0,		// 覐�
		EVENTID_MAX
	};

	//@brief �C�x���g���N����
	void AddEvent(EVENTID eventID);
private:
	//@brief �����_���Ȏ��Ԃ�ݒ肷��
	void SetRandomTime();

	float m_eventTimer;
	std::vector<EventTemplate*> m_eventList;		// ���݂̃C�x���g

	const int BASE_TIME = 60;
	const int RANDOM_TIME = 40;
};

#endif // !_EVENT_MANAGER_H_
