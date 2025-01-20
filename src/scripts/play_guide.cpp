//=============================================================
//
// �v���C�K�C�h [play_guide.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "play_guide.h"

//=============================================================
// [PlayGuideManager] ������
//=============================================================
void PlayGuideManager::Init()
{
	SetGuide(GUIDE_ACCEL);
}

//=============================================================
// [PlayGuideManager] �I��
//=============================================================
void PlayGuideManager::Uninit()
{
	if (m_guide != nullptr)
	{
		delete m_guide;
		m_guide = nullptr;
	}
}

//=============================================================
// [PlayGuideManager] �X�V
//=============================================================
void PlayGuideManager::Update()
{
	if (m_isAllCompleted) return;

	if (m_guide != nullptr)
	{
		m_guide->Update();

		// �i���x�̍X�V
		m_guideUI->SetProgress(m_guide->GetProgress());

		// ���������Ƃ�
		if (m_guide->IsCompleted())
		{
			GUIDE nextGuide = static_cast<GUIDE>(m_guideType + 1);

			if (nextGuide >= GUIDE_MAX)
			{ // �K�C�h�����ׂĊ��������Ƃ�
				m_isAllCompleted = true;
			}
			else
			{ // ���̃K�C�h��ݒ肷��
				SetGuide(nextGuide);
			}
		}
	}
	
}

//=============================================================
// [PlayGuideManager] �K�C�h�̕ύX
//=============================================================
void PlayGuideManager::SetGuide(const GUIDE& guide)
{
	// �V�����K�C�h��ݒ肷��
	m_guideType = guide;

	// �O��̃K�C�h��j������
	if (m_guide != nullptr)
	{
		delete m_guide;
		m_guide = nullptr;
	}
	if (m_guideUI != nullptr)
	{
		m_guideUI->gameObject->Destroy();
		m_guideUI = nullptr;
	}

	// �V�����K�C�hUI�𐶐�����
	GameObject* guideUIObj = new GameObject();
	m_guideUI = guideUIObj->AddComponent<PlayGuideUI>();
	guideUIObj->transform->SetPos(50.0f, 200.0f);

	// �K�C�h�𐶐�����
	switch (m_guideType)
	{
	case PlayGuideManager::GUIDE_ACCEL:
		m_guide = new AccelGuide();
		break;
	case PlayGuideManager::GUIDE_DIRECTION:
		m_guide = new AccelGuide();
		break;
	case PlayGuideManager::GUIDE_JUMP:
		m_guide = new AccelGuide();
		break;
	case PlayGuideManager::GUIDE_ACTION:
		m_guide = new AccelGuide();
		break;
	}

	// ���������������s����
	m_guide->Init();
}


//=============================================================
// [AccelGuide] �X�V
//=============================================================
void AccelGuide::Update()
{
	if (INPUT_INSTANCE->onInput("accel"))
	{
		m_progress += 0.005f;
	}
}





//=============================================================
// [PlayGuideUI] ������
//=============================================================
void PlayGuideUI::Init()
{
	// �ϐ��̏�����
	m_progress = PROGRESS_SEGMENT;
	m_progressCounter = 30;

	// �Z�O�����g�̍쐬
	m_segment = new SingleComponent<CPolygon>();
	m_segment->Init();
	m_segment->SetParent(gameObject);
	m_segment->transform->SetSize(SEGMENT_WIDTH, 0.0f);
	m_segment->SetColor(D3DCOLOR_RGBA(106, 187, 252, 255));

	// �w�i�̍쐬
	m_bg = new SingleComponent<CPolygon>();
	m_bg->Init();
	m_bg->SetParent(gameObject);
	m_bg->transform->SetPos(SEGMENT_WIDTH, 0.0f);
	m_bg->transform->SetSize(0.0f, SIZE.y);

	// �v���O���X�o�[�̍쐬
	m_progressBar = new SingleComponent<CPolygon>();
	m_progressBar->Init();
	m_progressBar->SetParent(gameObject);
	m_progressBar->transform->SetPos(SEGMENT_WIDTH, SIZE.y - PROGRESSBAR_HEIGHT);
	m_progressBar->transform->SetSize(0.0f, PROGRESSBAR_HEIGHT);
	m_progressBar->SetColor(D3DCOLOR_RGBA(255, 111, 33, 255));
}

//=============================================================
// [PlayGuideUI] �I��
//=============================================================
void PlayGuideUI::Uninit()
{
	if (m_segment != nullptr)
	{
		m_segment->Uninit();
		delete m_segment;
	}

	if (m_bg != nullptr)
	{
		m_bg->Uninit();
		delete m_bg;
	}

	if (m_progressBar != nullptr)
	{
		m_progressBar->Uninit();
		delete m_progressBar;
	}
}

//=============================================================
// [PlayGuideUI] �X�V
//=============================================================
void PlayGuideUI::Update()
{
	// �X�V����
	if (m_progress <= PlayGuideUI::PROGRESS_SEGMENT)
	{
		float height = m_segment->transform->GetSize().y;
		height += (SIZE.y - height) * 0.08f;
		m_segment->transform->SetSize(SEGMENT_WIDTH, height);
	}
	else if (m_progress <= PlayGuideUI::PROGRESS_BG)
	{
		float width = m_bg->transform->GetSize().x;
		width += ((SIZE.x - SEGMENT_WIDTH) - width) * 0.08f;
		m_bg->transform->SetSize(width, m_segment->transform->GetSize().y);
	}

	// �v���O���X�o�[�̍X�V
	float progressWidth = m_progressBar->transform->GetSize().x;
	progressWidth += (m_bg->transform->GetSize().x * m_progressValue - m_progressBar->transform->GetSize().x) * 0.08f;
	m_progressBar->transform->SetPos(SEGMENT_WIDTH, m_segment->transform->GetSize().y - PROGRESSBAR_HEIGHT);
	m_progressBar->transform->SetSize(progressWidth, PROGRESSBAR_HEIGHT);

	// �v���O���X�J�E���g
	if (m_progress != PlayGuideUI::PROGRESS_END)
	{
		// �J�E���^�[���f�N�������g
		m_progressCounter--;

		// �J�E���g���I�������Ƃ�
		if (m_progressCounter <= 0)
		{
			// ���̐i��
			m_progress = static_cast<PROGRESS>(m_progress + 1);

			// �J�E���g��ݒ肷��
			switch (m_progress)
			{
			case PlayGuideUI::PROGRESS_BG:
				m_progressCounter = 50;
				break;
			}
		}
	}


	// �X�V
	m_segment->Update();
	m_bg->Update();
	m_progressBar->Update();
}

//=============================================================
// [PlayGuideUI] �`��
//=============================================================
void PlayGuideUI::DrawUI()
{
	m_segment->DrawUI();
	m_bg->DrawUI();
	m_progressBar->DrawUI();
}