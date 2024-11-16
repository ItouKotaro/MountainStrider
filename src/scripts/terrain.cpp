//=============================================================
//
// �n�` [terrain.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain.h"
#include "benlib.h"

#include "component/3d/meshfield.h"
using namespace noise;

// �ÓI�����o�ϐ��̏�����
const float Terrain::TERRAIN_SCALE = 300.0f;

//=============================================================
// [Terrain] ������
//=============================================================
void Terrain::Init()
{
	// ���b�V���t�B�[���h���쐬����
	m_pField = new GameObject;
	m_pField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, TERRAIN_SCALE);

	m_terrainData = nullptr;
	m_producesManager = new ProducesManager();

	// ��Q����o�^����

	// ��
	CProdTree* prodTree = new CProdTree();
	prodTree->SetChance(10);
	prodTree->SetAdjacentRate("tree", 10.0f);
	RegisterProduces(prodTree);
	
	// �t�F���X
	CProdFence* prodFence = new CProdFence();
	prodFence->SetChance(1);
	prodFence->SetAdjacentRate("fence", 100.0f);
	RegisterProduces(prodFence);

	// ���x�J���[��ݒ肷��
	AddHeightColor(120.0f, D3DCOLOR_RGBA(11, 112, 0, 255));
	AddHeightColor(-1000.0f, D3DCOLOR_RGBA(50, 166, 8, 255));

	// �X�΃J���[��ݒ肷��
	AddSlantColor(300.0f, 500.0f, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f), 0.6f);
}

//=============================================================
// [Terrain] �I��
//=============================================================
void Terrain::Uninit()
{
	// �n�`�̔j��
	UninitTerrain();

	// �o�^���������������
	for (unsigned int i = 0; i < m_registerNatureProduces.size(); i++)
	{
		if (m_registerNatureProduces[i] != nullptr)
		{
			delete m_registerNatureProduces[i];
			m_registerNatureProduces[i] = nullptr;
		}
	}
	m_registerNatureProduces.clear();

	// �������Ǘ����������
	if (m_producesManager != nullptr)
	{
		m_producesManager->Uninit();
		delete m_producesManager;
		m_producesManager = nullptr;
	}
}

//=============================================================
// [Terrain] �X�V
//=============================================================
void Terrain::Update(const D3DXVECTOR3& pos)
{
	// ���������X�V����
	m_producesManager->Update(pos);
}

//=============================================================
// [Terrain] �n�`�̏I������
//=============================================================
void Terrain::UninitTerrain()
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
// [Terrain] ����
//=============================================================
void Terrain::Generate()
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
	for (int i = 0; i < 10000; i++)
	{
		GenerateProduces();
	}

}

//=============================================================
// [Terrain] �n�`�̐���
//=============================================================
void Terrain::GenerateTerrain()
{
	module::Perlin myModule;
	utils::NoiseMap heightMap;

	myModule.SetSeed((unsigned int)clock());

	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(TERRAIN_SIZE, TERRAIN_SIZE);
	heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	heightMapBuilder.Build();

	// �R���𐶐�����
	GenerateRoad();

	// �n�`�����i�[����
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = heightMap.GetValue(x, y) * 1500.0f;
		}
	}

	// ���b�V������ύX����
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			float height = GetVertexHeight(x, y);

			// ������ݒ肷��
			m_pField->GetComponent<CMeshField>()->SetHeight(x, y, height);

			// �F��ݒ肷��
			m_pField->GetComponent<CMeshField>()->SetColor(x, y, GetVertexColor(x, y));
		}
	}

	m_pField->GetComponent<CMeshField>()->ResetNormals();
}

//=============================================================
// [Terrain] �R���̐���
//=============================================================
void Terrain::GenerateRoad()
{

}

//=============================================================
// [Terrain] ���_�̍������擾����
//=============================================================
float Terrain::GetVertexHeight(const int& x, const int& y)
{
	if (0 <= x && x < TERRAIN_SIZE &&
		0 <= y && y < TERRAIN_SIZE)
	{
		return m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE];
	}
	return 0.0f;
}

//=============================================================
// [Terrain] �������̓o�^
//=============================================================
void Terrain::RegisterProduces(CNatureProduces* pNatureProduce)
{
	// �������ɓo�^����Ă��Ȃ������m�F����
	for (unsigned int i = 0; i < m_registerNatureProduces.size(); i++)
	{
		if (m_registerNatureProduces[i] == pNatureProduce)
		{ // ��v����Ƃ�
			return;
		}
	}

	// �o�^����
	m_registerNatureProduces.push_back(pNatureProduce);
}

