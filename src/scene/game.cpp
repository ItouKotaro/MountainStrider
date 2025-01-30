//=============================================================
//
// ゲームシーン [game.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "game.h"
#include "manager.h"

#include "component/3d/camera.h"
#include "component/3d/light.h"
#include "component/2d/text.h"
#include "component/3d/field.h"
#include "component/other/audio.h"
#include "component/3d/particle.h"
#include "component/3d/mesh.h"

#include "scripts/vehicle.h"
#include "scripts/terrain.h"
#include "scripts/camera_move.h"
#include "scripts/result/result_camera.h"
#include "scripts/result/mt_result.h"
#include "scripts/status_ui.h"
#include "scripts/item/item_manager.h"
#include "scripts/shop/shop.h"
#include "scripts/item/item_slot.h"
#include "scripts/speedmeter_ui.h"

#include "render/shadow_rb.h"
#include <noise/noise.h>

#include "scripts/lake.h"

int CGameScene::m_actionPoint = 0;

//=============================================================
// [CGameScene] 初期化
//=============================================================
void CGameScene::Init()
{
	// 変数の初期化
	m_endType = ENDTYPE_NONE;
	m_travellingCount = 0;
	m_travellingDatas.clear();
	m_actionPoint = 0;
	m_pause = nullptr;

	// 乱数のシード設定
	srand((unsigned int)clock());

	// ポーズ
	m_pause = new Pause();
	m_pause->Init();

	// 音を読み込む
	AudioManager::GetInstance()->LoadBank("data\\SOUND\\BANK\\Master.bank");
	AudioManager::GetInstance()->LoadBank("data\\SOUND\\BANK\\Master.strings.bank");
	AudioManager::GetInstance()->LoadBank("data\\SOUND\\BANK\\Vehicles.bank");

	// カメラの作成
	{
		m_camera = new GameObject("Camera", "Camera");
		m_camera->AddComponent<CCamera>();
		m_camera->GetComponent<CCamera>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
		m_camera->GetComponent<CCamera>()->m_fClippingPlanesFar = 5000.0f;
		m_camera->GetComponent<CCamera>()->GetSkybox()->LoadSkybox("data\\SKYBOX\\daylight.json");
		CMesh::SetCamera(m_camera->GetComponent<CCamera>());
	}

	// 読み込む地形ファイルを決定する
	auto terrainFiles = GetTerrainFiles();
	if (terrainFiles.empty())
	{ // 一つも地形ファイルが存在しないとき
		MessageBox(NULL, "地形ファイルが見つかりませんでした\ndata/TERRAINS フォルダ内に地形ファイルを入れてください", "エラー", MB_OK);
		Main::ExitApplication();
		return;
	}

	// 取得したパスの中から1つをランダムで取得して格納する
	auto terrainPath = terrainFiles[rand() % terrainFiles.size()];

	// 地面を作成
	{
		m_terrain = new Terrain();
		m_terrain->SetSeed(rand());
		m_terrain->Init();
		m_terrain->LoadTerrainFile(terrainPath);
		m_terrain->Generate();
	}

	// 装飾を生成する
	m_decoration = new DecorationManager();
	m_decoration->Init(m_terrain);
	m_decoration->LoadTerrainFile(terrainPath);
	m_decoration->Generate();

	// 奈落
	{
		m_voidField = new GameObject("Void");
		m_voidField->AddComponent<CField>();
		m_voidField->GetComponent<CField>()->Set(Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE * 2, Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE * 2);
		m_voidField->transform->SetPos(0.0f, m_terrain->GetMinHeight() - 20.0f, 0.0f);
	}

	// バイクの生成
	SpawnBike();

	// 湖を作成
	m_lake = new LakeManager();
	m_lake->Init(m_terrain, terrainPath);

	// アイテムスロット
	m_itemSlot = new GameObject("ItemSlot", "UI");
	m_itemSlot->AddComponent<ItemSlot>();
	m_itemSlot->transform->SetPos(CRenderer::SCREEN_WIDTH - 400.0f, 30.0f);

	// 環境効果
	m_environmental = new EnvironmentalEffect();
	m_environmental->Init(terrainPath);

	// イベント
	m_events = new EventManager();
	m_events->Init();

	// プレイガイド
	if (ResultBase::GetNumOfStep() == 0)
	{
		m_playGuide = new PlayGuideManager();
		m_playGuide->Init();
	}

	// カメラの移動設定を行う
	m_camera->AddComponent<CCameraMove>()->SetTarget(m_bike);
	m_camera->AddComponent<ResultCamera>();

	// 開始時間を記録する
	m_startTime = timeGetTime();

	// カメラをレンダーバッファに追加する
	CRenderer::GetInstance()->RegisterRenderBuffer<ShadowRenderBuffer>("main");
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(m_camera->GetComponent<CCamera>());
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetLightCamera(m_environmental->GetLightCamera());
	Main::SetShowCursor(false);
}

