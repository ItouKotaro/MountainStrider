//=============================================================
//
// �^�C�g�� [title.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "title.h"

#include "component/2d/text.h"
#include "component/2d/polygon.h"
#include "renderer.h"

//=============================================================
// [CTitleScene] ������
//=============================================================
void CTitleScene::Init()
{
	// �w�i
	GameObject* pBG = new GameObject;
	pBG->AddComponent<CPolygon>();
	pBG->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);

	// �e�L�X�g
	GameObject* pText = new GameObject;
	pText->AddComponent<CText>();
	pText->GetComponent<CText>()->SetText("<color=0,0,0><size=200>�Q�[����");
	pText->GetComponent<CText>()->SetAlign(CText::ALIGN::CENTER);
	pText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 120.0f);

	// �I�����𐶐�����
	for (int i = 0; i < SELECT::MAX; i++)
	{
		// ���ڂ̕\�������i�[����
		std::string sShowName;
		switch (i)
		{
		case SELECT::START:
			sShowName = "�J�n";
			break;
		case SELECT::EXIT:
			sShowName = "�I��";
			break;
		}

		// �I�u�W�F�N�g�𐶐�����
		m_pSelectObj[i] = new GameObject;
		m_pSelectObj[i]->AddComponent<CText>();
		m_pSelectObj[i]->GetComponent<CText>()->SetText("<color=0,0,0>" + sShowName);
		m_pSelectObj[i]->GetComponent<CText>()->SetAlign(CText::ALIGN::CENTER);
		m_pSelectObj[i]->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 600.0f + i * 200.0f);
	}
}

//=============================================================
// [CTitleScene] �I��
//=============================================================
void CTitleScene::Uninit()
{
}

//=============================================================
// [CTitleScene] �X�V
//=============================================================
void CTitleScene::Update()
{
	// �I���̍X�V
	UpdateSelect();

	// �I�������Ƃ��̏���
	if (INPUT_INSTANCE->onTrigger("space") ||
		INPUT_INSTANCE->onTrigger("enter"))
	{
		switch (m_select)
		{
		case CTitleScene::START:
			CSceneManager::GetInstance()->SetScene("game");
			break;
		case CTitleScene::EXIT:
			DestroyWindow(CRenderer::GetInstance()->GetHWND());
			break;
		}
	}
}

//=============================================================
// [CTitleScene] �`��
//=============================================================
void CTitleScene::Draw()
{
}


//=============================================================
// [CTitleScene] �I���̍X�V
//=============================================================
void CTitleScene::UpdateSelect()
{
	// ��I��
	if (INPUT_INSTANCE->onTrigger("w") ||
		INPUT_INSTANCE->onTrigger("up"))
	{
		if (m_select > 0)
		{
			m_select = static_cast<SELECT>(m_select - 1);
		}
	}

	// ���I��
	if (INPUT_INSTANCE->onTrigger("s") ||
		INPUT_INSTANCE->onTrigger("down"))
	{
		if (m_select < SELECT::MAX - 1)
		{
			m_select = static_cast<SELECT>(m_select + 1);
		}
	}

	// UI�̍X�V
	for (int i = 0; i < SELECT::MAX; i++)
	{
		m_pSelectObj[i]->GetComponent<CText>()->SetAlpha(i == m_select ? 1.0f : 0.2f);
	}
}
