//=============================================================
//
// �f�[�^�Ǘ� [data_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#include "data_manager.h"
#include "manager.h"
using namespace std;

//=============================================================
// [CDataManager] �R���X�g���N�^
//=============================================================
CDataManager::CDataManager()
{
	
}

//=============================================================
// [CDataManager] �f�X�g���N�^
//=============================================================
CDataManager::~CDataManager()
{

}

//=============================================================
// [CDataManager] ������
//=============================================================
void CDataManager::Init()
{

}

//=============================================================
// [CDataManager] �I��
//=============================================================
void CDataManager::Uninit()
{
	// ���ׂẴf�[�^��j������
	for (unsigned int i = 0; i < m_apData.size(); i++)
	{
		if (m_apData[i] != nullptr)
		{
			// �i�[�f�[�^��j������
			m_apData[i]->Clear();

			// �f�[�^��j������
			delete m_apData[i];
			m_apData[i] = nullptr;
		}
	}

	// �f�[�^���X�g����ɂ���
	m_apData.clear();
}

//=============================================================
// [CDataManager] �e�N�X�`���̎Q�Ɓi���݂��Ȃ��ꍇ�͍쐬�j
//=============================================================
CDataTexture* CDataManager::RefTexture(const std::string& sPath)
{
	// �f�[�^�����݂��邩����������
	for (unsigned int i = 0; i < m_apData.size(); i++)
	{
		if (m_apData[i]->GetPath() == sPath &&
			m_apData[i]->GetFormat() == CDataBase::FORMAT::TEXTURE)
		{ // ���������Ƃ�
			CDataTexture* pTextureData = (CDataTexture*)m_apData[i];	// �e�N�X�`���N���X�Ƀ_�E���L���X�g
			return pTextureData;
		}
	}

	// �f�[�^�����݂��Ȃ��ꍇ ---------------------------------------------------------------------------------------------

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �f�[�^�N���X�𐶐�
	CDataTexture* pNewData = new CDataTexture();
	pNewData->SetPath(sPath);

	// ���[�h
	if (FAILED(pNewData->Load(pNewData->GetPath())))
	{ // ���s
		delete pNewData;
		pNewData = nullptr;
		return nullptr;
	}

	m_apData.push_back(pNewData);		// �f�[�^��o�^����

	return pNewData;
}

//=============================================================
// [CDataManager] ���b�V���̎Q�Ɓi���݂��Ȃ��ꍇ�͍쐬�j
//=============================================================
CDataMesh* CDataManager::RefMesh(const std::string& sPath)
{
	// �f�[�^�����݂��邩����������
	for (unsigned int i = 0; i < m_apData.size(); i++)
	{
		if (m_apData[i]->GetPath() == sPath &&
			m_apData[i]->GetFormat() == CDataBase::FORMAT::MESH)
		{ // ���������Ƃ�
			CDataMesh* pMeshData = (CDataMesh*)m_apData[i];		// ���b�V���N���X�Ƀ_�E���L���X�g
			return pMeshData;
		}
	}

	// �f�[�^�����݂��Ȃ��ꍇ ---------------------------------------------------------------------------------------------

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �f�[�^�N���X�𐶐�
	CDataMesh* pNewData = new CDataMesh();
	pNewData->SetPath(sPath);

	// ���[�h
	if (FAILED(pNewData->Load(pNewData->GetPath())))
	{ // ���s
		delete pNewData;
		pNewData = nullptr;
		return nullptr;
	}

	m_apData.push_back(pNewData);		// �f�[�^��o�^����

	return pNewData;
}

//=============================================================
// [CDataManager] �f�[�^�̍폜
//=============================================================
void CDataManager::RemoveData(const std::string& path)
{
	// ���ׂẴf�[�^��j������
	for (auto itr = m_apData.begin(); itr != m_apData.end(); itr++)
	{
		if ((*itr)->GetPath() == path)
		{
			(*itr)->Clear();
			delete* itr;
			*itr = nullptr;
			m_apData.erase(itr);
			return;
		}
	}
}


//=============================================================
// [CDataBase] �R���X�g���N�^
//=============================================================
CDataBase::CDataBase(const FORMAT& format) : m_format(format)
{
}

//=============================================================
// [CDataBase] �t�H�[�}�b�g�̎擾
//=============================================================
CDataBase::FORMAT CDataBase::GetFormat()
{
	return m_format;	// �t�H�[�}�b�g��Ԃ�
}

//=============================================================
// [CDataBase] �p�X�̐ݒ�
//=============================================================
void CDataBase::SetPath(const std::string& sPath)
{
	m_sPath = sPath;
}

//=============================================================
// [CDataBase] �p�X�̎擾
//=============================================================
string CDataBase::GetPath()
{
	return m_sPath;
}





//=============================================================
// [CDataTexture] �R���X�g���N�^
//=============================================================
CDataTexture::CDataTexture() : CDataBase(TEXTURE)
{
	// �ϐ��̏�����
	m_texture = nullptr;
}

//=============================================================
// [CDataTexture] �N���A
//=============================================================
void CDataTexture::Clear()
{
	// �e�N�X�`���̔j��
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

//=============================================================
// [CDataTexture] ���[�h
//=============================================================
HRESULT CDataTexture::Load(const std::string& sPath)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �e�N�X�`���̍쐬
	if (FAILED(D3DXCreateTextureFromFile(pDevice, sPath.c_str(), &m_texture)))
	{ // ���s
		return E_FAIL;
	}

	// ����
	return S_OK;
}




//=============================================================
// [CDataMesh] �R���X�g���N�^
//=============================================================
CDataMesh::CDataMesh() : CDataBase(MESH)
{
	// �ϐ��̏�����
	m_pMesh = nullptr;
	m_pShadowMesh = nullptr;
	m_pBuffMat = nullptr;
	m_dwNumMat = 0;
}

//=============================================================
// [CDataMesh] �N���A
//=============================================================
void CDataMesh::Clear()
{
	// ���b�V���̔j��
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	// �e�p���b�V���̔j��
	if (m_pShadowMesh != nullptr)
	{
		m_pShadowMesh->Release();
		m_pShadowMesh = nullptr;
	}

	// �}�e���A���̔j��
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
}

//=============================================================
// [CDataMesh] ���[�h
//=============================================================
HRESULT CDataMesh::Load(const std::string& sPath)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// ���b�V���̐���
	if (FAILED(D3DXLoadMeshFromX(
		sPath.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		nullptr,
		&m_pBuffMat,
		nullptr,
		&m_dwNumMat,
		&m_pMesh
	)))
	{ // ���s
		return E_FAIL;
	}

	// �e�p���b�V���̐���
	if (CShadow::USE_SHADOW)
	{
		if (FAILED(CShadow::GenerateShadowMesh(pDevice, m_pMesh, &m_pShadowMesh)))
		{ // ���s
			return E_FAIL;
		}
	}

	// ����
	return S_OK;
}