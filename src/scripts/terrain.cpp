//=============================================================
//
// �n�` [terrain.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain.h"
#include "benlib.h"
#include <fstream>
#include "manager.h"

#include "component/3d/meshfield.h"
#include "scripts/vehicle.h"
#include "scripts/wreckage.h"
#include "component/3d/collision.h"
#include "component/3d/field.h"
#include "scripts/road.h"
#include "scripts/gem.h"
using namespace noise;

#include "component/2d/text.h"
#include "renderer.h"

// �ÓI�����o�ϐ��̏�����
const float Terrain::TERRAIN_SCALE = 150.0f;
const float Terrain::TERRAIN_DISTANCE = Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE;
const float Terrain::TERRAIN_DISTANCE_HALF = Terrain::TERRAIN_DISTANCE / (float)2.0f;

//=============================================================
// [Terrain] ������
//=============================================================
void Terrain::Init()
{
	m_pShadowField = new GameObject("ShadowTerrain");
	m_pShadowField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, TERRAIN_SCALE);
	m_pShadowField->transform->SetPos(0.0f, -10.0f);

	// ���b�V���t�B�[���h���쐬����
	m_pField = new GameObject;
	m_pField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, TERRAIN_SCALE);
	m_pField->AddComponent<Road>();

	m_terrainData = nullptr;
	m_producesManager = new ProducesManager();
	m_producesManager->Init();

	// ��Q����o�^����
	LoadTerrainFile("data\\TERRAINS\\terrains.json");
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
	CCollision::RemoveCollision(m_pShadowField);

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
	CCollision::Create(m_pShadowField);

	// �V�[�h�l��ݒ肷��
	srand(m_seed);

	// �n�`�f�[�^�𐶐�����
	GenerateTerrain();

	m_pField->GetComponent<CMeshField>()->SetTexture("data\\terrain.bmp");

	// HeightfieldTerrainShape���쐬����
	m_terrainShape = new btHeightfieldTerrainShape(TERRAIN_SIZE, TERRAIN_SIZE, m_terrainData, 1, -30000, 30000, 1, PHY_FLOAT, false);
	m_terrainShape->setLocalScaling(btVector3(TERRAIN_SCALE, 1.0f, TERRAIN_SCALE));
	CCollision::GetCollision(m_pField)->SetFriction(100.0f);
	CCollision::GetCollision(m_pField)->GetGhostObject()->setCollisionShape(m_terrainShape);
	CCollision::GetCollision(m_pShadowField)->GetGhostObject()->setCollisionShape(m_terrainShape);

	// �ݒ肵���V�F�[�v��K�p����
	CPhysics::GetInstance()->GetDynamicsWorld().stepSimulation(static_cast<btScalar>(1. / 60.), 1);

	// ���̃I�u�W�F�N�g�𐶐�����
	m_pField->GetComponent<Road>()->Generate();

	// �W�F���̐���
	for (int i = 0; i < 100; i++)
	{
		GenerateGem();
	}

	// �������𐶐�����
	//for (int i = 0; i < 70000; i++)
	//{
	//	GenerateProduces();
	//}

}

