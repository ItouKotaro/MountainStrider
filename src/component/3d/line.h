//=============================================================
//
// ライン [line.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _LINE_H_
#define _LINE_H_

#include <component.h>

// ラインコンポーネント
class CLine : public Component
{
public:
	CLine();
	CLine(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXCOLOR& color = {1.0f, 1.0f, 1.0f, 1.0f});
	~CLine();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetLine(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXCOLOR& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	void SetColor(const D3DXCOLOR& color);
private:
	D3DXVECTOR3 m_start;
	D3DXVECTOR3 m_end;
	D3DXCOLOR m_color;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
};

#endif // !_LINE_H_
