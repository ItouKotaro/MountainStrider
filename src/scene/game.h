//=============================================================
//
// ゲームシーン [game.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "internal/physics.h"
#include "scripts/terrain.h"
#include "scripts/environmental.h"
#include "scripts/decoration.h"
#include "scripts/result/mt_result.h"
#include "scripts/pause.h"
#include "scripts/event/event_manager.h"
#include "scripts/lake.h"
#include "scripts/play_guide.h"
#include "scripts/mode/mode_manager.h"

// ゲームシーン
class CGameScene : public CScene
{
public:
	CGameScene(){}
	void Init() override;				// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
	void LastUninit() override;	// 最終終了

	static void ResetGame();		// ゲームのリセット
	void HideUI();						// UIを非表示にする
	void CalcResultData();		// リザルトデータを計算する
	ResultBase::ResultData GetResultData() { return m_resultData; }	// リザルトデータを取得する

	Terrain* GetTerrain() { return m_terrain; }
	LakeManager* GetLake() { return m_lake; }
	GameObject* GetBike() { return m_bike; }
	EnvironmentalEffect* GetEnvironmental() { return m_environmental; }
	DecorationManager* GetDecoration() { return m_decoration; }
	Pause* GetPause() { return m_pause; }

	static void AddActionPoint(const int& point) { m_actionPoint += point; }		// アクションポイントの加算
	static int GetActionPoint() { return m_actionPoint; }										// アクションポイントの取得

	// 走行データ
	struct TravellingData
	{
		D3DXVECTOR3 pos;
		D3DXQUATERNION rot;
	};
	std::vector<TravellingData> GetTravellingData() { return m_travellingDatas; }

	// 便利関数
	void GenerateTerrain(const int& size = Terrain::TERRAIN_DEFAULT_SIZE, const float& scale = Terrain::TERRAIN_DEFAULT_SCALE, const std::string& terrain_path = "");

private:
	std::vector<std::string> GetTerrainFiles();			// 地形ファイル一覧を取得する
	void SpawnBike();													// バイクの生成

	int m_travellingCount;												// 突破数
	std::vector<TravellingData> m_travellingDatas;	// 走行データ

	Terrain* m_terrain;													// 地形
	EnvironmentalEffect* m_environmental;				// 環境
	DecorationManager* m_decoration;						// 装飾
	EventManager* m_events;										// イベント
	LakeManager* m_lake;											// 湖
	PlayGuideManager* m_playGuide;							// プレイガイド

	GameObject* m_voidField;										// 奈落
	GameObject* m_camera;										// カメラ
	GameObject* m_bike;												// バイク
	GameObject* m_statusUI;										// ステータスUI
	GameObject* m_speedmeterUI;								// スピードメーターUI
	GameObject* m_itemSlot;										// アイテムスロット
	GameObject* m_gemPopupUI;								// ジェムポップアップUI

	ResultBase* m_oldResult;										// 過去のリザルト
	ResultBase::ResultData m_resultData;					// リザルトデータ

	// リザルトデータ用
	DWORD m_startTime;												// 開始時間
	int m_highSpeed;													// 最高速度
	static int m_actionPoint;											// スコア

	// ポーズ
	Pause* m_pause;
};

#endif // !_GAME_H_
