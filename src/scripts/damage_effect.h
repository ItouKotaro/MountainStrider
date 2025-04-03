//=============================================================
//
// ダメージエフェクト [damage_effect.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _DAMAGE_EFFECT_H_
#define _DAMAGE_EFFECT_H_

#include "component.h"
#include "component/3d/billboard.h"

// ダメージエフェクト
class DamageEffect : public Component
{
public:
	DamageEffect() : m_time(0) {}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Generate(const float& damage, const float& time = 1.0f);
private:
	float m_time;																				// 消滅時間
	GameObject* m_camera;															// カメラ
	std::vector<GameObject*> m_billboards;									// ビルボード管理
	const float SIZE = 5.0f;
	const float UP_MOVE = 0.5f;
	const float ALPHA = 0.02f;
};

#endif // !_DAMAGE_EFFECT_H_
