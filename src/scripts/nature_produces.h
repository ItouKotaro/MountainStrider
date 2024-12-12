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
	std::string produceName;			// 産物名
	float rate;						// 確率
};

// 生成物（基底）
class CNatureProduces
{
public:
	CNatureProduces(const std::string& name, const std::string& path, const float& size, const float& damage = 0.0f, const float& offsetY = 0.0f, const float& angleRange = 0.0f, const bool& isMatchInclination = true, const float& adjacentDistance = 100.0f)
	{
		m_produceName = name;
		m_path = path;
		m_size = size;
		m_damage = damage;
		m_offsetY = offsetY;
		m_adjacentDistance = adjacentDistance;
		m_angleRange = angleRange;
		m_isMatchInclination = isMatchInclination;
	}

	// バイクとヒットしたときのイベント
	virtual void onHit(GameObject* gameObject) {}


	// オブジェクトの生成
	GameObject* Generate(const Transform& transform);

	// サイズの取得
	float GetSize() { return m_size; }
	// オフセットYの取得
	float GetOffsetY() { return m_offsetY; }
	// 産物名の取得
	std::string GetProduceName() { return m_produceName; }
	// ダメージ量の取得
	float GetDamage() { return m_damage; }

	// 確率を取得する
	unsigned int GetChance() { return m_chance; }
	// 確率を設定する
	void SetChance(const int& chance) { m_chance = chance; }
	// 確率倍率を取得する
	float GetAdjacentRate(const D3DXVECTOR3& pos);
	// 隣接距離の設定
	void SetAdjacentDistance(const float& distance) { m_adjacentDistance = distance; }
	// 生成物ごとの隣接倍率の設定
	void SetAdjacentRate(const std::string& produceName, const float& rate)
	{
		// 既にクラスが登録されていないか
		for (unsigned int i = 0; i < m_adjacentRates.size(); i++)
		{
			if (m_adjacentRates[i].produceName == produceName)
			{ // 一致するとき
				m_adjacentRates[i].rate = rate;	// 確率を書き換える
				return;
			}
		}

		// 新しく登録する
		AdjacentRate adjacent;
		adjacent.produceName = produceName;
		adjacent.rate = rate;
		m_adjacentRates.push_back(adjacent);
	}
	// 生成物のクラスの倍率を取得する
	float GetAdjacentObjectRate(const std::string& produceName)
	{
		for (unsigned int i = 0; i < m_adjacentRates.size(); i++)
		{
			if (m_adjacentRates[i].produceName == produceName)
			{ // 一致するとき
				return m_adjacentRates[i].rate;
			}
		}
		// 見つからなかったとき
		return 1.0f;
	}

	// ランダム角度
	float GetAngleRange() { return m_angleRange; }

	// 傾斜角に合わせるか
	bool IsMatchInclination() { return m_isMatchInclination; }

protected:
	std::string m_path;				// 設置プレハブのパス
	float m_size;							// サイズ
	float m_offsetY;						// Yのオフセット
	std::string m_produceName;	// 産物名
	float m_damage;					// ダメージ量（ヒット時）
	float m_angleRange;				// ランダム角度
	bool m_isMatchInclination;		// 傾斜角を考慮するか

	// 確率
	unsigned int m_chance;										// 標準確率（整数）
	std::vector<AdjacentRate> m_adjacentRates;		// 隣接確率リスト
	float m_adjacentDistance;									// 隣接距離
};

// --------------------------------- 障害物 ---------------------------------

// 木
class CProdTree00 : public CNatureProduces
{
public:
	CProdTree00() : CNatureProduces("tree00", "data\\PREFAB\\tree\\tree00.pref", 10.0f, 10.0f, 0.0f, 0.5f, false) {}
};
class CProdTree01 : public CNatureProduces
{
public:
	CProdTree01() : CNatureProduces("tree01", "data\\PREFAB\\tree\\tree01.pref", 10.0f, 10.0f, 0.0f, 0.5f, false) {}
};
class CProdFallenTree00 : public CNatureProduces
{
public:
	CProdFallenTree00() : CNatureProduces("fallen_tree00", "data\\PREFAB\\tree\\fallen_tree.pref", 10.0f, 30.0f) {}
};

#endif // !_NATURE_PRODUCES_H_
