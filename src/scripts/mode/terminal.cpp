//=============================================================
//
// �I�[���[�h [terminal.cpp]
// Author: Ito Kotaro
// 
// ���[���h�̒[�܂œ��B����ƃS�[������
//
//=============================================================
#include "terminal.h"
#include "scene/game.h"

#include "component/other/button.h"
#include "scripts/result/result_data.h"
#include "scripts/result/result_view.h"
#include "scripts/result/result_terrain.h"
#include "scripts/ranking_system.h"
#include "scripts/virtual_cursor.h"
#include "scripts/direction_arrow.h"

//=============================================================
// [TerminalMode] ������
//=============================================================
void TerminalMode::Init()
{
	// �n�`�𐶐�����
	m_game->GenerateTerrain();

	// �������𐶐�����
	m_directionObj = new GameObject("DirectionArrow");
	m_directionObj->transform->SetPos(0.0f, 0.0f, 50.0f);
	m_directionObj->AddComponent<DirectionArrow>();
}

//=============================================================
// [TerminalMode] �I��
//=============================================================
void TerminalMode::Uninit()
{
	m_directionObj->Destroy();
}

//=============================================================
// [TerminalMode] �X�V
//=============================================================
void TerminalMode::Update()
{
	// �o�C�N�̈ʒu���擾����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	D3DXVECTOR3 vehiclePos = gameScene->GetBike()->transform->GetWPos();

	// �n�`�̒[�ɍs�����Ƃ�
	float distanceHalf = (gameScene->GetTerrain()->GetTerrainSize() * gameScene->GetTerrain()->GetTerrainScale()) / 2.0f;
	if (vehiclePos.x <= -distanceHalf + EXTENSION_DISTANCE || vehiclePos.x >= distanceHalf - EXTENSION_DISTANCE ||
		vehiclePos.z <= -distanceHalf + EXTENSION_DISTANCE || vehiclePos.z >= distanceHalf - EXTENSION_DISTANCE)
	{
		//gameScene->CalcResultData();
		ModeManager::GetInstance()->SetResult(new ClearTerminalResult());
		return;
	}

	// �Œፂ�x�������ɍs�����Ƃ�
	if (vehiclePos.y < gameScene->GetTerrain()->GetMinHeight() - 5.0f)
	{
		ModeManager::GetInstance()->SetResult(new ClearTerminalResult());
		return;
	}

	// �R���������Ȃ����Ƃ�
	if (gameScene->GetBike()->GetComponent<CVehicle>()->GetFuel() <= 0.0f)
	{
		ModeManager::GetInstance()->SetResult(new GameOverTerminalResult());
		return;
	}

	// �ϋv�l�������Ȃ����Ƃ��̏���
	if (gameScene->GetBike()->GetComponent<CVehicle>()->GetEndurance() <= 0)
	{
		ModeManager::GetInstance()->SetResult(new GameOverTerminalResult());
		return;
	}

	if (INPUT_INSTANCE->onTrigger("@"))
	{
		ModeManager::GetInstance()->SetResult(new ClearTerminalResult());
		return;
	}

	// �������̖ړI�ʒu���X�V����
	m_directionObj->GetComponent<DirectionArrow>()->SetDestination(CalcNearGoal());
}

//=============================================================
// [TerminalMode] ���U���g�C�x���g
//=============================================================
void TerminalMode::OnResultEvent()
{
	// ���U���g�̃f�[�^���i�[����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	m_resultDatas.push_back(gameScene->GetResultData());
}

