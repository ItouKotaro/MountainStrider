//=============================================================
//
// ���� [decoration.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "decoration.h"

#include "component/2d/text.h"

const float DecorationManager::CHUNK_DIVISION = (Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE) / (float)MAX_CHUNK;
const int DecorationManager::DESTROY_LIMIT = 60;
//=============================================================
// [DecorationManager] ������
//=============================================================
void DecorationManager::Init(Terrain* terrain)
{
	m_terrain = terrain;

	m_oldChunkX = 0;
	m_oldChunkY = 0;

	for (int sx = -1; sx < 2; sx++)
	{
		for (int sy = -1; sy < 2; sy++)
		{
			LoadChunk(MAX_CHUNK / 2 + sx, MAX_CHUNK / 2 + sy);
		}
	}

	m_debugText = new GameObject();
	m_debugText->AddComponent<CText>();
}

//=============================================================
// [DecorationManager] �I��
//=============================================================
void DecorationManager::Uninit()
{
	// ��ނ��������
	for (auto itr = m_decoType.begin(); itr != m_decoType.end(); itr++)
	{
		if (*itr != nullptr)
		{
			delete* itr;
			*itr = nullptr;
		}
	}
	m_decoType.clear();

	// �ݒu�f�[�^��j������
	for (int x = 0; x < MAX_CHUNK; x++)
	{
		for (int y = 0; y < MAX_CHUNK; y++)
		{
			for (auto itr = m_decoData[x][y].begin(); itr != m_decoData[x][y].end(); itr++)
			{
				if (*itr != nullptr)
				{
					delete* itr;
					*itr = nullptr;
				}
			}
			m_decoData[x][y].clear();
		}
	}

	// �I�u�W�F�N�g�f�[�^��j������
	for (auto itr = m_decoObjects.begin(); itr != m_decoObjects.end(); itr++)
	{
		if (*itr != nullptr)
		{
			(*itr)->gameObject->Destroy();
			delete* itr;
		}
	}
	m_decoObjects.clear();
}

//=============================================================
// [DecorationManager] �X�V
//=============================================================
void DecorationManager::Update(const D3DXVECTOR3& pos)
{
	// �j���J�E���^�[�̍X�V
	UpdateDestroyObjects();
	m_debugText->GetComponent<CText>()->SetText(std::to_string(m_decoObjects.size()));

	// ���݈ʒu�̃`�����N���擾����
	int x, y;
	GetChunk(&x, &y, pos);

	// �O�񂩂�ʒu���ς���Ă��Ȃ��Ƃ�
	if (m_oldChunkX == x && m_oldChunkY == y)
		return;

	// �`�����N
	struct ChunkPos
	{
		int x;
		int y;
	};
	std::vector<ChunkPos> loadChunk;

	// ���Ӄ`�����N 3x3 ��ǂݍ��ݑΏۂɂ���
	for (int sx = -1; sx < 2; sx++)
	{
		for (int sy = -1; sy < 2; sy++)
		{
			// �͈̓`�F�b�N
			if (x + sx < 0 || y + sy < 0 ||
				x + sx >= MAX_CHUNK || y + sy >= MAX_CHUNK)
			{ // �͈͊O
				continue;
			}
			ChunkPos chunkPos;
			chunkPos.x = x + sx;
			chunkPos.y = y + sy;
			loadChunk.push_back(chunkPos);
		}
	}

	// �͈͂���O�ꂽ�`�����N���A�����[�h����
	for (int sx = -1; sx < 2; sx++)
	{
		for (int sy = -1; sy < 2; sy++)
		{
			// �A�����[�h�Ώۂ�
			bool isUnloadTarget = true;
			for (auto itr = loadChunk.begin(); itr != loadChunk.end(); itr++)
			{
				if (sx + m_oldChunkX == (*itr).x &&
					sy + m_oldChunkY == (*itr).y)
				{
					isUnloadTarget = false;
					break;
				}
			}

			// �A�����[�h�Ώۂ̂Ƃ�
			if (isUnloadTarget)
			{
				// --- �A�����[�h���� ---
				UnloadChunk(sx + m_oldChunkX, sy + m_oldChunkY);
			}
		}
	}

	// �V�����ǂݍ��ރ`�����N�����[�h����
	for (int sx = -1; sx < 2; sx++)
	{
		for (int sy = -1; sy < 2; sy++)
		{
			// �V�������[�h�Ώۂ�
			bool isLoadTarget = true;
			for (int ox = -1; ox < 2; ox++)
			{
				for (int oy = -1; oy < 2; oy++)
				{
					if (sx + x == ox + m_oldChunkX &&
						sy + y == oy + m_oldChunkY)
					{ // ���łɓǂݍ��܂�Ă���Ƃ�
						isLoadTarget = false;
						break;
					}
				}
				if (!isLoadTarget) break;
			}

			// ���[�h�Ώۂ̂Ƃ�
			if (isLoadTarget)
			{
				// --- ���[�h���� ---
				LoadChunk(sx + x, sy + y);
			}
		}
	}

	// �O��̃`�����N�Ƃ��ċL�^����
	m_oldChunkX = x;
	m_oldChunkY = y;
}

