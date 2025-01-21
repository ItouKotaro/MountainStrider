//=============================================================
//
// �v���C�K�C�h [play_guide.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "play_guide.h"
#include "vehicle.h"

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

	if (m_guideUI != nullptr)
	{
		m_guideUI->gameObject->Destroy();
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
		m_guide->SetAlpha(m_guideUI->GetAlpha());

		// �i���x�̍X�V
		m_guideUI->SetProgress(m_guide->GetProgress());

		// ���������Ƃ�
		if (m_guide->IsCompleted())
		{
			GUIDE nextGuide = static_cast<GUIDE>(m_guideType + 1);

			if (nextGuide >= GUIDE_MAX)
			{ // �K�C�h�����ׂĊ��������Ƃ�
				m_isAllCompleted = true;

				if (m_guideUI != nullptr)
				{
					m_guideUI->gameObject->Destroy();
					m_guideUI = nullptr;
				}
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
		m_guide->Uninit();
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
		m_guideUI->SetTitle("�A�N�Z��");
		break;
	case PlayGuideManager::GUIDE_DIRECTION:
		m_guide = new DirectionGuide();
		m_guideUI->SetTitle("�����]��");
		break;
	case PlayGuideManager::GUIDE_JUMP:
		m_guide = new JumpGuide();
		m_guideUI->SetTitle("�W�����v");
		break;
	case PlayGuideManager::GUIDE_ACTION:
		m_guide = new ActionGuide();
		m_guideUI->SetTitle("�A�N�V����");
		break;
	}

	// ���������������s����
	m_guide->Init(guideUIObj);
}



//=============================================================
// [PlayGuideUI] ������
//=============================================================
void PlayGuideUI::Init()
{
	// �ϐ��̏�����
	m_progress = PROGRESS_SEGMENT;
	m_progressCounter = 30;
	m_alpha = 0.0f;

	// �Z�O�����g�̍쐬
	m_segment = new SingleComponent<CPolygon>();
	m_segment->Init();
	m_segment->SetParent(gameObject);
	m_segment->transform->SetSize(SEGMENT_WIDTH, 0.0f);
	m_segment->SetColor(D3DCOLOR_RGBA(189, 255, 253, 255));

	// �w�i�̍쐬
	m_bg = new SingleComponent<CPolygon>();
	m_bg->Init();
	m_bg->SetParent(gameObject);
	m_bg->transform->SetPos(SEGMENT_WIDTH, 0.0f);
	m_bg->transform->SetSize(0.0f, SIZE.y);
	m_bg->SetColor(D3DCOLOR_RGBA(14, 92, 89, 255));

	// �v���O���X�o�[�̍쐬
	m_progressBar = new SingleComponent<CPolygon>();
	m_progressBar->Init();
	m_progressBar->SetParent(gameObject);
	m_progressBar->transform->SetPos(SEGMENT_WIDTH, SIZE.y - PROGRESSBAR_HEIGHT);
	m_progressBar->transform->SetSize(0.0f, PROGRESSBAR_HEIGHT);
	m_progressBar->SetColor(D3DCOLOR_RGBA(228, 92, 255, 255));

	// �e�L�X�g�̍쐬
	m_text = new SingleComponent<CText>();
	m_text->Init();
	m_text->SetParent(gameObject);
	m_text->SetFont("07�S�r�S�V�b�N");
	m_text->SetFontSize(60);
	m_text->transform->SetPos(40.0f, 15.0f);
	m_text->SetAlpha(0.0f);
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

	if (m_text != nullptr)
	{
		m_text->Uninit();
		delete m_text;
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
	else if (m_progress <= PlayGuideUI::PROGRESS_TEXT)
	{
		m_alpha = m_text->GetAlpha();
		m_alpha += (1.0f - m_alpha) * 0.08f;
		m_text->SetAlpha(m_alpha);
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
				m_progressCounter = 30;
				break;
			case PlayGuideUI::PROGRESS_TEXT:
				m_progressCounter = 50;
				break;
			}
		}
	}


	// �X�V
	m_segment->Update();
	m_bg->Update();
	m_progressBar->Update();
	m_text->Update();
}

