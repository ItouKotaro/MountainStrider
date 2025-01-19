//=============================================================
//
// メッシュフィールド [meshfield.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

#include "component.h"

// メッシュフィールド
class CMeshField : public Component
{
public:
	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Draw() override;

	void BindTexture(LPDIRECT3DTEXTURE9 texture) { m_pTexture = texture; }
	void SetTexture(const std::string& sPath);
	void SetLoopTexture(const int& num);

	// メッシュフィールドを用意する
	void Create(const int& x, const int& y, const float& spaceSize);

	// 指定の位置の高さを変更する
	void SetHeight(const int& x, const int& y, const float& height);

	// 指定の位置の色を変更する
	void SetColor(const int& x, const int& y, const D3DXCOLOR& color);

	// 法線を適切な向きに変更する
	void ResetNormals();

	// データを取得します
	std::vector<D3DXVECTOR3>& GetVertices() { return m_vertices; }
	std::vector<int>& GetIndices() { return m_indices; }
	int GetNumFaces() { return (m_sizeX * 2) * m_sizeY; }

	// 法線を表示する
	void ShowNormals(GameObject* pLineObj);

private:
	int m_sizeX;				// Xの面数
	int m_sizeY;				// Yの面数
	int m_loopTex;			// テクスチャのループ数
	float m_sizeSpace;		// 間隔

	std::vector<D3DXVECTOR3> m_vertices;	// 頂点データ
	std::vector<int> m_indices;						// インデックスデータ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファ
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャ
};

#endif // !_MESH_FIELD_H_
