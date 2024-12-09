//=============================================================
//
// ���U���g [mt_result.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "mt_result.h"
#include "scene/game.h"

#include "renderer.h"
#include "component/2d/text.h"
#include "component/2d/polygon.h"
#include "component/2d/bar.h"
#include "component/other/page.h"

#include "scripts/vehicle.h"
#include "scripts/result/result_data.h"
#include "scripts/result/result_view.h"
#include "scripts/result/result_terrain.h"
#include "component/other/button.h"

float ResultBase::m_beforeFuel = CVehicle::MAX_FUEL;
float ResultBase::m_beforeEndurance = CVehicle::MAX_ENDURANCE;
UINT ResultBase::m_goalCount = 0;
std::vector<ResultBase::ResultData> ResultBase::m_results = {};

//=============================================================
// [ResultBase] ���ʂ�ۑ�����
//=============================================================
void ResultBase::AddResult(ResultData data)
{
	m_results.push_back(data);
}

//=============================================================
// [ResultBase] ���Z�b�g
//=============================================================
void ResultBase::Reset()
{
	m_beforeFuel = CVehicle::MAX_FUEL;
	m_beforeEndurance = CVehicle::MAX_ENDURANCE;
	m_goalCount = 0;
	m_results.clear();
}


//=============================================================
// [ClearResult] ������
//=============================================================
void ClearResult::Init()
{
	// ���j�����C���N�������g
	m_goalCount++;

	// �i��
	m_progState = P_MTTEXT;
	m_progCounter = 120;

	// �y�[�W�̏�����
	m_page = new GameObject("PageManager");
	m_page->AddComponent<Pages>();
	m_page->GetComponent<Pages>()->SetNumPage(2);
	auto page = m_page->GetComponent<Pages>();
		
	// �V���b�v�𐶐�����
	m_shopManager = new ShopManager();
	m_shopManager->Init(page);

	// �N���A�e�L�X�g
	{
		m_mtText = new GameObject("MtClearText", "UI");
		m_mtText->AddComponent<CText>();
		m_mtText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		m_mtText->GetComponent<CText>()->SetFontSize(130);
		m_mtText->GetComponent<CText>()->SetOutlineColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		m_mtText->GetComponent<CText>()->SetOutlineSize(2);
		m_mtText->GetComponent<CText>()->SetText(std::to_string(m_goalCount) + "�ڂ̎R�𓥔j���܂���");
		m_mtText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_mtText->transform->SetPos(-2000.0f, 100.0f, 0.0f);
		page->AddObject(0, m_mtText);
	}

	// �f�[�^�\��
	{
		m_dataView = new GameObject("DataView", "UI");
		m_dataView->AddComponent<ResultDataView>();
		m_dataView->transform->Translate(60.0f, 350.0f, 0.0f);
		page->AddObject(0, m_dataView);
		
		// �ŐV�f�[�^���擾
		ResultData data = m_results[m_results.size()-1];
		m_dataView->GetComponent<ResultDataView>()->SetTime(data.time);
		m_dataView->GetComponent<ResultDataView>()->SetHighSpeed(data.highSpeed);
		m_dataView->GetComponent<ResultDataView>()->SetAction(data.action);
	}

	// �o�C�N���\��
	{
		// �R��
		m_fuelView = new GameObject("FuelView", "UI");
		m_fuelView->transform->SetPos(1310.0f, 360.0f);
		m_fuelView->AddComponent<ResultViewBar>(
			"�R��",
			D3DCOLOR_RGBA(232, 44, 44, 255),
			D3DCOLOR_RGBA(255, 0, 0, 255),
			D3DCOLOR_RGBA(255, 0, 0, 255)
			);
		m_fuelView->GetComponent<ResultViewBar>()->SetValue(
			static_cast<float>(m_beforeFuel / CVehicle::MAX_FUEL),
			static_cast<float>(m_gameScene->GetBike()->GetComponent<CVehicle>()->GetFuel() / CVehicle::MAX_FUEL));
		page->AddObject(0, m_fuelView);

		// �ϋv�l
		m_enduranceView = new GameObject("EnduranceView", "UI");
		m_enduranceView->transform->SetPos(1310.0f, 600.0f);
		m_enduranceView->AddComponent<ResultViewBar>(
			"�ϋv�l",
			D3DCOLOR_RGBA(78, 69, 255, 255),
			D3DCOLOR_RGBA(75, 67, 224, 255),
			D3DCOLOR_RGBA(61, 100, 255, 255)
			);
		m_enduranceView->GetComponent<ResultViewBar>()->SetValue(
			static_cast<float>(m_beforeEndurance / CVehicle::MAX_ENDURANCE),
			static_cast<float>(m_gameScene->GetBike()->GetComponent<CVehicle>()->GetEndurance() / CVehicle::MAX_ENDURANCE));
		page->AddObject(0, m_enduranceView);
	}

	// �n�`�摜
	{
		m_terrainImg = new GameObject("TerrainImg", "ResultData");
		m_terrainImg->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), static_cast<float>(CRenderer::SCREEN_HEIGHT / 2));
		m_terrainImg->AddComponent<ResultTerrain>();
		page->AddObject(0, m_terrainImg);
	}

	// �V�[�h�e�L�X�g
	{
		m_seedText = new GameObject("SeedText", "UI");
		m_seedText->AddComponent<CText>();
		m_seedText->GetComponent<CText>()->SetFontSize(50);
		m_seedText->GetComponent<CText>()->SetText("<color=150,150,150>Seed: " + std::to_string(m_gameScene->GetTerrain()->GetSeed()));
		m_seedText->transform->SetPos(5.0f, CRenderer::SCREEN_HEIGHT - 50.0f);
		page->AddObject(0, m_seedText);
	}

	// �w�i
	{
		m_bg = new GameObject("BG", "UI");
		m_bg->SetPriority(3);
		m_bg->AddComponent<CPolygon>();
		m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		m_bg->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));
	}

	// ���̎R�� or �I��
	{
		GameObject* pNextButton = new GameObject("NextMountain");
		page->AddObject(0, pNextButton);
		pNextButton->transform->SetSize(500.0f, 140.0f);
		pNextButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) + 400.0f, 850.0f);
		pNextButton->AddComponent<ButtonUI>();
		pNextButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		pNextButton->GetComponent<ButtonUI>()->setClickEvent([this, page]() { page->SetPage(1); });

		GameObject* pNextButtonText = new GameObject();
		pNextButtonText->SetParent(pNextButton);
		page->AddObject(0, pNextButtonText);
		pNextButtonText->transform->SetPos(250.0f, 35.0f);
		pNextButtonText->AddComponent<CText>();
		pNextButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pNextButtonText->GetComponent<CText>()->SetText("<color=0,0,0>Shop ��");
		pNextButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	}

	// ���X�g�ǉ�
	{
		GameObject* pListButton = new GameObject("NextMountain");
		page->AddObject(0, pListButton);
		pListButton->transform->SetSize(500.0f, 140.0f);
		pListButton->transform->SetPos((CRenderer::SCREEN_WIDTH/2 - 250.0f) - 400.0f, 850.0f);
		pListButton->AddComponent<ButtonUI>();
		pListButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");

		GameObject* pListButtonText = new GameObject();
		pListButtonText->SetParent(pListButton);
		page->AddObject(0, pListButtonText);
		pListButtonText->transform->SetPos(250.0f, 35.0f);
		pListButtonText->AddComponent<CText>();
		pListButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pListButtonText->GetComponent<CText>()->SetFontSize(80);
		pListButtonText->GetComponent<CText>()->SetText("<color=0,0,0>���X�g�ɒǉ�");
		pListButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	}

	// �y�[�W�̃��Z�b�g
	page->AllHideObjects();
	page->SetPage(0);

	// �O��̏��Ƃ��ĕۑ�
	m_beforeFuel = m_gameScene->GetBike()->GetComponent<CVehicle>()->GetFuel();
	m_beforeEndurance = m_gameScene->GetBike()->GetComponent<CVehicle>()->GetEndurance();
}

