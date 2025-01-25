//=============================================================
//
// パーティクル [particle.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "particle.h"
#include "renderer.h"
#include "internal/data_manager.h"
#include "manager.h"

//=============================================================
// [Particle] 初期化
//=============================================================
void Particle::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_vtxBuff, NULL);
	VERTEX_3D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	m_vtxBuff->Unlock();
}

//=============================================================
// [Particle] 終了
//=============================================================
void Particle::Uninit()
{
	// 頂点バッファの破棄
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// [Particle] 更新
//=============================================================
void Particle::Update()
{
}

//=============================================================
// [Particle] 描画
//=============================================================
void Particle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得
	D3DXMATRIX mtx;
	D3DXMATRIX mtxView;

	Component::BeginPass();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファの書き込みを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックスの設定
	if (!IsEnabledShader()) pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// マトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&mtx, NULL, &mtxView);	// 逆行列を求める
	mtx._41 = 0.0f;
	mtx._42 = 0.0f;
	mtx._43 = 0.0f;

	D3DXMATRIX rotMtx;
	D3DXMatrixRotationZ(&rotMtx, transform->GetWRot().z);
	mtx *= rotMtx;

	// オブジェクトのマトリックスを掛ける
	D3DXMATRIX mtxTrans = transform->GetTranslationMatrix();
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	if (!IsEnabledShader()) pDevice->SetTexture(0, m_texture);

	Shader::ParamData paramData;
	paramData.color = m_color;
	paramData.texture = m_texture;
	paramData.mtx = mtx;
	Component::SetParam(paramData);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2);				//描画するプリミティブ数

	Component::EndPass();

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zバッファの書き込みを有効に戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効に戻す
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================
// [Particle] サイズ変更
//=============================================================
void Particle::SetSize(const float& x, const float& y)
{
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-x / 2, y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(x / 2, y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-x / 2, -y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(x / 2, -y / 2, 0.0f);

	// 頂点バッファのアンロック
	m_vtxBuff->Unlock();
}

//=============================================================
// [Particle] テクスチャの設定
//=============================================================
void Particle::SetTexture(const std::string& path)
{
	BindTexture(CDataManager::GetInstance()->RefTexture(path)->GetTexture());
}


//=============================================================
// [ParticleSystem] 初期化
//=============================================================
void ParticleSystem::Init()
{
	// エミッションを生成する
	m_emission = new ParticleModule::Emission();

	// シェイプを生成する
	m_shape = new ParticleShape::SphereShape();

	// テクスチャを生成する
	m_texture = new ParticleModule::Texture();

	// 移動式
	m_moveEquation = [](ParticleSystem::ParticleData* data) {return data->move; };
}

//=============================================================
// [ParticleSystem] 終了
//=============================================================
void ParticleSystem::Uninit()
{
	// エミッションの破棄
	if (m_emission != nullptr)
	{
		delete m_emission;
		m_emission = nullptr;
	}

	// シェイプの破棄
	if (m_shape != nullptr)
	{
		delete m_shape;
		m_shape = nullptr;
	}

	// テクスチャの破棄
	if (m_texture != nullptr)
	{
		delete m_texture;
		m_texture = nullptr;
	}

	// パーティクルの終了/解放
	for (auto itr = m_particleData.begin(); itr != m_particleData.end(); itr++)
	{
		if ((*itr).particle != nullptr)
		{
			(*itr).particle->Uninit();
			delete (*itr).particle;
		}
	}
	m_particleData.clear();
}

//=============================================================
// [ParticleSystem] 更新
//=============================================================
void ParticleSystem::Update()
{
	// 生成処理
	UpdateGenerator();

	// パーティクルの更新
	UpdateParticles();

	// 破棄の更新
	UpdateDestroy();
}

//=============================================================
// [ParticleSystem] 生成処理
//=============================================================
void ParticleSystem::UpdateGenerator()
{
	// 必須設定がされているかをチェックする
	if (m_emission == nullptr || m_shape == nullptr)
		return;

	// エミッションによる放出量の決定
	m_emission->SetElapsedTime(CManager::GetInstance()->GetDeltaTime());
	int numPerticles = m_emission->GetResult();

	// パーティクルの生成
	for (int i = 0; i < numPerticles; i++)
	{
		// シェイプから結果を取得する（位置と方向）
		ParticleModule::Shape::ResultData shapeResult = m_shape->GetResult();
		D3DXVec3Normalize(&shapeResult.direction, &shapeResult.direction);

		// 力を取得する
		float power = Benlib::RandomFloat(m_power.min, m_power.max);

		// 生存時間を取得する
		int lifetime = Benlib::RandomInt(m_lifetime.min, m_lifetime.max);

		// パーティクルデータを設定する
		ParticleData data;
		data.move = shapeResult.direction * power;
		data.color = m_color;
		data.size = Benlib::RandomFloat(m_size.min, m_size.max);
		data.lifeCounter = lifetime;
		data.destroyCounter = DESTROY_TIME;
		data.use = true;

		// パーティクルを再利用する
		bool findParticle = false;
		for (auto itr = m_particleData.begin(); itr != m_particleData.end(); itr++)
		{
			ParticleData particle = *itr;
			if (!particle.use)
			{ // 使われていないパーティクルのとき
				data.particle = particle.particle;
				m_particleData.erase(itr);
				findParticle = true;
				break;
			}
		}

		// 使われていないパーティクルが見つからなかったとき（作成）
		if (!findParticle)
		{
			data.particle = new SingleComponent<Particle>();
			data.particle->Init();
		}

		// 生成設定
		data.particle->SetSize(data.size, data.size);
		data.particle->BindTexture(m_texture->GetResult());
		data.particle->transform->SetPos(transform->GetWPos() + shapeResult.position);
		data.particle->transform->SetRot(Benlib::RandomFloat(m_angle.min, m_angle.max));

		// 追加する
		m_particleData.push_back(data);
	}
}

//=============================================================
// [ParticleSystem] パーティクルの更新
//=============================================================
void ParticleSystem::UpdateParticles()
{
	for (auto itr = m_particleData.begin(); itr != m_particleData.end(); itr++)
	{
		ParticleData* data = &(*itr);
		if (data->use)
		{
			// 移動量を加算する
			data->particle->transform->Translate(m_moveEquation(data));

			// 渦を考慮する
			D3DXVECTOR3 point = data->particle->transform->GetWPos() - transform->GetWPos();
			D3DXMATRIX mtx;
			D3DXMatrixRotationY(&mtx, m_vortex);
			D3DXVec3TransformCoord(&point, &point, &mtx);
			data->particle->transform->SetPos(transform->GetWPos() + point);

			// 重力を加算する
			data->move.y += m_gravity;

			// サイズ
			data->size += m_fluctuationSize;
			data->particle->SetSize(data->size, data->size);
			if (data->size <= 0.0f) data->use = false;

			// カラー
			data->color.a -= m_fluctuationAlpha;
			data->particle->SetColor(data->color);
			if (data->color.a <= 0.0f) data->use = false;

			// 生存時間
			data->lifeCounter--;
			if (data->lifeCounter <= 0)
			{ // 寿命がなくなったとき
				data->use = false;
				continue;
			}
		}
	}
}

//=============================================================
// [ParticleSystem] 破棄の更新
//=============================================================
void ParticleSystem::UpdateDestroy()
{
	// 使われなくなったパーティクルの破棄カウンタを進める
	int idx = static_cast<int>(m_particleData.size());
	for (int i = idx - 1; i >= 0; i--)
	{
		ParticleData* data = &m_particleData[i];

		if (!data->use)
		{ // 使われていないとき
			data->destroyCounter--;
			
			// 破棄のタイミング
			if (data->destroyCounter <= 0)
			{
				data->particle->Uninit();
				delete data->particle;
				m_particleData.erase(m_particleData.begin() + i);
			}
		}
	}
}

//=============================================================
// [ParticleSystem] 描画
//=============================================================
void ParticleSystem::Draw()
{
	// 現在のカメラから一定距離離れている場合はスキップ
	if (CCamera::GetCurrentCamera() != nullptr &&
		Benlib::PosPlaneDistance(transform->GetWPos(), CCamera::GetCurrentCamera()->transform->GetWPos()) > RENDER_DISTANCE)
	{
		return;
	}

	for (auto itr = m_particleData.begin(); itr != m_particleData.end(); itr++)
	{
		if ((*itr).use)
		{
			(*itr).particle->SetShader(this->GetShaderInfo());
			(*itr).particle->Draw();
		}
	}
}

//=============================================================
// [ParticleSystem] エミッションの設定
//=============================================================
void ParticleSystem::SetEmission(ParticleModule::Emission* emission)
{
	if (m_emission != nullptr)
	{
		delete m_emission;
		m_emission = nullptr;
	}

	m_emission = emission;
}

//=============================================================
// [ParticleSystem] シェイプの設定
//=============================================================
void ParticleSystem::SetShape(ParticleModule::Shape* shape)
{
	if (m_shape != nullptr)
	{
		delete m_shape;
		m_shape = nullptr;
	}

	m_shape = shape;
}


//=============================================================
// [Emission] 放出量
//=============================================================
int ParticleModule::Emission::GetResult()
{
	if (m_elapsedTime >= m_rateOverTime)
	{
		int nNum = static_cast<int>((m_elapsedTime - fmod(m_elapsedTime, m_rateOverTime)) / m_rateOverTime);
		m_elapsedTime -= m_rateOverTime * nNum;
		return nNum;
	}
	return 0;
}

//=============================================================
// [Texture] テクスチャ追加
//=============================================================
void ParticleModule::Texture::AddTexture(const std::string& path, const int& chance)
{
	TextureData data;
	data.texture = CDataManager::GetInstance()->RefTexture(path)->GetTexture();
	data.chance = chance;
	m_textures.push_back(data);
}

//=============================================================
// [Texture] テクスチャ
//=============================================================
LPDIRECT3DTEXTURE9 ParticleModule::Texture::GetResult()
{
	int allChance = 0;
	for (auto itr = m_textures.begin(); itr != m_textures.end(); itr++)
		allChance += (*itr).chance;

	if (allChance <= 0)
		return nullptr;

	// ランダム
	int randNum = rand() % allChance;
	int beforeNum = 0;
	for (auto itr = m_textures.begin(); itr != m_textures.end(); itr++)
	{
		if (beforeNum <= randNum && randNum < beforeNum + (*itr).chance)
			return (*itr).texture;
		else
			beforeNum += (*itr).chance;
	}
	return nullptr;
}

//=============================================================
// [SphereShape] テクスチャの設定
//=============================================================
ParticleModule::Shape::ResultData ParticleShape::SphereShape::GetResult()
{
	float x = Benlib::RandomFloat(0.0f, D3DX_PI);
	float y = Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f);
	ResultData data;
	data.position = D3DXVECTOR3(sinf(y) * m_radius, cosf(x) * m_radius, cosf(y) * m_radius);
	D3DXVec3Normalize(&data.direction, &data.position);
	return data;
}