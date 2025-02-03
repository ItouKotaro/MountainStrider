//=============================================================
//
// タイトル [title.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "title.h"

#include "component/2d/text.h"
#include "component/2d/polygon.h"
#include "component/3d/motion.h"
#include "scene/game.h"
#include "renderer.h"
#include "render/shadow_rb.h"
#include "manager.h"
#include "component/3d/meshfield.h"
#include "component/3d/mesh.h"
#include "component/other/audio.h"

#include "scripts/mode/terminal.h"

using namespace noise;

AudioClip selectMoveSE;
AudioClip fireSE;

bool g_once = true;

//=============================================================
// [CTitleScene] 初期化
//=============================================================
void CTitleScene::Init()
{
	if (!selectMoveSE) selectMoveSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\select_move.mp3", FMOD_2D);
	if (!fireSE) fireSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\TITLE\\fire.mp3", FMOD_3D | FMOD_LOOP_NORMAL);

	// タイトルロゴ
	GameObject* titleLogo = new GameObject();
	titleLogo->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\logo.png");
	titleLogo->transform->SetSize(450.0f, 450.0f);
	titleLogo->transform->SetPos(80.0f, 30.0f);

	// 項目バーを作成する
	m_selectBar = new GameObject();
	m_selectBar->AddComponent<CPolygon>();
	m_selectBar->transform->SetSize(10.0f, 100.0f);
	m_selectBar->AddComponent<AudioSource>();

	// 項目を作成する
	m_sStart = (new GameObject())->AddComponent<CText>();
	m_sStart->transform->SetPos(150.0f, SELECT_START_Y);
	m_sStart->SetText("スタート");
	m_sStart->SetFont("07鉄瓶ゴシック");

	m_sEnd = (new GameObject())->AddComponent<CText>();
	m_sEnd->transform->SetPos(150.0f, SELECT_START_Y + SELECT_SPACE * 1);
	m_sEnd->SetText("終了する");
	m_sEnd->SetFont("07鉄瓶ゴシック");

	// 地形を生成する
	InitTerrain();

	// シーン
	m_titleScene = GameObject::LoadPrefab("data\\PREFAB\\title_scene.pref");
	m_titleScene->transform->SetPos(0.0f, 0.0f, 10.0f);
	m_titleScene->GetComponent<CMotionManager>()->Play("0");

	// テント
	GameObject* tentObj = new GameObject();
	tentObj->transform->SetPos(-20.0f, 0.0f, 40.0f);
	tentObj->transform->SetRot(0.0f, D3DX_PI * 0.3f, 0.0f);
	tentObj->transform->SetScale(1.2f);
	tentObj->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\TITLE\\tent.x");

	// バックパック
	GameObject* backpackObj = new GameObject();
	backpackObj->transform->SetPos(-12.5f, 0.5f, 35.0f);
	backpackObj->transform->SetRot(0.0f, D3DX_PI * 1.7f, -0.3f);
	backpackObj->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\TITLE\\backpack.x");

	// 焚火
	GameObject* firecampObj = new GameObject();
	firecampObj->transform->SetPos(25.0f, 0.5f, 5.0f);
	firecampObj->transform->SetScale(0.7f);
	firecampObj->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\TITLE\\campfire.x");

	// 焚火サウンド
	firecampObj->AddComponent<AudioSource>()->Play(fireSE);
	firecampObj->GetComponent<AudioSource>()->GetChannel()->setVolume(10.0f);

	// 焚火パーティクル
	GameObject* fireParticleObj = new GameObject();
	fireParticleObj->SetParent(firecampObj);
	auto fireParticle = fireParticleObj->AddComponent<ParticleSystem>();
	fireParticle->SetSize(1.5f, 3.0f);
	fireParticle->SetFluctuationSize(-0.04f);
	fireParticle->SetAngle(0.0f, D3DX_PI);
	fireParticle->GetEmission()->SetRateOverTime(20.0f);
	fireParticle->GetTexture()->AddTexture("data\\TEXTURE\\PARTICLE\\pixel.png");
	CampFireParticleShape* fireP = new CampFireParticleShape();
	fireP->SetRadius(3.0f);
	fireParticle->SetShape(fireP);
	fireParticle->SetPower(0.2f, 0.4f);
	fireParticle->SetLifetime(50, 80);
	fireParticle->SetColor(D3DCOLOR_RGBA(255, 133, 25, 255));
	fireParticle->SetFluctuationAlpha(0.03f);

	fireParticle = fireParticleObj->AddComponent<ParticleSystem>();
	fireParticle->SetSize(1.5f, 3.0f);
	fireParticle->SetFluctuationSize(-0.04f);
	fireParticle->SetAngle(0.0f, D3DX_PI);
	fireParticle->GetEmission()->SetRateOverTime(20.0f);
	fireParticle->GetTexture()->AddTexture("data\\TEXTURE\\PARTICLE\\pixel.png");
	fireP = new CampFireParticleShape();
	fireP->SetRadius(6.0f);
	fireParticle->SetShape(fireP);
	fireParticle->SetPower(0.2f, 0.4f);
	fireParticle->SetLifetime(50, 80);
	fireParticle->SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));
	fireParticle->SetFluctuationAlpha(0.03f);

	// カメラ
	GameObject* pCamera = new GameObject();
	pCamera->AddComponent<CCamera>()->GetSkybox()->LoadSkybox("data\\SKYBOX\\daylight00.json");
	pCamera->GetComponent<CCamera>()->m_fClippingPlanesFar = 5000.0f;
	pCamera->transform->SetPos(0.0f, 20.0f, -48.0f);
	pCamera->AddComponent<AudioListener>();

	// ライト
	GameObject* pLight = new GameObject();
	pLight->AddComponent<CCamera>();
	pLight->transform->SetPos(-30.0f, 100.0f, -20.0f);
	pLight->transform->LookAt({ 0.0f, 0.0f, 5.0f });

	// 起動クレジット
	m_credit = nullptr;
	if (g_once)
	{
		// 起動クレジット背景
		GameObject* creditBG = new GameObject();
		creditBG->AddComponent<CPolygon>();
		creditBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);

		// クレジットロゴ
		GameObject* creditObj = new GameObject();
		m_credit = creditObj->AddComponent<LaunchCredit>();
		m_credit->AddCredit("data\\TEXTURE\\CREDIT\\FMOD.png", { 728.0f, 192.0f });
		m_credit->SetShowTime(4.0f);
		m_credit->SetBG(creditBG->GetComponent<CPolygon>());
		m_credit->Start();

		g_once = false;
	}


	// 影用レンダーバッファの設定
	CRenderer::GetInstance()->RegisterRenderBuffer<ShadowRenderBuffer>("main");
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(pCamera->GetComponent<CCamera>());
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetLightCamera(pLight->GetComponent<CCamera>());
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetAmbient({ 0.7f, 0.7f, 0.7f, 1.0f });
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetShadowPoint({ 0.0f, 0.0f, 0.0f });
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetShadowRange(600.0f);

	Main::SetShowCursor(true);

	// ゲームをリセットする
	CGameScene::ResetGame();

	// モードを破棄する
	ModeManager::GetInstance()->LastUninit();
}