//=============================================================
// [PlayGuideUI] �`��
//=============================================================
void PlayGuideUI::DrawUI()
{
	m_segment->DrawUI();
	m_bg->DrawUI();
	m_progressBar->DrawUI();
	m_text->DrawUI();
}

//=============================================================
// [PlayGuideUI] �^�C�g���̐ݒ�
//=============================================================
void PlayGuideUI::SetTitle(const std::string& title)
{
	m_text->SetText(title);
}


//=============================================================
// [AccelGuide] ������
//=============================================================
void AccelGuide::Init(GameObject* parent)
{
	m_controlImg = new GameObject();
	m_controlImg->AddComponent<CPolygon>();
	m_controlImg->SetParent(parent);
	m_controlImg->transform->SetPos(35.0f, 90.0f);
	m_controlImg->transform->SetSize(70.0f, 70.0f);
	m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\button_a.png");
	m_controlImg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_pressText = new GameObject();
	m_pressText->SetParent(parent);
	m_pressText->AddComponent<CText>()->SetText("<color=225, 233, 245>������");
	m_pressText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_pressText->GetComponent<CText>()->SetFontSize(55);
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

	if (INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER)
	{
		m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\button_a.png");
		m_controlImg->transform->SetSize(70.0f, 70.0f);
		m_pressText->transform->SetPos(140.0f, 100.0f);
	}
	else
	{
		m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_space.png");
		m_controlImg->transform->SetSize(210.0f, 70.0f);
		m_pressText->transform->SetPos(260.0f, 100.0f);
	}
	
	// �����x
	m_controlImg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_pressText->GetComponent<CText>()->SetAlpha(m_alpha);
}


//=============================================================
// [DirectionGuide] ������
//=============================================================
void DirectionGuide::Init(GameObject* parent)
{
	m_controlL = new GameObject();
	m_controlL->AddComponent<CPolygon>();
	m_controlL->SetParent(parent);
	m_controlL->transform->SetPos(135.0f, 90.0f);
	m_controlL->transform->SetSize(70.0f, 70.0f);
	m_controlL->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_a.png");
	m_controlL->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_controlR = new GameObject();
	m_controlR->AddComponent<CPolygon>();
	m_controlR->SetParent(parent);
	m_controlR->transform->SetPos(260.0f, 90.0f);
	m_controlR->transform->SetSize(70.0f, 70.0f);
	m_controlR->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_d.png");
	m_controlR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_controlLR = new GameObject();
	m_controlLR->AddComponent<CPolygon>();
	m_controlLR->SetParent(parent);
	m_controlLR->transform->SetPos(170.0f, 70.0f);
	m_controlLR->transform->SetSize(130.0f, 110.0f);
	m_controlLR->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\lstick-lr.png");
	m_controlLR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_descText = new GameObject();
	m_descText->SetParent(parent);
	m_descText->AddComponent<CText>()->SetText("<color=225, 233, 245>��     ��");
	m_descText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_descText->GetComponent<CText>()->SetFontSize(100);
	m_descText->transform->SetPos(40.0f, 105.0f);

	// �o�C�N���擾����
	m_vehicle = GameObject::Find("Vehicle");
}

//=============================================================
// [DirectionGuide] �X�V
//=============================================================
void DirectionGuide::Update()
{
	// �R���g���[���[�̏����擾����
	auto pGamepadDev = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>();
	short stickLX = pGamepadDev->GetState().Gamepad.sThumbLX;

	if (!m_vehicle->GetComponent<CVehicle>()->GetFlying() && (fabsf(stickLX) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		INPUT_INSTANCE->onInput("left") || INPUT_INSTANCE->onInput("right")))
	{
		m_progress += 0.004f;
	}

	if (INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER)
	{
		m_controlL->SetVisible(false);
		m_controlR->SetVisible(false);
		m_controlLR->SetVisible(true);
	}
	else
	{
		m_controlL->SetVisible(true);
		m_controlR->SetVisible(true);
		m_controlLR->SetVisible(false);
	}

	// �����x
	m_controlL->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_controlR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_controlLR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_descText->GetComponent<CText>()->SetAlpha(m_alpha);
}