//=============================================================
// [Terrain] �n�`�̐���
//=============================================================
void Terrain::GenerateTerrain()
{
	module::Perlin myModule;
	utils::NoiseMap heightMap;

	// �V�[�h��ݒ肷��
	myModule.SetSeed(rand());

	module::ScaleBias scaled;
	scaled.SetSourceModule(0, myModule);
	scaled.SetScale(100.0f);

	// �n�`�𐶐�����
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(scaled);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(TERRAIN_SIZE, TERRAIN_SIZE);
	heightMapBuilder.SetBounds(0.0, 20.0, 0.0, 20.0);
	heightMapBuilder.Build();

	// �摜�ɏ����o��
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.ClearGradient();
	for (auto itr = m_heightColor.begin(); itr != m_heightColor.end(); itr++)
	{
		renderer.AddGradientPoint((*itr).height, utils::Color(static_cast<noise::uint8>((*itr).color.r * 255), static_cast<noise::uint8>((*itr).color.g * 255), static_cast<noise::uint8>((*itr).color.b * 255), static_cast<noise::uint8>((*itr).color.a * 255)));
	}
	renderer.EnableLight();
	renderer.SetLightContrast(0.01f);
	renderer.SetLightBrightness(1.5);
	renderer.SetLightIntensity(4.5f);
	renderer.Render();

	// �t�@�C���ɏ����o��
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("data\\terrain.bmp");
	writer.WriteDestFile();


	// �n�`�����i�[����
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = heightMap.GetValue(x, y);
		}
	}

	// �Œ�ƍō����x���擾����
	m_minHeight = GetVertexHeight(0, 0);
	m_maxHeight = GetVertexHeight(0, 0);
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			float value = GetVertexHeight(x, y);
			if (value < m_minHeight)
			{
				m_minHeight = value;
			}
			if (value > m_maxHeight)
			{
				m_maxHeight = value;
			}
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
			m_pShadowField->GetComponent<CMeshField>()->SetHeight(x, y, height);
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
	// �|�C���g ------------------------------------------------

	// ���̃R���|�[�l���g���擾����
	auto pRoad = m_pField->GetComponent<Road>();

	// ���𐶐�����
	int routeData[TERRAIN_SIZE][TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			routeData[x][y] = 0;
		}
	}

	// ���ӂ̍������
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			float centerHeight = GetVertexHeight(x, y);
			for (int sX = 0; sX < 3; sX++)
			{
				for (int sY = 0; sY < 3; sY++)
				{
					if (!(sX == 1 && sY == 1))
					{ // ���S�ȊO
						float height = GetVertexHeight(x - 1 + sX, y - 1 + sY);
						
						// ���፷���擾����
						float heightDifference = height - centerHeight;

						if (heightDifference > 0)
						{ // �����Ƃ�
							routeData[x][y] += static_cast<int>(fabsf(heightDifference) * 0.1f);
						}
						if (fabsf(heightDifference) < 100.0f)
						{ // �Ⴂ�Ƃ�
							routeData[x][y] += static_cast<int>(100.0f - fabsf(heightDifference));
						}
					}
				}
			}
		}
	}

	// ���S�ɂ��قǃ|�C���g�������Ȃ�
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			int dX = abs(x) <= abs(x - TERRAIN_SIZE) ? abs(x) : abs(x - TERRAIN_SIZE);
			int dY = abs(y) <= abs(y - TERRAIN_SIZE) ? abs(y) : abs(y - TERRAIN_SIZE);
			int disNear = dX <= dY ? dX : dY;

			routeData[x][y] += static_cast<int>(disNear * 25);
		}
	}

	// ���I�� ------------------------------------------------
	for (int i = 0; i < 2; i++)
	{
		pRoad->AddIdx();

		bool isAxisX = rand() % 2 == 0;
		int currentX = isAxisX ? (rand() % TERRAIN_SIZE) : (rand() % 2 == 0 ? 0 : TERRAIN_SIZE - 1);
		int currentY = isAxisX ? (rand() % 2 == 0 ? 0 : TERRAIN_SIZE - 1) : (rand() % TERRAIN_SIZE);

		// �ŏI�n�_��ݒ肷��i���j
		Area startArea = NONE;
		while (1)
		{
			isAxisX = rand() % 2 == 0;
			currentX = isAxisX ? (rand() % TERRAIN_SIZE) : (rand() % 2 == 0 ? 0 : TERRAIN_SIZE - 1);
			currentY = isAxisX ? (rand() % 2 == 0 ? 0 : TERRAIN_SIZE - 1) : (rand() % TERRAIN_SIZE);

			// �ӂ�����o��
			if (currentX == 0) startArea = LEFT;
			if (currentX == TERRAIN_SIZE - 1) startArea = RIGHT;
			if (currentY == 0) startArea = TOP;
			if (currentY == TERRAIN_SIZE - 1) startArea = BOTTOM;

			if (m_startArea != startArea)
			{
				m_startArea = startArea;
				break;
			}
		}
		pRoad->AddPoint(i, currentX, currentY);

		// ����L�΂�
		for (int pass = 0; pass < 200; pass++)
		{
			// ���ݒn�_����|�C���g�̍��������Ɍ�����
			int selectX = 0;
			int selectY = 0;
			int high = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					// ���O���X�g
					if ((x == 0 && y == 0) ||
						currentX + x < 0 || TERRAIN_SIZE <= currentX + x ||
						currentY + y < 0 || TERRAIN_SIZE <= currentY + y)
					{ // ���݈ʒu�Ɣ͈͊O
						continue;	// �X�L�b�v
					}

					// ���Ƀ��[�g�̎������O
					if (pRoad->IsRoutedByIdx(i, currentX + x, currentY + y))
					{
						continue;	// �X�L�b�v
					}

					// ���͂ɓ�������Ƃ�
					int nearRoad = 0;
					for (int sx = -1; sx < 2; sx++)
					{
						for (int sy = -1; sy < 2; sy++)
						{
							// ���O���X�g
							if ((sx == 0 && sy == 0) ||
								x + currentX + sx < 0 || TERRAIN_SIZE <= x + currentX + sx ||
								y + currentY + sy < 0 || TERRAIN_SIZE <= y + currentY + sy)
							{ // ���݈ʒu�Ɣ͈͊O
								continue;	// �X�L�b�v
							}

							// ���ɓ��̏ꍇ
							if (pRoad->IsRoutedByIdx(i, x + currentX + sx, y + currentY + sy))
							{
								nearRoad++;
							}
						}
					}
					if (nearRoad > 1)
					{
						routeData[currentX + x][currentY + y] *= 0.2f;
					}

					if (routeData[currentX + x][currentY + y] > high)
					{ // �ō��X�R�A�̎�
						selectX = currentX + x;
						selectY = currentY + y;
						high = routeData[currentX + x][currentY + y];
					}
				}
			}
			currentX = selectX;
			currentY = selectY;

			bool isEnded = false;
			// �I������: ���S�ɂ��ǂ蒅��
			if (currentX == TERRAIN_SIZE / 2 && currentY == TERRAIN_SIZE / 2)
			{
				isEnded = true;
			}

			// �I������: ���̓��ƏՓ�
			int routedIdx = pRoad->IsIndexRouted(currentX, currentY);
			if (routedIdx != -1 && routedIdx != i)
			{
				isEnded = true;
			}

			// �I������: �ʂ̕ӂɏՓ�
			Area currentArea = NONE;
			if (currentX == 0) currentArea = LEFT;
			if (currentX == TERRAIN_SIZE - 1) currentArea = RIGHT;
			if (currentY == 0) currentArea = TOP;
			if (currentY == TERRAIN_SIZE - 1) currentArea = BOTTOM;
			if (currentArea != NONE && startArea != currentArea)
			{
				isEnded = true;
			}

			// ����ǉ�����
			pRoad->AddPoint(i, currentX, currentY);

			// ���𕽒n������
			int aveCount = 0;
			float ave = 0.0f;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					// ���O���X�g
					if (currentX + x < 0 || TERRAIN_SIZE <= currentX + x ||
						currentY + y < 0 || TERRAIN_SIZE <= currentY + y)
					{ // ���݈ʒu�Ɣ͈͊O
						continue;	// �X�L�b�v
					}
					ave += GetVertexHeight(currentX + x, currentY + y);
					aveCount++;
				}
			}
			// ���ϒl���o��
			ave /= (float)aveCount;
			// �ݒ肷��
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					// ���O���X�g
					if (currentX + x < 0 || TERRAIN_SIZE <= currentX + x ||
						currentY + y < 0 || TERRAIN_SIZE <= currentY + y)
					{ // ���݈ʒu�Ɣ͈͊O
						continue;	// �X�L�b�v
					}
					float ori = (GetVertexHeight(currentX + x, currentY + y) - ave) * 0.5f;
					SetVertexHeight(currentX + x, currentY + y, ave + ori);
				}
			}


			if (isEnded)
				break;
		}
	}

	// ��������
	pRoad->OutputText();
}

