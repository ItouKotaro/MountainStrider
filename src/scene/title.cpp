//=============================================================
//
// �^�C�g�� [title.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "title.h"

#include "component/2d/text.h"
#include "component/2d/polygon.h"
#include "component/other/button.h"
#include "component/3d/motion.h"
#include "component/3d/field.h"
#include "scene/game.h"
#include "renderer.h"
#include "render/shadow_rb.h"
#include "manager.h"
#include "component/other/page.h"

//=============================================================
// [CTitleScene] ������
//=============================================================
void CTitleScene::Init()
{
	// �w�i
	GameObject* pBack = new GameObject();
	pBack->AddComponent<CField>()->Set(580.0f, 420.0f);
	pBack->GetComponent<CField>()->SetTexture("data\\TEXTURE\\TITLE\\back.png");
	pBack->transform->SetPos(0.0f, 0.0f, 300.0f);
	pBack->transform->SetRot(-D3DX_PI * 0.5f, 0.0f, 0.0f);

	// �n��
	GameObject* pShadowField = new GameObject;
	pShadowField->AddComponent<CField>()->Set(500.0f, 500.0f);
	pShadowField->transform->SetPos(0.0f, -70.0f, 0.0f);

	GameObject* pField = new GameObject;
	pField->AddComponent<CField>()->Set(500.0f, 500.0f);
	pField->GetComponent<CField>()->SetTexture("data\\TEXTURE\\TITLE\\grass.png");
	pField->GetComponent<CField>()->SetLoopTexture(10);
	pField->transform->SetPos(0.0f, -69.0f, 0.0f);

	// �V�[��
	m_titleScene = GameObject::LoadPrefab("data\\PREFAB\\title_scene.pref");
	m_titleScene->transform->SetPos(0.0f, -20.0f, 10.0f);
	m_titleScene->GetComponent<CMotionManager>()->Play("0");

	// �e�L�X�g
	GameObject* pLogo = new GameObject;
	pLogo->AddComponent<CField>()->Set(200.0f, 200.0f);
	pLogo->GetComponent<CField>()->SetTexture("data\\TEXTURE\\logo.png");
	pLogo->transform->SetPos(0.0f, 30.0f, 290.0f);
	pLogo->transform->SetRot(-D3DX_PI * 0.5f, 0.0f, 0.0f);

	// �Q�[���J�n
	{
		GameObject* pStartButton = new GameObject("StartMountain");
		pStartButton->transform->SetSize(500.0f, 140.0f);
		pStartButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) - 400.0f, 800.0f);
		pStartButton->AddComponent<ButtonUI>();
		pStartButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		pStartButton->GetComponent<ButtonUI>()->setClickEvent([]() { 
			// �ǂݍ��ݒ��̉摜��\��
			GameObject* loadingPoly = new GameObject();
			loadingPoly->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\TITLE\\loading.png");
			loadingPoly->transform->SetSize(1220.0f, 304.0f);
			loadingPoly->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 610.0f, CRenderer::SCREEN_HEIGHT / 2 - 152.0f);

			// �Q�[������
			static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->ResetGame();
			CSceneManager::GetInstance()->SetScene("game");
			});

		GameObject* pStartButtonText = new GameObject();
		pStartButtonText->SetParent(pStartButton);
		pStartButtonText->transform->SetPos(250.0f, 30.0f);
		pStartButtonText->AddComponent<CText>();
		pStartButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pStartButtonText->GetComponent<CText>()->SetText("<color=0,0,0>�J�n");
		pStartButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	}

	// ���X�g�ǉ�
	{
		GameObject* pEndButton = new GameObject("EndMountain");
		pEndButton->transform->SetSize(500.0f, 140.0f);
		pEndButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) + 400.0f, 800.0f);
		pEndButton->AddComponent<ButtonUI>();
		pEndButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		pEndButton->GetComponent<ButtonUI>()->setClickEvent([]() { DestroyWindow(CRenderer::GetInstance()->GetHWND()); });

		GameObject* pEndButtonText = new GameObject();
		pEndButtonText->SetParent(pEndButton);
		pEndButtonText->transform->SetPos(250.0f, 30.0f);
		pEndButtonText->AddComponent<CText>();
		pEndButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pEndButtonText->GetComponent<CText>()->SetText("<color=0,0,0>�I��");
		pEndButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	}

	// �J����
	GameObject* pCamera = new GameObject();
	pCamera->AddComponent<CCamera>();
	pCamera->transform->SetPos(0.0f, 0.0f, -50.0f);

	// ���C�g
	GameObject* pLight = new GameObject();
	pLight->AddComponent<CCamera>();
	pLight->transform->SetPos(-30.0f, 100.0f, -20.0f);
	pLight->transform->LookAt({ 0.0f, 0.0f, 5.0f });

	// �`���[�g���A��
	GameObject* tutorialImg = new GameObject();
	tutorialImg->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\TITLE\\tutorial.png");
	tutorialImg->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);
	tutorialImg->SetVisible(false);

	GameObject* tutorialButton = new GameObject();
	tutorialButton->transform->SetSize(100.0f, 100.0f);
	tutorialButton->transform->SetPos(CRenderer::SCREEN_WIDTH - 100.0f, 0.0f);
	tutorialButton->AddComponent<ButtonUI>();
	tutorialButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\TITLE\\tutorial_book.png");
	tutorialButton->GetComponent<ButtonUI>()->setClickEvent([this, tutorialImg]() {tutorialImg->SetVisible(!tutorialImg->GetVisible()); });
	tutorialButton->SetPriority(9);

	// �e�p�����_�[�o�b�t�@�̐ݒ�
	CRenderer::GetInstance()->RegisterRenderBuffer<ShadowRenderBuffer>("main");
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(pCamera->GetComponent<CCamera>());
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetLightCamera(pLight->GetComponent<CCamera>());
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetAmbient({ 0.7f, 0.7f, 0.7f, 1.0f });
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetShadowPoint({ 0.0f, 0.0f, 0.0f });
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetShadowRange(600.0f);

	CManager::GetInstance()->SetShowCursor(true);
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
}

//=============================================================
// [CTitleScene] �`��
//=============================================================
void CTitleScene::Draw()
{
}