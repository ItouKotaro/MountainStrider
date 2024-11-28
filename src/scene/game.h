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

	void onGameOver();			// ゲームオーバー処理
	Terrain* GetTerrain() { return m_pTerrain; }
private:
	void SpawnBike();				// バイクの生成

	bool m_isGameOvered;									// ゲームオーバー済みか
	MountainResultManager* m_resultManager;		// マウンテンリザルトマネージャー

	Terrain* m_pTerrain;				// 地形
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
