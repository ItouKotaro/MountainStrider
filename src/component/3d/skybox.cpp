//=============================================================
//
// スカイボックス [skybox.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "skybox.h"
#include "manager.h"
#include <fstream>

// 静的メンバ変数の初期化
const float CSkybox::SKYBOX_SIZE = 10.0f;

//=============================================================
// [CSkybox] 初期化
//=============================================================
void CSkybox::Init()
{
	// 変数の初期化
	for (int i = 0; i < 6; i++)
	{
		m_pVtxBuff[i] = nullptr;		// 頂点バッファ
		m_pTexture[i] = nullptr;		// テクスチャ
	}
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// カラー

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	for (int i = 0; i < 6; i++)
	{
		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff[i], NULL);
		VERTEX_3D* pVtx; //頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff[i]->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		switch (i)
		{
		case UP:
			pVtx[0].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			break;
		case DOWN:
			pVtx[0].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			break;
		case FRONT:
			pVtx[0].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			break;
		case BACK:
			pVtx[0].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			break;
		case LEFT:
			pVtx[0].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			break;
		case RIGHT:
			pVtx[0].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			break;
		}

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点バッファをアンロックする
		m_pVtxBuff[i]->Unlock();
	}

	// テクスチャタイプ
	SetTextureType(TEXTURETYPE::SINGLE);
}

//=============================================================
// [CSkybox] 終了
//=============================================================
void CSkybox::Uninit()
{
	// 破棄する
	for (int i = 0; i < 6; i++)
	{
		// 頂点バッファの破棄
		if (m_pVtxBuff[i] != nullptr)
		{
			m_pVtxBuff[i]->Release();
			m_pVtxBuff[i] = nullptr;
		}
	}
}

//=============================================================
// [CSkybox] 描画
//=============================================================
void CSkybox::Draw()
{
	if (m_pTexture[0] == nullptr)
	{ // テクスチャが設定されていないときは無効
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得
	D3DXMATRIX mtx = m_pCameraTransform->GetTranslationMatrix();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < 6; i++)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff[i], 0, sizeof(VERTEX_3D));

		// テクスチャの設定
		if (m_textureType == TEXTURETYPE::SINGLE)
		{
			pDevice->SetTexture(0, m_pTexture[0]);
		}
		else
		{
			pDevice->SetTexture(0, m_pTexture[i]);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
			0, //描画する最初の頂点インデックス
			2);				//描画するプリミティブ数
	}

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================
// [CSkybox] テクスチャタイプの変更
//=============================================================
void CSkybox::SetTextureType(TEXTURETYPE textureType)
{
	VERTEX_3D* pVtx; // 頂点情報へのポインタ
	m_textureType = textureType;

	for (int i = 0; i < 6; i++)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff[i]->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		switch (m_textureType)
		{
		case CSkybox::SINGLE:
			switch (i)
			{
			case UP:
				pVtx[0].tex = D3DXVECTOR2(1.0f / 4 * 1, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4 * 2, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 1);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 1);
				break;
			case DOWN:
				pVtx[0].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 2);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 2);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f);
				break;
			case BACK:
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / 3 * 1);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 1);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / 3 * 2);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 2);
				break;
			case LEFT:
				pVtx[0].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 1);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 1);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 2);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 2);
				break;
			case FRONT:
				pVtx[0].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 1);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4 * 3, 1.0f / 3 * 1);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 2);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4 * 3, 1.0f / 3 * 2);
				break;
			case RIGHT:
				pVtx[0].tex = D3DXVECTOR2(1.0f / 4 * 3, 1.0f / 3 * 1);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f / 3 * 1);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 4 * 3, 1.0f / 3 * 2);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f / 3 * 2);
				break;
			}
			break;
		case CSkybox::SPLIT:
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff[i]->Unlock();
	}
}

//=============================================================
// [CSkybox] テクスチャの設定
//=============================================================
void CSkybox::SetTexture(const int& idx, const std::string& path)
{
	if (0 <= idx && idx < 6)
	{
		CDataTexture* pTexData = CDataManager::GetInstance()->RefTexture(path);
		m_pTexture[idx] = pTexData->GetTexture();
	}
}

//=============================================================
// [CSkybox] スカイボックスの読み込み
//=============================================================
void CSkybox::LoadSkybox(const std::string& sPath)
{
	// jsonファイルを読み込む
	std::ifstream ifs(sPath.c_str());

	if (ifs.fail())
	{ // ファイルの読み込みに失敗
		return;
	}

	// json形式に変換
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto jSkybox = json::parse(sInputData);

	// タイプを設定する
	if (jSkybox.contains("type"))
	{
		if (jSkybox["type"] == "single")
		{ // 1枚
			SetTextureType(TEXTURETYPE::SINGLE);
		}
		else if (jSkybox["type"] == "split")
		{ // 複数枚（6枚）
			SetTextureType(TEXTURETYPE::SPLIT);
		}
	}

	// テクスチャを変更する
	if (jSkybox.contains("texture"))
	{
		for (unsigned int i = 0; i < jSkybox["texture"].size(); i++)
		{
			SetTexture(i, jSkybox["texture"][i]);
		}
	}

	ifs.close();
}