//=============================================================
// [DecorationManager] �f�R���[�V�����̎�ނ�ǉ�����
//=============================================================
void DecorationManager::AddDecorationType(const std::string& path, const int& chance, const float& offsetY)
{
	DecorationType* type = new DecorationType();
	type->path = path;
	type->chance = chance;
	type->offsetY = offsetY;
	type->minSlant = 0.0f;
	type->maxSlant = D3DX_PI;
	m_decoType.push_back(type);
}

//=============================================================
// [DecorationManager] �f�R���[�V�����𐶐�����
//=============================================================
void DecorationManager::GenerateDecoration()
{
	// �f�R���[�V���������߂�
	DecorationType* decoType = RandomDecoType();
	if (decoType == nullptr) return;

	// �ʒu�����߂�
	D3DXVECTOR3 pos;
	pos.x = Benlib::RandomFloat(-Terrain::TERRAIN_DISTANCE_HALF, Terrain::TERRAIN_DISTANCE_HALF);
	pos.z = Benlib::RandomFloat(-Terrain::TERRAIN_DISTANCE_HALF, Terrain::TERRAIN_DISTANCE_HALF);
	pos.y = 0.0f;

	// ���C�|�C���g�̈ʒu�����߂�
	D3DXVECTOR3 rayPoint[4];
	rayPoint[0].x = -2.0f;
	rayPoint[0].z = 2.0f;
	rayPoint[1].x = 2.0f;
	rayPoint[1].z = 2.0f;
	rayPoint[2].x = 0.0f;
	rayPoint[2].z = -2.0f;
	rayPoint[3].x = 0.0f;
	rayPoint[3].z = 0.0f;

	// �������W�Ɉړ�����
	for (int i = 0; i < 4; i++)
	{
		rayPoint[i] += pos;
	}

	// ���C���΂�
	D3DXVECTOR3 rayReachPoint[4];
	bool bReached = true;
	for (int i = 0; i < 4; i++)
	{
		btVector3 Start = btVector3(rayPoint[i].x, m_terrain->GetMaxHeight() + 10.0f, rayPoint[i].z);
		btVector3 End = btVector3(rayPoint[i].x, m_terrain->GetMinHeight() - 10.0f, rayPoint[i].z);

		btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
		CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
		if (RayCallback.hasHit())
		{ // �q�b�g�����Ƃ�
			rayReachPoint[i] = { RayCallback.m_hitPointWorld.getX(), RayCallback.m_hitPointWorld.getY(), RayCallback.m_hitPointWorld.getZ() };
		}
		else
		{
			bReached = false;
			break;
		}
	}

	// ���C���������Ă��Ȃ��Ƃ�
	if (!bReached)
	{
		return;
	}

	// �ʒu���X�V����
	pos = rayReachPoint[3];

	// �@���x�N�g�����v�Z����
	D3DXVECTOR3 normal = Benlib::CalcNormalVector(rayReachPoint[0], rayReachPoint[1], rayReachPoint[2]);

	// �X�ΐ����̏����ɖ������Ă��Ȃ��Ƃ�
	if (!(fabsf(atan2f(normal.z, normal.y)) >= decoType->minSlant &&
		fabsf(atan2f(normal.z, normal.y)) <= decoType->maxSlant))
	{
		return;
	}

	// ��]�����߂� ------------------------------------------------------------------------------------------------------------
	D3DXQUATERNION rot;
	D3DXQuaternionIdentity(&rot);

	// �����_���p�x���擾����
	float angleRange = 0.0f;
	std::function<float()> randAngle = [angleRange]() {
		if (angleRange == 0)
			return 0.0f;
		return (rand() % static_cast<int>(angleRange * 1000) - static_cast<int>(angleRange * 500)) * 0.001f;
	};

	// �n�`�ɍ��킹���p�x���v�Z����
	D3DXVECTOR3 axis = { 1.0f, 0.0f, 0.0f };
	D3DXQuaternionRotationAxis(&rot, &axis, fabsf(atan2f(normal.z, normal.y)) + randAngle());

	D3DXQUATERNION mulRot;
	axis = { 0.0f, 1.0f, 0.0f };
	D3DXQuaternionRotationAxis(&mulRot, &axis, atan2f(normal.x, normal.z) + randAngle());
	rot *= mulRot;


	// �g�����X�t�H�[���ɐݒ肷��
	Transform transform;
	transform.SetPos(pos);
	transform.SetQuaternion(rot);

	// �ݒu����i�f�[�^�ǉ��j
	AddDecorationData(decoType, transform);
}