//=============================================================
// [CGameScene] 終了
//=============================================================
void CGameScene::Uninit()
{
	// メッシュに設定したカメラをリセットする
	CMesh::SetCamera(nullptr);

	// 地形の破棄
	if (m_terrain != nullptr)
	{
		m_terrain->Uninit();
		delete m_terrain;
		m_terrain = nullptr;
	}

	// 装飾の破棄
	if (m_decoration != nullptr)
	{
		m_decoration->Uninit();
		delete m_decoration;
		m_decoration = nullptr;
	}

	// 湖の破棄
	if (m_lake != nullptr)
	{
		m_lake->Uninit();
		delete m_lake;
		m_lake = nullptr;
	}

	// 環境効果の破棄
	if (m_environmental != nullptr)
	{
		m_environmental->Uninit();
		delete m_environmental;
		m_environmental = nullptr;
	}

	// イベントの破棄
	if (m_events != nullptr)
	{
		m_events->Uninit();
		delete m_events;
		m_events = nullptr;
	}

	// プレイガイドの破棄
	if (m_playGuide != nullptr)
	{
		m_playGuide->Uninit();
		delete m_playGuide;
		m_playGuide = nullptr;
	}

	// リザルトマネージャーの破棄
	if (m_result != nullptr)
	{
		m_result->Uninit();
		delete m_result;
		m_result = nullptr;
	}

	// ポーズの破棄
	if (m_pause != nullptr)
	{
		delete m_pause;
	}

	// レンダーバッファのカメラをnullにする
	static_cast<CameraRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(nullptr);
}

//=============================================================
// [CGameScene] 更新
//=============================================================
void CGameScene::Update()
{
	if (INPUT_INSTANCE->onTrigger("@"))
	{
		onClear();
	}

	// ポーズ
	if (INPUT_INSTANCE->onTrigger("p") || INPUT_INSTANCE->onTrigger("esc") || INPUT_INSTANCE->onTrigger("p:start"))
	{
		m_pause->SetPause(!m_pause->GetPause());
	}
	if (m_pause->GetPause())
	{
		m_pause->Update();
		return;
	}

	// 地形を更新する
	m_terrain->Update();

	// 装飾を更新する
	m_decoration->Update(m_camera->transform->GetWPos());

	// 環境効果を更新する
	m_environmental->Update();

	// プレイガイドを更新する
	if (m_playGuide != nullptr)
	{
		m_playGuide->Update();
	}


	// リザルトでは更新しないオブジェクト
	if (m_endType == ENDTYPE_NONE)
	{
		// 湖を更新する
		m_lake->Update();

		// イベントを更新する
		m_events->Update();
	}

	// 未ゲームオーバー時
	if (m_endType == ENDTYPE_NONE)
	{
		// カメラの情報を記録する
		m_camera->GetComponent<ResultCamera>()->RecordData();

		// 走行データを記録する
		m_travellingCount++;
		if (m_travellingCount >= 20)
		{
			TravellingData travelling;
			travelling.pos = m_bike->transform->GetWPos();
			travelling.rot = m_bike->transform->GetWQuaternion();
			m_travellingDatas.push_back(travelling);
			m_travellingCount = 0;
		}

		// 最高速度を記録する
		int bikeSpeed = static_cast<int>(m_bike->GetComponent<CVehicle>()->GetSpeed());
		m_highSpeed = m_highSpeed < bikeSpeed ? bikeSpeed : m_highSpeed;

		// クリア条件
		ClearCondition();
	}

	// リザルトの更新処理
	if (m_endType != ENDTYPE_NONE)
	{ // ゲームオーバーのとき
		// リザルトの更新
		m_result->Update();

		// バイクを無効化する
		m_bike->SetActive(false);
	}
}

