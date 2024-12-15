//=============================================================
//
// レンダーバッファー [render_buffer.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _RENDER_BUFFER_H_
#define _RENDER_BUFFER_H_

// レンダーバッファー
class RenderBuffer
{
public:
	RenderBuffer() : m_name(""), m_surface(nullptr), m_texture(nullptr), enabled(true){}
	virtual ~RenderBuffer() {}

	//@brief 描画
	virtual void Render();
	//@brief 初期化
	virtual void Init(){}
	//@brief 終了
	virtual void Uninit();

	//@brief ターゲットの変更
	void ChangeTarget();
	//@brief もとのバッファに戻る
	void ResetBuffer();

	//@brief テクスチャの生成
	bool CreateTexture(const int& width, const int& height, const D3DFORMAT& format = D3DFMT_R8G8B8);
	//@brief テクスチャの破棄
	void DestroyTexture();

	//@brief 名前の設定
	void SetName(const std::string& name) { m_name = name; }
	//@brief 名前の取得
	std::string GetName() { return m_name; }
	//@brief テクスチャの取得
	LPDIRECT3DTEXTURE9 GetTexture() { return m_texture; }

	bool enabled;	// 有効か

protected:
	//@brief シーンの開始
	bool BeginScene();
	//@brief シーンの終了
	bool EndScene();
private:
	std::string m_name;	// 名前

	LPDIRECT3DSURFACE9 m_surface;				// サーフェイス
	LPDIRECT3DSURFACE9 m_depthSurface;	// 深度バッファ
	LPDIRECT3DTEXTURE9 m_texture;				// テクスチャ

	LPDIRECT3DSURFACE9 m_beforeBufferSurface;
	LPDIRECT3DSURFACE9 m_beforeDepthSurface;
};


#include "component/3d/camera.h"
// カメラレンダーバッファ
class CameraRenderBuffer : public RenderBuffer
{
public:
	CameraRenderBuffer() : m_camera(nullptr) {}
	void SetCamera(CCamera* camera) { m_camera = camera; }
	void Render() override;
protected:
	CCamera* m_camera;
};

#endif // !_RENDER_BUFFER_H_
