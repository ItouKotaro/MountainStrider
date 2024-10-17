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

// ゲームシーン
class CGameScene : public CScene
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
private:
	void CreateBike();				// バイクを生成する

	GameObject* m_pCamera;	// カメラ
	btHinge2Constraint* m_hinge2;
};

#endif // !_GAME_H_
