//=============================================================
//
// �Q�[���V�[�� [game.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "internal/physics.h"
#include "scripts/terrain.h"
#include "scripts/environmental.h"
#include "scripts/decoration.h"
#include "scripts/result/mt_result.h"
#include "scripts/pause.h"
#include "scripts/event/event_manager.h"
#include "scripts/lake.h"
#include "scripts/play_guide.h"
#include "scripts/mode/mode_manager.h"

// �Q�[���V�[��
class CGameScene : public CScene
{
public:
	CGameScene(){}
	void Init() override;				// ������
	void Uninit() override;			// �I��
	void Update() override;		// �X�V
	void Draw() override;			// �`��
	void LastUninit() override;	// �ŏI�I��

	static void ResetGame();		// �Q�[���̃��Z�b�g
	void HideUI();						// UI���\���ɂ���
	void CalcResultData();		// ���U���g�f�[�^���v�Z����
	ResultBase::ResultData GetResultData() { return m_resultData; }	// ���U���g�f�[�^���擾����

	Terrain* GetTerrain() { return m_terrain; }
	LakeManager* GetLake() { return m_lake; }
	GameObject* GetBike() { return m_bike; }
	EnvironmentalEffect* GetEnvironmental() { return m_environmental; }
	DecorationManager* GetDecoration() { return m_decoration; }
	Pause* GetPause() { return m_pause; }

	static void AddActionPoint(const int& point) { m_actionPoint += point; }		// �A�N�V�����|�C���g�̉��Z
	static int GetActionPoint() { return m_actionPoint; }										// �A�N�V�����|�C���g�̎擾

	// ���s�f�[�^
	struct TravellingData
	{
		D3DXVECTOR3 pos;
		D3DXQUATERNION rot;
	};
	std::vector<TravellingData> GetTravellingData() { return m_travellingDatas; }

	// �֗��֐�
	void GenerateTerrain(const int& size = Terrain::TERRAIN_DEFAULT_SIZE, const float& scale = Terrain::TERRAIN_DEFAULT_SCALE, const std::string& terrain_path = "");

private:
	std::vector<std::string> GetTerrainFiles();			// �n�`�t�@�C���ꗗ���擾����
	void SpawnBike();													// �o�C�N�̐���

	int m_travellingCount;												// �˔j��
	std::vector<TravellingData> m_travellingDatas;	// ���s�f�[�^

	Terrain* m_terrain;													// �n�`
	EnvironmentalEffect* m_environmental;				// ��
	DecorationManager* m_decoration;						// ����
	EventManager* m_events;										// �C�x���g
	LakeManager* m_lake;											// ��
	//PlayGuideManager* m_playGuide;							// �v���C�K�C�h

	GameObject* m_voidField;										// �ޗ�
	GameObject* m_camera;										// �J����
	GameObject* m_bike;												// �o�C�N
	GameObject* m_statusUI;										// �X�e�[�^�XUI
	GameObject* m_speedmeterUI;								// �X�s�[�h���[�^�[UI
	GameObject* m_itemSlot;										// �A�C�e���X���b�g

	ResultBase* m_oldResult;										// �ߋ��̃��U���g
	ResultBase::ResultData m_resultData;					// ���U���g�f�[�^

	// ���U���g�f�[�^�p
	DWORD m_startTime;												// �J�n����
	int m_highSpeed;													// �ō����x
	static int m_actionPoint;											// �X�R�A

	// �|�[�Y
	Pause* m_pause;
};

#endif // !_GAME_H_
