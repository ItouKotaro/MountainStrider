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
	void SetSeed(const int& seed);

protected:
	// シード値に基づいて初期値を代入する
	void InitValue();

	int m_seed;					// シード値
	int m_seedValue[512];	// シードタイル
};

// パーリンノイズ
class PerlinNoise : public Noise
{
public:
	double Get(const double& x = 0.0, const double& y = 0.0, const double& z = 0.0);
private:
	double SetNoise(double x_, double y_, double z_);
};

// ノイズ
namespace noise
{
	double getFade(const double t_);
	double getLerp(const double t_, const double a_, const double b_);
	double makeGrad(const int hash_, const double u_, const double v_);
	double makeGrad(const int hash_, const double x_, const double y_, const double z_);
	double getGrad(const int hash_, const double x_, const double y_, const double z_);
}

#endif // !_NOISE_H_
