//=============================================================
//
// ���U���g [mt_result.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "mt_result.h"
#include "scene/game.h"
#include "manager.h"

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
#include "scripts/virtual_cursor.h"

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
// [ResultBase] ���ώ���
//=============================================================
int ResultBase::GetAverageTime()
{
	int allTime = 0;
	int count = 0;
	for (auto itr = m_results.begin(); itr != m_results.end(); itr++)
	{
		if ((*itr).time != -1)
		{
			allTime += (*itr).time;
			count++;
		}
	}

	if (count > 0)
	{
		return allTime / count;
	}
	return allTime;
}

//=============================================================
// [ResultBase] ���σA�N�V����
//=============================================================
int ResultBase::GetAverageAction()
{
	int allAction = 0;
	int count = 0;
	for (auto itr = m_results.begin(); itr != m_results.end(); itr++)
	{
		allAction += (*itr).action;
		count++;
	}

	if (count > 0)
	{
		return allAction / count;
	}
	return allAction;
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
		m_shopButton = new GameObject("NextMountain");
		page->AddObject(0, m_shopButton);
		m_shopButton->transform->SetSize(500.0f, 140.0f);
		m_shopButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) + 400.0f, 850.0f);
		m_shopButton->AddComponent<ButtonUI>();
		m_shopButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		m_shopButton->GetComponent<ButtonUI>()->setClickEvent([this, page]() { 
			page->SetPage(1); 
			});

		GameObject* pShopButtonText = new GameObject();
		pShopButtonText->SetParent(m_shopButton);
		page->AddObject(0, pShopButtonText);
		pShopButtonText->transform->SetPos(250.0f, 35.0f);
		pShopButtonText->AddComponent<CText>();
		pShopButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pShopButtonText->GetComponent<CText>()->SetText("<color=0,0,0>Shop ��");
		pShopButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	}

	// ���X�g�ǉ�
	{
		m_addListButton = new GameObject("NextMountain");
		page->AddObject(0, m_addListButton);
		m_addListButton->transform->SetSize(500.0f, 140.0f);
		m_addListButton->transform->SetPos((CRenderer::SCREEN_WIDTH/2 - 250.0f) - 400.0f, 850.0f);
		m_addListButton->AddComponent<ButtonUI>();
		m_addListButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");

		GameObject* pListButtonText = new GameObject();
		pListButtonText->SetParent(m_addListButton);
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

	// ���z�J�[�\���̍쐬
	GameObject* cursorObj = new GameObject();
	cursorObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\cursor.png");
	cursorObj->transform->SetSize(60.0f, 60.0f);
	cursorObj->SetPriority(9);
	cursorObj->AddComponent<VirtualCursor>();
	Main::SetShowCursor(false);

	// BGM���Đ�����
	m_volumeFade = 0.0f;
	m_bgm = AudioManager::GetInstance()->CreateClip("data\\SOUND\\BGM\\result.mp3", FMOD_2D | FMOD_LOOP_NORMAL, true);
	m_bgmObj = new GameObject();
	m_bgmObj->AddComponent<AudioSource>()->Play(m_bgm);
	m_bgmObj->GetComponent<AudioSource>()->GetChannel()->setVolume(m_volumeFade);

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

	// �w�i�̔j��
	m_bg->Destroy();

	// �����폜����
	AudioManager::GetInstance()->RemoveClip(m_bgm);
}

