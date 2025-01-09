//=============================================================
//
// �^�C�g�� [title.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"
#include "internal/launch_credit.h"
#include "component/2d/text.h"
#include "component/3d/particle.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

// �^�C�g���V�[��
class CTitleScene : public CScene
{
public:
	void Init() override;						// ������
	void Uninit() override;						// �I��
	void Update() override;					// �X�V
	void Draw() override;						// �`��

private:
	void UpdateControl();
	void InitTerrain();
	void GenerateDecoration();

	enum SELECT
	{
		SELECT_START,
		SELECT_END,
		SELECT_MAX,
	};
	SELECT m_select;
	GameObject* m_selectBar;
	CText* m_sStart;
	CText* m_sEnd;
	
	GameObject* m_field;
	GameObject* m_titleScene;
	LaunchCredit* m_credit;
	float* m_terrainData;
	btHeightfieldTerrainShape* m_terrainShape;
	
	const float SELECT_START_Y = 600.0f;
	const float SELECT_SPACE = 150.0f;
	const float NOT_SELECTED_ALPHA = 0.3f;
	const float SELECTED_ALPHA = 1.0f;
	const float TERRAIN_SCALE = 30.0f;
	const int TERRAIN_SIZE = 30;
};

// �L�����v�t�@�C�A�̃p�[�e�B�N������
class CampFireParticleShape : public ParticleModule::Shape
{
public:
	CampFireParticleShape():m_radius(6.0f){}
	ResultData GetResult() override;
	void SetRadius(const float& radius) { m_radius = radius; }
private:
	float m_radius;
};

#endif // !_TITLE_H_