//=============================================================
// [TerminalMode] �ł��߂��S�[���̈ʒu���擾����
//=============================================================
D3DXVECTOR3 TerminalMode::CalcNearGoal()
{
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	D3DXVECTOR3 vehiclePos = gameScene->GetBike()->transform->GetWPos();

	// �㉺���E�̂ǂ̒n�_����ԋ߂���
	enum NEAR_PT
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};
	NEAR_PT nearPt;

	// ��ԋ߂��|�C���g�𓱂��o��
	float nearDis = gameScene->GetTerrain()->GetTerrainSize() * gameScene->GetTerrain()->GetTerrainScale();
	float distanceHalf = nearDis / 2.0f;
	if (fabsf(vehiclePos.x - distanceHalf) < nearDis)
	{
		nearPt = LEFT;
		nearDis = fabsf(vehiclePos.x - distanceHalf);
	}
	if (fabsf(vehiclePos.x + distanceHalf) < nearDis)
	{
		nearPt = RIGHT;
		nearDis = fabsf(vehiclePos.x + distanceHalf);
	}
	if (fabsf(vehiclePos.z + distanceHalf) < nearDis)
	{
		nearPt = TOP;
		nearDis = fabsf(vehiclePos.z + distanceHalf);
	}
	if (fabsf(vehiclePos.z - distanceHalf) < nearDis)
	{
		nearPt = BOTTOM;
		nearDis = fabsf(vehiclePos.z - distanceHalf);
	}

	// ���ꂼ��̕����̖ڕW�n�_��ݒ肷��
	D3DXVECTOR3 destination = { 0.0f, 0.0f, 0.0f };
	switch (nearPt)
	{
	case TOP:
		destination = { 0.0f, 0.0f, -distanceHalf };
		break;
	case BOTTOM:
		destination = { 0.0f, 0.0f, distanceHalf };
		break;
	case LEFT:
		destination = { distanceHalf, 0.0f, 0.0f };
		break;
	case RIGHT:
		destination = { -distanceHalf, 0.0f, 0.0f };
		break;
	}

	return destination;
}


//=============================================================
// [TerminalResult] ���ώ���
//=============================================================
int TerminalResult::GetAverageTime()
{
	int allTime = 0;
	int count = 0;

	// �^�[�~�i�����[�h���擾����
	TerminalMode* terminalMode = dynamic_cast<TerminalMode*>(ModeManager::GetInstance()->GetMode());
	if (terminalMode == nullptr) { return -1; }

	// ���v���Ԃ��Z�o����
	for (auto itr = terminalMode->GetResultData().begin(); itr != terminalMode->GetResultData().end(); itr++)
	{
		if ((*itr).time != -1)
		{
			allTime += (*itr).time;
			count++;
		}
	}

	// ���ϒl�ɂ���
	if (count > 0)
	{
		return allTime / count;
	}
	return allTime;
}

//=============================================================
// [TerminalResult] ���σA�N�V����
//=============================================================
int TerminalResult::GetAverageAction()
{
	int allAction = 0;
	int count = 0;

	// �^�[�~�i�����[�h���擾����
	TerminalMode* terminalMode = dynamic_cast<TerminalMode*>(ModeManager::GetInstance()->GetMode());
	if (terminalMode == nullptr) { return 0; }

	// ���v�A�N�V�����|�C���g���Z�o����
	for (auto itr = terminalMode->GetResultData().begin(); itr != terminalMode->GetResultData().end(); itr++)
	{
		allAction += (*itr).action;
		count++;
	}

	// ���ϒl�ɂ���
	if (count > 0)
	{
		return allAction / count;
	}
	return allAction;
}

//=============================================================
// [TerminalResult] ���v���j�����擾����
//=============================================================
int TerminalResult::GetNumOfStep()
{
	// �^�[�~�i�����[�h���擾����
	TerminalMode* terminalMode = dynamic_cast<TerminalMode*>(ModeManager::GetInstance()->GetMode());
	if (terminalMode == nullptr) { return 0; }

	// ���j����Ԃ�
	return static_cast<int>(terminalMode->GetResultData().size());
}