//=============================================================
// [JumpGuide] ������
//=============================================================
void JumpGuide::Init(GameObject* parent)
{
	m_controlImg = new GameObject();
	m_controlImg->AddComponent<CPolygon>();
	m_controlImg->SetParent(parent);
	m_controlImg->transform->SetPos(35.0f, 90.0f);
	m_controlImg->transform->SetSize(70.0f, 70.0f);
	m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\button_x.png");
	m_controlImg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	// �o�C�N���擾����
	m_vehicle = GameObject::Find("Vehicle");
}

//=============================================================
// [JumpGuide] �X�V
//=============================================================
void JumpGuide::Update()
{
	if (m_vehicle->GetComponent<CVehicle>()->GetGroundDistance() <= CVehicle::GROUND_DISTANCE && INPUT_INSTANCE->onInput("jump"))
	{
		m_progress += 0.25f;
	}

	if (INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER)
	{
		m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\button_x.png");
		m_controlImg->transform->SetSize(70.0f, 70.0f);
	}
	else
	{
		m_controlImg->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_shift.png");
		m_controlImg->transform->SetSize(225.0f, 80.0f);
	}

	// �����x
	m_controlImg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
}


//=============================================================
// [ActionGuide] ������
//=============================================================
void ActionGuide::Init(GameObject* parent)
{
	m_controlL = new GameObject();
	m_controlL->AddComponent<CPolygon>();
	m_controlL->SetParent(parent);
	m_controlL->transform->SetPos(290.0f, 95.0f);
	m_controlL->transform->SetSize(60.0f, 60.0f);
	m_controlL->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_a.png");
	m_controlL->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_controlR = new GameObject();
	m_controlR->AddComponent<CPolygon>();
	m_controlR->SetParent(parent);
	m_controlR->transform->SetPos(365.0f, 95.0f);
	m_controlR->transform->SetSize(60.0f, 60.0f);
	m_controlR->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\KEYBOARD\\key_d.png");
	m_controlR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_controlLR = new GameObject();
	m_controlLR->AddComponent<CPolygon>();
	m_controlLR->SetParent(parent);
	m_controlLR->transform->SetPos(305.0f, 80.0f);
	m_controlLR->transform->SetSize(110.0f, 90.0f);
	m_controlLR->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\UI\\CONTROLLER\\lstick-lr.png");
	m_controlLR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));

	m_descText = new GameObject();
	m_descText->SetParent(parent);
	m_descText->AddComponent<CText>()->SetText("<color=225, 233, 245>�󒆂ŉ�] ��       ��");
	m_descText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_descText->GetComponent<CText>()->SetFontSize(45);
	m_descText->transform->SetPos(40.0f, 105.0f);

	// �o�C�N���擾����
	m_vehicle = GameObject::Find("Vehicle");
}

//=============================================================
// [ActionGuide] �X�V
//=============================================================
void ActionGuide::Update()
{
	// �R���g���[���[�̏����擾����
	auto pGamepadDev = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>();
	short stickLX = pGamepadDev->GetState().Gamepad.sThumbLX;

	if (m_vehicle->GetComponent<CVehicle>()->GetFlying() && (fabsf(stickLX) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		INPUT_INSTANCE->onInput("left") || INPUT_INSTANCE->onInput("right")))
	{
		m_progress += 0.004f;
	}

	if (INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER)
	{
		m_controlL->SetVisible(false);
		m_controlR->SetVisible(false);
		m_controlLR->SetVisible(true);
	}
	else
	{
		m_controlL->SetVisible(true);
		m_controlR->SetVisible(true);
		m_controlLR->SetVisible(false);
	}

	// �����x
	m_controlL->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_controlR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_controlLR->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
	m_descText->GetComponent<CText>()->SetAlpha(m_alpha);
}