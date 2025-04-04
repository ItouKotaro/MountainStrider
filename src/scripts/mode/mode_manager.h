//=============================================================
//
// ���[�h�Ǘ� [mode_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MODE_MANAGER_H_
#define _MODE_MANAGER_H_

#include "scripts/result/mt_result.h"

// ���[�h��`
class ModeTemplate
{
public:
	ModeTemplate() {}
	virtual ~ModeTemplate() {}
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void OnResultEvent() {}
};

// ���[�h�Ǘ�
class ModeManager final
{
public:
	void Init();
	void Uninit();
	void Update();
	void LastUninit();

	//@brief ���[�h��ݒ肷��
	void SetMode(ModeTemplate* mode);

	//@brief ���U���g��ݒ肷��
	template<class T> void SetResult()
	{
		// ���U���g���ݒ肳��Ă��Ȃ��ꍇ�̂�
		if (m_result == nullptr)
		{
			// �V�������[�h��ݒ肷��
			m_result = new T();
			m_result->Init();
		}
	}

	//@brief ���[�h���擾����
	ModeTemplate* GetMode() { return m_mode; }

	//@brief ���U���g���擾����
	ResultBase* GetResult() { return m_result; }

	//@brief �C���X�^���X���擾����
	static ModeManager* GetInstance()
	{
		static ModeManager instance;
		return &instance;
	}
private:
	ModeTemplate* m_mode;
	ResultBase* m_result;
};


#endif // !_MODE_MANAGER_H_
