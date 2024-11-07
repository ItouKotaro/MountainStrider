//=============================================================
//
// 自然の産物 [nature_produces.h]
// Author: Ito Kotaro
//
// 【確率の仕組み】
// 生成候補の確率をすべて加算し、ランダムで選ぶ
// 
//=============================================================
#ifndef _NATURE_PRODUCES_H_
#define _NATURE_PRODUCES_H_

#include "component.h"

// 隣接確率
struct AdjacentRate
{
	std::string className;	// クラス名
	float rate;						// 確率
};

// ゲームオブジェクト追跡用コンポーネント
class CProduceTag : public Component
{
public:
	CProduceTag(const std::string& className) { m_className = className; }
	std::string GetName() { return m_className; }
private:
	std::string m_className;
};

// 自然の産物（基底）
class CNatureProduces
{
public:
	CNatureProduces(const std::string& path, const D3DXVECTOR2& size, const float& offsetY)
	{
		m_path = path;
		m_size = size;
		m_offsetY = offsetY;
	}

	// オブジェクトの生成
	void Generate(const Transform& transform);

	// サイズの取得
	D3DXVECTOR2 GetSize() { return m_size; }
	// オフセットYの取得
	float GetOffsetY() { return m_offsetY; }

	// 確率を取得する
	unsigned int GetChance() { return m_chance; }
	// 確率を設定する
	void SetChance(const int& chance) { m_chance = chance; }
	// 確率倍率を取得する
	float GetAdjacentRate(const D3DXVECTOR3& pos);
	// 隣接倍率の設定
	template<class T> void SetAdjacentRate(const float& rate)
	{
		// 既にクラスが登録されていないか
		for (unsigned int i = 0; i < m_adjacentRates.size(); i++)
		{
			if (m_adjacentRates[i].className == typeid(T).name())
			{ // 一致するとき
				m_adjacentRates[i].rate = rate;	// 確率を書き換える
				return;
			}
		}

		// 新しく登録する
		AdjacentRate adjacent;
		adjacent.className = typeid(T).name();
		adjacent.rate = rate;
		m_adjacentRates.push_back(adjacent);
	}

protected:
	std::string m_path;		// 設置プレハブのパス
	D3DXVECTOR2 m_size;	// サイズ（XZ）
	float m_offsetY;				// Yのオフセット

	// 確率
	unsigned int m_chance;										// 標準確率（整数）
	std::vector<AdjacentRate> m_adjacentRates;		// 隣接確率リスト
};


// --------------------------------- 障害物 ---------------------------------

class CProdTree : public CNatureProduces
{
public:
	CProdTree() : CNatureProduces("data\\PREFAB\\bench.pref", {10.0f, 10.0f}, 5.0f) {}
private:

};

#endif // !_NATURE_PRODUCES_H_