//=============================================================
// [CGameScene] 描画
//=============================================================
void CGameScene::Draw()
{
	// リザルトの描画処理
	if (m_endType != ENDTYPE_NONE)
	{ // ゲームオーバーのとき
		// リザルトの描画
		m_result->Draw();
	}
}

//=============================================================
// [CGameScene] 最終終了
//=============================================================
void CGameScene::LastUninit()
{
	ResetGame();
}

//=============================================================
// [CGameScene] ゲームのリセット
//=============================================================
void CGameScene::ResetGame()
{
	// バイクの燃料と耐久値を回復させる
	CVehicle::ResetState();

	// リザルトデータのリセット
	ResultBase::Reset();

	// ショップ情報のリセット
	ShopManager::Reset();

	// アイテムの情報をリセット
	ItemManager::GetInstance()->AllRemoveItem();

	m_actionPoint = 0;
}

//=============================================================
// [CGameScene] バイクの生成
//=============================================================
void CGameScene::SpawnBike()
{
	// 中心からレイを打ち、高さを取得する
	btVector3 Start = btVector3(0.0f, 3000.0f, 0.0f);
	btVector3 End = btVector3(0.0f, -3000.0f, 0.0f);
	float hitY = 0.0f;

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit())
	{ // ヒットしたとき
		hitY = RayCallback.m_hitPointWorld.getY();
	}

	// バイクを生成する
	m_bike = new GameObject("Vehicle");
	m_bike->AddComponent<CVehicle>();
	m_bike->GetComponent<CVehicle>()->SetPos({ 0.0f, hitY + 50.0f, 0.0f });
	m_bike->AddComponent<AudioListener>();

	// ステータスUIを生成
	m_statusUI = new GameObject("StatusUI", "UI");
	m_statusUI->AddComponent<CStatusUI>();
	m_bike->GetComponent<CVehicle>()->SetStatusUI(m_statusUI->GetComponent<CStatusUI>());

	// スピードメーターUIを生成
	m_speedmeterUI = new GameObject("SpeedMeterUI", "UI");
	m_speedmeterUI->transform->SetPos(CRenderer::SCREEN_WIDTH - 380.0f, CRenderer::SCREEN_HEIGHT - 350.0f);
	m_speedmeterUI->AddComponent<SpeedMeterUI>(m_bike->GetComponent<CVehicle>());
}

//=============================================================
// [CGameScene] クリアの条件
//=============================================================
void CGameScene::ClearCondition()
{
	D3DXVECTOR3 vehiclePos = m_bike->transform->GetWPos();

	// 地形の端に行ったとき
	if (vehiclePos.x <= -Terrain::TERRAIN_DISTANCE_HALF + EXTENSION_DISTANCE || vehiclePos.x >= Terrain::TERRAIN_DISTANCE_HALF - EXTENSION_DISTANCE ||
		vehiclePos.z <= -Terrain::TERRAIN_DISTANCE_HALF + EXTENSION_DISTANCE || vehiclePos.z >= Terrain::TERRAIN_DISTANCE_HALF - EXTENSION_DISTANCE)
	{
		onClear();
	}

	// 最低高度よりも下に行ったとき
	if (vehiclePos.y < m_terrain->GetMinHeight() - 5.0f)
	{
		onClear();
	}
}

