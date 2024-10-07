//==================================================================
//
// 便利関数ライブラリ [benlib.cpp]
// by Kotaro Ito
//
//==================================================================
#include "benlib.h"

float Benlib::PosDistance(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2)
{
	float xz = fabsf(pos1.x - pos2.x) * fabsf(pos1.x - pos2.x) + fabsf(pos1.z - pos2.z) * fabsf(pos1.z - pos2.z);
	xz = sqrtf(xz);
	float y = xz * xz + fabsf(pos1.y - pos2.y) * fabsf(pos1.y - pos2.y);
	float result = sqrtf(y);
	return result;
}

float Benlib::PosPlaneDistance(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2)
{
	float xz = fabsf(pos1.x - pos2.x) * fabsf(pos1.x - pos2.x) + fabsf(pos1.z - pos2.z) * fabsf(pos1.z - pos2.z);
	xz = sqrtf(xz);
	return xz;
}

D3DXVECTOR3 Benlib::VectorDistance(const D3DXVECTOR3& start, D3DXVECTOR3 vec, const float& fLength)
{
	D3DXVECTOR3 resultPos;
	D3DXVec3Normalize(&vec, &vec);
	resultPos.x = start.x + (fLength * vec.x) / (float)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	resultPos.y = start.y + (fLength * vec.y) / (float)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	resultPos.z = start.z + (fLength * vec.z) / (float)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return resultPos;
}

float Benlib::PosAngle(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2)
{
	return atan2f(pos2.x - pos1.x, pos2.z - pos1.z);
}

D3DXVECTOR3 Benlib::PosRotation(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2)
{
	float fTargetAngle = atan2f(pos2.x - pos1.x, pos2.z - pos1.z);
	float fTargetHeightAngle = atan2f(sqrtf(fabsf(pos2.x - pos1.x) * fabsf(pos2.x - pos1.x) +
		fabsf(pos2.z - pos1.z) * fabsf(pos2.z - pos1.z)), (pos2.y - pos1.y));
	return D3DXVECTOR3(sinf(fTargetAngle), cosf(fTargetHeightAngle), cosf(fTargetAngle));
}

float Benlib::DegreeToRadian(const int& nDegree)
{
	return nDegree * D3DX_PI / (float)180.0f;
}

int Benlib::RadianToDegree(const float& fRadian)
{
	return static_cast<int>(fRadian * 180.0f / D3DX_PI);
}

D3DXVECTOR3 Benlib::QuaternionToEuler(D3DXQUATERNION q)
{
	auto sx = -(2 * q.y * q.z - 2 * q.x * q.w);
	auto unlocked = std::abs(sx) < 0.99999f;
	return D3DXVECTOR3(
		std::asin(sx),
		unlocked ? std::atan2(2 * q.x * q.z + 2 * q.y * q.w, 2 * q.w * q.w + 2 * q.z * q.z - 1)
		: std::atan2(-(2 * q.x * q.z - 2 * q.y * q.w), 2 * q.w * q.w + 2 * q.x * q.x - 1),
		unlocked ? std::atan2(2 * q.x * q.y + 2 * q.z * q.w, 2 * q.w * q.w + 2 * q.y * q.y - 1) : 0
	);
}

void Benlib::BackFileChar(FILE* pFile, const int& nBackCount)
{
	fpos_t filePos;
	fgetpos(pFile, &filePos);
	filePos -= nBackCount;
	fsetpos(pFile, &filePos);
}

void Benlib::RemovePathSplit(char* FilePath, const int& nSplit)
{
	int nLength = strlen(&FilePath[0]);
	int nPathSplit = 0;												 // パスの区切りカウント
	int nLimitPathDel = nSplit;										 // パス削除のリミット

	for (int nCntLength = 0; nCntLength < nLength; nCntLength++)
	{ // 文字数回繰り返す
		if (FilePath[nLength - nCntLength - 1] == '\\')
		{ // 区切り文字の時
			nPathSplit++;
		}

		FilePath[nLength - nCntLength - 1] = '\0';	//文字を消す

		// 消す上限に達したとき
		if (nPathSplit >= nLimitPathDel)
		{
			break;
		}
	}
}

void Benlib::RemovePathSplit(std::string& FilePath, const int& nSplit)
{
	int nLength = (int)FilePath.length();
	int nPathSplit = 0;												 // パスの区切りカウント
	int nLimitPathDel = nSplit;										 // パス削除のリミット

	for (int nCntLength = 0; nCntLength < nLength; nCntLength++)
	{ // 文字数回繰り返す
		if (FilePath[nLength - nCntLength - 1] == '\\')
		{ // 区切り文字の時
			nPathSplit++;
		}

		FilePath.erase(nLength - nCntLength - 1);	//文字を消す

		// 消す上限に達したとき
		if (nPathSplit >= nLimitPathDel)
		{
			break;
		}
	}
}

void Benlib::ReplacePathSplit(char* FilePath, const char& cSplit)
{
	for (int nCntLength = 0; nCntLength < (int)strlen(&FilePath[0]); nCntLength++)
	{ // 文字数回繰り返す

		if (FilePath[nCntLength] == '\\')
		{
			FilePath[nCntLength] = cSplit;
		}
	}
}