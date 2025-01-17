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
	EventTemplate() : m_endEvent(false) {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;

	//@brief �C�x���g���I������
	void EndEvent() { m_endEvent = true; }

	//@brief �C�x���g���I�����Ă��邩���擾����
	bool GetEndEvent() { return m_endEvent; }

private:
	bool m_endEvent;
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
		EVENTID_TORNADO,			// �g���l�[�h
		EVENTID_MAX
	};

	//@brief �C�x���g���N����
	void AddEvent(EVENTID eventID);
private:
	//@brief �����_���Ȏ��Ԃ�ݒ肷��
	void SetRandomTime();

	float m_eventTimer;
	std::vector<EventTemplate*> m_eventList;		// ���݂̃C�x���g

	const int BASE_TIME = 10;
	const int RANDOM_TIME = 10;
};

#endif // !_EVENT_MANAGER_H_
