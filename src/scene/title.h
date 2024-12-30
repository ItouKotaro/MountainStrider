//=============================================================
//
// タイトル [title.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"
#include "internal/launch_credit.h"

// タイトルシーン
class CTitleScene : public CScene
{
public:
	void Init() override;						// 初期化
	void Uninit() override;						// 終了
	void Update() override;					// 更新
	void Draw() override;						// 描画

private:
	GameObject* m_titleScene;
	LaunchCredit* m_credit;
};

#endif // !_TITLE_H_