//=============================================================
// [ClearResult] �N���A���̍X�V
//=============================================================
void ClearResult::Update()
{
	// ���U���g�̃A�j���[�V����
	UpdateResultAnim();

	// BGM�̃t�F�[�h
	m_volumeFade += BGM_FADE;
	if (m_volumeFade > BGM_VOLUME) m_volumeFade = BGM_VOLUME;
	m_bgmObj->GetComponent<AudioSource>()->GetChannel()->setVolume(m_volumeFade);

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
	// �y�[�W�̏�����
	m_page = new GameObject("PageManager");
	m_page->AddComponent<Pages>();
	m_page->GetComponent<Pages>()->SetNumPage(2);
	auto page = m_page->GetComponent<Pages>();

	// �i��
	m_progState = PROG_STATE::P_FUEL;
	m_progCounter = 120;

	// �Q�[���I�[�o�[�e�L�X�g
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
		page->AddObject(0, m_mtText);
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

		page->AddObject(0, m_dataView);
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

	// �ŏI����
	{
		GameObject* pFinalResultButton = new GameObject("FinalResult");
		pFinalResultButton->transform->SetSize(500.0f, 140.0f);
		pFinalResultButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) + 400.0f, 850.0f);
		pFinalResultButton->AddComponent<ButtonUI>();
		pFinalResultButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		pFinalResultButton->GetComponent<ButtonUI>()->setClickEvent([this, page]() {
			page->SetPage(1); 
			});
		page->AddObject(0, pFinalResultButton);

		GameObject* pFinalResultButtonText = new GameObject();
		pFinalResultButtonText->SetParent(pFinalResultButton);
		pFinalResultButtonText->transform->SetPos(250.0f, 35.0f);
		pFinalResultButtonText->AddComponent<CText>();
		pFinalResultButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pFinalResultButtonText->GetComponent<CText>()->SetText("<color=0,0,0>�ŏI���ʂ�");
		pFinalResultButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		page->AddObject(0, pFinalResultButtonText);
	}

	// ���X�g�ǉ�
	{
		GameObject* pListButton = new GameObject("NextMountain");
		pListButton->transform->SetSize(500.0f, 140.0f);
		pListButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) - 400.0f, 850.0f);
		pListButton->AddComponent<ButtonUI>();
		pListButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		page->AddObject(0, pListButton);

		GameObject* pListButtonText = new GameObject();
		pListButtonText->SetParent(pListButton);
		pListButtonText->transform->SetPos(250.0f, 35.0f);
		pListButtonText->AddComponent<CText>();
		pListButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pListButtonText->GetComponent<CText>()->SetFontSize(80);
		pListButtonText->GetComponent<CText>()->SetText("<color=0,0,0>���X�g�ɒǉ�");
		pListButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		page->AddObject(0, pListButtonText);
	}

	// ���z�J�[�\���̍쐬
	GameObject* cursorObj = new GameObject();
	cursorObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\cursor.png");
	cursorObj->transform->SetSize(60.0f, 60.0f);
	cursorObj->SetPriority(9);
	cursorObj->AddComponent<VirtualCursor>();
	Main::SetShowCursor(false);

	// BGM���Đ�����
	m_volumeFade = 0.0f;
	m_bgm = AudioManager::GetInstance()->CreateClip("data\\SOUND\\BGM\\gameover.mp3", FMOD_2D | FMOD_LOOP_NORMAL, true);
	m_bgmObj = new GameObject();
	m_bgmObj->AddComponent<AudioSource>()->Play(m_bgm);
	m_bgmObj->GetComponent<AudioSource>()->GetChannel()->setVolume(m_volumeFade);

	// �ŏI���ʂ̏�����
	InitFinalResult();

	// �O��̏��Ƃ��ĕۑ�
	m_beforeFuel = m_gameScene->GetBike()->GetComponent<CVehicle>()->GetFuel();
	m_beforeEndurance = m_gameScene->GetBike()->GetComponent<CVehicle>()->GetEndurance();

	// �y�[�W�̃��Z�b�g
	page->AllHideObjects();
	page->SetPage(0);
}

//=============================================================
// [GameOverResult] �I��
//=============================================================
void GameOverResult::Uninit()
{
	// �w�i�̔j��
	m_bg->Destroy();

	// �����폜����
	AudioManager::GetInstance()->RemoveClip(m_bgm);
}