//=============================================================
// [ClearResult] ������
//=============================================================
void ClearTerminalResult::Init()
{
	// �i��
	m_progState = P_MTTEXT;
	m_progCounter = 120;

	// ���j�����擾����
	TerminalMode* terminalMode = dynamic_cast<TerminalMode*>(ModeManager::GetInstance()->GetMode());
	int goalNum = 0;
	if (terminalMode != nullptr)
	{
		goalNum = static_cast<int>(terminalMode->GetResultData().size()) + 1;
	}

	// BGM���Đ�����
	m_volumeFade = 0.0f;
	m_bgm = AudioManager::GetInstance()->CreateClip("data\\SOUND\\BGM\\SYSTEM\\result.mp3", FMOD_2D | FMOD_LOOP_NORMAL, true);
	m_clickSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\click.mp3", FMOD_2D);

	m_bgmObj = new GameObject();
	m_bgmObj->AddComponent<AudioSource>()->Play(m_bgm);
	m_bgmObj->GetComponent<AudioSource>()->GetChannel()->setVolume(m_volumeFade);

	// �y�[�W�̏�����
	m_page = new GameObject("PageManager");
	m_page->AddComponent<Pages>();
	m_page->GetComponent<Pages>()->SetNumPage(3);
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
		m_mtText->GetComponent<CText>()->SetText(std::to_string(goalNum) + "�ڂ̎R�𓥔j���܂���");
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
		ResultData data = m_gameScene->GetResultData();
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
		// ���֐i�ރ{�^���𐶐�����
		m_nextButton = new GameObject("NextMountain");
		m_nextButton->transform->SetSize(500.0f, 140.0f);
		m_nextButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) + 400.0f, 850.0f);
		m_nextButton->AddComponent<ButtonUI>();
		m_nextButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		m_nextButton->GetComponent<ButtonUI>()->setClickEvent([this, page]() {
			// ����炷
			m_bgmObj->GetComponent<AudioSource>()->PlayOneShot(m_clickSE, 2.0f);

			// ���̃y�[�W��
			page->SetPage(1);
			});
		page->AddObject(0, m_nextButton);

		// ���֐i�ރ{�^���̃e�L�X�g�𐶐�����
		GameObject* nextButtonText = new GameObject();
		nextButtonText->SetParent(m_nextButton);
		nextButtonText->transform->SetPos(250.0f, 35.0f);
		nextButtonText->AddComponent<CText>();
		nextButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		nextButtonText->GetComponent<CText>()->SetFontSize(80);
		nextButtonText->GetComponent<CText>()->SetText("<color=0,0,0>���֐i��");
		nextButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		page->AddObject(0, nextButtonText);
	}

	// �I��
	{
		// �I���{�^���𐶐�����
		m_endButton = new GameObject("EndMountain");
		page->AddObject(0, m_endButton);
		m_endButton->transform->SetSize(500.0f, 140.0f);
		m_endButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) - 400.0f, 850.0f);
		m_endButton->AddComponent<ButtonUI>();
		m_endButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		m_endButton->GetComponent<ButtonUI>()->setClickEvent([this, page]() {
			// ����炷
			m_bgmObj->GetComponent<AudioSource>()->PlayOneShot(m_clickSE, 2.0f);

			// �󔒂̃y�[�W�ɐ؂�ւ���
			page->SetPage(2);

			// �ŏI���ʉ�ʂ𐶐�����
			FinalResult(true);
			});

		// �I���{�^���̃e�L�X�g�𐶐�����
		GameObject* endButtonText = new GameObject();
		endButtonText->SetParent(m_endButton);
		page->AddObject(0, endButtonText);
		endButtonText->transform->SetPos(250.0f, 35.0f);
		endButtonText->AddComponent<CText>();
		endButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		endButtonText->GetComponent<CText>()->SetFontSize(80);
		endButtonText->GetComponent<CText>()->SetText("<color=0,0,0>���߂�");
		endButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	}

	// ����UI���쐬����
	m_descriptionUI = new GameObject();
	m_descriptionUI->transform->SetSize(DESC_SIZE);
	m_descriptionUI->AddComponent<CPolygon>();
	m_descriptionUI->GetComponent<CPolygon>()->SetTexture(DESC_END_TEXTURE);
	m_descriptionUI->SetVisible(false);
	page->AddObject(0, m_descriptionUI);

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

	// �O��̏��Ƃ��ĕۑ�
	m_beforeFuel = m_gameScene->GetBike()->GetComponent<CVehicle>()->GetFuel();
	m_beforeEndurance = m_gameScene->GetBike()->GetComponent<CVehicle>()->GetEndurance();
}