//=============================================================
// [DecorationManager] �f�R���[�V�����f�[�^��ǉ�����
//=============================================================
void DecorationManager::AddDecorationData(DecorationType* decoType, Transform transform)
{
	DecorationData* data = new DecorationData();
	data->type = decoType;
	data->transform = transform;
	
	// �ʒu����`�����N�����߂�
	int x, y;
	GetChunk(&x, &y, transform.GetPos());

	// �f�[�^��ǉ�����
	m_decoData[x][y].push_back(data);
}

//=============================================================
// [DecorationManager] �����_���Ńf�R���[�V�����̎�ނ����߂�
//=============================================================
DecorationManager::DecorationType* DecorationManager::RandomDecoType()
{
	int nAllChance = 0;
	for (auto itr = m_decoType.begin(); itr != m_decoType.end(); itr++)
	{
		nAllChance += (*itr)->chance;
	}

	int nRandValue = rand() % nAllChance;	// �����_���l
	int nMinChance = 0;								// �`�����X�͈͂̍ŏ��l
	int nMaxChance = 0;								// �`�����X�͈͂̍ő�l
	for (auto itr = m_decoType.begin(); itr != m_decoType.end(); itr++)
	{
		nMaxChance = nMinChance + (*itr)->chance;
		if (nMinChance <= nRandValue && nRandValue < nMaxChance)
		{ // �͈͓��̂Ƃ�
			return *itr;
		}
		nMinChance += (*itr)->chance;
	}

	return nullptr;
}

//=============================================================
// [DecorationManager] �ʒu����`�����N���擾����
//=============================================================
void DecorationManager::GetChunk(int* x, int* y, const D3DXVECTOR3& pos)
{
	// �ʒu����`�����N�����߂�
	int sx, sy;
	sx = static_cast<int>((pos.x + Terrain::TERRAIN_DISTANCE_HALF) / (float)CHUNK_DIVISION);
	sy = static_cast<int>((pos.z + Terrain::TERRAIN_DISTANCE_HALF) / (float)CHUNK_DIVISION);
	if (sx >= MAX_CHUNK) sx = MAX_CHUNK - 1;
	if (sy >= MAX_CHUNK) sy = MAX_CHUNK - 1;
	if (sx < 0) sx = 0;
	if (sy < 0) sy = 0;

	// �Ԃ�
	*x = sx;
	*y = sy;
}

