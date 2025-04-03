//==================================================================
//
// 便利関数ライブラリ [benlib.h]
// by Kotaro Ito
//
//==================================================================
#ifndef _BENLIB_H_
#define _BENLIB_H_

#include <stdio.h>
#include "d3dx9.h"	

// 便利な機能を提供します
namespace Benlib
{
	/*
	@brief 座標1と座標2の距離を計算します
	@param[in] pos1 : 座標1
	@param[in] pos2 : 座標2
	@return 距離 (float)
	*/
	float PosDistance(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

	/*
	@brief 座標1と座標2の平面距離を計算します
	@param[in] pos1 : 座標1
	@param[in] pos2 : 座標2
	@return 平面距離 (float)
	*/
	float PosPlaneDistance(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

	/*
	@brief 方向ベクトルと長さから座標を計算します
	@param[in] start : スタート位置
	@param[in] vec : 方向ベクトル
	@param[in] fLength : 距離
	@return 座標 (D3DXVECTOR3)
	*/
	D3DXVECTOR3 VectorDistance(const D3DXVECTOR3& start, D3DXVECTOR3 vec, const float& fLength);

	/*
	@brief 座標1から座標2の角度を計算します
	@param[in] pos1 : 座標1
	@param[in] pos2 : 座標2
	@return 角度 (float)
	*/
	float PosAngle(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

	/*
	@brief 座標1から座標2の回転を計算します
	@param[in] pos1 : 座標1
	@param[in] pos2 : 座標2
	@return 回転 (D3DXVECTOR3)
	*/
	D3DXVECTOR3 PosRotation(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

	/*
	@brief 法線ベクトルを算出します
	@param[in] pos1 : 座標1
	@param[in] pos2 : 座標2
	@param[in] pos3 : 座標3
	@return 法線ベクトル (D3DXVECTOR3)
	*/
	D3DXVECTOR3 CalcNormalVector(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& pos3);

	/*
	@brief 座標1から座標2の回転を計算します
	@param[in] pos1 : 座標1
	@param[in] pos2 : 座標2
	@return 回転 (D3DXVECTOR3)
	*/
	D3DXQUATERNION LookAt(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

	/*
	@brief 視錐台にオブジェクトがあるかを判定します
	@param[in] matProj : プロジェクションマトリックス
	@param[in] matView : ビューマトリックス
	@param[in] center : オブジェクトの中心位置
	@param[in] radius : オブジェクトの半径
	@return 判定 (bool)
	*/
	bool IsObjectInFrustum(const D3DXMATRIX& matProj, const D3DXMATRIX& matView, const D3DXVECTOR3& center, const float& radius);

	/*
	@brief 整数型のランダム値を生成します
	@param[in] min : 最小値
	@param[in] max : 最大値
	@return ランダム値 (int)
	*/
	int RandomInt(const int& min, const int& max);

	/*
	@brief 浮動小数点型のランダム値を生成します
	@param[in] min : 最小値
	@param[in] max : 最大値
	@return ランダム値 (float)
	*/
	float RandomFloat(const float& min, const float& max);

	/*
	@brief 角度からラジアンに変換します
	@param[in] nDegree : 角度
	@return ラジアン (float)
	*/
	float DegreeToRadian(const int& nDegree);

	/*
	@brief ラジアンから角度に変換します
	@param[in] fRadian : ラジアン
	@return 角度 (int)
	*/
	int RadianToDegree(const float& fRadian);

	/*
	@brief クォータニオンからYXZオイラー角に変換します
	@param[in] q : クォータニオン
	@return オイラー角（YXZ）
	*/
	D3DXVECTOR3 QuaternionToEuler(D3DXQUATERNION q);

	/*
	@brief ファイルポインタを現在位置から指定数分戻します
	@param[in] pFile : ファイルポインタ
	@param[in] nBackCount : 戻る文字数
	*/
	void BackFileChar(FILE* pFile, const int& nBackCount);

	/*
	@brief パスを指定数分削除します
	@param[out] FilePath : パス (文字列)
	@param[in] nSplit : パス数
	*/
	void RemovePathSplit(char* FilePath, const int& nSplit);
	void RemovePathSplit(std::string& FilePath, const int& nSplit);

	/*
	@brief パスの区切りを指定文字に置き換える
	@param[out] FilePath : パス (文字列)
	@param[in] cSplit : 新しい区切り文字
	*/
	void ReplacePathSplit(char* FilePath, const char& cSplit);

	//@brief 範囲
	template<class T = float>
	class Range
	{
	public:
		Range() {}

		//@param vMin: 最小値
		//@param vMax: 最大値
		Range(const T& vMin, const T& vMax)
		{
			min = vMin;
			max = vMax;
		}

		T min;	// 最小値
		T max;	// 最大値

		//@brief 最小値と最大値を比べて、正しく並べ替える
		void Sort()
		{
			if (min > max)
			{
				T temp = min;
				min = max;
				max = temp;
			}
		}
	};
}

using namespace Benlib;

#endif