//=============================================================
// [ClearResult] �I��
//=============================================================
void ClearTerminalResult::Uninit()
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
	AudioManager::GetInstance()->RemoveClip(m_clickSE);
}

//=============================================================
// [ClearResult] �N���A���̍X�V
//=============================================================
void ClearTerminalResult::Update()
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

	// ����UI�̍X�V
	if (m_endButton->GetComponent<ButtonUI>()->GetOnCursor())
	{ // �I���{�^���ɃJ�[�\��������Ƃ�
		m_descriptionUI->GetComponent<CPolygon>()->SetTexture(DESC_END_TEXTURE);
		m_descriptionUI->transform->SetPos(m_endButton->transform->GetWPos() + D3DXVECTOR3(-60.0f, -280.0f, 0.0f));
		m_descriptionUI->GetComponent<CPolygon>()->Update();
		m_descriptionUI->SetVisible(true);
	}
	else if (m_nextButton->GetComponent<ButtonUI>()->GetOnCursor())
	{ // ���̎R�{�^���ɃJ�[�\��������Ƃ�
		m_descriptionUI->GetComponent<CPolygon>()->SetTexture(DESC_NEXT_TEXTURE);
		m_descriptionUI->transform->SetPos(m_nextButton->transform->GetWPos() + D3DXVECTOR3(-60.0f, -280.0f, 0.0f));
		m_descriptionUI->GetComponent<CPolygon>()->Update();
		m_descriptionUI->SetVisible(true);
	}
	else
	{ // �ǂ̃{�^���ɂ��J�[�\�����Ȃ��Ƃ�
		// ��\��
		m_descriptionUI->SetVisible(false);
	}
}

//=============================================================
// [ClearResult] ���U���g�A�j���[�V�����̍X�V
//=============================================================
void ClearTerminalResult::UpdateResultAnim()
{
	// �w�i�̃t�F�[�h
	float currentAlpha = m_bg->GetComponent<CPolygon>()->GetColor().a;
	m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, currentAlpha + (0.5f - currentAlpha) * 0.02f));

	// �N���A�e�L�X�g�̃A�j���[�V����
	if (m_progState <= ClearTerminalResult::P_MTTEXT)
	{
		m_mtText->transform->SetPos(m_mtText->transform->GetWPos().x +
			(CRenderer::SCREEN_WIDTH / 2 - m_mtText->transform->GetWPos().x) * 0.08f, 100.0f
		);
	}

	// �o�C�N�̏���\��
	if (m_progState == ClearTerminalResult::P_FUEL)
	{
		if (m_progCounter == 80)
		{
			m_fuelView->GetComponent<ResultViewBar>()->StartAnim();
		}
	}
	if (m_progState == ClearTerminalResult::P_ENDURANCE)
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
void ClearTerminalResult::Draw()
{

}



