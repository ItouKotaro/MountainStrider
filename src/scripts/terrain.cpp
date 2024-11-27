//=============================================================
//
// �n�` [terrain.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain.h"
#include "benlib.h"
#include <fstream>

#include "component/3d/meshfield.h"
#include "scripts/vehicle.h"
#include "scripts/wreckage.h"
#include "component/3d/collision.h"
using namespace noise;

#include "component/2d/text.h"
#include "renderer.h"

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
	m_producesManager->Init();

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

	// �V�[�h�l��ݒ肷��
	srand(m_seed);

	// �n�`�f�[�^�𐶐�����
	GenerateTerrain();

	// HeightfieldTerrainShape���쐬����
	m_terrainShape = new btHeightfieldTerrainShape(TERRAIN_SIZE, TERRAIN_SIZE, m_terrainData, 1, -30000, 30000, 1, PHY_FLOAT, false);
	m_terrainShape->setLocalScaling(btVector3(TERRAIN_SCALE, 1.0f, TERRAIN_SCALE));
	CCollision::GetCollision(m_pField)->GetGhostObject()->setCollisionShape(m_terrainShape);

	// �ݒ肵���V�F�[�v��K�p����
	CPhysics::GetInstance()->GetDynamicsWorld().stepSimulation(static_cast<btScalar>(1. / 60.), 1);

	// �������𐶐�����
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

	myModule.SetSeed(rand());

	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(TERRAIN_SIZE, TERRAIN_SIZE);
	heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	heightMapBuilder.Build();

	// �n�`�����i�[����
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = heightMap.GetValue(x, y) * 1500.0f;
		}
	}

	// ���𐶐�����
	GenerateRoad();

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

			if (m_routeData[x][y])
			{
				m_pField->GetComponent<CMeshField>()->SetColor(x, y, D3DCOLOR_RGBA(179, 119, 0, 255));
			}
		}
	}

	// �@�������Z�b�g����
	m_pField->GetComponent<CMeshField>()->ResetNormals();
}

