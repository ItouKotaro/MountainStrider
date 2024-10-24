//=============================================================
//
// �m�C�Y [noise.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _NOISE_H_
#define _NOISE_H_

// �m�C�Y�i���N���X�j
class Noise
{
public:
	Noise();

	// �V�[�h�ݒ�
	void SetSeed(const int& seed);

protected:
	// �V�[�h�l�Ɋ�Â��ď����l��������
	void InitValue();

	int m_seed;					// �V�[�h�l
	int m_seedValue[512];	// �V�[�h�^�C��
};

// �p�[�����m�C�Y
class PerlinNoise : public Noise
{
public:
	double Get(const double& x = 0.0, const double& y = 0.0, const double& z = 0.0);
private:
	double SetNoise(double x_, double y_, double z_);
};

// �m�C�Y
namespace noise
{
	double getFade(const double t_);
	double getLerp(const double t_, const double a_, const double b_);
	double makeGrad(const int hash_, const double u_, const double v_);
	double makeGrad(const int hash_, const double x_, const double y_, const double z_);
	double getGrad(const int hash_, const double x_, const double y_, const double z_);
}

#endif // !_NOISE_H_
