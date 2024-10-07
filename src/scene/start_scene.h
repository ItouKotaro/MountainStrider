//=============================================================
//
// スタートシーン [start_scene.h]
// Author: Ito Kotaro
// 
// アプリケーション開始後、最初に読み込まれるシーンです
//
//=============================================================
#ifndef _START_SCENE_H_
#define _START_SCENE_H_

#include "scene.h"

// スタートシーン
class CStartScene : public CScene
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
};

#endif // !_START_SCENE_H_
