//=============================================================
//
// テキストコンポーネント [text.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TEXT_COMPONENT_H_
#define _TEXT_COMPONENT_H_

#include "component.h"

// テキストコンポーネント
class CText : public Component
{
public:
	// 整列
	enum ALIGN
	{
		LEFT,
		CENTER,
		RIGHT
	};

	// 文字情報
	struct TextInfo
	{
		IDirect3DTexture9* pTex;	// 文字のテクスチャ
		D3DXVECTOR3 pos;			// 文字位置
		int line;								// 行数
	};

	// フォントのデータ
	struct FontData
	{
		std::string sFontPath;
		std::string sFontName;
	};

	void Init() override;
	void Uninit() override;
	virtual void DrawUI() override;

	void SetText(const std::string& text);
	void SetFontSize(const int& size);
	void SetOutlineSize(const int& size);
	void SetOutlineColor(const D3DXCOLOR& color);
	void SetFontColor(const D3DXCOLOR& color);
	void SetFont(const std::string& fontName);
	void SetAlign(const ALIGN& align);
	void SetAlpha(const float& fAlpha);
	float GetAlpha() { return m_alpha; }

	int GetFontSize() { return m_fontSize; }

	static const FontData LoadFonts[];
	static void InitLoadFont();
protected:
	// 描画
	LPD3DXSPRITE m_pSprite;							// スプライト
	std::vector<TextInfo> m_textInfos;				// 文字管理
	float m_fMaxHeight;									// 文字の中でも最大サイズ
	float m_alpha;											// 不透明度
private:
	void UpdateText();

	// 1文字のテクスチャを生成する
	IDirect3DTexture9* CreateFontTexture(
		const char* faceName = "MS ゴシック",
		unsigned char charSet = SHIFTJIS_CHARSET,
		const char* str = "T",
		unsigned fontHeight = 450,
		unsigned weight = 0,
		int penSize = 5,
		D3DXCOLOR edgeColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXCOLOR fillColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		int quality = 3,
		RECT* info = 0);

	// 情報
	std::string m_text;					// 文字
	unsigned char m_charSet;		// 文字セット
	int m_fontSize;					// フォントサイズ
	float m_weight;						// フォントの太さ
	int m_outlineSize;				// アウトラインサイズ
	D3DXCOLOR m_fillColor;			// 塗りつぶし色
	D3DXCOLOR m_outlineColor;		// アウトライン色
	int m_quality;						// アンチエイリアス（1以上）
	RECT* m_info;
	ALIGN m_align;						// 整列

	// フォント情報
	std::string m_fontName;		// フォント名
};

// タイピングテキスト
class CTypingText : public CText
{
public:
	CTypingText();
	void Update() override;
	void DrawUI() override;
	void SetTypingSpeed(const int& nSpeed) { m_nTypingSpeed = nSpeed; }
private:
	int m_nTypingCounter;	// タイピングカウンター
	int m_nTypingSpeed;		// タイピングスピード
	int m_nTypingNum;		// タイピング済み
};

// テキストタグ（基底）
class CTextTag
{
public:
	enum TYPE
	{
		SIZE,
		COLOR,
		FONT_COLOR,
		EDGE_COLOR,
	};
	CTextTag(const TYPE& type) { m_type = type; }
	void SetIdx(const int& idx) { m_idx = idx; }
	int GetIdx() { return m_idx; }
	TYPE GetType() { return m_type; }
private:
	int m_idx;
	TYPE m_type;
};

// サイズタグ
class CSizeTag : public CTextTag
{
public:
	CSizeTag() : CTextTag(SIZE) {}
	void SetSize(const int& size) { m_size = size; }
	int GetSize() { return m_size; }
private:
	int m_size;
};

// カラータグ
class CColorTag : public CTextTag
{
public:
	CColorTag() : CTextTag(COLOR) {}
	void SetColor(const D3DXCOLOR& color) { 
		m_fillColor = color; 
		m_edgeColor = color;
	}
	void SetFillColor(const D3DXCOLOR& color) { m_fillColor = color; }
	void SetEdgeColor(const D3DXCOLOR& color) { m_edgeColor = color; }
	D3DXCOLOR GetFillColor() { return m_fillColor; }
	D3DXCOLOR GetEdgeColor() { return m_edgeColor; }
private:
	D3DXCOLOR m_fillColor;
	D3DXCOLOR m_edgeColor;
};

// フォントカラータグ
class CFontColorTag : public CTextTag
{
public:
	CFontColorTag() : CTextTag(FONT_COLOR) {}
	void SetColor(const D3DXCOLOR& color) { m_color = color; }
	D3DXCOLOR GetColor() { return m_color; }
private:
	D3DXCOLOR m_color;
};

// エッジカラータグ
class CEdgeColorTag : public CTextTag
{
public:
	CEdgeColorTag() : CTextTag(EDGE_COLOR) {}
	void SetColor(const D3DXCOLOR& color) { m_color = color; }
	D3DXCOLOR GetColor() { return m_color; }
private:
	D3DXCOLOR m_color;
};

#endif // !_TEXT_COMPONENT_H_