//=============================================================
// [CGameScene] ゲームオーバー時の処理
//=============================================================
void CGameScene::onGameOver()
{
	if (m_endType == ENDTYPE_NONE)
	{ // 1回のみの処理
		// ステータスUIを非表示にする
		m_statusUI->GetComponent<CStatusUI>()->SetVisible(false);

		// スピードメーターを非表示にする
		m_speedmeterUI->SetActive(false);

		// アイテムスロットを非表示にする
		m_itemSlot->SetActive(false);

		// プレイガイドを非表示にする
		if (m_playGuide != nullptr)
		{
			m_playGuide->Uninit();
			delete m_playGuide;
			m_playGuide = nullptr;
		}

		// 走行距離を計算する
		float mileage = 0.0f;
		for (UINT i = 0; i < m_travellingDatas.size(); i++)
		{
			if (i + 1 < m_travellingDatas.size())
			{ // 次があるとき
				mileage += Benlib::PosDistance(m_travellingDatas[i].pos, m_travellingDatas[i + 1].pos);
			}
		}

		// リザルトデータの格納
		ResultBase::ResultData data;
		data.time = -1;
		data.highSpeed = m_highSpeed;
		data.action = 50;
		data.mileage = mileage;
		data.fuel = m_bike->GetComponent<CVehicle>()->GetFuelConsumption();
		ResultBase::AddResult(data);

		// ゲームオーバーリザルトの初期化
		m_result = new GameOverResult(this);
		m_result->Init();

		// リザルトカメラを起動する
		m_camera->GetComponent<ResultCamera>()->Play();
		m_camera->GetComponent<CCameraMove>()->enabled = false;

		// ゲームオーバートリガーを有効にする
		m_endType = ENDTYPE_GAMEOVER;
	}
}

//=============================================================
// [CGameScene] クリア時の処理
//=============================================================
void CGameScene::onClear()
{
	if (m_endType == ENDTYPE_NONE)
	{ // 1回のみの処理
		// ステータスUIを非表示にする
		m_statusUI->GetComponent<CStatusUI>()->SetVisible(false);

		// スピードメーターを非表示にする
		m_speedmeterUI->SetActive(false);

		// アイテムスロットを非表示にする
		m_itemSlot->SetActive(false);

		// プレイガイドを非表示にする
		if (m_playGuide != nullptr)
		{
			m_playGuide->Uninit();
			delete m_playGuide;
			m_playGuide = nullptr;
		}

		// 走行距離を計算する
		float mileage = 0.0f;
		for (UINT i = 0; i < m_travellingDatas.size(); i++)
		{
			if (i + 1 < m_travellingDatas.size())
			{ // 次があるとき
				mileage += Benlib::PosDistance(m_travellingDatas[i].pos, m_travellingDatas[i + 1].pos);
			}
		}

		// リザルトデータの格納
		ResultBase::ResultData data;
		data.time = (timeGetTime() - m_startTime) / 1000;
		data.highSpeed = m_highSpeed;
		data.action = m_actionPoint;
		data.mileage = mileage;
		data.fuel = m_bike->GetComponent<CVehicle>()->GetFuelConsumption();
		ResultBase::AddResult(data);

		// クリアリザルトの初期化
		m_result = new ClearResult(this);
		m_result->Init();

		// リザルトカメラを起動する
		m_camera->GetComponent<ResultCamera>()->Play();
		m_camera->GetComponent<CCameraMove>()->enabled = false;

		// クリアトリガーを有効にする
		m_endType = ENDTYPE_CLEAR;
	}
}

//=============================================================
// [CGameScene] 地形ファイル一覧を取得する
//=============================================================
std::vector<std::string> CGameScene::GetTerrainFiles()
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	std::vector<std::string> file_names;

	//探すファイル名指定　ワイルドカードを使用
	std::string search_name = "data\\TERRAINS\\*.json";
	//ファイル検索
	hFind = FindFirstFile(search_name.c_str(), &win32fd);

	//見つからなかったか
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return file_names;
	}

	//次のファイルがある限り読み込み続ける
	do
	{
		//ディレクトリなら無視
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {}
		else
		{
			//ファイル名をパス付で取得
			std::string fileName = win32fd.cFileName;
			fileName.insert(0, "data\\TERRAINS\\");
			file_names.push_back(fileName);
		}
	} while (FindNextFile(hFind, &win32fd));

	//閉じる
	FindClose(hFind);

	return file_names;
}