//=============================================================
//
// リザルトシーン [result.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"

// リザルトシーン
class CResultScene : public CScene
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
private:
};

#endif // !_RESULT_H_
