//=============================================================
//
// ノイズ [noise.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _NOISE_H_
#define _NOISE_H_

// ノイズ（基底クラス）
class Noise
{
public:
	Noise();

	// シード設定
	void SetSeed(const int& seed) {m_seed = seed;}

	// サイズ設定
	void SetSize(const int& x, const int& y);

	// 結果を取得する
	int Get(const int& x, const int& y);

	// サイズの取得
	int GetSizeX() { return static_cast<int>(m_value.size()); }
	int GetSizeY() { return static_cast<int>(m_value[0].size()); }

protected:
	// シード値に基づいて初期値を代入する
	void InitValue();

	int m_seed;	// シード値
	std::vector<std::vector<int>> m_value;
};

// パーリンノイズ
class PerlinNoise : public Noise
{
public:
	void Generate();
private:

};

#endif // !_NOISE_H_
