//=============================================================
//
// 残骸 [wreckage.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _WRECKAGE_H_
#define _WRECKAGE_H_

#include "component.h"

#include "component/3d/mesh.h"

class CWreckage : public Component
{
public:
	void Init() override;
	void Update() override;

	static const float FADE_VALUE;
	static const int START_TIME;
private:
	std::vector<CMesh*> m_pMeshs;	// メッシュリスト
	float m_fFade;								// フェード
	DWORD m_dwStartTime;				// 開始時間
};

#endif // !_WRECKAGE_H_
