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
#include "scripts/gem_popup.h"

#include "scripts/mode/terminal.h"

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
	m_travellingCount = 0;
	m_travellingDatas.clear();
	m_actionPoint = 0;
	m_highSpeed = 0;
	m_pause = nullptr;
	m_oldResult = nullptr;

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

	// バイクのゲームオブジェクトのみ作成する
	m_bike = new GameObject("Vehicle");

	// モードの初期化
	ModeManager::GetInstance()->Init();

	// バイクの生成
	SpawnBike();

	// アイテムスロット
	m_itemSlot = new GameObject("ItemSlot", "UI");
	m_itemSlot->AddComponent<ItemSlot>();
	m_itemSlot->transform->SetPos(CRenderer::SCREEN_WIDTH - 400.0f, 30.0f);

	// ジェムポップアップUI
	m_gemPopupUI = new GameObject("GemPopupUI", "UI");
	m_gemPopupUI->AddComponent<GemPopupUI>();

	// イベント
	m_events = new EventManager();
	m_events->Init();

	// プレイガイド
	if (static_cast<TerminalMode*>(ModeManager::GetInstance()->GetMode())->GetResultData().size() == 0)
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

	// モードの終了
	ModeManager::GetInstance()->Uninit();

	// ポーズの破棄
	if (m_pause != nullptr)
	{
		delete m_pause;
		m_pause = nullptr;
	}

	// レンダーバッファのカメラをnullにする
	static_cast<CameraRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(nullptr);
}

//=============================================================
// [CGameScene] 更新
//=============================================================
void CGameScene::Update()
{
	// ポーズ
	if (ModeManager::GetInstance()->GetResult() == nullptr && (INPUT_INSTANCE->onTrigger("p") || INPUT_INSTANCE->onTrigger("esc") || INPUT_INSTANCE->onTrigger("p:start")))
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
	if (ModeManager::GetInstance()->GetResult() == nullptr)
	{
		// 湖を更新する
		m_lake->Update();

		// イベントを更新する
		m_events->Update();

		// モードを更新する
		ModeManager::GetInstance()->Update();
	}

	// 未ゲームオーバー時
	if (ModeManager::GetInstance()->GetResult() == nullptr)
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
	}

	// リザルトの更新処理
	if (ModeManager::GetInstance()->GetResult() != nullptr)
	{
		// 入ったフレームだけの処理
		if (m_oldResult == nullptr)
		{
			// UIを非表示にする
			HideUI();

			// リザルトカメラを起動する
			m_camera->GetComponent<ResultCamera>()->Play();
			m_camera->GetComponent<CCameraMove>()->enabled = false;

			// モードのリザルトイベント
			ModeManager::GetInstance()->GetMode()->OnResultEvent();
		}

		// リザルトの更新
		ModeManager::GetInstance()->GetResult()->Update();

		// バイクを無効化する
		m_bike->SetActive(false);
	}

	// 前回のリザルトとして更新する
	m_oldResult = ModeManager::GetInstance()->GetResult();
}

//=============================================================
// [CGameScene] 描画
//=============================================================
void CGameScene::Draw()
{
	// リザルトの描画処理
	if (ModeManager::GetInstance()->GetResult() != nullptr)
	{
		// リザルトの描画
		ModeManager::GetInstance()->GetResult()->Draw();
	}
}

//=============================================================
// [CGameScene] 最終終了
//=============================================================
void CGameScene::LastUninit()
{
	// ゲームをリセットする
	ResetGame();
}

//=============================================================
// [CGameScene] ゲームのリセット
//=============================================================
void CGameScene::ResetGame()
{
	// バイクの燃料と耐久値を回復させる
	CVehicle::ResetState();

	// ショップ情報のリセット
	ShopManager::Reset();

	// アイテムの情報をリセット
	ItemManager::GetInstance()->AllRemoveItem();

	m_actionPoint = 0;
}

//=============================================================
// [CGameScene] UIを非表示にする
//=============================================================
void CGameScene::HideUI()
{
	// ステータスUIを非表示にする
	m_statusUI->GetComponent<CStatusUI>()->SetVisible(false);

	// スピードメーターを非表示にする
	m_speedmeterUI->SetActive(false);

	// アイテムスロットを非表示にする
	m_itemSlot->SetActive(false);

	// ジェムポップアップUIを非表示にする
	m_gemPopupUI->SetActive(false);

	// プレイガイドを非表示にする
	if (m_playGuide != nullptr)
	{
		m_playGuide->Uninit();
		delete m_playGuide;
		m_playGuide = nullptr;
	}
}

//=============================================================
// [CGameScene] バイクの生成
//=============================================================
void CGameScene::SpawnBike()
{
	// バイクを生成する
	m_bike->AddComponent<CVehicle>();
	m_bike->AddComponent<AudioListener>();

	// バイクの初期値移動
	btVector3 Start = btVector3(0.0f, 3000.0f, 0.0f);
	btVector3 End = btVector3(0.0f, -3000.0f, 0.0f);
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit())
	{ // ヒットしたとき
		m_bike->GetComponent<CVehicle>()->SetPos({ 0.0f, RayCallback.m_hitPointWorld.getY() + 50.0f, 0.0f });
	}

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
// [CGameScene] リザルトデータを計算する
//=============================================================
void CGameScene::CalcResultData()
{
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
	m_resultData.time = (timeGetTime() - m_startTime) / 1000;
	m_resultData.highSpeed = m_highSpeed;
	m_resultData.action = m_actionPoint;
	m_resultData.mileage = mileage;
	m_resultData.fuel = m_bike->GetComponent<CVehicle>()->GetFuelConsumption();
}

//=============================================================
// [CGameScene] 地形を生成する
//=============================================================
void CGameScene::GenerateTerrain(const int& size, const float& scale, const std::string& terrain_path)
{
	// 地形パスの決定（未入力でランダム）
	std::string terrainPath = terrain_path;
	if (terrainPath == "")
	{
		// 読み込む地形ファイルを決定する
		auto terrainFiles = GetTerrainFiles();
		if (terrainFiles.empty())
		{ // 一つも地形ファイルが存在しないとき
			MessageBox(NULL, "地形ファイルが見つかりませんでした\ndata/TERRAINS フォルダ内に地形ファイルを入れてください", "エラー", MB_OK);
			Main::ExitApplication();
			return;
		}

		// 取得したパスの中から1つをランダムで取得して格納する
		terrainPath = terrainFiles[rand() % terrainFiles.size()];
	}

	// 地形を作成
	m_terrain = new Terrain();
	m_terrain->SetSeed(rand());
	m_terrain->Init(size, scale);
	m_terrain->LoadTerrainFile(terrainPath);
	m_terrain->Generate();

	// 装飾を生成する
	m_decoration = new DecorationManager();
	m_decoration->Init(m_terrain);
	m_decoration->LoadTerrainFile(terrainPath);
	m_decoration->Generate();

	// 奈落
	m_voidField = new GameObject("Void");
	m_voidField->AddComponent<CField>();
	m_voidField->GetComponent<CField>()->Set(size * scale * 2, size * scale * 2);
	m_voidField->transform->SetPos(0.0f, m_terrain->GetMinHeight() - 20.0f, 0.0f);

	// 湖を作成
	m_lake = new LakeManager();
	m_lake->Init(m_terrain, terrainPath);

	// 環境効果
	m_environmental = new EnvironmentalEffect();
	m_environmental->Init(terrainPath);
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