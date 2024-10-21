//=============================================================
//
// ユーティリティ [util.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _UTIL_H_
#define _UTIL_H_

#include "renderer.h"

namespace UTIL
{
	/*
	@brief 頂点・インデックス情報からメッシュに変換する
	@return LPD3DXMESH (メッシュ)
	@param [in] 面の数
	@param [in] 頂点数
	@param [in] インデックス数
	@param [in] 頂点情報のポインタ
	@param [in] インデックス情報のポインタ
	*/
	LPD3DXMESH ConvertToMesh(int numFace, int numVertices, int numIndices, VERTEX_3D* vertices, int* indices);
}

#endif // !_UTIL_H_