//=============================================================
// [ClearResult] �I��
//=============================================================
void ClearResult::Uninit()
{
	// �V���b�v��j������
	if (m_shopManager != nullptr)
	{
		m_shopManager->Uninit();
		delete m_shopManager;
		m_shopManager = nullptr;
	}
}

//=============================================================
// [ClearResult] �N���A���̍X�V
//=============================================================
void ClearResult::Update()
{
	// ���U���g�̃A�j���[�V����
	UpdateResultAnim();

	// �V���b�v���X�V����
	if (m_page->GetComponent<Pages>()->GetPage() == 1)
		m_shopManager->Update();
}

//=============================================================
// [ClearResult] ���U���g�A�j���[�V�����̍X�V
//=============================================================
void ClearResult::UpdateResultAnim()
{
	// �w�i�̃t�F�[�h
	float currentAlpha = m_bg->GetComponent<CPolygon>()->GetColor().a;
	m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, currentAlpha + (0.5f - currentAlpha) * 0.02f));

	// �N���A�e�L�X�g�̃A�j���[�V����
	if (m_progState <= ClearResult::P_MTTEXT)
	{
		m_mtText->transform->SetPos(m_mtText->transform->GetWPos().x +
			(CRenderer::SCREEN_WIDTH / 2 - m_mtText->transform->GetWPos().x) * 0.08f, 100.0f
		);
	}

	// �o�C�N�̏���\��
	if (m_progState == ClearResult::P_FUEL)
	{
		if (m_progCounter == 80)
		{
			m_fuelView->GetComponent<ResultViewBar>()->StartAnim();
		}
	}
	if (m_progState == ClearResult::P_ENDURANCE)
	{
		if (m_progCounter == 80)
		{
			m_enduranceView->GetComponent<ResultViewBar>()->StartAnim();
		}
	}

	// ���̏�ԂɈڍs���鏈��
	m_progCounter--;
	if (m_progCounter <= 0 && m_progState != P_END)
	{
		m_progState = static_cast<PROG_STATE>(m_progState + 1);
		m_progCounter = 80;
	}
}

