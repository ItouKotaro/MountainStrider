//=============================================================
//
// ���� [decoration.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _DECORATION_H_
#define _DECORATION_H_

#include "scripts/terrain.h"
#include "component.h"
#include "component/other/audio.h"

// �����Ǘ�
class DecorationManager
{
public:
	struct Range
	{
		float min;
		float max;
	};

	// �f�R���[�V�����̎��
	struct DecorationType
	{
		std::string path;					// �v���n�u�̃p�X
		int chance;							// �m��
		float offsetY;						// Y�̃I�t�Z�b�g
		float randomAngle;				// �����_���Ȋp�x
		bool isMatchInclination;		// �X�Ίp�ɍ��킹�邩
		bool isDestructible;				// �j��\
		float damage;					// �_���[�W

		AudioClip hitSound;			// �Փˉ�
		float volume;						// ����

		// ����
		Range slantLimit;				// �X�Ίp����
		Range heightLimit;				// ���x����
		float radiusSize;					// ���a�T�C�Y
	};

	// �f�R���[�V�����̐ݒu�f�[�^
	struct DecorationData
	{
		DecorationType* type;
		Transform transform;
	};

public:
	void Init(Terrain* terrain);
	void Uninit();
	void Update(const D3DXVECTOR3& pos);

	//@brief �f�R���[�V�����̎�ނ�ǉ�
	DecorationType* AddDecorationType(const std::string& path, const int& chance = 1, const float& offsetY = 0.0f);

	//@brief �f�R���[�V�����𐶐�����i�P�́j
	void GenerateDecoration();

	//@brief �f�[�^��j������
	void RemoveData(DecorationData* data);

	//@brief �n�`�t�@�C����ǂݍ���
	void LoadTerrainFile(const std::string path);

	//@brief ��������
	void Generate();

	static const int MAX_CHUNK = 25;
	static const float CHUNK_DIVISION;
	static const int DESTROY_LIMIT;
private:

	// �f�R���[�V�����̃Q�[���I�u�W�F�N�g
	struct DecorationObject
	{
		DecorationData* decoDeta;
		DecorationType* decoType;
		GameObject* gameObject;
		int destroyCounter;
	};

	//@brief �f�R���[�V������ݒu����
	void AddDecorationData(DecorationType* decoType, Transform transform);

	//@brief �f�R���[�V�����̎�ނ����߂�
	DecorationType* RandomDecoType();

	//@brief ���݈ʒu����`�����N���擾����
	void GetChunk(int* x, int* y, const D3DXVECTOR3& pos);

	//@brief �`�����N�����[�h����
	void LoadChunk(const int& x, const int& y);

	//@brief �`�����N���A�����[�h����
	void UnloadChunk(const int& x, const int& y);

	//@brief �ݒu�f�[�^�̃I�u�W�F�N�g��L��������
	void ActiveData(DecorationData* decoData);

	//@brief �ݒu�f�[�^�̃I�u�W�F�N�g�𖳌�������
	void PassiveData(DecorationData* decoData);

	//@brief �g���Ă��Ȃ��I�u�W�F�N�g�̔j���J�E���^�[�X�V
	void UpdateDestroyObjects();

	std::vector<DecorationType*> m_decoType;											// ���
	std::vector<DecorationData*> m_decoData[MAX_CHUNK][MAX_CHUNK];	// �ݒu�f�[�^�i�`�����N�Ǘ��j
	std::vector<DecorationObject*> m_decoObjects;										// �I�u�W�F�N�g
	Terrain* m_terrain;
	int m_generateNum;

	// �O��̃`�����N
	int m_oldChunkX, m_oldChunkY;
};

#endif // !_DECORATION_H_
