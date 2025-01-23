//=============================================================
//
// �c�[ [wreckage.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "wreckage.h"

// �ÓI�����o�ϐ��̏�����
const float CWreckage::FADE_VALUE = 0.03f;
const int CWreckage::START_TIME = 500;

//=============================================================
// [CWreckage] ������
//=============================================================
void CWreckage::Init()
{
	// ���b�V�����X�g���擾����
	std::vector<GameObject*> pMeshList = gameObject->GetChildren(true);
	for (unsigned int i = 0; i < pMeshList.size(); i++)
	{
		if (pMeshList[i]->GetComponent<CMesh>() != nullptr)
		{
			m_pMeshs.push_back(pMeshList[i]->GetComponent<CMesh>());
		}
	}

	// �t�F�[�h�l
	m_fFade = 1.0f;

	// �J�n���Ԃ̐ݒ�
	m_dwStartTime = timeGetTime() + START_TIME;
}

//=============================================================
// [CWreckage] �X�V
//=============================================================
void CWreckage::Update()
{
	// �J�n����
	if (timeGetTime() < m_dwStartTime)
	{
		return;
	}

	// �t�F�[�h����
	if (m_fFade > 0.0f)
	{
		m_fFade -= FADE_VALUE;
	}
	else
	{ // �I��
		gameObject->Destroy(true);
		return;
	}

	// ���f���̕s�����x�ݒ�
	for (unsigned int i = 0; i < m_pMeshs.size(); i++)
	{
		m_pMeshs[i]->SetAlpha(m_fFade);
	}
}