//=============================================================
// [ClearResult] �`��
//=============================================================
void ClearResult::Draw()
{

}



//=============================================================
// [GameOverResult] ������
//=============================================================
void GameOverResult::Init()
{
	// ���j�����C���N�������g
	m_goalCount++;

	// �N���A�e�L�X�g
	{
		m_mtText = new GameObject("MtClearText", "UI");
		m_mtText->AddComponent<CText>();
		m_mtText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		m_mtText->GetComponent<CText>()->SetFontSize(130);
		m_mtText->GetComponent<CText>()->SetOutlineColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		m_mtText->GetComponent<CText>()->SetOutlineSize(2);
		m_mtText->GetComponent<CText>()->SetText("<size=150>GAME OVER");
		m_mtText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_mtText->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH/2), 100.0f, 0.0f);
	}

	// �f�[�^�\��
	{
		m_dataView = new GameObject("DataView", "UI");
		m_dataView->AddComponent<ResultDataView>();
		m_dataView->transform->Translate(60.0f, 350.0f, 0.0f);

		// �ŐV�f�[�^���擾
		ResultData data = m_results[m_results.size() - 1];
		m_dataView->GetComponent<ResultDataView>()->SetTime(-1);
		m_dataView->GetComponent<ResultDataView>()->SetHighSpeed(data.highSpeed);
		m_dataView->GetComponent<ResultDataView>()->SetAction(data.action);
	}

	// �o�C�N���\��
	{
		// �R��
		m_fuelView = new GameObject("FuelView", "UI");
		m_fuelView->transform->SetPos(1310.0f, 360.0f);
		m_fuelView->AddComponent<ResultViewBar>(
			"�R��",
			D3DCOLOR_RGBA(232, 44, 44, 255),
			D3DCOLOR_RGBA(255, 0, 0, 255),
			D3DCOLOR_RGBA(255, 0, 0, 255)
			);
		m_fuelView->GetComponent<ResultViewBar>()->SetValue(
			static_cast<float>(m_beforeFuel / CVehicle::MAX_FUEL),
			static_cast<float>(m_gameScene->GetBike()->GetComponent<CVehicle>()->GetFuel() / CVehicle::MAX_FUEL));

		// �ϋv�l
		m_enduranceView = new GameObject("EnduranceView", "UI");
		m_enduranceView->transform->SetPos(1310.0f, 600.0f);
		m_enduranceView->AddComponent<ResultViewBar>(
			"�ϋv�l",
			D3DCOLOR_RGBA(78, 69, 255, 255),
			D3DCOLOR_RGBA(75, 67, 224, 255),
			D3DCOLOR_RGBA(61, 100, 255, 255)
			);
		m_enduranceView->GetComponent<ResultViewBar>()->SetValue(
			static_cast<float>(m_beforeEndurance / CVehicle::MAX_ENDURANCE),
			static_cast<float>(m_gameScene->GetBike()->GetComponent<CVehicle>()->GetEndurance() / CVehicle::MAX_ENDURANCE));
	}

	// �n�`�摜
	{
		m_terrainImg = new GameObject("TerrainImg", "ResultData");
		m_terrainImg->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), static_cast<float>(CRenderer::SCREEN_HEIGHT / 2));
		m_terrainImg->AddComponent<ResultTerrain>();
	}

	// �V�[�h�e�L�X�g
	{
		m_seedText = new GameObject("SeedText", "UI");
		m_seedText->AddComponent<CText>();
		m_seedText->GetComponent<CText>()->SetFontSize(50);
		m_seedText->GetComponent<CText>()->SetText("<color=150,150,150>Seed: " + std::to_string(m_gameScene->GetTerrain()->GetSeed()));
		m_seedText->transform->SetPos(5.0f, CRenderer::SCREEN_HEIGHT - 50.0f);
	}

	// �w�i
	{
		m_bg = new GameObject("BG", "UI");
		m_bg->SetPriority(3);
		m_bg->AddComponent<CPolygon>();
		m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		m_bg->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));
	}

	// ���̎R�� or �I��
	{
		GameObject* pNextButton = new GameObject("NextMountain");
		pNextButton->transform->SetSize(500.0f, 140.0f);
		pNextButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) + 400.0f, 850.0f);
		pNextButton->AddComponent<ButtonUI>();
		pNextButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		pNextButton->GetComponent<ButtonUI>()->setClickEvent([]() { CSceneManager::GetInstance()->SetScene("title"); });

		GameObject* pNextButtonText = new GameObject();
		pNextButtonText->SetParent(pNextButton);
		pNextButtonText->transform->SetPos(250.0f, 35.0f);
		pNextButtonText->AddComponent<CText>();
		pNextButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pNextButtonText->GetComponent<CText>()->SetText("<color=0,0,0>END");
		pNextButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	}

	// ���X�g�ǉ�
	{
		GameObject* pListButton = new GameObject("NextMountain");
		pListButton->transform->SetSize(500.0f, 140.0f);
		pListButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) - 400.0f, 850.0f);
		pListButton->AddComponent<ButtonUI>();
		pListButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");

		GameObject* pListButtonText = new GameObject();
		pListButtonText->SetParent(pListButton);
		pListButtonText->transform->SetPos(250.0f, 35.0f);
		pListButtonText->AddComponent<CText>();
		pListButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pListButtonText->GetComponent<CText>()->SetFontSize(80);
		pListButtonText->GetComponent<CText>()->SetText("<color=0,0,0>���X�g�ɒǉ�");
		pListButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	}

	// �O��̏��Ƃ��ĕۑ�
	m_beforeFuel = m_gameScene->GetBike()->GetComponent<CVehicle>()->GetFuel();
	m_beforeEndurance = m_gameScene->GetBike()->GetComponent<CVehicle>()->GetEndurance();
}

//=============================================================
// [GameOverResult] �I��
//=============================================================
void GameOverResult::Uninit()
{
}

//=============================================================
// [GameOverResult] �N���A���̍X�V
//=============================================================
void GameOverResult::Update()
{
}

//=============================================================
// [GameOverResult] ���U���g�A�j���[�V�����̍X�V
//=============================================================
void GameOverResult::UpdateResultAnim()
{
	// �w�i�̃t�F�[�h
	float currentAlpha = m_bg->GetComponent<CPolygon>()->GetColor().a;
	m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, currentAlpha + (0.5f - currentAlpha) * 0.02f));
}

//=============================================================
// [GameOverResult] �`��
//=============================================================
void GameOverResult::Draw()
{
}