//=============================================================
//
// �V���b�v [shop.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "shop.h"

#include "renderer.h"
#include "component/2d/text.h"

//=============================================================
// [ShopManager] ������
//=============================================================
void ShopManager::Init()
{
	m_text = new GameObject();
	m_text->AddComponent<CText>()->SetText("�V���b�v");

	m_ptView = new PointView();
	m_ptView->Init();
}

//=============================================================
// [ShopManager] �I��
//=============================================================
void ShopManager::Uninit()
{
	m_ptView->Uninit();
	delete m_ptView;
}

//=============================================================
// [ShopManager] �X�V
//=============================================================
void ShopManager::Update()
{
	m_ptView->Update();
}

//=============================================================
// [ShopManager] �`��
//=============================================================
void ShopManager::Draw()
{
	m_ptView->DrawUI();
}


//=============================================================
// [PointView] ������
//=============================================================
void PointView::Init()
{
	// �|�C���g�t���[���̏�����
	m_pointFrame = new CPolygon();
	m_pointFrame->transform = new Transform();
	m_pointFrame->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);
	m_pointFrame->Init();
	m_pointFrame->SetTexture("data\\TEXTURE\\SHOP\\shop.png");
	

}

//=============================================================
// [PointView] �I��
//=============================================================
void PointView::Uninit()
{
	// �|�C���g�t���[���̏I��
	if (m_pointFrame != nullptr)
	{
		m_pointFrame->Uninit();
		delete m_pointFrame->transform;
		delete m_pointFrame;
		m_pointFrame = nullptr;
	}
}

//=============================================================
// [PointView] �X�V
//=============================================================
void PointView::Update()
{
	// �|�C���g�t���[���̍X�V
	m_pointFrame->Update();
}

//=============================================================
// [PointView] �`��
//=============================================================
void PointView::DrawUI()
{
	// �|�C���g�t���[���̕`��
	m_pointFrame->DrawUI();
}