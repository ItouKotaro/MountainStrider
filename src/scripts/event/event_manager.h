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
private:
	std::vector<EventTemplate*> m_eventList;		// ���݂̃C�x���g
};

#endif // !_EVENT_MANAGER_H_
