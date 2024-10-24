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
	GameObject* m_pCamera;	// カメラ
	GameObject* m_pBike;
	GameObject* m_pField;		// フィールド
};

#endif // !_GAME_H_
