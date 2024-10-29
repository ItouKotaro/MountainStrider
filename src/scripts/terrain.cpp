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

//=============================================================
// [CTerrain] ������
//=============================================================
void CTerrain::Init()
{
	// ���b�V���t�B�[���h���쐬����
	m_pField = new GameObject;
	m_pField->SetParent(gameObject);
	m_pField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, 10.0f);

	m_indices = nullptr;
	m_vertices = nullptr;
	m_pMeshData = nullptr;
	m_pMeshCollision = nullptr;
}

//=============================================================
// [CTerrain] �I��
//=============================================================
void CTerrain::Uninit()
{
	CCollision::RemoveCollision(m_pField);
	if (m_indices != nullptr)
	{
		delete[] m_indices;
		m_indices = nullptr;
	}
	if (m_vertices != nullptr)
	{
		delete[] m_vertices;
		m_vertices = nullptr;
	}
	if (m_pMeshData != nullptr)
	{
		delete m_pMeshData;
		m_pMeshData = nullptr;
	}
	if (m_pMeshCollision != nullptr)
	{
		delete m_pMeshCollision;
		m_pMeshCollision = nullptr;
	}
}

//=============================================================
// [CTerrain] ����
//=============================================================
void CTerrain::Generate()
{
	// �I������
	this->Uninit();

	// �R���W�������쐬����
	CCollision::Create(m_pField);

	// ���x�}�b�v�𐶐�����
	dtl::shape::DiamondSquareAverageIsland<int>(5, 100, 0).draw(m_terrainHeight);

	// ���b�V���̍�����ύX����
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_pField->GetComponent<CMeshField>()->SetHeight(x, y, m_terrainHeight[x][y]);
		}
	}
	m_pField->GetComponent<CMeshField>()->ResetNormals();

	// �C���f�b�N�X�����i�[����
	std::vector<int>& indices = m_pField->GetComponent<CMeshField>()->GetIndices();
	m_indices = new int[indices.size()];
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		m_indices[i] = indices[i];
	}

	// ���_�����i�[����
	std::vector<D3DXVECTOR3>& vertices = m_pField->GetComponent<CMeshField>()->GetVertices();
	m_vertices = new float[vertices.size() * 3];
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		m_vertices[i * 3] = vertices[i].x;
		m_vertices[i * 3 + 1] = vertices[i].y;
		m_vertices[i * 3 + 2] = vertices[i].z;
	}

	// ���b�V���f�[�^���쐬����
	m_pMeshData = new btTriangleIndexVertexArray(
		m_pField->GetComponent<CMeshField>()->GetNumFaces(),
		m_indices,
		sizeof(int),
		static_cast<int>(m_pField->GetComponent<CMeshField>()->GetVertices().size()),
		m_vertices,
		sizeof(D3DXVECTOR3));

	// �R���W�����𐶐�����
	m_pMeshCollision = new btConvexTriangleMeshShape(m_pMeshData, true);
	CCollision::GetCollision(m_pField)->GetGhostObject()->setCollisionShape(m_pMeshCollision);
}