//=============================================================
// [CTitleScene] 終了
//=============================================================
void CTitleScene::Uninit()
{
	m_field->Destroy();
	CCollision::RemoveCollision(m_field);
	CPhysics::GetInstance()->Update();

	if (m_terrainData != nullptr)
	{
		delete[] m_terrainData;
		m_terrainData = nullptr;
	}

	if (m_terrainShape != nullptr)
	{
		delete m_terrainShape;
		m_terrainShape = nullptr;
	}
}

//=============================================================
// [CTitleScene] 更新
//=============================================================
void CTitleScene::Update()
{
	if (m_credit != nullptr && !m_credit->IsEnded()) return;

	// 操作の更新
	UpdateControl();

	// 項目バーを移動させる
	m_selectBar->transform->SetPos(100.0f, SELECT_START_Y - 5.0f + SELECT_SPACE * m_select);

	// 透明度のリセット
	m_sStart->SetAlpha(NOT_SELECTED_ALPHA);
	m_sEnd->SetAlpha(NOT_SELECTED_ALPHA);

	// 項目処理
	switch (m_select)
	{
	case CTitleScene::SELECT_START:
		m_sStart->SetAlpha(SELECTED_ALPHA);
		break;
	case CTitleScene::SELECT_END:
		m_sEnd->SetAlpha(SELECTED_ALPHA);
		break;
	}

	// 確定処理
	if (INPUT_INSTANCE->onTrigger("space") || INPUT_INSTANCE->onTrigger("enter") ||
		INPUT_INSTANCE->onTrigger("p:a") || INPUT_INSTANCE->onTrigger("p:start"))
	{
		// ゲームシーンを取得する
		auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

		switch (m_select)
		{
		case CTitleScene::SELECT_START:
			ModeManager::GetInstance()->SetMode(new TerminalMode());
			CSceneManager::GetInstance()->SetScene("game");
			break;
		case CTitleScene::SELECT_END:
			Main::ExitApplication();
			break;
		}
	}
}