//=============================================================
// [Terrain] �������̐���
//=============================================================
void Terrain::GenerateProduces()
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
		for (unsigned int i = 0; i < m_registerNatureProduces.size(); i++)
		{
			nAllChance += static_cast<int>(m_registerNatureProduces[i]->GetChance() * m_registerNatureProduces[i]->GetAdjacentRate(generatePos));
		}
		int nRandValue = rand() % nAllChance;	// �����_���l
		int nMinChance = 0;								// �`�����X�͈͂̍ŏ��l
		int nMaxChance = 0;								// �`�����X�͈͂̍ő�l
		for (unsigned int i = 0; i < m_registerNatureProduces.size(); i++)
		{
			nMaxChance = nMinChance + m_registerNatureProduces[i]->GetChance();
			if (nMinChance <= nRandValue && nRandValue < nMaxChance)
			{ // �͈͓��̂Ƃ�
				pSelectProduce = m_registerNatureProduces[i];
				break;
			}
			nMinChance += m_registerNatureProduces[i]->GetChance();
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
				m_producesManager->AddProduce(Transform(generatePos), pSelectProduce);

				// ���[�v�𔲂���
				break;
			}
		}
	}
}

//=============================================================
// [Terrain] ���x�̐F��ǉ�����
//=============================================================
void Terrain::AddHeightColor(const float& height, const D3DXCOLOR& color)
{
	// �f�[�^������
	HeightColor heightColor;
	heightColor.height = height;
	heightColor.color = color;
	m_heightColor.push_back(heightColor);
}

//=============================================================
// [Terrain] �X�΂̐F��ǉ�����
//=============================================================
void Terrain::AddSlantColor(const float& minHeight, const float& maxHeight, const D3DXCOLOR& color, const float& rate)
{
	// �f�[�^������
	SlantColor slantColor;
	slantColor.minHeight = minHeight;
	slantColor.maxHeight = maxHeight;
	slantColor.color = color;
	slantColor.rate = rate;
	m_slantColor.push_back(slantColor);
}

