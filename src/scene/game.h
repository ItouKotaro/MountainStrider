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
#include "scripts/result/mt_result.h"

// ゲームシーン
class CGameScene : public CScene
{
public:
	CGameScene(){}
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画

	void ResetGame();				// ゲームのリセット

	void onGameOver();			// ゲームオーバー処理
	void onClear();					// クリア処理
	Terrain* GetTerrain() { return m_pTerrain; }
	GameObject* GetBike() { return m_pBike; }

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
		ENDTYPE_NONE,			// なし（通常）
		ENDTYPE_CLEAR,			// クリア（山踏破）
		ENDTYPE_GAMEOVER,	// ゲームオーバー
	};
	ENDTYPE m_endType;		// 終了理由
private:
	void SpawnBike();				// バイクの生成
	void ClearCondition();			// クリア条件

	int m_travellingCount;
	std::vector<TravellingData> m_travellingDatas;	// 走行データ
	ResultBase* m_result;			// リザルト

	Terrain* m_pTerrain;				// 地形
	GameObject* m_voidField;		// 奈落
	GameObject* m_pCamera;		// カメラ
	GameObject* m_pBike;			// バイク
	GameObject* m_pStatusUI;	// ステータスUI

	// リザルトデータ用
	DWORD m_startTime;			// 開始時間
	int m_highSpeed;					// 最高速度

	// FPS表示
	GameObject* m_pFPS;
};

#endif // !_GAME_H_
