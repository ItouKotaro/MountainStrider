//=============================================================
//
// ���[�h�Ǘ� [mode_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MODE_MANAGER_H_
#define _MODE_MANAGER_H_

// ���[�h��`
class ModeTemplate
{
public:
	// ���
	enum STATE
	{
		STATE_GAME,	// �Q�[�����i�ʏ펞�j
		STATE_GOAL,		// �S�[����
		STATE_FAIL,		// ���s��
	};

	ModeTemplate() : m_state(STATE_GAME) {}
	virtual ~ModeTemplate() {}
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}

	//@brief ��Ԃ��擾����
	STATE GetState() { return m_state; }

	//@brief ��Ԃ����Z�b�g����
	void ResetState() { m_state = STATE::STATE_GAME; }
protected:
	//@brief �S�[���C�x���g���N����
	void onGoal() { m_state = STATE::STATE_GOAL; }

	//@brief ���s�C�x���g���N����
	void onFail() { m_state = STATE::STATE_FAIL; }

private:
	STATE m_state;		// ���
};

// ���[�h�Ǘ�
class ModeManager final
{
public:
	void Init();
	void Uninit();
	void Update();
	void LastUninit();

	//@brief ��Ԃ��擾����
	ModeTemplate::STATE GetState() { return m_mode->GetState(); }

	//@brief ���[�h��ݒ肷��
	void SetMode(ModeTemplate* mode);

	//@brief �C���X�^���X���擾����
	static ModeManager* GetInstance()
	{
		static ModeManager instance;
		return &instance;
	}
private:
	ModeTemplate* m_mode;
};


#endif // !_MODE_MANAGER_H_
