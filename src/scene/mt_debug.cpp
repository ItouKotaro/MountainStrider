//=============================================================
//
// マウンテンのデバッグシーン [mt_debug.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "mt_debug.h"

#include "component/3d/camera.h"
#include "component/3d/movement_space.h"
#include "component/3d/light.h"
#include "component/3d/meshfield.h"

#include <DTL.hpp>
#include "DTL/Storage/FilePNG.hpp"

//=============================================================
// [CMountainDebug] 初期化
//=============================================================
void CMountainDebug::Init()
{
	// カメラ
	m_pCamera = new GameObject;
	m_pCamera->SetName("Camera");
	m_pCamera->SetTag("Camera");
	m_pCamera->AddComponent<CCamera>();
	m_pCamera->GetComponent<CCamera>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
	m_pCamera->GetComponent<CCamera>()->m_fClippingPlanesFar = 5000.0f;
	m_pCamera->AddComponent<CMovementSpace>();

	// ライト
	GameObject* pLight = new GameObject;
	CD3DLight::SetDefaultD3DLight(pLight);

	// メッシュフィールド
	m_pField = new GameObject;
	m_pField->AddComponent<CMeshField>()->Create(249, 249, 1.0f);
	m_pField->transform->Translate(0.0f, -100.0f, -100.0f);

	GameObject* pSLight = new GameObject;
	pSLight->AddComponent<CLight>()->SetIntensity(500.0f);
	pSLight->transform->Translate(0.0f, 100.0f, 0.0f);
}

//=============================================================
// [CMountainDebug] 終了
//=============================================================
void CMountainDebug::Uninit()
{
}

//=============================================================
// [CMountainDebug] 更新
//=============================================================
void CMountainDebug::Update()
{
	if (INPUT_INSTANCE->onTrigger("o"))
	{
		std::array<std::array<int, 250>, 250> matrix{ {} };
		dtl::shape::PerlinSolitaryIsland<int>(0.6f, 0.4f, 6.0f, 6, 100).draw(matrix);
		dtl::random::seed = 12;

		dtl::storage::FilePNG<int>("file_sample.png", 3).write(matrix, [](const int value, unsigned char* const color) {
			if (value < 20)
			{
				color[0] = 41;
				color[1] = 40;
				color[2] = 159;
			}
			else if (value < 40)
			{
				color[0] = 101;
				color[1] = 163;
				color[2] = 56;
			}
			else if (value < 60)
			{
				color[0] = 223;
				color[1] = 203;
				color[2] = 140;
			}
			else if (value < 80)
			{
				color[0] = 9;
				color[1] = 100;
				color[2] = 5;
			}
			else
			{
				color[0] = 164;
				color[1] = 143;
				color[2] = 50;
			}
			});

		// 地形を変更する
		for (int x = 0; x < 250; x++)
		{
			for (int y = 0; y < 250; y++)
			{
				m_pField->GetComponent<CMeshField>()->SetHeight(x, y, matrix[x][y]);
			}
		}

		// テクスチャを適用する
		m_pField->GetComponent<CMeshField>()->SetTexture("file_sample.png");
	}
}

//=============================================================
// [CMountainDebug] 描画
//=============================================================
void CMountainDebug::Draw()
{
}