//=============================================================
// [Terrain] �W�F���̐���
//=============================================================
void Terrain::GenerateGem()
{
	// �����_���ňʒu�����߂�
	D3DXVECTOR3 generatePos;
	generatePos = {
		rand() % static_cast<int>(TERRAIN_SIZE * TERRAIN_SCALE) - TERRAIN_SIZE * TERRAIN_SCALE * 0.5f,
		0.0f,
		rand() % static_cast<int>(TERRAIN_SIZE * TERRAIN_SCALE) - TERRAIN_SIZE * TERRAIN_SCALE * 0.5f
	};

	btVector3 Start = btVector3(generatePos.x, m_maxHeight + 10.0f, generatePos.z);
	btVector3 End = btVector3(generatePos.x, m_minHeight - 10.0f, generatePos.z);

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit())
	{ // �q�b�g�����Ƃ�
		GameObject* gemObj = GameObject::LoadPrefab("data\\PREFAB\\item\\gem.pref", Transform({generatePos.x, RayCallback.m_hitPointWorld.getY() + 20.0f, generatePos.z}));
		gemObj->AddComponent<Gem>();
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
// [Terrain] ���_�̍�����ݒ肷��
//=============================================================
void Terrain::SetVertexHeight(const int& x, const int& y, const float& height)
{
	if (0 <= x && x < TERRAIN_SIZE &&
		0 <= y && y < TERRAIN_SIZE)
	{
		m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = height;
	}
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

		// ���̋߂��̂Ƃ�
		int chunkX = static_cast<int>((generatePos.x + TERRAIN_SIZE * TERRAIN_SCALE * 0.5f) / (float)TERRAIN_SCALE);
		int chunkY = static_cast<int>((generatePos.y + TERRAIN_SIZE * TERRAIN_SCALE * 0.5f) / (float)TERRAIN_SCALE);
		if (m_pField->GetComponent<Road>()->IsRouted(chunkX, chunkY))
		{ // ���łɓ��̂Ƃ�
			continue;
		}

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

		// ���ӂɐ��������Ȃ������m�F����
		if (m_producesManager->FindProducesByDistance(generatePos, pSelectProduce->GetSize()))
		{ // ���݂���Ƃ�
			continue;
		}

		// ���C�|�C���g�̈ʒu�����߂�
		D3DXVECTOR3 rayPoint[3];
		rayPoint[0].x = -pSelectProduce->GetSize() / 2;
		rayPoint[0].z = pSelectProduce->GetSize() / 2;
		rayPoint[1].x = pSelectProduce->GetSize() / 2;
		rayPoint[1].z = pSelectProduce->GetSize() / 2;
		rayPoint[2].x = 0.0f;
		rayPoint[2].z = -pSelectProduce->GetSize() / 2;

		// �������W�Ɉړ�����
		for (int i = 0; i < 3; i++)
		{
			rayPoint[i] += generatePos;
		}

		// ���C���΂�
		D3DXVECTOR3 rayReachPoint[3];
		bool bReached = true;
		for (int i = 0; i < 3; i++)
		{
			btVector3 Start = btVector3(rayPoint[i].x, m_maxHeight + 10.0f, rayPoint[i].z);
			btVector3 End = btVector3(rayPoint[i].x, m_minHeight - 10.0f, rayPoint[i].z);

			btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
			CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
			if (RayCallback.hasHit())
			{ // �q�b�g�����Ƃ�
				if (RayCallback.m_collisionObject != CCollision::GetCollision(m_pField)->GetGhostObject())
				{ // �n�ʂɐڐG���Ȃ������Ƃ�
					bReached = false;
					break;
				}
				rayReachPoint[i] = { RayCallback.m_hitPointWorld.getX(), RayCallback.m_hitPointWorld.getY(), RayCallback.m_hitPointWorld.getZ()};
			}
			else
			{
				bReached = false;
				break;
			}
		}

		// �@���x�N�g�����v�Z����
		D3DXVECTOR3 normal = Benlib::CalcNormalVector(rayReachPoint[0], rayReachPoint[1], rayReachPoint[2]);

		// �X�ΐ����̏����ɖ������Ă��Ȃ��Ƃ�
		if (!(fabsf(atan2f(normal.z, normal.y)) >= pSelectProduce->GetSlantAngleLimitMin() &&
			fabsf(atan2f(normal.z, normal.y)) <= pSelectProduce->GetSlantAngleLimitMax()))
		{
			continue;
		}

		// ���ׂẴ��C���n�ʂɓ��B�����Ƃ�
		if (bReached)
		{
			D3DXQUATERNION rot;
			D3DXQuaternionIdentity(&rot);

			// �����_���p�x���擾����
			float angleRange = pSelectProduce->GetAngleRange();
			std::function<float()> randAngle = [angleRange]() {
				if (angleRange == 0)
					return 0.0f;
				return (rand() % static_cast<int>(angleRange * 1000) - static_cast<int>(angleRange * 500)) * 0.001f; 
			};

			D3DXVECTOR3 axis = { 0.0f, 0.0f, 0.0f };
			if (!pSelectProduce->IsMatchInclination())
			{ // �X�Ίp���l�����Ȃ�
				D3DXQuaternionRotationYawPitchRoll(&rot, randAngle(), 0.0f, randAngle());
			}
			else
			{ // �X�Ίp���l������
				// �n�`�ɍ��킹���p�x���v�Z����
				axis = { 1.0f, 0.0f, 0.0f };
				D3DXQuaternionRotationAxis(&rot, &axis, fabsf(atan2f(normal.z, normal.y)) + randAngle());

				D3DXQUATERNION mulRot;
				axis = { 0.0f, 1.0f, 0.0f };
				D3DXQuaternionRotationAxis(&mulRot, &axis, atan2f(normal.x, normal.z) + randAngle());
				rot *= mulRot;
			}

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
				Transform trans;
				trans.SetPos(generatePos);
				trans.SetQuaternion(rot);
				m_producesManager->AddProduce(trans, pSelectProduce);

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

	float heightNormalize = ((height - m_minHeight) / static_cast<float>(m_maxHeight - m_minHeight) * 2) - 1.0f;

	// ���� �F���擾
	D3DXCOLOR highColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float highHeight = 0.0f;
	bool foundHighHeight = false;
	for (unsigned int i = 0; i < m_heightColor.size(); i++)
	{
		if (heightNormalize < m_heightColor[i].height &&
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
		if (heightNormalize > m_heightColor[i].height &&
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
	float currentPercent = (heightNormalize - lowHeight) / (highHeight - lowHeight);

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
// [Terrain] �n�`�t�@�C���̓ǂݍ���
//=============================================================
void Terrain::LoadTerrainFile(const std::string path)
{
	// json�t�@�C����ǂݍ���
	std::ifstream ifs(path.c_str());

	if (ifs.fail())
	{ // �t�@�C���̓ǂݍ��݂Ɏ��s
		MessageBox(CManager::GetInstance()->GetHWND(), "�n�`���t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK); //�I�����b�Z�[�W
		return;
	}

	// json�`���ɕϊ�
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto jInput = json::parse(sInputData);

	// �n�`����ǂݍ���
	if (jInput.contains("terrain"))
	{
		// ���x�J���[
		if (jInput["terrain"].contains("height_color"))
		{
			for (auto itr = jInput["terrain"]["height_color"].begin(); itr != jInput["terrain"]["height_color"].end(); itr++)
			{
				if ((*itr).contains("color") && (*itr).contains("height"))
				{
					AddHeightColor((*itr)["height"], D3DCOLOR_RGBA((*itr)["color"][0], (*itr)["color"][1], (*itr)["color"][2], (*itr)["color"][3]));
				}
			}
		}

		// �X�΃J���[
		if (jInput["terrain"].contains("slant_color"))
		{
			for (auto itr = jInput["terrain"]["slant_color"].begin(); itr != jInput["terrain"]["slant_color"].end(); itr++)
			{
				if ((*itr).contains("color") && (*itr).contains("min") && (*itr).contains("max"))
				{
					AddSlantColor((*itr)["min"], (*itr)["max"], D3DCOLOR_RGBA((*itr)["color"][0], (*itr)["color"][1], (*itr)["color"][2], (*itr)["color"][3]));
				}
			}
		}
	}

	// ����������ǂݍ���
	if (jInput.contains("produces"))
	{ // �������̍��ڂ�����Ƃ�
		for (auto itr = jInput["produces"].begin(); itr != jInput["produces"].end(); itr++)
		{
			CNatureProduces* produce = new CNatureProduces();

			// ���O
			if ((*itr).contains("name"))
			{
				produce->SetName((*itr)["name"]);
			}

			// �p�X
			if ((*itr).contains("path"))
			{
				produce->SetPath((*itr)["path"]);
			}

			// �T�C�Y
			if ((*itr).contains("size"))
			{
				produce->SetSize((*itr)["size"]);
			}

			// �_���[�W
			if ((*itr).contains("damage"))
			{
				produce->SetSize((*itr)["damage"]);
			}

			// �I�t�Z�b�gY
			if ((*itr).contains("offset"))
			{
				produce->SetOffsetY((*itr)["offset"]);
			}

			// �����_���p�x
			if ((*itr).contains("angle_range"))
			{
				produce->SetAngleRange((*itr)["angle_range"]);
			}

			// �X�Ίp�ɍ��킹�邩
			if ((*itr).contains("match_inclination"))
			{
				produce->SetIsMatchInclination((*itr)["match_inclination"]);
			}

			// �אڂ͈̔�
			if ((*itr).contains("adjacent_distance"))
			{
				produce->SetAdjacentDistance((*itr)["adjacent_distance"]);
			}

			// �m��
			if ((*itr).contains("chance"))
			{
				produce->SetChance((*itr)["chance"]);
			}

			// �X�Ίp�x����
			if ((*itr).contains("limit_slant"))
			{
				produce->SetSlantAngleLimit((*itr)["limit_slant"][0], (*itr)["limit_slant"][1]);
			}

			// �אڏ��
			if ((*itr).contains("adjacent_rate"))
			{ // �אڏ�񂪊܂܂�Ă���Ƃ�
				for (auto adjItr = (*itr)["adjacent_rate"].begin(); adjItr != (*itr)["adjacent_rate"].end(); adjItr++)
				{
					if ((*adjItr).contains("name") && (*adjItr).contains("rate"))
					{
						produce->SetAdjacentRate((*adjItr)["name"], (*adjItr)["rate"]);
					}
				}
			}

			// ��������o�^����
			RegisterProduces(produce);
		}
	}
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
		if (Benlib::PosDistance(pos, (*itrManagedProduces)->transform.GetWPos()) < 1500.0f)
		{ // �����͈͓��̂Ƃ�

			if ((*itrManagedProduces)->managedGameObject != nullptr)
			{ // ���łɐݒu�ς݂̂Ƃ�
				// �j���J�E���^�[���X�V����
				(*itrManagedProduces)->managedGameObject->destroyCounter = 600;

				// �o�C�N�Ƃ̏Փ�
				std::vector<GameObject*>& pOverlappingObj = CCollision::GetCollision((*itrManagedProduces)->managedGameObject->gameObject)->GetOverlappingGameObjects();
				for (auto itrOverlappingObj = pOverlappingObj.begin(); itrOverlappingObj != pOverlappingObj.end(); itrOverlappingObj++)
				{
					if (m_pVehicle == *itrOverlappingObj || (*itrOverlappingObj)->GetTag() == "HIT")
					{ // �o�C�N/�Փ˕��ƏՓ˂����Ƃ�
						// �I�u�W�F�N�g��j������
						ManagedGameObject* pManagedGameObj = nullptr;
						for (auto itrManagedGameObj = m_managedGameObjects.begin(); itrManagedGameObj != m_managedGameObjects.end(); itrManagedGameObj++)
						{
							if (*itrManagedGameObj == (*itrManagedProduces)->managedGameObject)
							{
								// �Q�[���I�u�W�F�N�g�̊Ǘ���j������
								pManagedGameObj = *itrManagedGameObj;
								m_managedGameObjects.erase(itrManagedGameObj);
								break;
							}
						}

						// �Q�[���I�u�W�F�N�g�Ɣz�u����j��
						(*itrManagedProduces)->managedGameObject->gameObject->AddComponent<CRigidBody>();
						(*itrManagedProduces)->managedGameObject->gameObject->AddComponent<CWreckage>();

						// �q�b�g�C�x���g�����s
						(*itrManagedProduces)->natureProduce->onHit((*itrManagedProduces)->managedGameObject->gameObject);

						// �o�C�N�Ƀ_���[�W��^����
						if (m_pVehicle == *itrOverlappingObj)
						{ // �o�C�N�̂�
							m_pVehicle->GetComponent<CVehicle>()->AddDamage((*itrManagedProduces)->natureProduce->GetDamage());
						}

						// �j��
						delete pManagedGameObj;
						delete (*itrManagedProduces);
						itrManagedProduces = m_managedProduces.erase(itrManagedProduces);
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
					(*itrManagedGameObj)->destroyCounter = 600;

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
			managedGameObject->destroyCounter = 600;
			if (managedGameObject->gameObject == nullptr || managedGameObject->natureProduce == nullptr)
			{
				continue;
			}

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


	// �Ǘ��Q�[���I�u�W�F�N�g�̐���
	for (auto itr = m_managedGameObjects.begin(); itr != m_managedGameObjects.end(); itr++)
	{
		(*itr)->destroyCounter--;
		if ((*itr)->destroyCounter <= 0)
		{ // �j���J�E���^�[��0�ɂȂ����Ƃ�
			ManagedGameObject* pManaged = *itr;
			pManaged->gameObject->Destroy();
			itr = m_managedGameObjects.erase(itr);
			delete pManaged;

			if (itr == m_managedGameObjects.end())
				break;
		}
	}
}

//=============================================================
// [ProducesManager] �߂��̐��������擾����
//=============================================================
float ProducesManager::GetNearProducesRate(const std::string& name, const D3DXVECTOR3& pos, const float& range)
{
	float rate = 1.0f;
	//for (auto itr = m_managedProduces.begin(); itr != m_managedProduces.end(); itr++)
	//{
	//	if (Benlib::PosDistance((*itr)->transform.GetPos(), pos) < range)
	//	{ // �͈͓��̎�
	//		rate *= (*itr)->natureProduce->GetAdjacentObjectRate(name);
	//	}
	//}

	return rate;
}

//=============================================================
// [ProducesManager] �w�苗�����ɐ����������邩
//=============================================================
bool ProducesManager::FindProducesByDistance(const D3DXVECTOR3& pos, const float& distance)
{
	//for (auto itr = m_managedProduces.begin(); itr != m_managedProduces.end(); itr++)
	//{
	//	if (Benlib::PosDistance((*itr)->transform.GetPos(), pos) < distance)
	//	{ // �͈͓��̎�
	//		return true;
	//	}
	//}
	return false;
}