//=============================================================
// [Terrain] ���̐���
//=============================================================
void Terrain::GenerateRoad()
{
	// ���݈ʒu���ϐ�
	int currentX = TERRAIN_SIZE / 2;
	int currentY = TERRAIN_SIZE / 2;
	float currentHeight = GetVertexHeight(currentX, currentY);

	// ���[�g�z��̏�����
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_routeData[x][y] = false;
		}
	}
	m_routeData[currentX][currentY] = true;

	// �o�H�v�Z�Ɏg���\����
	struct RouteData
	{
		int x;				// X
		int y;				// Y
		float height;	// ���x
		int point;		// �|�C���g�i�Ⴂ�ƗD��j
	};

	// �o�H�A���S���Y��
	while (1)
	{
		//--------------------------------------------------------------------
		// �o�H����

		// ���ӂ̍��������擾����
		std::vector<RouteData> aroundRouteData;
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (!(x == 1 && y == 1))
				{ // ���S�ȊO
					RouteData data;
					data.x = currentX - 1 + x;
					data.y = currentY - 1 + y;
					data.height = GetVertexHeight(currentX - 1 + x, currentY - 1 + y);
					aroundRouteData.push_back(data);
				}
			}
		}

		// ���፷�Ń|�C���g�����Z����
		std::sort(aroundRouteData.begin(), aroundRouteData.end(),
			[currentHeight](RouteData& com1, RouteData& com2) {return fabsf(com1.height - currentHeight) < fabsf(com2.height - currentHeight); }
		);
		for (unsigned int i = 0; i < aroundRouteData.size(); i++)
		{
			// ���፷���Ƀ|�C���g�����Z����
			aroundRouteData[i].point += i;

			if (aroundRouteData[i].height < currentHeight)
			{ // ���ݍ��x���Ⴂ�Ƃ��|�C���g������
				aroundRouteData[i].point -= 3;
			}
		}

		// ����Ɏ��ӂɓ������݂���Ƃ��|�C���g�����Z����
		for (unsigned int i = 0; i < aroundRouteData.size(); i++)
		{
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					// ���O���X�g
					if ((x == 1 && y == 1) ||
						(x == currentX && y == currentY))
					{ // �����ƌ��݈ʒu
						continue;	// �X�L�b�v
					}

					if (aroundRouteData[i].x + x >= 0 && aroundRouteData[i].y + y > 0)
					{
						if (m_routeData[aroundRouteData[i].x + x][aroundRouteData[i].y + y])
						{ // ���ӂɑ��݂���Ƃ�
							aroundRouteData[i].point += 5;
						}
					}
				}
			}
		}


		// ���Ӄf�[�^���|�C���g�������Ń\�[�g����
		std::sort(aroundRouteData.begin(), aroundRouteData.end(),
			[](RouteData& com1, RouteData& com2) {return com1.point < com2.point;}
		);

		// �ł��ɂ₩�ȕ����ɓ���L�΂�
		for (unsigned int i = 0; i < aroundRouteData.size(); i++)
		{
			if (!m_routeData[aroundRouteData[i].x][aroundRouteData[i].y])
			{ // �o�H�ɂȂ��Ă��Ȃ��Ƃ�
				m_routeData[aroundRouteData[i].x][aroundRouteData[i].y] = true;
				currentX = aroundRouteData[i].x;
				currentY = aroundRouteData[i].y;

				// �t�@�C���Ɍo�H�f�[�^�������o��
				std::ofstream outputRoute("route.txt");
				for (int y = 0; y < TERRAIN_SIZE; y++)
				{
					for (int x = 0; x < TERRAIN_SIZE; x++)
					{
						char mark = m_routeData[x][y] ? '#' : 'X';
						outputRoute << mark;
					}
					outputRoute << std::endl;
				}
				outputRoute.close();
				break;
			}
		}

		//--------------------------------------------------------------------
		// �I������

		// �R�̎���ɂ��ǂ蒅�����Ƃ�
		if (currentX == 0 || currentX == TERRAIN_SIZE - 1 ||
			currentY == 0 || currentY == TERRAIN_SIZE - 1)
		{
			break;	// �I��
		}
	}
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
// [ProducesManager] ������
//=============================================================
void ProducesManager::Init()
{
	m_pNumObj = new GameObject("TerrainNumObj");
	m_pNumObj->AddComponent<CText>();
	m_pNumObj->GetComponent<CText>()->SetFontSize(80);
	m_pNumObj->GetComponent<CText>()->SetAlign(CText::ALIGN::RIGHT);
	m_pNumObj->transform->SetPos(CRenderer::SCREEN_WIDTH - 50.0f, 30.0f);
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
	// �f�o�b�O�p
	m_pNumObj->GetComponent<CText>()->SetText("����������: " + std::to_string(m_managedGameObjects.size()));

	// �o�C�N�̎擾
	if (m_pVehicle == nullptr)
	{
		m_pVehicle = GameObject::Find("Vehicle");
	}

	// �Q�[���I�u�W�F�N�g�̍X�V
	UpdateGameObjects(pos);
}

