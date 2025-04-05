//=============================================================
//
// �I�[���[�h [terminal.h]
// Author: Ito Kotaro
// 
// ���[���h�̒[�܂œ��B����ƃS�[������
//
//=============================================================
#ifndef _TERMINAL_MODE_H_
#define _TERMINAL_MODE_H_

#include "mode_manager.h"

// �I�[���[�h
class TerminalMode : public ModeTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void OnResultEvent() override;

	// ���U���g�f�[�^���擾����
	std::vector<ResultBase::ResultData>& GetResultData() { return m_resultDatas; }
private:
	D3DXVECTOR3 CalcNearGoal();
	GameObject* m_directionObj;
	std::vector<ResultBase::ResultData> m_resultDatas;
	const float EXTENSION_DISTANCE = 150.0f;
};


// �^�[�~�i�����[�h�̃��U���g
class TerminalResult : public ResultBase
{
public:
	int GetAverageTime();
	int GetAverageAction();
	int GetNumOfStep();
	float GetBeforeFuel() { return m_beforeFuel; }
	float GetBeforeEndurance() { return m_beforeEndurance; }
protected:
	void FinalResult(bool isSuccess);

	static float m_beforeFuel;							// �O��̔R��
	static float m_beforeEndurance;					// �O��̑ϋv�l
	GameObject* m_page;			// �y�[�W�Ǘ�

	// �ŏI���ʗp
	GameObject* m_scoreText;			// �X�R�A
	GameObject* m_timeRate;				// ���Ԃ̕]��
	GameObject* m_actionRate;			// �A�N�V�����̕]��
	GameObject* m_fuelRate;				// �R��̕]��
	GameObject* m_timeValue;			// ���ԕ\��
	GameObject* m_actionValue;			// �A�N�V�����\��
	GameObject* m_fuelValue;				// �R��\��

	const int CLEAR_POINT = 2500;
	const int END_POINT = 5000;
};

// �N���A���̃��U���g
class ClearTerminalResult : public TerminalResult
{
public:
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

	GameObject* m_bg;						// �w�i
	GameObject* m_mtText;				// �}�E���e���e�L�X�g�i�R�̓��j���j
	GameObject* m_seedText;				// �V�[�h�e�L�X�g
	GameObject* m_dataView;				// �f�[�^�\��
	GameObject* m_fuelView;				// �R���\��
	GameObject* m_enduranceView;	// �ϋv�l�\��
	GameObject* m_terrainImg;			// �n�`�摜
	GameObject* m_descriptionUI;		// ����UI

	// ��
	AudioClip m_bgm;
	AudioClip m_clickSE;
	GameObject* m_bgmObj;
	float m_volumeFade;
	const float BGM_VOLUME = 0.2f;
	const float BGM_FADE = 0.001f;
	const char* DESC_END_TEXTURE = "data\\TEXTURE\\RESULT\\desc_end.png";
	const char* DESC_NEXT_TEXTURE = "data\\TEXTURE\\RESULT\\desc_next.png";
	const D3DXVECTOR2 DESC_SIZE = { 620.0f, 297.0f };

	// �{�^��
	GameObject* m_endButton;
	GameObject* m_nextButton;

	ShopManager* m_shopManager;		// �V���b�v�Ǘ�
};


// �Q�[���I�[�o�[���̃��U���g
class GameOverTerminalResult : public TerminalResult
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	// ���U���g�A�j���[�V�����̍X�V
	void UpdateResultAnim();

	enum PROG_STATE
	{
		P_FUEL,							// �o�C�N�̔R��
		P_ENDURANCE,				// �o�C�N�̑ϋv�l
		P_END
	};
	PROG_STATE m_progState;	// ���ʕ\���i�s�x
	int m_progCounter;				// �J�E���^�[

	GameObject* m_mtText;				// �}�E���e���e�L�X�g�i�Q�[���I�[�o�[�\���j

	GameObject* m_bg;						// �w�i
	GameObject* m_seedText;				// �V�[�h�e�L�X�g
	GameObject* m_dataView;				// �f�[�^�\��
	GameObject* m_fuelView;				// �R���\��
	GameObject* m_enduranceView;	// �ϋv�l�\��
	GameObject* m_terrainImg;			// �n�`�摜

	// ��
	AudioClip m_bgm;
	GameObject* m_bgmObj;
	float m_volumeFade;
	const float BGM_VOLUME = 0.6f;
	const float BGM_FADE = 0.004f;
};

#endif // !_TERMINAL_MODE_H_
