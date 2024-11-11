//=============================================================
//
// �n�` [terrain.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain.h"
#include "component/3d/meshfield.h"
#include <DTL.hpp>

// �ÓI�����o�ϐ��̏�����
const float CTerrain::TERRAIN_SCALE = 300.0f;

//=============================================================
// [CTerrain] ������
//=============================================================
void CTerrain::Init()
{
	// ���b�V���t�B�[���h���쐬����
	m_pField = new GameObject;
	m_pField->SetParent(gameObject);
	m_pField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, TERRAIN_SCALE);

	m_terrainData = nullptr;

	// ��Q����o�^����

	// ��
	CProdTree* prodTree = new CProdTree();
	prodTree->SetChance(10);
	prodTree->SetAdjacentRate("tree", 10.0f);
	RegisterProduces(prodTree);
	
	// �t�F���X
	CProdFence* prodFence = new CProdFence();
	prodFence->SetChance(0);
	prodFence->SetAdjacentRate("fence", 100.0f);
	RegisterProduces(prodFence);
}

//=============================================================
// [CTerrain] �I��
//=============================================================
void CTerrain::Uninit()
{
	UninitTerrain();

	for (unsigned int i = 0; i < m_natureProduces.size(); i++)
	{
		if (m_natureProduces[i] != nullptr)
		{
			delete m_natureProduces[i];
			m_natureProduces[i] = nullptr;
		}
	}
	m_natureProduces.clear();
}

//=============================================================
// [CTerrain] �n�`�̏I������
//=============================================================
void CTerrain::UninitTerrain()
{
	CCollision::RemoveCollision(m_pField);

	if (m_terrainData != nullptr)
	{
		delete[] m_terrainData;
		m_terrainData = nullptr;
	}
	if (m_terrainShape != nullptr)
	{
		delete m_terrainShape;
		m_terrainShape = nullptr;
	}
}

//=============================================================
// [CTerrain] ����
//=============================================================
void CTerrain::Generate()
{
	// �I������
	this->UninitTerrain();

	// �R���W�������쐬����
	CCollision::Create(m_pField);

	// �n�`�f�[�^�𐶐�����
	GenerateTerrain();

	// HeightfieldTerrainShape���쐬����
	m_terrainShape = new btHeightfieldTerrainShape(TERRAIN_SIZE, TERRAIN_SIZE, m_terrainData, 1, -30000, 30000, 1, PHY_FLOAT, false);
	m_terrainShape->setLocalScaling(btVector3(TERRAIN_SCALE, 1.0f, TERRAIN_SCALE));
	CCollision::GetCollision(m_pField)->GetGhostObject()->setCollisionShape(m_terrainShape);

	// �ݒ肵���V�F�[�v��K�p����
	CPhysics::GetInstance()->GetDynamicsWorld().stepSimulation(static_cast<btScalar>(1. / 60.), 1);

	// �������𐶐�����
	srand((unsigned int)clock());
	for (int i = 0; i < 1500; i++)
	{
		GenerateProduces();
	}

}

//=============================================================
// [CTerrain] �n�`�̐���
//=============================================================
void CTerrain::GenerateTerrain()
{
	// ���x�}�b�v�𐶐�����
	dtl::shape::PerlinSolitaryIsland<int>(0.5f, 0.45f, 6.0f, 6, 300, -200).draw(m_terrainHeight);
	//dtl::shape::DiamondSquareAverageIsland<int>(0, 50, 0).draw(m_terrainHeight);

	// �R���𐶐�����


	// ���b�V���̍�����ύX����
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainHeight[x][y] *= 10.0f;
			m_pField->GetComponent<CMeshField>()->SetHeight(x, y, static_cast<float>(m_terrainHeight[x][y]));
		}
	}

	// �e�N�X�`���𐶐�����
	m_pField->GetComponent<CMeshField>()->SetTexture("data\\TEXTURE\\ground.png");
	m_pField->GetComponent<CMeshField>()->SetLoopTexture(100);
	m_pField->GetComponent<CMeshField>()->ResetNormals();

	// �n�`�����i�[����
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = static_cast<float>(m_terrainHeight[x][y]);
		}
	}
}

//=============================================================
// [CTerrain] �R���̐���
//=============================================================
void CTerrain::GenerateRoad()
{

}

