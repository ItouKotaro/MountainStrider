//=============================================================
//
// ポリゴンコンポーネント [polygon.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "component.h"

// ポリゴンクラス
class CPolygon : public Component
{
public:
	CPolygon();
	CPolygon(std::string sTexPath);
	virtual ~CPolygon() override;
	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void DrawUI() override;
	void SetColor(const D3DXCOLOR& color);
	void SetColor(const int& idx, const D3DXCOLOR& color);
	D3DXCOLOR GetColor() { return m_color; }
	void SetAnchorPoint(const D3DXVECTOR2& anchorPoint) { m_anchorPoint = anchorPoint; }
	void SetTexture(std::string sPath);
	void BindTexture(LPDIRECT3DTEXTURE9 tex) { m_pTexture = tex; }
	void SetReverse(bool bReverse) { m_bReverse = bReverse; }
	bool GetReverse() { return m_bReverse; }
	void SetAnim(int nSplit, int nTime);
private:
	void UpdateAnim();
	void UpdateVertex();
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点情報
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャ
	D3DXVECTOR2 m_anchorPoint;					// アンカーポイント
	D3DXCOLOR m_color;

	// アニメーション
	int m_nSplit;					// 分割数
	int m_nTime;					// 時間
	DWORD m_nextTime;		// 次の時間
	int m_nAnimIdx;			// インデックス
	bool m_bReverse;			// 反転
};

#endif // !_IMAGE_H_
