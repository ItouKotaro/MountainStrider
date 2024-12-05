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
	void SetLength(const float& length) { m_fBarLength = length; }
	void SetBold(const float& bold) { m_fBarBold = bold; }

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

// 高度なバー
class CAdvancedBar : public Component
{
public:
	CAdvancedBar():m_pVtxBar(nullptr),m_align(ALIGN::LEFT),m_fBold(30.0f),m_fLength(500.0f),m_fProgress(1.0f),m_texture(nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			m_color[i] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		m_bgColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	void Init() override;
	void Uninit() override;
	void DrawUI() override;

	// 色変更
	void SetColor(int index, D3DXCOLOR color);
	// 色変更
	void SetColor(D3DXCOLOR color);
	// 背景色変更
	void SetBGColor(D3DXCOLOR color);
	// 進捗度変更
	void SetProgress(const float& percent);
	// 進捗度の取得
	float GetProgress() { return m_fProgress; }
	// 長さの変更
	void SetLength(const float& length);
	// 長さの取得
	float GetLength() { return m_fLength; }
	// 太さの変更
	void SetBold(const float& bold);
	// 太さの取得
	float GetBold() { return m_fBold; }
	// テクスチャの設定
	void SetTexture(std::string sPath);

	// 整列
	enum ALIGN
	{
		LEFT,
		CENTER
	};

	// 整列の変更
	void SetAlign(const ALIGN& align);
	// 整列の取得
	ALIGN GetAlign() { return m_align; }

	// 頂点更新
	void UpdateVertex();
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBar;	// バー頂点情報
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBG;	// 背景頂点情報
	LPDIRECT3DTEXTURE9 m_texture;				// テクスチャ

	ALIGN m_align;											// 整列
	float m_fBold;											// 太さ
	float m_fLength;										// 長さ
	float m_fProgress;										// 進捗度
	D3DXCOLOR m_color[4];							// 色
	D3DXCOLOR m_bgColor;							// 背景色
};


#endif // !_HP_BAR_H_
