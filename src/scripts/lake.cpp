//=============================================================
//
// 湖 [lake.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "lake.h"
#include "renderer.h"
#include "internal/data_manager.h"
#include "scene/game.h"
#include "component/3d/collision.h"

//=============================================================
// [LakeManager] 初期化
//=============================================================
void LakeManager::Init(Terrain* terrain, const std::string& path)
{
	// SEを読み込む
	m_underwaterSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\ENVIRONMENTAL\\underwater.mp3", FMOD_2D | FMOD_LOOP_NORMAL);
	m_diveWaterSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\divewater.mp3", FMOD_2D);
	
	m_audioPlayer = new GameObject();
	m_audioPlayer->AddComponent<AudioSource>();
	m_audioPlayer->GetComponent<AudioSource>()->Play(m_underwaterSE);
	m_audioPlayer->GetComponent<AudioSource>()->SetPause(true);

	// バイクを取得する
	m_vehicle = GameObject::Find("Vehicle")->GetComponent<CVehicle>();

	// jsonファイルを読み込む
	std::ifstream ifs(path.c_str());

	if (ifs.fail())
	{ // ファイルの読み込みに失敗
		return;
	}

	// json形式に変換
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto jInput = json::parse(sInputData);

	if (jInput.contains("terrain") && jInput["terrain"].contains("lake"))
	{
		m_enabled = true;

		// 湖の高さを設定する
		if (jInput["terrain"]["lake"].contains("height"))
		{
			float height = jInput["terrain"]["lake"]["height"];
			float rate = (height + 1.0f) / static_cast<float>(2.0f);
			m_height = terrain->GetMinHeight() + (terrain->GetMaxHeight() - terrain->GetMinHeight()) * rate;
		}

		// 燃料ダメージを設定する
		if (jInput["terrain"]["lake"].contains("fuel_damage"))
		{
			m_fuelDamage = jInput["terrain"]["lake"]["fuel_damage"];
		}

		// 耐久値ダメージを設定する
		if (jInput["terrain"]["lake"].contains("endurance_damage"))
		{
			m_enduranceDamage = jInput["terrain"]["lake"]["endurance_damage"];
		}

		// 基本色を設定する
		if (jInput["terrain"]["lake"].contains("base_color"))
		{
			m_baseColor = D3DCOLOR_RGBA(
				jInput["terrain"]["lake"]["base_color"][0], jInput["terrain"]["lake"]["base_color"][1], jInput["terrain"]["lake"]["base_color"][2], jInput["terrain"]["lake"]["base_color"][3]
			);
		}

		// テクスチャを設定する
		if (jInput["terrain"]["lake"].contains("textures"))
		{
			for (auto itr = jInput["terrain"]["lake"]["textures"].begin(); itr != jInput["terrain"]["lake"]["textures"].end(); itr++)
			{
				GameObject* lakeFieldObj = new GameObject();
				lakeFieldObj->transform->SetPos(0.0f, m_height, 0.0f);
				auto lakeField = lakeFieldObj->AddComponent<LakeField>();

				// サイズを設定する
				lakeField->SetSize(Terrain::TERRAIN_DISTANCE * 1.5f, Terrain::TERRAIN_DISTANCE * 1.5f);

				std::string lakePath = "";
				int lakeLoop = 1;
				D3DXVECTOR2 lakeMove = { 0.0f, 0.0f };

				// テクスチャパスの取得
				if (itr.value().contains("path"))
				{
					lakePath = itr.value()["path"];
				}

				// ループ数の取得
				if (itr.value().contains("loop"))
				{
					lakeLoop = itr.value()["loop"];
				}

				// 移動量の取得
				if (itr.value().contains("move"))
				{
					lakeMove.x = itr.value()["move"][0];
					lakeMove.y = itr.value()["move"][1];
				}

				// テクスチャを設定する
				lakeField->SetTexture(lakePath, lakeLoop, lakeMove);

				// 色を設定する
				if (itr.value().contains("color"))
				{
					lakeField->SetColor(D3DCOLOR_RGBA(itr.value()["color"][0], itr.value()["color"][1], itr.value()["color"][2], itr.value()["color"][3]));
				}
			}
		}
	}

	ifs.close();
}

//=============================================================
// [LakeManager] 終了
//=============================================================
void LakeManager::Uninit()
{
	AudioManager::GetInstance()->RemoveClip(m_underwaterSE);
	AudioManager::GetInstance()->RemoveClip(m_diveWaterSE);
}

