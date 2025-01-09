//=============================================================
//
// �|�[�Y [pause.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "pause.h"
#include "renderer.h"
#include "component/other/page.h"
#include "component/other/button.h"
#include "component/2d/text.h"
#include "scripts/camera_move.h"
#include "internal/physics.h"

//=============================================================
// [Pause] ������
//=============================================================
void Pause::Init()
{
	m_pausePage = new GameObject();
	auto page = m_pausePage->AddComponent<Pages>();
	page->SetNumPage(2);

	// �w�i
	GameObject* pauseBG = new GameObject();
	pauseBG->AddComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	pauseBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);
	pauseBG->SetPriority(8);
	page->AddObject(1, pauseBG);

	// �|�[�Y�e�L�X�g
	m_pauseText = new GameObject();
	m_pauseText->AddComponent<CText>()->SetText("<size=150>PAUSE");
	m_pauseText->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_pauseText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_pauseText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 150.0f);
	m_pauseText->SetPriority(9);
	page->AddObject(1, m_pauseText);

	// �߂�{�^��
	m_backButton = new GameObject();
	m_backButton->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 300.0f, 400.0f);
	m_backButton->transform->SetSize(600.0f, 160.0f);
	m_backButton->AddComponent<ButtonUI>();
	m_backButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\PAUSE\\back.png");
	m_backButton->GetComponent<ButtonUI>()->setClickEvent([this]() {SetPause(false); });
	m_backButton->SetPriority(9);
	page->AddObject(1, m_backButton);

	// �I���{�^��
	m_endButton = new GameObject();
	m_endButton->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 300.0f, 650.0f);
	m_endButton->transform->SetSize(600.0f, 160.0f);
	m_endButton->AddComponent<ButtonUI>();
	m_endButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\PAUSE\\end.png");
	m_endButton->GetComponent<ButtonUI>()->setClickEvent([this]() {
		SetPause(false);
		CSceneManager::GetInstance()->SetScene("title"); 
		});
	m_endButton->SetPriority(9);
	page->AddObject(1, m_endButton);

	// �y�[�W��ύX����
	page->AllHideObjects();
}

//=============================================================
// [Pause] �|�[�Y�ɂ��邩
//=============================================================
void Pause::SetPause(const bool& enabled)
{
	m_pausePage->GetComponent<Pages>()->SetPage(enabled ? 1 : 0);
	m_isPause = enabled;

	Main::SetShowCursor(enabled);

	// �����ݒ�
	CPhysics::GetInstance()->SetUpdatePhysics(!m_isPause);
	GameObject::Find("Camera")->GetComponent<CCameraMove>()->enabled = !enabled;
}
