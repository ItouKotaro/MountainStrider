//=============================================================
//
// �N���N���W�b�g [launch_credit.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _LAUNCH_CREDIT_H_
#define  _LAUNCH_CREDIT_H_

#include "component/2d/polygon.h"

// �N���N���W�b�g
class LaunchCredit : public Component
{
public:
	LaunchCredit() : m_currentCreditID(-1), m_time(0.0f), m_showTime(2.0f), m_fadeTime(0.5f), m_alpha(1.0f), m_bgPoly(nullptr), m_state(STATE::NOT_STARTED){}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	//@brief �J�n����
	void Start();

	//@brief �N���W�b�g��ǉ�����
	//@return �N���W�b�gID
	int AddCredit(const std::string& path, const D3DXVECTOR2& size, const float& scale = 1.0f);

	//@brief �\�����Ԃ̐ݒ�
	void SetShowTime(const float& time) { m_showTime = time; }

	//@brief �t�F�[�h���Ԃ̐ݒ�
	void SetFadeTime(const float& time) { m_fadeTime = time; }

	//@brief �I��������
	bool IsEnded() { return m_state == STATE::ENDED; }

	//@brief �\�����̃N���W�b�gID���擾����
	//@return �N���W�b�gID
	int GetCurrentShow() { return m_currentCreditID; }

	//@brief �N���W�b�g�̕s�����x���擾����
	//@return �s�����x (0.0~1.0)
	float GetCurrentAlpha() { return m_alpha; }

	//@brief �w�i���w�肵�Ĕw�i�̕s�����x�𒲐����܂�
	void SetBG(CPolygon* polygon) { m_bgPoly = polygon; }
private:
	// �N���W�b�g���S
	struct CreditLogo
	{
		std::string path;			// �p�X
		D3DXVECTOR2 size;	// �T�C�Y
		float scale;					// �X�P�[��
	};

	// ���
	enum STATE
	{
		NOT_STARTED,	// �n�܂��Ă��Ȃ�
		MIDST,				// �i�s��
		ENDED,				// �I��
	};

	SingleComponent<CPolygon>* m_creditLogo;		// �N���W�b�g���S
	std::vector<CreditLogo> m_credits;					// �N���W�b�g���X�g

	// ��������
	float m_time;														// ����
	int m_currentCreditID;										// ���݂̃N���W�b�gID
	STATE m_state;													// ���
	float m_alpha;													// �s�����x
	CPolygon* m_bgPoly;											// �w�i�|���S��

	// �ݒ荀��
	float m_showTime;												// �\������
	float m_fadeTime;												// �t�F�[�h����
};

#endif // !_LAUNCH_CREDIT_H_
