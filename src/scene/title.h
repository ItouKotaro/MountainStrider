//=============================================================
//
// タイトル [title.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"

// タイトルシーン
class CTitleScene : public CScene
{
public:
	void Init() override;						// 初期化
	void Uninit() override;						// 終了
	void Update() override;					// 更新
	void Draw() override;						// 描画

	// 選択肢
	//enum SELECT
	//{
	//	START = 0,	// スタート
	//	EXIT,				// 終了
	//	MAX
	//};
private:
	//void UpdateSelect();						// 選択の更新

	//GameObject* m_pSelectObj[MAX];		// 選択オブジェクト
	//SELECT m_select;								// 選択
};

#endif // !_TITLE_H_