//=============================================================
// [CTitleScene] 描画
//=============================================================
void CTitleScene::Draw()
{
}

//=============================================================
// [CTitleScene] 操作の更新
//=============================================================
void CTitleScene::UpdateControl()
{
	auto padInfo = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad;
	short stickX = padInfo.sThumbLX;
	short stickY = padInfo.sThumbLY;
	static int stickCounter = 0;

	SELECT oldSelect = m_select;

	if (stickCounter > 0) stickCounter--;
	if (stickCounter <= 0 && stickY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_select = static_cast<SELECT>(m_select - 1);
		stickCounter = 15;
	}
	if (stickCounter <= 0 && stickY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_select = static_cast<SELECT>(m_select + 1);
		stickCounter = 15;
	}


	// 上
	if (INPUT_INSTANCE->onTrigger("w") || INPUT_INSTANCE->onTrigger("up") ||
		INPUT_INSTANCE->onTrigger("p:up"))
	{
		m_select = static_cast<SELECT>(m_select - 1);
	}

	// 下
	if (INPUT_INSTANCE->onTrigger("s") || INPUT_INSTANCE->onTrigger("down") ||
		INPUT_INSTANCE->onTrigger("p:down"))
	{
		m_select = static_cast<SELECT>(m_select + 1);
	}

	// 正しい数値に直す
	if (m_select < 0) m_select = SELECT_START;
	if (m_select >= SELECT_MAX) m_select = static_cast<SELECT>(SELECT_MAX - 1);

	// 前回の選択と異なるとき
	if (oldSelect != m_select)
	{
		m_selectBar->GetComponent<AudioSource>()->PlayOneShot(selectMoveSE);
	}
}

//=============================================================
// [CTitleScene] 地形の初期化
//=============================================================
void CTitleScene::InitTerrain()
{
	GameObject* pShadowField = new GameObject();
	pShadowField->AddComponent<CMeshField>()->Create(30, 30, 30.0f);
	pShadowField->transform->SetPos(0.0f, -0.5f, TERRAIN_SCALE * 15 );

	m_field = new GameObject();
	m_field->AddComponent<CMeshField>()->Create(30, 30, 30.0f);
	m_field->transform->SetPos(0.0f, 0.0f, TERRAIN_SCALE * 15);
	CCollision::Create(m_field);

	module::Perlin myModule;
	utils::NoiseMap heightMap;

	// シードを設定する
	srand((unsigned int)time(NULL));
	myModule.SetSeed(rand());

	module::ScaleBias scaled;
	scaled.SetSourceModule(0, myModule);
	scaled.SetScale(50.0f);

	// 地形を生成する
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(scaled);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(TERRAIN_SIZE, TERRAIN_SIZE);
	heightMapBuilder.SetBounds(0.0, 3.0, 0.0, 3.0);
	heightMapBuilder.Build();

	// 地形データ
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE];

	// 高さを変更する
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = heightMap.GetValue(x, y);

			if (y >= 27)
			{
				pShadowField->GetComponent<CMeshField>()->SetHeight(x, y, 0.0f);
				m_field->GetComponent<CMeshField>()->SetHeight(x, y, 0.0f);
				continue;
			}
			pShadowField->GetComponent<CMeshField>()->SetHeight(x, y, heightMap.GetValue(x, y));
			m_field->GetComponent<CMeshField>()->SetHeight(x, y, heightMap.GetValue(x, y));
		}
	}

	// テクスチャを生成する
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.ClearGradient();
	renderer.AddGradientPoint(1.0f, utils::Color(11, 122, 0, 255));
	renderer.AddGradientPoint(0.0f, utils::Color(101, 191, 125, 255));
	renderer.AddGradientPoint(-1.0f, utils::Color(50, 166, 8, 255));
	renderer.EnableLight();
	renderer.SetLightContrast(0.01f);
	renderer.SetLightBrightness(1.5);
	renderer.SetLightIntensity(4.5f);
	renderer.Render();

	// ファイルに書き出す
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("data\\title_terrain.bmp");
	writer.WriteDestFile();

	// テクスチャを適用する
	CDataManager::GetInstance()->RemoveData("data\\title_terrain.bmp");
	m_field->GetComponent<CMeshField>()->SetTexture("data\\title_terrain.bmp");

	// 地形に当たり判定をつける
	m_terrainShape = new btHeightfieldTerrainShape(TERRAIN_SIZE, TERRAIN_SIZE, m_terrainData, 1, -30000, 30000, 1, PHY_FLOAT, false);
	m_terrainShape->setLocalScaling(btVector3(TERRAIN_SCALE, 1.0f, TERRAIN_SCALE));
	CCollision::GetCollision(m_field)->GetGhostObject()->setCollisionShape(m_terrainShape);
	CPhysics::GetInstance()->GetDynamicsWorld().stepSimulation(static_cast<btScalar>(1. / 60.), 1);

	// 障害物
	for (int i = 0; i < 50; i++)
	{
		GenerateDecoration();
	}
}