//=============================================================
// [CTerrain] �������̓o�^
//=============================================================
void CTerrain::RegisterProduces(CNatureProduces* pNatureProduce)
{
	// �������ɓo�^����Ă��Ȃ������m�F����
	for (unsigned int i = 0; i < m_natureProduces.size(); i++)
	{
		if (m_natureProduces[i] == pNatureProduce)
		{ // ��v����Ƃ�
			return;
		}
	}

	// �o�^����
	m_natureProduces.push_back(pNatureProduce);
}

//=============================================================
// [CTerrain] �������̐���
//=============================================================
void CTerrain::GenerateProduces()
{
	for (int nTryCount = 0; nTryCount < 500; nTryCount++)
	{
		// �����_���ňʒu�����߂�
		D3DXVECTOR3 generatePos;
		generatePos = {
			rand() % static_cast<int>(TERRAIN_SIZE * TERRAIN_SCALE) - TERRAIN_SIZE * TERRAIN_SCALE * 0.5f,
			0.0f,
			rand() % static_cast<int>(TERRAIN_SIZE * TERRAIN_SCALE) - TERRAIN_SIZE * TERRAIN_SCALE * 0.5f
		};

		// �����������肷��
		CNatureProduces* pSelectProduce = nullptr;
		int nAllChance = 0;		// ���ׂĂ̊m��
		for (unsigned int i = 0; i < m_natureProduces.size(); i++)
		{
			nAllChance += static_cast<int>(m_natureProduces[i]->GetChance() * m_natureProduces[i]->GetAdjacentRate(generatePos));
		}
		int nRandValue = rand() % nAllChance;	// �����_���l
		int nMinChance = 0;								// �`�����X�͈͂̍ŏ��l
		int nMaxChance = 0;								// �`�����X�͈͂̍ő�l
		for (unsigned int i = 0; i < m_natureProduces.size(); i++)
		{
			nMaxChance = nMinChance + m_natureProduces[i]->GetChance();
			if (nMinChance <= nRandValue && nRandValue < nMaxChance)
			{ // �͈͓��̂Ƃ�
				pSelectProduce = m_natureProduces[i];
				break;
			}
			nMinChance += m_natureProduces[i]->GetChance();
		}

		if (pSelectProduce == nullptr)
		{ // �����������肵�Ȃ������Ƃ��i��O�j
			continue;
		}

		// ���C�|�C���g�̈ʒu�����߂�
		D3DXVECTOR3 rayPoint[4];
		rayPoint[0].x = -pSelectProduce->GetSize().x / 2;
		rayPoint[0].z = -pSelectProduce->GetSize().y / 2;
		rayPoint[1].x = pSelectProduce->GetSize().x / 2;
		rayPoint[1].z = -pSelectProduce->GetSize().y / 2;
		rayPoint[2].x = -pSelectProduce->GetSize().x / 2;
		rayPoint[2].z = pSelectProduce->GetSize().y / 2;
		rayPoint[3].x = pSelectProduce->GetSize().x / 2;
		rayPoint[3].z = pSelectProduce->GetSize().y / 2;

		// �������W�Ɉړ�����
		for (int i = 0; i < 4; i++)
		{
			rayPoint[i] += generatePos;
		}

		// ���C���΂�
		bool bReached = true;
		for (int i = 0; i < 4; i++)
		{
			btVector3 Start = btVector3(rayPoint[i].x, 3000.0f, rayPoint[i].z);
			btVector3 End = btVector3(rayPoint[i].x, -3000.0f, rayPoint[i].z);

			btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
			CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
			if (RayCallback.hasHit())
			{ // �q�b�g�����Ƃ�
				if (RayCallback.m_collisionObject != CCollision::GetCollision(m_pField)->GetGhostObject())
				{ // �n�ʂɐڐG���Ȃ������Ƃ�
					bReached = false;
					break;
				}
			}
		}

		// ���ׂẴ��C���n�ʂɓ��B�����Ƃ�
		if (bReached)
		{
			// ���S�Ƀ��C���΂��Đݒu���x���擾����
			btVector3 Start = btVector3(generatePos.x, 3000.0f, generatePos.z);
			btVector3 End = btVector3(generatePos.x, -3000.0f, generatePos.z);

			btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
			CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
			if (RayCallback.hasHit())
			{ // �q�b�g�����Ƃ�
				// �q�b�g����Y�̍�����ݒu�ʒu�ɂ���
				generatePos.y = RayCallback.m_hitPointWorld.getY() + pSelectProduce->GetOffsetY();

				// �I�u�W�F�N�g��ݒu����
				pSelectProduce->Generate(Transform(generatePos))->SetParent(gameObject);

				// ���[�v�𔲂���
				break;
			}
		}
	}
}