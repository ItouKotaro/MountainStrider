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

#include "scripts/vehicle.h"
#include "scripts/result/result_data.h"

float MountainResultManager::m_oldFuel = CVehicle::MAX_FUEL;
float MountainResultManager::m_oldEndurance = CVehicle::MAX_ENDURANCE;
UINT MountainResultManager::m_goalCount = 0;

//=============================================================
// [MountainResultManager] ������
//=============================================================
void MountainResultManager::Init()
{
	// ���j�����C���N�������g
	m_goalCount++;

	// �i��
	m_progState = P_MTTEXT;
	m_progCounter = 120;

	// �N���A�e�L�X�g
	{
		m_mtText = new GameObject("MtClearText", "UI");
		m_mtText->AddComponent<CText>();
		m_mtText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		m_mtText->GetComponent<CText>()->SetFontSize(130);
		m_mtText->GetComponent<CText>()->SetOutlineColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		m_mtText->GetComponent<CText>()->SetOutlineSize(5);
		m_mtText->GetComponent<CText>()->SetText(std::to_string(m_goalCount) + "�ڂ̎R�𓥔j���܂���");
		m_mtText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_mtText->transform->SetPos(-2000.0f, 100.0f, 0.0f);
	}

	// �f�[�^�\��
	{
		m_dataView = new GameObject("DataView", "UI");
		m_dataView->AddComponent<ResultDataView>();
		m_dataView->transform->Translate(60.0f, 350.0f, 0.0f);
	}

	{
		// �n�`�摜
		m_terrainImg = new GameObject("TerrainImg", "ResultData");
		m_terrainImg->AddComponent<CPolygon>();
		m_terrainImg->transform->SetSize(500.0f, 500.0f);
		m_terrainImg->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 250.0f, 300.0f);
	}

	// �V�[�h�e�L�X�g
	{
		m_seedText = new GameObject("SeedText", "UI");
		m_seedText->AddComponent<CText>();
		m_seedText->GetComponent<CText>()->SetFontSize(50);
		m_seedText->GetComponent<CText>()->SetText("<color=150,150,150>Seed: " + std::to_string(m_gameScene->GetTerrain()->GetSeed()));
		m_seedText->transform->SetPos(5.0f, CRenderer::SCREEN_HEIGHT - 50.0f);
	}

	// �R���o�[
	{
		m_fuelBar = new GameObject("FuelBar", "UI");
		m_fuelBar->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 500.0f);
		m_fuelBar->AddComponent<CAdvancedBar>();
		m_fuelBar->GetComponent<CAdvancedBar>()->SetAlign(CAdvancedBar::CENTER);
		m_fuelBar->GetComponent<CAdvancedBar>()->SetLength(800.0f);
		m_fuelBar->GetComponent<CAdvancedBar>()->SetBold(50.0f);
		m_fuelBar->GetComponent<CAdvancedBar>()->SetColor(0, D3DCOLOR_RGBA(252, 185, 40, 255));
		m_fuelBar->GetComponent<CAdvancedBar>()->SetColor(2, D3DCOLOR_RGBA(252, 185, 40, 255));
		m_fuelBar->GetComponent<CAdvancedBar>()->SetColor(1, D3DCOLOR_RGBA(255, 0, 0, 255));
		m_fuelBar->GetComponent<CAdvancedBar>()->SetColor(3, D3DCOLOR_RGBA(255, 0, 0, 255));
	}

	// �ϋv�l�o�[
	{
		m_enduranceBar = new GameObject("FuelBar", "UI");
		m_enduranceBar->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 700.0f);
		m_enduranceBar->AddComponent<CAdvancedBar>();
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetAlign(CAdvancedBar::CENTER);
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetLength(800.0f);
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetBold(50.0f);
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetColor(0, D3DCOLOR_RGBA(7, 232, 104, 255));
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetColor(2, D3DCOLOR_RGBA(7, 232, 104, 255));
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetColor(1, D3DCOLOR_RGBA(10, 201, 163, 255));
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetColor(3, D3DCOLOR_RGBA(10, 201, 163, 255));
	}

	// �w�i
	{
		m_bg = new GameObject("BG", "UI");
		m_bg->SetPriority(3);
		m_bg->AddComponent<CPolygon>();
		m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_bg->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));
	}
}

//=============================================================
// [MountainResultManager] �I��
//=============================================================
void MountainResultManager::Uninit()
{
	m_mtText->Destroy();
	m_bg->Destroy();

}

//=============================================================
// [MountainResultManager] �X�V
//=============================================================
void MountainResultManager::Update()
{
	// �w�i�̃t�F�[�h
	float currentAlpha = m_bg->GetComponent<CPolygon>()->GetColor().a;
	m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, currentAlpha + (0.2f - currentAlpha) * 0.02f));

	// �N���A�e�L�X�g�̃A�j���[�V����
	if (m_progState <= MountainResultManager::P_MTTEXT)
	{
		m_mtText->transform->SetPos(m_mtText->transform->GetWPos().x +
			(CRenderer::SCREEN_WIDTH / 2 - m_mtText->transform->GetWPos().x) * 0.08f, 100.0f
		);
	}

	// 


	// ���̏�ԂɈڍs���鏈��
	m_progCounter--;
	if (m_progCounter <= 0 && m_progState != P_END)
	{
		m_progState = static_cast<PROG_STATE>(m_progState + 1);	
		m_progCounter = 60;
	}
}