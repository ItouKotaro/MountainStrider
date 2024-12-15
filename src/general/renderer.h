//=============================================================
//
// レンダラー [renderer.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "internal/shadow.h"
#include "render_buffer.h"

// マクロ定義
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット[3D]

// レンダラークラスの定義
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();

	static CRenderer* GetInstance() {
		static CRenderer renderer;
		return &renderer;
	}

	// レンダーバッファの登録
	template<class T> T* RegisterRenderBuffer(const std::string& name)
	{
		RenderBuffer* findRenderBuffer = GetRenderBuffer(name);
		if (findRenderBuffer == nullptr)
		{
			T* renderBuffer = new T();
			renderBuffer->SetName(name);
			m_renderBuffers.push_back(renderBuffer);
			renderBuffer->Init();
			return renderBuffer;
		}
		return static_cast<T*>(findRenderBuffer);
	}
	// レンダーバッファの取得
	RenderBuffer* GetRenderBuffer(const std::string& name);
	// レンダーバッファの削除
	void RemoveRenderBuffer(const std::string& name);

	// 影の描画
	CShadow* GetShadow() { return m_pShadow; }

	// フルスクリーン
	bool GetFullScreen() { return m_bFullScreen; }
	void SetFullScreen(const bool& bFullScreen);

	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }
	HWND GetHWND() { return m_hwnd; }

	static const int SCREEN_WIDTH;					// ウィンドウサイズ（横）
	static const int SCREEN_HEIGHT;				// ウィンドウサイズ（高さ）
	static const bool USE_FOG;							// フォグを使用するか
private:
	LPDIRECT3D9 m_pD3D;								// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			// Direct3Dデバイスへのポインタ
	HWND m_hwnd;										// ハンドル
	CShadow* m_pShadow;								// 影
	bool m_bFullScreen;									// フルスクリーン

	// レンダーバッファ
	std::vector<RenderBuffer*> m_renderBuffers;

	// フォグ
	float m_fFogStartPos;									// フォグのスタート位置
	float m_fFogEndPos;									// フォグの終了位置
};


// 頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;					// 座標変換用係数（1.0fで固定）
	D3DCOLOR col;			// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

// 頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;			// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;

#endif // !_RENDERER_H_
