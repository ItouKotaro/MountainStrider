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
	void Update();
	void SetPause(const bool& enabled);
	bool GetPause() { return m_isPause; }

	enum SELECT
	{
		SELECT_BACK,
		SELECT_EXIT,
		SELECT_MAX,
	};
private:
	void UpdateControl();

	bool m_isPause;
	GameObject* m_pausePage;
	SELECT m_select;
	bool m_onCursor;

	GameObject* m_pauseText;
	GameObject* m_sBack;
	GameObject* m_sExit;
};

#endif // !_PAUSE_H_