//=============================================================
// [LakeManager] 更新
//=============================================================
void LakeManager::Update()
{
	if (!m_vehicle->gameObject->GetActive()) return;

	D3DXVECTOR3 pos = m_vehicle->transform->GetWPos();
	if (m_enabled && pos.y <= m_height - LAKE_INDEPTH &&
		-Terrain::TERRAIN_DISTANCE_HALF <= pos.x && pos.x <= Terrain::TERRAIN_DISTANCE_HALF &&
		-Terrain::TERRAIN_DISTANCE_HALF <= pos.z && pos.z <= Terrain::TERRAIN_DISTANCE_HALF)
	{
		// ダメージを与える
		if (m_vehicle->gameObject->GetActive())
		{
			m_vehicle->AddDamage(m_enduranceDamage);
			m_vehicle->AddFuel(-m_fuelDamage);
		}

		// 浮力
		m_vehicle->gameObject->GetComponent<CRigidBody>()->GetRigidBody()->applyCentralForce(
			btVector3(0.0f, 15000.0f, 0.0f)
		);

		btVector3 linerVelocity = m_vehicle->gameObject->GetComponent<CRigidBody>()->GetRigidBody()->getLinearVelocity();
		linerVelocity.setX(linerVelocity.getX() * LAKE_RESISTANCE);
		linerVelocity.setZ(linerVelocity.getZ() * LAKE_RESISTANCE);
		m_vehicle->gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setLinearVelocity(linerVelocity);

		// 音を再生する
		m_audioPlayer->GetComponent<AudioSource>()->SetPause(false);

		// 水中に入ったときの処理
		if (!m_isUnderWater)
		{
			m_audioPlayer->GetComponent<AudioSource>()->PlayOneShot(m_diveWaterSE);
		}

		// 水中状態
		m_isUnderWater = true;
	}
	else
	{
		// 音を停止させる
		m_audioPlayer->GetComponent<AudioSource>()->SetPause(true);

		// 水中から出たとき処理
		if (m_isUnderWater)
		{
			m_audioPlayer->GetComponent<AudioSource>()->PlayOneShot(m_diveWaterSE);
		}

		// 水中状態
		m_isUnderWater = false;
	}
}



//=============================================================
// [LakeField] 初期化
//=============================================================
void LakeField::Init()
{
	// 変数の初期化
	m_pVtxBuff = nullptr;											// 頂点バッファ
	m_pTexture = nullptr;										// テクスチャ
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// カラー
	m_move = { 0.0f, 0.0f };
	m_texMove = { 0.0f, 0.0f };

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	VERTEX_3D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	m_pVtxBuff->Unlock();
}

//=============================================================
// [LakeField] 終了
//=============================================================
void LakeField::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================
// [LakeField] 更新
//=============================================================
void LakeField::Update()
{
	// テクスチャの移動値を加算する
	m_texMove += m_move;

	// 制限をつける
	if (m_texMove.x <= -1.0f)
		m_texMove.x += 1.0f;
	else if (m_texMove.x >= 1.0f)
		m_texMove.x -= 1.0f;
	if (m_texMove.y <= -1.0f)
		m_texMove.y += 1.0f;
	else if (m_texMove.y >= 1.0f)
		m_texMove.y -= 1.0f;

	// テクスチャを更新する
	VERTEX_3D* pVtx; //頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	float numLoop = static_cast<float>(m_loopNum);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f) + m_texMove;
	pVtx[1].tex = D3DXVECTOR2(numLoop, 0.0f) + m_texMove;
	pVtx[2].tex = D3DXVECTOR2(0.0f, numLoop) + m_texMove;
	pVtx[3].tex = D3DXVECTOR2(numLoop, numLoop) + m_texMove;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================
// [LakeField] 描画
//=============================================================
void LakeField::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得
	D3DXMATRIX mtx = transform->GetMatrix();

	Component::BeginPass();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングをオフにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ワールドマトリックスの設定
	if (!IsEnabledShader()) pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	if (!IsEnabledShader()) pDevice->SetTexture(0, m_pTexture);

	Shader::ParamData paramData;
	paramData.color = m_color;
	paramData.texture = m_pTexture;
	paramData.mtx = transform->GetMatrix();
	Component::SetParam(paramData);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2);				//描画するプリミティブ数

	Component::EndPass();

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリングを元に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=============================================================
// [LakeField] サイズの設定
//=============================================================
void LakeField::SetSize(const float& x, const float& y)
{
	VERTEX_3D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-x * 0.5f, 0.0f, y * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(x * 0.5f, 0.0f, y * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-x * 0.5f, 0.0f, -y * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(x * 0.5f, 0.0f, -y * 0.5f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================
// [LakeField] テクスチャを設定する
//=============================================================
void LakeField::SetTexture(const std::string& texturePath, const int& loopNum, const D3DXVECTOR2& move)
{
	// テクスチャを設定する
	BindTexture(CDataManager::GetInstance()->RefTexture(texturePath)->GetTexture());

	// ループ回数を設定する
	m_loopNum = loopNum;

	// 移動値を設定する
	m_move = move;
}

//=============================================================
// [LakeField] 色を設定する
//=============================================================
void LakeField::SetColor(const D3DXCOLOR& color)
{
	m_color = color;

	VERTEX_3D* pVtx; //頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラー
	pVtx[0].col = m_color	;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}