//=============================================================
//
// バー [bar.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _BAR_H_
#define _BAR_H_

#include "component.h"
#include "component/2d/polygon.h"

// バー
class CBar : public Component
{
public:
	void Init() override;
	void Update() override;

	CPolygon* GetPolyBG() { return m_pBgObj->GetComponent<CPolygon>(); }
	CPolygon* GetPolyBar() { return m_pBarObj->GetComponent<CPolygon>(); }
	void SetProgress(const float& fProgress) { m_fBarProgress = fProgress; }

	static const float DEFAULT_BAR_LENGTH;	// デフォルトのバーの長さ
	static const float DEFAULT_BAR_BOLD;		// デフォルトのバーの太さ
private:
	float m_fBarLength;
	float m_fBarBold;
	float m_fBarProgress;

	GameObject* m_pBgObj;
	GameObject* m_pBarObj;
};

// プログレスバー
class CProgressBar : public Component
{
public:
	CProgressBar(const int& nNumBar);
	void Init() override;
	void Uninit() override;
	void Update() override;
	void SetProgress(const float& fProgress) { m_fBarProgress = fProgress; }
	void SetLength(const float& fLength) { m_fBarLength = fLength; }
	void SetBold(const float& fBold) { m_fBarBold = fBold; }
	void SetBarSpace(const float& fSpace) { m_fBarSpace = fSpace; }
	void SetSpace(const float& fSpace) { m_fSpace = fSpace; }
	void SetBGColor(const D3DXCOLOR& color);
	void SetFillColor(const D3DXCOLOR& color) { m_fillColor = color; }
	void SetNonFillColor(const D3DXCOLOR& color) { m_nonFillCollor = color; }
	void SetAlpha(const float& fAlpha);
	float GetProgress() { return m_fBarProgress; }

	static const float DEFAULT_BAR_SPACE;			// デフォルトのバーの空間
	static const D3DXCOLOR DEFAULT_FILL_COLOR;			// デフォルトの埋まっている色
	static const D3DXCOLOR DEFAULT_NONFILL_COLOR;	// デフォルトの埋まっていないときの色
private:
	float m_fBarLength;
	float m_fBarBold;
	float m_fBarProgress;
	float m_fBarSpace;
	float m_fSpace;
	float m_fAlpha;
	int m_nNumBar;
	D3DXCOLOR m_fillColor, m_nonFillCollor;

	GameObject* m_pBgObj;
	std::vector<GameObject*> m_pBars;
};


#endif // !_HP_BAR_H_