//=============================================================
// [Terrain] ���x�̐F���擾����
//=============================================================
D3DXCOLOR Terrain::GetHeightColor(const float& height)
{
	if (m_heightColor.size() <= 0)
	{ // ���x�J���[�������Ƃ��͔���Ԃ�
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// ���� �F���擾
	D3DXCOLOR highColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float highHeight = 0.0f;
	bool foundHighHeight = false;
	for (unsigned int i = 0; i < m_heightColor.size(); i++)
	{
		if (height < m_heightColor[i].height &&
			(!foundHighHeight || highHeight > m_heightColor[i].height))
		{
			highColor = m_heightColor[i].color;
			highHeight = m_heightColor[i].height;
			foundHighHeight = true;
		}
	}

	// �Ⴂ �F���擾
	D3DXCOLOR lowColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	float lowHeight = 0.0f;
	bool foundLowHeight = false;
	for (unsigned int i = 0; i < m_heightColor.size(); i++)
	{
		if (height > m_heightColor[i].height &&
			(!foundLowHeight || lowHeight < m_heightColor[i].height))
		{
			lowColor = m_heightColor[i].color;
			lowHeight = m_heightColor[i].height;
			foundLowHeight = true;
		}
	}

	// ����������Ȃ������Ƃ��̏���
	if (!foundHighHeight & !foundLowHeight)
	{
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �Е��̂݌��������Ƃ��̏���
	if (foundHighHeight ^ foundLowHeight)
	{
		return foundHighHeight ? highColor : lowColor;
	}

	// �����͈̔͂��猻�݈ʒu�̊������v�Z����
	float currentPercent = (height - lowHeight) / (highHeight - lowHeight);

	// ��������F���v�Z����
	D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	color = highColor * currentPercent + lowColor * (1.0f - currentPercent);

	return color;
}

//=============================================================
// [Terrain] ���x�̐F���擾����
//=============================================================
D3DXCOLOR Terrain::GetVertexColor(const int& x, const int& y)
{
	// ���x���擾����
	float height = GetVertexHeight(x, y);

	// �w��ʒu����̍ő卂�x�����߂�
	float positiveHeight = 0.0f;
	for (int vx = -1; vx < 2; vx++)
	{
		for (int vy = -1; vy < 2; vy++)
		{
			if (0 <= x + vx && x + vx < TERRAIN_SIZE &&
				0 <= y + vy && y + vy < TERRAIN_SIZE)
			{
				positiveHeight = positiveHeight < fabsf(height - GetVertexHeight(x + vx, y + vy)) ? fabsf(height - GetVertexHeight(x + vx, y + vy)) : positiveHeight;
			}
		}
	}

	// �͈͓��̌X�΃J���[���擾����
	D3DXCOLOR slantColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float slantRate = 0.0f;
	for (unsigned int i = 0; i < m_slantColor.size(); i++)
	{
		if (m_slantColor[i].minHeight <= positiveHeight &&
			m_slantColor[i].maxHeight >= positiveHeight)
		{ // �͈͓������������Ƃ�
			slantColor = m_slantColor[i].color;
			slantRate = m_slantColor[i].rate;
			break;
		}
	}

	// ���ʂ̐F���v�Z����
	D3DXCOLOR result = GetHeightColor(height) * (1.0f - slantRate) + slantColor * slantRate;
	return result;
}


//=============================================================
// [ProducesManager] �������̔z�u����ǉ�����
//=============================================================
void ProducesManager::AddProduce(const Transform& transform, CNatureProduces* pNatureProduce)
{
	// �Ǘ��f�[�^�ɒǉ�����
	ManagedProduce* managedProduce = new ManagedProduce;
	managedProduce->transform = transform;
	managedProduce->natureProduce = pNatureProduce;
	managedProduce->managedGameObject = nullptr;
	m_managedProduces.push_back(managedProduce);
}

//=============================================================
// [ProducesManager] �I��
//=============================================================
void ProducesManager::Uninit()
{
	// ���������Q�[���I�u�W�F�N�g��j������
	for (unsigned int i = 0; i < m_managedGameObjects.size(); i++)
	{
		if (m_managedGameObjects[i] != nullptr)
		{
			m_managedGameObjects[i]->gameObject->Destroy();
			delete m_managedGameObjects[i];
			m_managedGameObjects[i] = nullptr;
		}
	}

	for (unsigned int i = 0; i < m_managedProduces.size(); i++)
	{
		if (m_managedProduces[i] != nullptr)
		{
			delete m_managedProduces[i];
			m_managedProduces[i] = nullptr;
		}
	}
}

//=============================================================
// [ProducesManager] �X�V
//=============================================================
void ProducesManager::Update(const D3DXVECTOR3& pos)
{
	for (unsigned int i = 0; i < m_managedProduces.size(); i++)
	{
		if (Benlib::PosDistance(pos, m_managedProduces[i]->transform.GetWPos()) < 3500.0f)
		{ // �����͈͓��̂Ƃ�

			if (m_managedProduces[i]->managedGameObject != nullptr)
			{ // ���łɐݒu�ς݂̂Ƃ�
				continue;
			}

			// �I�u�W�F�N�g���]���Ă��Ȃ���
			bool findVacancy = false;
			for (unsigned int n = 0; n < m_managedGameObjects.size(); n++)
			{
				if (!m_managedGameObjects[n]->gameObject->GetActive() &&
					m_managedProduces[i]->natureProduce == m_managedGameObjects[n]->natureProduce)
				{ // ������ނ̐������Ŕ�A�N�e�B�u�̂Ƃ�

					// �Q�[���I�u�W�F�N�g�̃g�����X�t�H�[����K�p����
					m_managedGameObjects[n]->gameObject->transform->SetPos(m_managedProduces[i]->transform.GetPos());
					m_managedGameObjects[n]->gameObject->transform->SetQuaternion(m_managedProduces[i]->transform.GetQuaternion());
					m_managedGameObjects[n]->gameObject->transform->SetScale(m_managedProduces[i]->transform.GetScale());

					// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���
					m_managedGameObjects[n]->gameObject->SetActive(true);

					// �ݒu���ɃQ�[���I�u�W�F�N�g����ݒ肷��
					m_managedProduces[i]->managedGameObject = m_managedGameObjects[n];

					findVacancy = true;
					break;
				}
			}
			if (findVacancy)
			{ // ���������Ƃ�
				continue;	// ����
			}

			// �]�肪������Ȃ������Ƃ��i�V�K�쐬�j
			ManagedGameObject* managedGameObject = new ManagedGameObject;
			managedGameObject->gameObject = m_managedProduces[i]->natureProduce->Generate(m_managedProduces[i]->transform);
			managedGameObject->natureProduce = m_managedProduces[i]->natureProduce;
			m_managedGameObjects.push_back(managedGameObject);

			// �ݒu���ɃQ�[���I�u�W�F�N�g����ݒ肷��
			m_managedProduces[i]->managedGameObject = managedGameObject;
		}
		else
		{ // �����͈͊O�̂Ƃ�
			if (m_managedProduces[i]->managedGameObject != nullptr)
			{
				// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���
  				m_managedProduces[i]->managedGameObject->gameObject->SetActive(false);

				// �ݒu�I�u�W�F�N�g���珜�O����
				m_managedProduces[i]->managedGameObject = nullptr;
			}
		}
	}
}