//=============================================================
//
// プレイガイド [play_guide.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _PLAY_GUIDE_H_
#define _PLAY_GUIDE_H_

#include "component.h"
#include "component/2d/polygon.h"

// プレイガイドUI
class PlayGuideUI : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;
	void SetProgress(const float& value) { m_progressValue = value; }
private:
	enum PROGRESS
	{
		PROGRESS_SEGMENT,
		PROGRESS_BG,
		PROGRESS_END,
	};
	PROGRESS m_progress;
	int m_progressCounter;
	float m_progressValue;

	SingleComponent<CPolygon>* m_segment;
	SingleComponent<CPolygon>* m_bg;
	SingleComponent<CPolygon>* m_progressBar;

	// 定数
	const D3DXVECTOR2 SIZE = { 550.0f, 200.0f };
	const float SEGMENT_WIDTH = 20.0f;
	const float PROGRESSBAR_HEIGHT = 10.0f;
};

// ガイド基底
class GuideContent
{
public:
	GuideContent() : m_progress(0.0f){}
	virtual void Init() {};
	virtual void Update() {};

	float GetProgress() { return m_progress; }
	bool IsCompleted() { return m_progress >= 1.0f; }
protected:
	float m_progress;
};

// プレイガイドマネージャー
class PlayGuideManager
{
public:
	void Init();
	void Uninit();
	void Update();

private:
	// ガイド
	enum GUIDE
	{
		GUIDE_ACCEL,			// アクセル
		GUIDE_DIRECTION,	// 傾き
		GUIDE_JUMP,			// ジャンプ
		GUIDE_ACTION,			// アクション
		GUIDE_MAX
	};
	GUIDE m_guideType;
	GuideContent* m_guide;
	bool m_isAllCompleted;

	PlayGuideUI* m_guideUI;

	void SetGuide(const GUIDE& guide);
};


// ------------------ 各ガイド ------------------

// アクセル
class AccelGuide : public GuideContent
{
public:
	void Update() override;
};



#endif // !_PLAY_GUIDE_H_