//=============================================================
// [DecorationManager] �`�����N�����[�h����
//=============================================================
void DecorationManager::LoadChunk(const int& x, const int& y)
{
	if (0 <= x && x < MAX_CHUNK &&
		0 <= y && y < MAX_CHUNK)
	{
		for (auto itr = m_decoData[x][y].begin(); itr != m_decoData[x][y].end(); itr++)
		{
			ActiveData(*itr);
		}
	}
}

//=============================================================
// [DecorationManager] �`�����N���A�����[�h����
//=============================================================
void DecorationManager::UnloadChunk(const int& x, const int& y)
{
	if (0 <= x && x < MAX_CHUNK &&
		0 <= y && y < MAX_CHUNK)
	{
		for (auto itr = m_decoData[x][y].begin(); itr != m_decoData[x][y].end(); itr++)
		{
			PassiveData(*itr);
		}
	}
}

//=============================================================
// [DecorationManager] �f�[�^��L��������
//=============================================================
void DecorationManager::ActiveData(DecorationData* decoData)
{
	DecorationObject* targetDecoObj = nullptr;
	for (auto itr = m_decoObjects.begin(); itr != m_decoObjects.end(); itr++)
	{
		// ���łɓǂݍ��܂�Ă���Ƃ�
		if ((*itr)->decoDeta == decoData)
		{
			return;
		}

		// ��̃I�u�W�F�N�g���������Ƃ��i��ވ�v�j
		if (targetDecoObj == nullptr && 
			(*itr)->decoDeta == nullptr && (*itr)->decoType == decoData->type)
		{
			targetDecoObj = *itr;
		}
	}
	
	if (targetDecoObj == nullptr)
	{ // �����ɍ����I�u�W�F�N�g���Ȃ�������
		targetDecoObj = new DecorationObject();
		targetDecoObj->decoDeta = decoData;
		targetDecoObj->decoType = decoData->type;
		targetDecoObj->gameObject = GameObject::LoadPrefab(decoData->type->path, decoData->transform);
		targetDecoObj->destroyCounter = DESTROY_LIMIT;
		m_decoObjects.push_back(targetDecoObj);
	}
	else
	{ // �����ɍ����I�u�W�F�N�g�����������Ƃ�
		targetDecoObj->decoDeta = decoData;
		targetDecoObj->gameObject->transform->SetPos(decoData->transform.GetPos());
		targetDecoObj->gameObject->transform->SetQuaternion(decoData->transform.GetQuaternion());
		targetDecoObj->gameObject->transform->SetScale(decoData->transform.GetScale());
		targetDecoObj->destroyCounter = DESTROY_LIMIT;
	}

	// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���
	targetDecoObj->gameObject->SetActive(true);
}

//=============================================================
// [DecorationManager] �f�[�^�𖳌�������
//=============================================================
void DecorationManager::PassiveData(DecorationData* decoData)
{
	for (auto itr = m_decoObjects.begin(); itr != m_decoObjects.end(); itr++)
	{
		if ((*itr)->decoDeta == decoData)
		{
			(*itr)->decoDeta = nullptr;
			(*itr)->gameObject->SetActive(false);
			(*itr)->destroyCounter = DESTROY_LIMIT;
			return;
		}
	}
}

//=============================================================
// [DecorationManager] �g���Ă��Ȃ��I�u�W�F�N�g�̔j���J�E���^�[�X�V����
//=============================================================
void DecorationManager::UpdateDestroyObjects()
{
	for (auto itr = m_decoObjects.begin(); itr != m_decoObjects.end(); itr++)
	{
		DecorationObject* decoObj = *itr;
		if (decoObj->decoDeta == nullptr)
		{
			decoObj->destroyCounter--;

			// �j���^�C�~���O�������Ƃ�
			if (decoObj->destroyCounter <= 0)
			{ 
				decoObj->gameObject->Destroy();
				delete decoObj;
				itr = m_decoObjects.erase(itr);
				if (itr == m_decoObjects.end()) return;
			}
		}
	}
}