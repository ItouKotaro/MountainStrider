//=============================================================
//
// �n�` [terrain.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain.h"
#include "component/3d/meshfield.h"
using namespace noise;

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
	for (int i = 0; i < 5000; i++)
	{
		GenerateProduces();
	}

}

//=============================================================
// [CTerrain] �n�`�̐���
//=============================================================
void CTerrain::GenerateTerrain()
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

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.ClearGradient();
	renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); // deeps
	renderer.AddGradientPoint(-0.2500, utils::Color(0, 0, 255, 255)); // shallow
	renderer.AddGradientPoint(0.0000, utils::Color(0, 128, 255, 255)); // shore
	renderer.AddGradientPoint(0.0625, utils::Color(240, 240, 64, 255)); // sand
	renderer.AddGradientPoint(0.1250, utils::Color(32, 160, 0, 255)); // grass
	renderer.AddGradientPoint(0.3750, utils::Color(224, 224, 0, 255)); // dirt
	renderer.AddGradientPoint(0.7500, utils::Color(128, 128, 128, 255)); // rock
	renderer.AddGradientPoint(1.0000, utils::Color(255, 255, 255, 255)); // snow
	renderer.EnableLight();
	renderer.SetLightContrast(3.0);
	renderer.SetLightBrightness(2.0); // Double the brightness
	renderer.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("tutorial.bmp");
	writer.WriteDestFile();

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

	// �e�N�X�`���𐶐�����
	//m_pField->GetComponent<CMeshField>()->SetTexture("data\\TEXTURE\\ground.png");
	//m_pField->GetComponent<CMeshField>()->SetLoopTexture(100);

	m_pField->GetComponent<CMeshField>()->ResetNormals();
}

//=============================================================
// [CTerrain] �R���̐���
//=============================================================
void CTerrain::GenerateRoad()
{

}

//=============================================================
// [CTerrain] ���_�̍������擾����
//=============================================================
float CTerrain::GetVertexHeight(const int& x, const int& y)
{
	if (0 <= x && x < TERRAIN_SIZE &&
		0 <= y && y < TERRAIN_SIZE)
	{
		return m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE];
	}
	return 0.0f;
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

//=============================================================
// [CTerrain] ���x�̐F��ǉ�����
//=============================================================
void CTerrain::AddHeightColor(const float& height, const D3DXCOLOR& color)
{
	// �f�[�^������
	HeightColor heightColor;
	heightColor.height = height;
	heightColor.color = color;
	m_heightColor.push_back(heightColor);
}

//=============================================================
// [CTerrain] �X�΂̐F��ǉ�����
//=============================================================
void CTerrain::AddSlantColor(const float& minHeight, const float& maxHeight, const D3DXCOLOR& color, const float& rate)
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
// [CTerrain] ���x�̐F���擾����
//=============================================================
D3DXCOLOR CTerrain::GetHeightColor(const float& height)
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
			(!foundHighHeight | highHeight > m_heightColor[i].height))
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
			(!foundLowHeight | lowHeight < m_heightColor[i].height))
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
// [CTerrain] ���x�̐F���擾����
//=============================================================
D3DXCOLOR CTerrain::GetVertexColor(const float& x, const float& y)
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