//=============================================================
// [GameOverResult] ������
//=============================================================
void GameOverTerminalResult::Init()
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
		m_mtText->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 100.0f, 0.0f);
		page->AddObject(0, m_mtText);
	}

	// �f�[�^�\��
	{
		m_dataView = new GameObject("DataView", "UI");
		m_dataView->AddComponent<ResultDataView>();
		m_dataView->transform->Translate(60.0f, 350.0f, 0.0f);

		// �ŐV�f�[�^���擾
		ResultData data = m_gameScene->GetResultData();
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
		// �ŏI���ʃ{�^���𐶐�����
		GameObject* pFinalResultButton = new GameObject("FinalResult");
		pFinalResultButton->transform->SetSize(500.0f, 140.0f);
		pFinalResultButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f), 850.0f);
		pFinalResultButton->AddComponent<ButtonUI>();
		pFinalResultButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		pFinalResultButton->GetComponent<ButtonUI>()->setClickEvent([this, page]() {
			page->SetPage(1);
			FinalResult(false);
			});
		page->AddObject(0, pFinalResultButton);

		// �ŏI���ʃ{�^���̃e�L�X�g�𐶐�����
		GameObject* pFinalResultButtonText = new GameObject();
		pFinalResultButtonText->SetParent(pFinalResultButton);
		pFinalResultButtonText->transform->SetPos(250.0f, 35.0f);
		pFinalResultButtonText->AddComponent<CText>();
		pFinalResultButtonText->GetComponent<CText>()->SetFontSize(80);
		pFinalResultButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pFinalResultButtonText->GetComponent<CText>()->SetText("<color=0,0,0>�ŏI���ʂ�");
		pFinalResultButtonText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		page->AddObject(0, pFinalResultButtonText);
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
	m_bgm = AudioManager::GetInstance()->CreateClip("data\\SOUND\\BGM\\SYSTEM\\gameover.mp3", FMOD_2D | FMOD_LOOP_NORMAL, true);
	m_bgmObj = new GameObject();
	m_bgmObj->AddComponent<AudioSource>()->Play(m_bgm);
	m_bgmObj->GetComponent<AudioSource>()->GetChannel()->setVolume(m_volumeFade);

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
void GameOverTerminalResult::Uninit()
{
	// �w�i�̔j��
	m_bg->Destroy();

	// �����폜����
	AudioManager::GetInstance()->RemoveClip(m_bgm);
}

//=============================================================
// [GameOverResult] �Q�[���I�[�o�[���̍X�V
//=============================================================
void GameOverTerminalResult::Update()
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
void GameOverTerminalResult::UpdateResultAnim()
{
	// �w�i�̃t�F�[�h
	float currentAlpha = m_bg->GetComponent<CPolygon>()->GetColor().a;
	m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, currentAlpha + (0.5f - currentAlpha) * 0.02f));

	// �o�C�N�̏���\��
	if (m_progState == GameOverTerminalResult::P_FUEL)
	{
		if (m_progCounter == 80)
		{
			m_fuelView->GetComponent<ResultViewBar>()->StartAnim();
		}
	}
	if (m_progState == GameOverTerminalResult::P_ENDURANCE)
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
void GameOverTerminalResult::Draw()
{
}

