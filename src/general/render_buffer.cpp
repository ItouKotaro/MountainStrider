//=============================================================
//
// レンダーバッファー [render_buffer.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "render_buffer.h"
#include "renderer.h"
#include "manager.h"

//=============================================================
// [RenderBuffer] 終了
//=============================================================
void RenderBuffer::Uninit()
{
	// テクスチャの破棄
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	// テクスチャサーフェイスの破棄
	if (m_surface != nullptr)
	{
		m_surface->Release();
		m_surface = nullptr;
	}

	// 深度バッファの破棄
	if (m_depthSurface != nullptr)
	{
		m_depthSurface->Release();
		m_depthSurface = nullptr;
	}
}

//=============================================================
// [RenderBuffer] ターゲットの変更
//=============================================================
void RenderBuffer::ChangeTarget()
{
	// デバイスの取得
	auto device = CRenderer::GetInstance()->GetDevice();

	// 前回の設定として保存しておく
	device->GetRenderTarget(0, &m_beforeBufferSurface);
	device->GetDepthStencilSurface(&m_beforeDepthSurface);

	// レンダーターゲットを設定する
	if (m_texture != nullptr)
	{
		device->SetDepthStencilSurface(m_depthSurface);

		if (FAILED(m_texture->GetSurfaceLevel(0, &m_surface)))
		{
			return;
		}
	}
	device->SetRenderTarget(0, m_surface);
}

//=============================================================
// [RenderBuffer] バッファをもとに戻す
//=============================================================
void RenderBuffer::ResetBuffer()
{
	// デバイスの取得
	auto device = CRenderer::GetInstance()->GetDevice();

	// 元のレンダーターゲットを設定する
	device->SetRenderTarget(0, m_beforeBufferSurface);

	// 元の設定に戻す
	device->SetDepthStencilSurface(m_beforeDepthSurface);
}

//=============================================================
// [RenderBuffer] レンダー
//=============================================================
void RenderBuffer::Render()
{
	// デバイスの取得
	auto device = CRenderer::GetInstance()->GetDevice();

	// 画面クリア（バッファクリア＆Zバッファクリア）
	device->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	// ゲームオブジェクトの描画処理
	GameObject::DrawAll();

	// ゲームオブジェクトのUI描画処理
	GameObject::DrawUIAll();
}

//=============================================================
// [RenderBuffer] テクスチャの生成
//=============================================================
bool RenderBuffer::CreateTexture(const int& width, const int& height, const D3DFORMAT& format)
{
	// デバイスの取得
	auto device = CRenderer::GetInstance()->GetDevice();

	// 既存テクスチャを破棄
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	// テクスチャの作成
	if (FAILED(D3DXCreateTexture(
		device,
		width,
		height,
		0,
		D3DUSAGE_RENDERTARGET,
		format,
		D3DPOOL_DEFAULT,
		&m_texture)))
	{
		return false;
	}

	// 深度バッファの作成
	if (FAILED(device->CreateDepthStencilSurface(
		width,
		height,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_depthSurface,
		NULL)))
	{
		return false;
	}

	return true;
}

//=============================================================
// [RenderBuffer] テクスチャの破棄
//=============================================================
void RenderBuffer::DestroyTexture()
{
	RenderBuffer::Uninit();
}

//=============================================================
// [RenderBuffer] シーン描画開始
//=============================================================
bool RenderBuffer::BeginScene()
{
	auto device = CRenderer::GetInstance()->GetDevice();
	if (SUCCEEDED(device->BeginScene()))
	{
		return true;
	}
	return false;
}

//=============================================================
// [RenderBuffer] シーン描画終了
//=============================================================
bool RenderBuffer::EndScene()
{
	auto device = CRenderer::GetInstance()->GetDevice();
	if (SUCCEEDED(device->EndScene()))
	{
		return true;
	}
	return false;
}

//=============================================================
// [CameraRenderBuffer] レンダー
//=============================================================
void CameraRenderBuffer::Render()
{
	auto device = CRenderer::GetInstance()->GetDevice();

	if (BeginScene())
	{
		// 画面クリア（バッファクリア＆Zバッファクリア）
		device->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

		if (m_camera != nullptr)
		{
			m_camera->SetCamera();
			m_camera->GetSkybox()->Draw();

			if (CShadow::USE_SHADOW)
				CRenderer::GetInstance()->GetShadow()->Draw(m_camera);
		}
		
		GameObject::DrawAll();
		GameObject::DrawUIAll();

		EndScene();
	}
}