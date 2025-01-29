//=============================================================
//
// ���C������ [main.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "main.h"
#include "manager.h"
#include <windowsx.h>
#include <thread>

// �������[���[�N���o
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define FIND_MEM_LEAKS
#endif // _DEBUG

#ifdef FIND_MEM_LEAKS
#define _NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define _NEW new
#endif // FIND_MEM_LEAKS

// �O���[�o���ϐ�
bool g_isActiveWindow;
bool g_isEnded = false;
bool g_showCursor = true;
bool g_beforeShowCursor = true;

//=============================================================
// ���C���֐�
//=============================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	// �������[���[�N���m
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1424590);
#endif

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						// �������T�C�Y
		CS_CLASSDC,									// �E�B���h�E�̃X�^�C��
		WindowProc,										// �E�B���h�E�̃v���V�[�W��
		0,
		0,
		hInstance,											// �C���X�^���X�n���h��
		LoadIcon(nullptr, IDI_APPLICATION),	// �^�X�N�o�[�̃A�C�R��
		LoadCursor(nullptr,IDC_ARROW),			// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),		// �N���C�A���g�̈�̔w�i�F
		nullptr,												// ���j���[�o�[
		CLASS_NAME,									// �E�B���h�E�N���X�̖��O
		LoadIcon(nullptr, IDI_APPLICATION)		// �t�@�C���̃A�C�R��
	};

	HWND hWnd;	// �E�B���h�E�n���h���i���ʎq�j
	MSG msg;			// ���b�Z�[�W���i�[����ϐ�

	// �E�B���h�E�̍��W���i�[
	RECT rect = { 0, 0, CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT };

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈���w��T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�B���h�E�𐶐�
	hWnd = CreateWindowEx(
		0,														// �g���E�B���h�E�X�^�C��
		CLASS_NAME,									// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,								// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,				// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,								// �E�B���h�E�̍����X���W
		CW_USEDEFAULT,								// �E�B���h�E�̍����Y���W
		(rect.right - rect.left),							// �E�B���h�E�̕�
		(rect.bottom - rect.top),						// �E�B���h�E�̍���
		nullptr,												// �e�E�B���h�E�̃n���h��
		nullptr,												// ���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,											// �C���X�^���X�o���h��
		nullptr												// �E�B���h�E�쐬�f�[�^
	);

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);		// �E�B���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);						// �N���C�A���g�̈���X�V

	// �}�l�[�W���[�̐����E������
	CManager::GetInstance()->Init(hInstance, hWnd, TRUE);

	// Main�̐���
	Main* mainApp = new Main();

	// �X���b�h�J�n
	mainApp->ThreadStart();

	while (!g_isEnded)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{ // Windows�̏���
			if (msg.message == WM_QUIT)
			{ // WM_QUIT���b�Z�[�W
				g_isEnded = true;
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// �A�N�e�B�u���̍X�V
		g_isActiveWindow = GetActiveWindow() != NULL;

		// �J�[�\���̕\���ݒ�̍X�V
		if (g_showCursor != g_beforeShowCursor)
		{
			ShowCursor(g_showCursor ? TRUE : FALSE);
			g_beforeShowCursor = g_showCursor;
		}
	}

	// ���C�����[�v�̃X���b�h���I���܂ő҂�
	mainApp->ThreadJoin();

	// �}�l�[�W���[�̏I���E���
	CManager::GetInstance()->Uninit();

	// Main�̔j��
	if (mainApp != nullptr)
	{
		delete mainApp;
		mainApp = nullptr;
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================
// �E�B���h�E�v���V�[�W��
//=============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
	auto hdc = GetDC(hWnd);

	switch (uMsg)
	{
	case WM_MOUSEWHEEL:
		// �}�E�X�z�C�[���̈ړ��ʂ��擾
		CManager::GetInstance()->SetMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		return 0;

	case WM_DESTROY:
		//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); //����̏�����Ԃ�
}

//=============================================================
// [Main] �R���X�g���N�^
//=============================================================
Main::Main()
{
	// ����\��ݒ�
	timeBeginPeriod(1);
	m_dwCurrentTime = 0;							// ����������
	m_dwExecLastTime = timeGetTime();		// ���ݎ������擾

	// FPS�v���̏�����
	m_dwFrameCount = 0;
	m_dwFPSLastTime = timeGetTime();
}

//=============================================================
// [Main] �f�X�g���N�^
//=============================================================
Main::~Main()
{
	// ����\��߂�
	timeEndPeriod(1);

	// �X���b�h��j������
	if (m_thread != nullptr)
	{
		delete m_thread;
		m_thread = nullptr;
	}
}

//=============================================================
// [Main] �X���b�h�̊J�n
//=============================================================
void Main::ThreadStart()
{
	// �X���b�h���쐬����
	m_thread = new std::thread(&Main::MainLoop, this);
}

//=============================================================
// [Main] �X���b�h���I������܂ő҂�
//=============================================================
void Main::ThreadJoin()
{
	if (m_thread != nullptr)
	{
		m_thread->join();
	}
}

//=============================================================
// [Main] ���C�����[�v
//=============================================================
void Main::MainLoop()
{
	while (!g_isEnded)
	{
		m_dwCurrentTime = timeGetTime();		// ���ݎ������擾

		if ((m_dwCurrentTime - m_dwFPSLastTime) >= 500)
		{ // 0.5�b�o�ߖ�
			// FPS���v��
			CManager::GetInstance()->SetFPS((m_dwFrameCount * 1000) / (m_dwCurrentTime - m_dwFPSLastTime));
			m_dwFPSLastTime = m_dwCurrentTime;							// �v�������������L�^
			m_dwFrameCount = 0;												// �t���[���J�E���g���N���A
		}

		if ((m_dwCurrentTime - m_dwExecLastTime) >= (1000 / FIXED_FPS))
		{ // 60����1�b�o��

			// �f���^�^�C����ݒ肷��
			CManager::GetInstance()->SetDeltaTime((m_dwCurrentTime - m_dwExecLastTime) * 0.001f);

			//�����J�n����
			m_dwExecLastTime = m_dwCurrentTime;

			// �X�V����
			CManager::GetInstance()->Update();

			// �`�揈��
			CManager::GetInstance()->Draw();

			// �V�[���̕ύX����
			CSceneManager::GetInstance()->ChangingScene();

			// ���S�t���O�̂����I�u�W�F�N�g��j������
			GameObject::DestroyDeathFlag();

			// �t���[���J�E���g�����Z
			m_dwFrameCount++;
		}
	}
}

//=============================================================
// [Main] �A�N�e�B�u�E�B���h�E��
//=============================================================
bool Main::IsActiveWindow()
{
	return g_isActiveWindow;
}

//=============================================================
// [Main] �A�v���P�[�V�����I������
//=============================================================
void Main::ExitApplication()
{
	g_isEnded = true;
}

//=============================================================
// [Main] �J�[�\���̕\��
//=============================================================
void Main::SetShowCursor(const bool& show)
{
	g_showCursor = show;
}
