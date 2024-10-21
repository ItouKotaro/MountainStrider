//=============================================================
//
// ���[�e�B���e�B [util.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _UTIL_H_
#define _UTIL_H_

#include "renderer.h"

namespace UTIL
{
	/*
	@brief ���_�E�C���f�b�N�X��񂩂烁�b�V���ɕϊ�����
	@return LPD3DXMESH (���b�V��)
	@param [in] �ʂ̐�
	@param [in] ���_��
	@param [in] �C���f�b�N�X��
	@param [in] ���_���̃|�C���^
	@param [in] �C���f�b�N�X���̃|�C���^
	*/
	LPD3DXMESH ConvertToMesh(int numFace, int numVertices, int numIndices, VERTEX_3D* vertices, int* indices);
}

#endif // !_UTIL_H_