//=============================================================
// [TerminalResult] �ŏI����
//=============================================================
void TerminalResult::FinalResult(bool isSuccess)
{
	// �f�[�^�v�Z ------------------------------------------------------------------------------------

	// �^�[�~�i�����[�h���擾����
	TerminalMode* terminalMode = dynamic_cast<TerminalMode*>(ModeManager::GetInstance()->GetMode());
	if (terminalMode == nullptr) { return; }

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
	for (auto itr = terminalMode->GetResultData().begin(); itr != terminalMode->GetResultData().end(); itr++)
	{
		totalMileage += (*itr).mileage;
		totalFuel += (*itr).fuel;
	}
	fuelConsumption = totalMileage / totalFuel;
	char fuelTextPara[64];
	sprintf(&fuelTextPara[0], "%.1f<size=40>Km/L", fuelConsumption);

	// �X�R�A�v�Z
	int score = 0;
	score += GetAverageAction() * static_cast<int>(terminalMode->GetResultData().size());
	for (auto itr = terminalMode->GetResultData().begin(); itr != terminalMode->GetResultData().end(); itr++)
	{
		if ((*itr).time != -1)
		{
			score += (120 - (*itr).time) * 2;
		}
	}
	score += CLEAR_POINT * static_cast<int>(isSuccess ? terminalMode->GetResultData().size() : terminalMode->GetResultData().size() - 1);

	// �������Ă����ꍇ�͏I���|�C���g�����Z����
	if (isSuccess)
	{
		score += END_POINT;
	}

	// ���s���Ă������͔����ɂ���
	if (!isSuccess)
	{
		score /= 2;
	}

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
	else if (time < 120) timeRank = RANK_A;
	else if (time < 240) timeRank = RANK_B;
	else timeRank = RANK_C;
	if (timeRank == 0) timeRank = RANK_C;

	// �A�N�V����
	RANK actionRank = RANK_S;
	int actionScore = GetAverageAction();
	if (actionScore >= 1200) actionRank = RANK_S;
	else if (actionScore >= 600) actionRank = RANK_A;
	else if (actionScore >= 100) actionRank = RANK_B;
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

	// �ŏI�X�R�A
	m_scoreText = new GameObject();
	m_scoreText->AddComponent<CText>()->SetText(std::to_string(score));
	m_scoreText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_scoreText->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_scoreText->GetComponent<CText>()->SetFontSize(200);
	m_scoreText->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 200.0f);

	// �X�R�A�o�[
	GameObject* scoreBar = new GameObject();
	scoreBar->AddComponent<CPolygon>();
	scoreBar->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	scoreBar->transform->SetSize(CRenderer::SCREEN_WIDTH - 500.0f, 6.0f);
	scoreBar->transform->SetPos(250.0f, 380.0f);

	// �^�C���g
	GameObject* timeFrame = new GameObject();
	timeFrame->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\RESULT\\result_frame.png");
	timeFrame->transform->SetSize(350.0f, 540.0f);
	timeFrame->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 175.0f - 450.0f, 460.0f);
	timeFrame->SetPriority(7);

	// �^�C���w�i
	GameObject* timeTextBG = new GameObject();
	timeTextBG->AddComponent<CPolygon>();
	timeTextBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(36, 39, 255, 255));
	timeTextBG->SetParent(timeFrame);
	timeTextBG->transform->SetSize(340.0f, 140.0f);
	timeTextBG->transform->SetPos(5.0f, 5.0f);

	// �^�C���e�L�X�g
	GameObject* timeText = new GameObject();
	timeText->SetParent(timeFrame);
	timeText->transform->SetPos(175.0f, 60.0f);
	timeText->AddComponent<CText>()->SetText("�^�C��");
	timeText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	timeText->GetComponent<CText>()->SetFontSize(60);
	timeText->GetComponent<CText>()->SetAlign(CText::CENTER);

	// �^�C�������N
	m_timeRate = new GameObject();
	m_timeRate->transform->SetPos(75.0f, 170.0f);
	m_timeRate->transform->SetSize(200.0f, 200.0f);
	m_timeRate->SetParent(timeFrame);
	m_timeRate->AddComponent<CPolygon>()->SetTexture(GetRankPath(timeRank));

	// �^�C������
	m_timeValue = new GameObject();
	m_timeValue->transform->SetPos(0.0f, 350.0f);
	m_timeValue->SetParent(timeText);
	m_timeValue->AddComponent<CText>()->SetText(timeTextPara);
	m_timeValue->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_timeValue->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_timeValue->GetComponent<CText>()->SetFontSize(60);

	// �A�N�V�����g
	GameObject* actionFrame = new GameObject();
	actionFrame->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\RESULT\\result_frame.png");
	actionFrame->transform->SetSize(350.0f, 540.0f);
	actionFrame->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 175.0f, 460.0f);
	actionFrame->SetPriority(7);

	// �A�N�V�����w�i
	GameObject* actionTextBG = new GameObject();
	actionTextBG->AddComponent<CPolygon>();
	actionTextBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(178, 36, 255, 255));
	actionTextBG->SetParent(actionFrame);
	actionTextBG->transform->SetSize(340.0f, 140.0f);
	actionTextBG->transform->SetPos(5.0f, 5.0f);

	// �A�N�V�����e�L�X�g
	GameObject* actionText = new GameObject();
	actionText->SetParent(actionFrame);
	actionText->transform->SetPos(180.0f, 60.0f);
	actionText->AddComponent<CText>()->SetText("�A�N�V����");
	actionText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	actionText->GetComponent<CText>()->SetFontSize(60);
	actionText->GetComponent<CText>()->SetAlign(CText::CENTER);

	// �A�N�V���������N
	m_actionRate = new GameObject();
	m_actionRate->transform->SetPos(75.0f, 170.0f);
	m_actionRate->transform->SetSize(200.0f, 200.0f);
	m_actionRate->SetParent(actionFrame);
	m_actionRate->AddComponent<CPolygon>()->SetTexture(GetRankPath(actionRank));

	// �A�N�V��������
	m_actionValue = new GameObject();
	m_actionValue->transform->SetPos(0.0f, 350.0f);
	m_actionValue->SetParent(actionText);
	m_actionValue->AddComponent<CText>()->SetText(std::to_string(GetAverageAction()));
	m_actionValue->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_actionValue->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_actionValue->GetComponent<CText>()->SetFontSize(60);

	// �R��g
	GameObject* fuelFrame = new GameObject();
	fuelFrame->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\RESULT\\result_frame.png");
	fuelFrame->transform->SetSize(350.0f, 540.0f);
	fuelFrame->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 175.0f + 450.0f, 460.0f);
	fuelFrame->SetPriority(7);

	// �R���w�i
	GameObject* fuelTextBG = new GameObject();
	fuelTextBG->AddComponent<CPolygon>();
	fuelTextBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(252, 135, 18, 255));
	fuelTextBG->SetParent(fuelFrame);
	fuelTextBG->transform->SetSize(340.0f, 140.0f);
	fuelTextBG->transform->SetPos(5.0f, 5.0f);

	// �R���e�L�X�g
	GameObject* fuelText = new GameObject();
	fuelText->SetParent(fuelFrame);
	fuelText->transform->SetPos(175.0f, 60.0f);
	fuelText->AddComponent<CText>()->SetText("�R��");
	fuelText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	fuelText->GetComponent<CText>()->SetFontSize(60);
	fuelText->GetComponent<CText>()->SetAlign(CText::CENTER);

	// �R�������N
	m_fuelRate = new GameObject();
	m_fuelRate->transform->SetPos(75.0f, 170.0f);
	m_fuelRate->transform->SetSize(200.0f, 200.0f);
	m_fuelRate->SetParent(fuelFrame);
	m_fuelRate->AddComponent<CPolygon>()->SetTexture(GetRankPath(fuelRank));

	// �R������
	m_fuelValue = new GameObject();
	m_fuelValue->transform->SetPos(0.0f, 350.0f);
	m_fuelValue->SetParent(fuelText);
	m_fuelValue->AddComponent<CText>()->SetText(fuelTextPara);
	m_fuelValue->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_fuelValue->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_fuelValue->GetComponent<CText>()->SetFontSize(60);

	// �^�C�g����
	GameObject* titleButton = new GameObject();
	titleButton->AddComponent<ButtonUI>();
	titleButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\go_title.png");
	titleButton->transform->SetSize(145.0f, 568.0f);
	titleButton->transform->SetPos(CRenderer::SCREEN_WIDTH - 145.0f, CRenderer::SCREEN_HEIGHT - 568.0f);
	titleButton->GetComponent<ButtonUI>()->setClickEvent([this]()
		{
			CSceneManager::GetInstance()->SetScene("title");
		});


	// �����L���O�ɓo�^����
	RankingSystem rankSystem;
	rankSystem.RegisterData(score);
	int rank = rankSystem.GetRanking(score);

	if (rank != 0)
	{
		GameObject* rankText = new GameObject();
		rankText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 + 700.0f, 100.0f);
		rankText->AddComponent<CText>();
		rankText->GetComponent<CText>()->SetText(std::to_string(rank) + "�ʂɃ����N�C��!");
		rankText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		rankText->GetComponent<CText>()->SetAlign(CText::RIGHT);
		rankText->GetComponent<CText>()->SetFontSize(60);
		rankText->GetComponent<CText>()->SetOutlineSize(2);
		rankText->GetComponent<CText>()->SetFontColor(D3DCOLOR_RGBA(255, 0, 0, 255));
		rankText->GetComponent<CText>()->SetOutlineColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}