//=============================================================
//
// �n�` [terrain.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "internal/physics.h"
#include "nature_produces.h"
#include "component.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

// �������Ǘ��i�K�v�Œ���̃Q�[���I�u�W�F�N�g�ŊǗ�����j
// �ڍ�: �����������Ŏg���Ă��Ȃ��ꍇ�̓Q�[���I�u�W�F�N�g���g����
class ProducesManager
{
public:
	// �������f�[�^��ǉ�����
	void AddProduce(const Transform& transform, CNatureProduces* pNatureProduce);
	// ������
	void Init();
	// �j��
	void Uninit();
	// �X�V
	void Update(const D3DXVECTOR3& pos);

	// �w��ʒu�̎��͂̃I�u�W�F�N�g���擾����
	float GetNearProducesRate(const std::string& name , const D3DXVECTOR3& pos, const float& range);

	// �w��ʒu����w��͈̔͂ɐ����������݂��邩
	bool FindProducesByDistance(const D3DXVECTOR3& pos, const float& distance);
private:
	void UpdateGameObjects(const D3DXVECTOR3& pos);

	// �Q�[���I�u�W�F�N�g�f�[�^
	struct ManagedGameObject
	{
		GameObject* gameObject;
		CNatureProduces* natureProduce;
	};

	// �������Ǘ��f�[�^�i�z�u���j
	struct ManagedProduce
	{
		Transform transform;
		CNatureProduces* natureProduce;
		ManagedGameObject* managedGameObject;	// �z�u�Q�[���I�u�W�F�N�g
	};

	std::vector<ManagedProduce*> m_managedProduces;				// �Ǘ�����Ă��鐶���z�u��
	std::vector<ManagedGameObject*> m_managedGameObjects;	// �Ǘ�����Ă���Q�[���I�u�W�F�N�g

	GameObject* m_pVehicle;		// �o�C�N�i�[

	// �f�o�b�O�p
	GameObject* m_pNumObj;	// �I�u�W�F�N�g�̑����e�L�X�g
};

// �n�`
class Terrain
{
public:
	void Init();
	void Uninit();
	void Update(const D3DXVECTOR3& pos);
	void Generate();
	void SetSeed(const unsigned int& seed) { m_seed = seed; }
	unsigned int GetSeed() { return m_seed; }
	float GetMinHeight() { return m_minHeight; }
	float GetMaxHeight() { return m_maxHeight; }
	void LoadTerrainFile(const std::string path);

	ProducesManager* GetProducesManager() { return m_producesManager; }

	static const int TERRAIN_SIZE = 150;
	static const float TERRAIN_SCALE;
private:
	void UninitTerrain();

	unsigned int m_seed;										// �V�[�h�l
	GameObject* m_pField;									// ���b�V���t�B�[���h
	float* m_terrainData;										// ���x�f�[�^
	btHeightfieldTerrainShape* m_terrainShape;	// �n�`�R���W�����V�F�C�v

	float m_maxHeight;	// �ō����x
	float m_minHeight;	// �Œፂ�x

	// ���_�̍������擾����
	float GetVertexHeight(const int& x, const int& y);

	// �n�`�𐶐�����
	void GenerateTerrain();
	// �R���𐶐�����
	void GenerateRoad();
	// �������𐶐�����
	void GenerateProduces();
	// ��������o�^����
	void RegisterProduces(CNatureProduces* pNatureProduce);

	std::vector<CNatureProduces*> m_registerNatureProduces;		// ���������X�g�i�o�^�j
	ProducesManager* m_producesManager;									// �������Ǘ�
	bool m_routeData[TERRAIN_SIZE][TERRAIN_SIZE];
	

	// ���x�J���[��ǉ�����
	void AddHeightColor(const float& height, const D3DXCOLOR& color);
	// �X�΃J���[��ǉ�����
	void AddSlantColor(const float& minHeight, const float& maxHeight, const D3DXCOLOR& color, const float& rate = 1.0f);
	// �w��̍��x�̐F���擾����
	D3DXCOLOR GetHeightColor(const float& height);
	// �w��̈ʒu�̐F���擾����
	D3DXCOLOR GetVertexColor(const int& x, const int& y);

	// ���x�J���[
	struct HeightColor
	{
		float height;				// ���x
		D3DXCOLOR color;		// �F
	};
	std::vector<HeightColor> m_heightColor;

	// �p�x�J���[
	struct SlantColor
	{
		float minHeight;			// ���x�͈́i�ŏ��j
		float maxHeight;		// ���x�͈́i�ő�j
		D3DXCOLOR color;		// �F
		float rate;					// �K�p����
	};
	std::vector<SlantColor> m_slantColor;
};

#endif // !_TERRAIN_H_
