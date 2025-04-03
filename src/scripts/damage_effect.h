//=============================================================
//
// �_���[�W�G�t�F�N�g [damage_effect.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _DAMAGE_EFFECT_H_
#define _DAMAGE_EFFECT_H_

#include "component.h"
#include "component/3d/billboard.h"

// �_���[�W�G�t�F�N�g
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
	float m_time;																				// ���Ŏ���
	GameObject* m_camera;															// �J����
	std::vector<GameObject*> m_billboards;									// �r���{�[�h�Ǘ�
	const float SIZE = 5.0f;
};

#endif // !_DAMAGE_EFFECT_H_
