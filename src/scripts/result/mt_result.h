//=============================================================
//
// ���U���g [mt_result.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _MT_RESULT_H_
#define _MT_RESULT_H_

#include "gameobject.h"
#include "scripts/shop/shop.h"

class CGameScene;

// ���U���g�̊��
class ResultBase
{
public:
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	// ���ʊi�[
	struct ResultData
	{
		int time;			// �N���A����
		int highSpeed;	// �ō����x
		int action;			// �A�N�V����
	};
	static void AddResult(ResultData data);
	static void Reset();
protected:
	CGameScene* m_gameScene;
	static float m_beforeFuel;							// �O��̔R��
	static float m_beforeEndurance;					// �O��̑ϋv�l
	static UINT m_goalCount;							// ���j��
	static std::vector<ResultData> m_results;	// ����
};

// �N���A���̃��U���g
class ClearResult : public ResultBase
{
public:
	ClearResult(CGameScene* scene) { m_gameScene = scene; }
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	void UpdateResultAnim();
	enum PROG_STATE
	{
		P_MTTEXT,					// ���j���e�L�X�g
		P_FUEL,							// �o�C�N�̔R��
		P_ENDURANCE,				// �o�C�N�̑ϋv�l
		P_END
	};
	PROG_STATE m_progState;	// ���ʕ\���i�s�x
	int m_progCounter;				// �J�E���^�[

	GameObject* m_mtText;		// �}�E���e���e�L�X�g�i�R�̓��j���j

	GameObject* m_bg;						// �w�i
	GameObject* m_seedText;				// �V�[�h�e�L�X�g
	GameObject* m_dataView;				// �f�[�^�\��
	GameObject* m_fuelView;				// �R���\��
	GameObject* m_enduranceView;	// �ϋv�l�\��
	GameObject* m_terrainImg;			// �n�`�摜

	ShopManager* m_shopManager;		// �V���b�v�Ǘ�
};


// �Q�[���I�[�o�[���̃��U���g
class GameOverResult : public ResultBase
{
public:
	GameOverResult(CGameScene* scene) { m_gameScene = scene; }
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	void UpdateResultAnim();

	GameObject* m_mtText;		// �}�E���e���e�L�X�g�i�R�̓��j���j

	GameObject* m_bg;						// �w�i
	GameObject* m_seedText;				// �V�[�h�e�L�X�g
	GameObject* m_dataView;				// �f�[�^�\��
	GameObject* m_fuelView;				// �R���\��
	GameObject* m_enduranceView;	// �ϋv�l�\��
	GameObject* m_terrainImg;			// �n�`�摜
};


#endif // !_RESULT_H_
