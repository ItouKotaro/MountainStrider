//=============================================================
//
// É|Å[ÉY [pause.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

class Pause
{
public:
	Pause() : m_isPause(false){}
	void Init();
	void SetPause(const bool& enabled);
	bool GetPause() { return m_isPause; }
private:
	bool m_isPause;
	GameObject* m_pausePage;

	GameObject* m_backButton;
	GameObject* m_endButton;
};

#endif // !_PAUSE_H_