//=============================================================
// [ProducesManager] �Q�[���I�u�W�F�N�g�̍X�V
//=============================================================
void ProducesManager::UpdateGameObjects(const D3DXVECTOR3& pos)
{
	for (auto itrManagedProduces = m_managedProduces.begin(); itrManagedProduces != m_managedProduces.end(); itrManagedProduces++)
	{
		if (Benlib::PosDistance(pos, (*itrManagedProduces)->transform.GetWPos()) < 3500.0f)
		{ // �����͈͓��̂Ƃ�

			if ((*itrManagedProduces)->managedGameObject != nullptr)
			{ // ���łɐݒu�ς݂̂Ƃ�
				std::vector<GameObject*>& pOverlappingObj = CCollision::GetCollision((*itrManagedProduces)->managedGameObject->gameObject)->GetOverlappingGameObjects();
				for (auto itrOverlappingObj = pOverlappingObj.begin(); itrOverlappingObj != pOverlappingObj.end(); itrOverlappingObj++)
				{
					if (m_pVehicle == *itrOverlappingObj)
					{ // �o�C�N�ƏՓ˂����Ƃ�
						// �I�u�W�F�N�g��j������
						ManagedGameObject* pManagedGameObj = nullptr;
						for (auto itrManagedGameObj = m_managedGameObjects.begin(); itrManagedGameObj != m_managedGameObjects.end(); itrManagedGameObj++)
						{
							if (*itrManagedGameObj == (*itrManagedProduces)->managedGameObject)
							{
								// �Q�[���I�u�W�F�N�g�̊Ǘ���j������
								pManagedGameObj = *itrManagedGameObj;
								SAFE_ERASE(m_managedGameObjects, itrManagedGameObj)
								break;
							}
						}

						// �Q�[���I�u�W�F�N�g�Ɣz�u����j��
						(*itrManagedProduces)->managedGameObject->gameObject->AddComponent<CRigidBody>();
						(*itrManagedProduces)->managedGameObject->gameObject->AddComponent<CWreckage>();

						// �q�b�g�C�x���g�����s
						(*itrManagedProduces)->natureProduce->onHit((*itrManagedProduces)->managedGameObject->gameObject);

						// �o�C�N�Ƀ_���[�W��^����
						m_pVehicle->GetComponent<CVehicle>()->AddDamage((*itrManagedProduces)->natureProduce->GetDamage());

						// �j��
						delete pManagedGameObj;
						delete (*itrManagedProduces);
						SAFE_ERASE(m_managedProduces, itrManagedProduces);

						break;
					}
				}
				continue;
			}

			// �I�u�W�F�N�g���]���Ă��Ȃ���
			bool findVacancy = false;
			for (auto itrManagedGameObj = m_managedGameObjects.begin(); itrManagedGameObj != m_managedGameObjects.end(); itrManagedGameObj++)
			{
				if (!(*itrManagedGameObj)->gameObject->GetActive() &&
					(*itrManagedProduces)->natureProduce == (*itrManagedGameObj)->natureProduce)
				{ // ������ނ̐������Ŕ�A�N�e�B�u�̂Ƃ�
					// �Q�[���I�u�W�F�N�g�̃g�����X�t�H�[����K�p����
					(*itrManagedGameObj)->gameObject->transform->SetPos((*itrManagedProduces)->transform.GetPos());
					(*itrManagedGameObj)->gameObject->transform->SetQuaternion((*itrManagedProduces)->transform.GetQuaternion());
					(*itrManagedGameObj)->gameObject->transform->SetScale((*itrManagedProduces)->transform.GetScale());

					// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���
					(*itrManagedGameObj)->gameObject->SetActive(true);

					// �ݒu���ɃQ�[���I�u�W�F�N�g����ݒ肷��
					(*itrManagedProduces)->managedGameObject = (*itrManagedGameObj);

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
			managedGameObject->gameObject = (*itrManagedProduces)->natureProduce->Generate((*itrManagedProduces)->transform);
			managedGameObject->natureProduce = (*itrManagedProduces)->natureProduce;

			// �R���W������ύX����
			managedGameObject->gameObject->Destroy(managedGameObject->gameObject->GetComponent<CRigidBody>());
			CCollision::GetCollision(managedGameObject->gameObject)->IsTrigger(true);

			if (managedGameObject->gameObject != nullptr)
			{ // �Q�[���I�u�W�F�N�g������ɐ������ꂽ�Ƃ�
				m_managedGameObjects.push_back(managedGameObject);
			}
			else
			{ // �Q�[���I�u�W�F�N�g�̐����Ɏ��s�����Ƃ�
				if (managedGameObject != nullptr)
				{
					delete managedGameObject;
					managedGameObject = nullptr;
				}
			}

			// �ݒu���ɃQ�[���I�u�W�F�N�g����ݒ肷��
			(*itrManagedProduces)->managedGameObject = managedGameObject;
		}
		else
		{ // �����͈͊O�̂Ƃ�
			if ((*itrManagedProduces)->managedGameObject != nullptr)
			{
				// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���
				(*itrManagedProduces)->managedGameObject->gameObject->SetActive(false);

				// �ݒu�I�u�W�F�N�g���珜�O����
				(*itrManagedProduces)->managedGameObject = nullptr;
			}
		}
	}
}

//=============================================================
// [ProducesManager] �߂��̐��������擾����
//=============================================================
float ProducesManager::GetNearProduces(const std::string& name, const D3DXVECTOR3& pos, const float& range)
{
	float rate = 1.0f;
	for (auto itr = m_managedProduces.begin(); itr != m_managedProduces.end(); itr++)
	{
		if (Benlib::PosDistance((*itr)->transform.GetPos(), pos) < range)
		{ // �͈͓��̎�
			rate *= (*itr)->natureProduce->GetAdjacentObjectRate(name);
		}
	}

	return rate;
}