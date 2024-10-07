//=============================================================
//
// カメラコンポーネント [camera.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <component.h>
#include "renderer.h"
#include "skybox.h"

// カメラコンポーネントの定義
class CCamera : public Component
{
public:
	void Init() override;
	void Uninit() override;

	void Update();

	// 取得
	D3DXMATRIX GetProjectionMatrix() { return m_mtxProjection; }
	D3DXMATRIX GetViewMatrix() { return m_mtxView; }
	CSkybox* GetSkybox() { return m_pSkybox; }

	// 設定変数
	float m_fClippingPlanesNear;			// クリップ距離（近い）
	float m_fClippingPlanesFar;				// クリップ距離（遠い）

	// カメラの設定
	void SetCamera();																			// カメラの設定
	D3DXVECTOR3 GetPosR() { return m_posR; }									// 注視点の取得
	void SetParallel(const bool& isParallel) { m_bParallel = isParallel; }		// 平行投影の設定
	bool GetParallel() { return m_bParallel; }											// 平行投影状態の取得
	void SetVisible(const bool& isVisible) { m_bVisible = isVisible; }			// 表示の設定
	bool GetVisible() { return m_bVisible; }											// 表示の取得
	void SetAmbient(const float& fAmbient) { m_fAmbient = fAmbient; }// 明るさの設定
	float GetAmbient() { return m_fAmbient; }										// 明るさの取得

	// ビューポート設定
	void SetViewportPosition(float x, float y) {
		m_viewport.X = (DWORD)x;
		m_viewport.Y = (DWORD)y;
	}
	void SetViewportSize(float width, float height) {
		m_viewport.Width = (DWORD)width;
		m_viewport.Height = (DWORD)height;
	}

	// 背景色の設定
	void SetColor(D3DXCOLOR col) { m_backColor = col; }
	D3DXCOLOR GetColor() { return m_backColor; }

private:
	D3DXCOLOR m_backColor;				// 背景色
	D3DXVECTOR3 m_vecU;					// 上向きベクトル
	D3DXVECTOR3 m_posR;					// 注視点
	float m_fLength;							// 視点と注視点の長さ
	D3DXMATRIX m_mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;				// ビューマトリックス
	D3DVIEWPORT9 m_viewport;			// ビューポート
	bool m_bParallel;							// 平行投影にするか
	bool m_bVisible;								// 表示するか

	CSkybox* m_pSkybox;					// スカイボックス
	float m_fAmbient;							// 明るさ
};

#endif