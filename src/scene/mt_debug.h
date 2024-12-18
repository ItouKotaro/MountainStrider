//=============================================================
//
// マウンテンのデバッグシーン [mt_debug.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _MOUNTAIN_DEBUG_H_
#define _MOUNTAIN_DEBUG_H_

#include "scene.h"

// マウンテンデバッグシーン
class CMountainDebug : public CScene
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
private:
	GameObject* m_pCamera;
	GameObject* m_pTerrain;
	GameObject* m_pRollTest;
	GameObject* m_pText;
	//GameObject* m_pParticle;
};

#endif // !_MOUNTAIN_DEBUG_H_
