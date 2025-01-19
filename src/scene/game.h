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

// ゲームシーン
class CGameScene : public CScene
{
public:
	CGameScene(){}
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
	void LastUninit() override;	// 最終終了

	static void ResetGame();				// ゲームのリセット

	void onGameOver();			// ゲームオーバー処理
	void onClear();					// クリア処理
	Terrain* GetTerrain() { return m_pTerrain; }
	GameObject* GetBike() { return m_pBike; }

	void AddScore(const int& score) { m_score += score; }
	Pause* GetPause() { return m_pause; }

	// 走行データ
	struct TravellingData
	{
		D3DXVECTOR3 pos;
		D3DXQUATERNION rot;
	};
	std::vector<TravellingData> GetTravellingData() { return m_travellingDatas; }

	// 終了の種類
	enum ENDTYPE
	{
		ENDTYPE_NONE,											// なし（通常）
		ENDTYPE_CLEAR,											// クリア（山踏破）
		ENDTYPE_GAMEOVER,									// ゲームオーバー
	};
	ENDTYPE m_endType;										// 終了理由
private:
	std::vector<std::string> GetTerrainFiles();			// 地形ファイル一覧を取得する
	void SpawnBike();												// バイクの生成
	void ClearCondition();											// クリア条件

	int m_travellingCount;										// 突破数
	std::vector<TravellingData> m_travellingDatas;	// 走行データ
	ResultBase* m_result;										// リザルト

	Terrain* m_pTerrain;											// 地形
	EnvironmentalEffect* m_environmental;				// 環境
	DecorationManager* m_decoration;						// 装飾
	EventManager* m_events;									// イベント
	LakeManager* m_lake;										// 湖

	GameObject* m_voidField;									// 奈落
	GameObject* m_pCamera;									// カメラ
	GameObject* m_pBike;										// バイク
	GameObject* m_pStatusUI;								// ステータスUI
	GameObject* m_pItemSlot;								// アイテムスロット

	// リザルトデータ用
	DWORD m_startTime;										// 開始時間
	int m_highSpeed;												// 最高速度
	int m_score;														// スコア

	// ポーズ
	Pause* m_pause;
};

#endif // !_GAME_H_
