//=============================================================
//
// �n�` [terrain.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain.h"
#include "component/3d/meshfield.h"
#include <DTL.hpp>
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

// �ÓI�����o�ϐ��̏�����

//=============================================================
// [CTerrain] ������
//=============================================================
void CTerrain::Init()
{
	// ���b�V���t�B�[���h���쐬����
	m_pField = new GameObject;
	m_pField->SetParent(gameObject);
	m_pField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, 10.0f);

	m_heightfieldData = nullptr;
	m_pCollisionObj = nullptr;
}

//=============================================================
// [CTerrain] �I��
//=============================================================
void CTerrain::Uninit()
{
	// ���x�f�[�^��j������
	if (m_heightfieldData != nullptr)
	{
		delete[] m_heightfieldData;
		m_heightfieldData = nullptr;
	}

	// �R���W������j������
	if (m_pCollisionObj != nullptr)
	{
		CPhysics::GetInstance()->GetDynamicsWorld().removeCollisionObject(m_pCollisionObj);
		delete m_pCollisionObj;
		m_pCollisionObj = nullptr;
	}
}

//=============================================================
// [CTerrain] ����
//=============================================================
void CTerrain::Generate()
{
	// �I������
	this->Uninit();

	// ���x�}�b�v�𐶐�����
	dtl::shape::DiamondSquareAverageIsland<int>(5, 100, 0).draw(m_terrainHeight);

	// ���x�f�[�^���쐬����
	m_heightfieldData = new unsigned short[TERRAIN_SIZE * TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_heightfieldData[x + y * TERRAIN_SIZE] = static_cast<unsigned short>(m_terrainHeight[x][y]);
		}
	}

	// �R���W�����𐶐�����
	m_pCollisionObj = new btCollisionObject();
	//btHeightfieldTerrainShape* heightFieldShape = new btHeightfieldTerrainShape(
	//	TERRAIN_SIZE, TERRAIN_SIZE, m_heightfieldData, 1.0f, 1, false, false);
	btBoxShape* pBox = new btBoxShape(btVector3(10.0f, 10.0f, 10.0f));
	m_pCollisionObj->setCollisionShape(pBox);
	CPhysics::GetInstance()->GetDynamicsWorld().addCollisionObject(m_pCollisionObj);
}