//=============================================================
// [CTitleScene] 装飾の生成
//=============================================================
void CTitleScene::GenerateDecoration()
{
	// 位置を決める
	D3DXVECTOR3 pos;
	pos.x = Benlib::RandomFloat(TERRAIN_SIZE * TERRAIN_SCALE * -0.5f, TERRAIN_SIZE * TERRAIN_SCALE * 0.5f);
	pos.z = Benlib::RandomFloat(0.0f, (TERRAIN_SIZE - 2) * TERRAIN_SCALE);
	pos.z += 2.0f * TERRAIN_SCALE;
	pos.y = 0.0f;

	// レイポイントの位置を決める
	D3DXVECTOR3 rayPoint[4];
	rayPoint[0].x = -2.0f;
	rayPoint[0].z = 2.0f;
	rayPoint[1].x = 2.0f;
	rayPoint[1].z = 2.0f;
	rayPoint[2].x = 0.0f;
	rayPoint[2].z = -2.0f;
	rayPoint[3].x = 0.0f;
	rayPoint[3].z = 0.0f;

	// 生成座標に移動する
	for (int i = 0; i < 4; i++)
	{
		rayPoint[i] += pos;
	}

	// レイを飛ばす
	D3DXVECTOR3 rayReachPoint[4];
	bool bReached = true;
	for (int i = 0; i < 4; i++)
	{
		btVector3 Start = btVector3(rayPoint[i].x, 30000.0f, rayPoint[i].z);
		btVector3 End = btVector3(rayPoint[i].x, -30000.0f, rayPoint[i].z);

		btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
		CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
		if (RayCallback.hasHit())
		{ // ヒットしたとき
			rayReachPoint[i] = { RayCallback.m_hitPointWorld.getX(), RayCallback.m_hitPointWorld.getY(), RayCallback.m_hitPointWorld.getZ() };
		}
		else
		{
			bReached = false;
			break;
		}
	}

	if (!bReached) return;

	// 法線ベクトルを計算する
	D3DXVECTOR3 normal = Benlib::CalcNormalVector(rayReachPoint[0], rayReachPoint[1], rayReachPoint[2]);

	// 回転を決める ------------------------------------------------------------------------------------------------------------
	D3DXQUATERNION rot;
	D3DXQuaternionIdentity(&rot);


	// ランダムY軸回転
	float randomY = 0.0f;
	randomY = rand() % static_cast<int>(D3DX_PI * 2 * 10000) * 0.0001f;

	D3DXVECTOR3 axis = { 0.0f, 1.0f, 0.0f };
	D3DXQuaternionRotationAxis(&rot, &axis, randomY);

	// 地形に合わせた角度を計算する
	D3DXQUATERNION mulRot;
	axis = { 1.0f, 0.0f, 0.0f };
	D3DXQuaternionRotationAxis(&mulRot, &axis, fabsf(atan2f(normal.z, normal.y)) * 0.7f);
	rot *= mulRot;

	D3DXQuaternionIdentity(&mulRot);
	axis = { 0.0f, 1.0f, 0.0f };
	D3DXQuaternionRotationAxis(&mulRot, &axis, atan2f(normal.x, normal.z));
	rot *= mulRot;

	// オブジェクトを作成する
	Transform trans;
	trans.SetPos(pos);
	trans.SetQuaternion(rot);

	if (rand() % 10 < 8)
	{
		GameObject::LoadPrefab("data\\PREFAB\\tree\\tree00.pref", trans);
	}
	else
	{
		GameObject::LoadPrefab("data\\PREFAB\\tree\\fallen_tree.pref", trans);
	}
}

//=============================================================
// [CampFireParticleShape] キャンプファイアのパーティクル
//=============================================================
CampFireParticleShape::ResultData CampFireParticleShape::GetResult()
{
	ResultData data;
	float angle = Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f);
	float radius = Benlib::RandomFloat(0.0f, m_radius);
	data.position = { sinf(angle) * radius, 0.0f, cosf(angle) * radius };
	data.direction = D3DXVECTOR3(0.0f, 10.0f, 0.0f) - data.position;
	return data;
}