//=============================================================
// [GameOverResult] �Q�[���I�[�o�[���̍X�V
//=============================================================
void GameOverResult::Update()
{
	UpdateResultAnim();

	// BGM�̃t�F�[�h
	m_volumeFade += BGM_FADE;
	if (m_volumeFade > BGM_VOLUME) m_volumeFade = BGM_VOLUME;
	m_bgmObj->GetComponent<AudioSource>()->GetChannel()->setVolume(m_volumeFade);
}

//=============================================================
// [GameOverResult] ���U���g�A�j���[�V�����̍X�V
//=============================================================
void GameOverResult::UpdateResultAnim()
{
	// �w�i�̃t�F�[�h
	float currentAlpha = m_bg->GetComponent<CPolygon>()->GetColor().a;
	m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, currentAlpha + (0.5f - currentAlpha) * 0.02f));

	// �o�C�N�̏���\��
	if (m_progState == GameOverResult::P_FUEL)
	{
		if (m_progCounter == 80)
		{
			m_fuelView->GetComponent<ResultViewBar>()->StartAnim();
		}
	}
	if (m_progState == GameOverResult::P_ENDURANCE)
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
// [GameOverResult] �`��
//=============================================================
void GameOverResult::Draw()
{
}

//=============================================================
// [GameOverResult] �ŏI����
//=============================================================
void GameOverResult::InitFinalResult()
{
	auto page = m_page->GetComponent<Pages>();

	// �f�[�^�v�Z ------------------------------------------------------------------------------------

	// ���ώ��Ԃ̕\���`����ύX����
	int time = GetAverageTime();
	int min = (time - time % 60) / 60;
	int sec = time % 60;
	char timeTextPara[64];
	sprintf(&timeTextPara[0], "%d:%02d", min, sec);

	// �R����v�Z����
	float fuelConsumption = 0.0f;
	float totalMileage = 0.0f;
	float totalFuel = 0.0f;
	for (auto itr = m_results.begin(); itr != m_results.end(); itr++)
	{
		totalMileage += (*itr).mileage;
		totalFuel += (*itr).fuel * 0.6f;
	}
	fuelConsumption = totalMileage / totalFuel;
	char fuelTextPara[64];
	sprintf(&fuelTextPara[0], "%.1f<size=40>Km/L", fuelConsumption);

	// �X�R�A�v�Z
	int score = 0;
	score += GetAverageAction() * static_cast<int>(m_results.size());
	for (auto itr = m_results.begin(); itr != m_results.end(); itr++)
	{
		score += (500 - (*itr).time) * 2;
	}
	score += static_cast<int>(m_results.size()) * 1200;

	// �����N���� ------------------------------------------------------------------------------------
	enum RANK
	{
		RANK_S,
		RANK_A,
		RANK_B,
		RANK_C
	};

	// �����N�̃e�N�X�`���擾
	auto GetRankPath = [](RANK rank)
	{
		if (rank == RANK_S) return "data\\TEXTURE\\RESULT\\rank_s.png";
		else if (rank == RANK_A) return "data\\TEXTURE\\RESULT\\rank_a.png";
		else if (rank == RANK_B) return "data\\TEXTURE\\RESULT\\rank_b.png";
		else if (rank == RANK_C) return "data\\TEXTURE\\RESULT\\rank_c.png";
		return "";
	};

	// ���σ^�C��
	RANK timeRank;
	if (time < 60) timeRank = RANK_S;
	else if (time < 100) timeRank = RANK_A;
	else if (time < 180) timeRank = RANK_B;
	else timeRank = RANK_C;
	if (timeRank == 0) timeRank = RANK_C;

	// �A�N�V����
	RANK actionRank = RANK_S;
	int actionScore = GetAverageAction();
	if (actionScore >= 3000) actionRank = RANK_S;
	else if (actionScore >= 1800) actionRank = RANK_A;
	else if (actionScore >= 600) actionRank = RANK_B;
	else actionRank = RANK_C;

	// �R��
	RANK fuelRank = RANK_S;
	if (fuelConsumption >= 20.0f) fuelRank = RANK_S;
	else if (fuelConsumption >= 16.0f) fuelRank = RANK_A;
	else if (fuelConsumption >= 11.0f) fuelRank = RANK_B;
	else fuelRank = RANK_C;


	// �\�� ------------------------------------------------------------------------------------

	// �ŏI����
	GameObject* finalText = new GameObject();
	finalText->AddComponent<CText>()->SetText("�X�R�A");
	finalText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	finalText->GetComponent<CText>()->SetAlign(CText::CENTER);
	finalText->GetComponent<CText>()->SetFontSize(100);
	finalText->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 70.0f);
	page->AddObject(1, finalText);

	// �ŏI�X�R�A
	m_scoreText = new GameObject();
	m_scoreText->AddComponent<CText>()->SetText(std::to_string(score));
	m_scoreText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_scoreText->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_scoreText->GetComponent<CText>()->SetFontSize(200);
	m_scoreText->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 200.0f);
	page->AddObject(1, m_scoreText);

	// �X�R�A�o�[
	GameObject* scoreBar = new GameObject();
	scoreBar->AddComponent<CPolygon>();
	scoreBar->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	scoreBar->transform->SetSize(CRenderer::SCREEN_WIDTH - 500.0f, 6.0f);
	scoreBar->transform->SetPos(250.0f, 380.0f);
	page->AddObject(1, scoreBar);

	// �^�C���g
	GameObject* timeFrame = new GameObject();
	timeFrame->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\RESULT\\result_frame.png");
	timeFrame->transform->SetSize(350.0f, 540.0f);
	timeFrame->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 175.0f - 450.0f, 460.0f);
	timeFrame->SetPriority(7);
	page->AddObject(1, timeFrame);
	
	GameObject* timeTextBG = new GameObject();
	timeTextBG->AddComponent<CPolygon>();
	timeTextBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(36, 39, 255, 255));
	timeTextBG->SetParent(timeFrame);
	timeTextBG->transform->SetSize(340.0f, 140.0f);
	timeTextBG->transform->SetPos(5.0f, 5.0f);
	page->AddObject(1, timeTextBG);

	GameObject* timeText = new GameObject();
	timeText->SetParent(timeFrame);
	timeText->transform->SetPos(175.0f, 60.0f);
	timeText->AddComponent<CText>()->SetText("�^�C��");
	timeText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	timeText->GetComponent<CText>()->SetFontSize(60);
	timeText->GetComponent<CText>()->SetAlign(CText::CENTER);
	page->AddObject(1, timeText);

	m_timeRate = new GameObject();
	m_timeRate->transform->SetPos(75.0f, 170.0f);
	m_timeRate->transform->SetSize(200.0f, 200.0f);
	m_timeRate->SetParent(timeFrame);
	m_timeRate->AddComponent<CPolygon>()->SetTexture(GetRankPath(timeRank));
	page->AddObject(1, m_timeRate);

	m_timeValue = new GameObject();
	m_timeValue->transform->SetPos(0.0f, 350.0f);
	m_timeValue->SetParent(timeText);
	m_timeValue->AddComponent<CText>()->SetText(timeTextPara);
	m_timeValue->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_timeValue->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_timeValue->GetComponent<CText>()->SetFontSize(60);
	page->AddObject(1, m_timeValue);

	// �A�N�V�����g
	GameObject* actionFrame = new GameObject();
	actionFrame->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\RESULT\\result_frame.png");
	actionFrame->transform->SetSize(350.0f, 540.0f);
	actionFrame->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 175.0f, 460.0f);
	actionFrame->SetPriority(7);
	page->AddObject(1, actionFrame);

	GameObject* actionTextBG = new GameObject();
	actionTextBG->AddComponent<CPolygon>();
	actionTextBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(178, 36, 255, 255));
	actionTextBG->SetParent(actionFrame);
	actionTextBG->transform->SetSize(340.0f, 140.0f);
	actionTextBG->transform->SetPos(5.0f, 5.0f);
	page->AddObject(1, actionTextBG);

	GameObject* actionText = new GameObject();
	actionText->SetParent(actionFrame);
	actionText->transform->SetPos(180.0f, 60.0f);
	actionText->AddComponent<CText>()->SetText("�A�N�V����");
	actionText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	actionText->GetComponent<CText>()->SetFontSize(60);
	actionText->GetComponent<CText>()->SetAlign(CText::CENTER);
	page->AddObject(1, actionText);

	m_actionRate = new GameObject();
	m_actionRate->transform->SetPos(75.0f, 170.0f);
	m_actionRate->transform->SetSize(200.0f, 200.0f);
	m_actionRate->SetParent(actionFrame);
	m_actionRate->AddComponent<CPolygon>()->SetTexture(GetRankPath(actionRank));
	page->AddObject(1, m_actionRate);

	m_actionValue = new GameObject();
	m_actionValue->transform->SetPos(0.0f, 350.0f);
	m_actionValue->SetParent(actionText);
	m_actionValue->AddComponent<CText>()->SetText(std::to_string(GetAverageAction()));
	m_actionValue->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_actionValue->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_actionValue->GetComponent<CText>()->SetFontSize(60);
	page->AddObject(1, m_actionValue);

	// �R��g
	GameObject* fuelFrame = new GameObject();
	fuelFrame->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\RESULT\\result_frame.png");
	fuelFrame->transform->SetSize(350.0f, 540.0f);
	fuelFrame->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 175.0f + 450.0f, 460.0f);
	fuelFrame->SetPriority(7);
	page->AddObject(1, fuelFrame);

	GameObject* fuelTextBG = new GameObject();
	fuelTextBG->AddComponent<CPolygon>();
	fuelTextBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(252, 135, 18, 255));
	fuelTextBG->SetParent(fuelFrame);
	fuelTextBG->transform->SetSize(340.0f, 140.0f);
	fuelTextBG->transform->SetPos(5.0f, 5.0f);
	page->AddObject(1, fuelTextBG);

	GameObject* fuelText = new GameObject();
	fuelText->SetParent(fuelFrame);
	fuelText->transform->SetPos(175.0f, 60.0f);
	fuelText->AddComponent<CText>()->SetText("�R��");
	fuelText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	fuelText->GetComponent<CText>()->SetFontSize(60);
	fuelText->GetComponent<CText>()->SetAlign(CText::CENTER);
	page->AddObject(1, fuelText);

	m_fuelRate = new GameObject();
	m_fuelRate->transform->SetPos(75.0f, 170.0f);
	m_fuelRate->transform->SetSize(200.0f, 200.0f);
	m_fuelRate->SetParent(fuelFrame);
	m_fuelRate->AddComponent<CPolygon>()->SetTexture(GetRankPath(fuelRank));
	page->AddObject(1, m_fuelRate);

	m_fuelValue = new GameObject();
	m_fuelValue->transform->SetPos(0.0f, 350.0f);
	m_fuelValue->SetParent(fuelText);
	m_fuelValue->AddComponent<CText>()->SetText(fuelTextPara);
	m_fuelValue->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_fuelValue->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_fuelValue->GetComponent<CText>()->SetFontSize(60);
	page->AddObject(1, m_fuelValue);

	// �^�C�g����
	GameObject* titleButton = new GameObject();
	titleButton->AddComponent<ButtonUI>();
	titleButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\go_title.png");
	titleButton->transform->SetSize(145.0f, 568.0f);
	titleButton->transform->SetPos(CRenderer::SCREEN_WIDTH - 145.0f, CRenderer::SCREEN_HEIGHT - 568.0f);
	titleButton->GetComponent<ButtonUI>()->setClickEvent([this]()
		{
			m_gameScene->ResetGame();
			CSceneManager::GetInstance()->SetScene("title"); 
		});
	page->AddObject(1, titleButton);
}