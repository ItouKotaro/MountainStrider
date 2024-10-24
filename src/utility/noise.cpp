//=============================================================
//
// �m�C�Y [noise.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "noise.h"
#include <algorithm>
#include <array>
#include <random>
using namespace noise;

//=============================================================
// [Noise] �R���X�g���N�^
//=============================================================
Noise::Noise() :
	m_seed(0),
	m_seedValue()
{
}

//=============================================================
// [Noise] �V�[�h�l��ݒ肷��
//=============================================================
void Noise::SetSeed(const int& seed)
{
	m_seed = seed;
	InitValue();
}

//=============================================================
// [Noise] �V�[�h�l�Ɋ�Â��ď����l��������
//=============================================================
void Noise::InitValue()
{
	// 256�̏���e�[�u�����쐬����
	for (int i = 0; i < 256; i++)
	{
		m_seedValue[i] = i;
	}

	// �V�[�h�l�Ɋ�Â��ăV���b�t������
	std::mt19937 g(m_seed);
	std::shuffle(std::begin(m_seedValue), std::begin(m_seedValue) + 256, g);

	// �f�[�^�Ɋi�[����
	for (int i = 0; i < 256; i++)
	{
		m_seedValue[256 + i] = m_seedValue[i];
	}
}

//=============================================================
// [PerlinNoise] �擾
//=============================================================
double PerlinNoise::Get(const double& x, const double& y, const double& z)
{
	// �l���擾����
	double val = SetNoise(x, y, z);

	// �Ԃ�l��Ԃ�
	return val;
}

//=============================================================
// [PerlinNoise] �m�C�Y��ݒ肷��
//=============================================================
double PerlinNoise::SetNoise(double x_, double y_, double z_)
{
	const std::size_t x_int{ static_cast<std::size_t>(static_cast<std::size_t>(std::floor(x_)) & 255) };
	const std::size_t y_int{ static_cast<std::size_t>(static_cast<std::size_t>(std::floor(y_)) & 255) };
	const std::size_t z_int{ static_cast<std::size_t>(static_cast<std::size_t>(std::floor(z_)) & 255) };
	x_ -= std::floor(x_);
	y_ -= std::floor(y_);
	z_ -= std::floor(z_);
	const double u{ getFade(x_) };
	const double v{ getFade(y_) };
	const double w{ getFade(z_) };
	const std::size_t a0{ static_cast<std::size_t>(m_seedValue[x_int] + y_int) };
	const std::size_t a1{ static_cast<std::size_t>(m_seedValue[a0] + z_int) };
	const std::size_t a2{ static_cast<std::size_t>(m_seedValue[a0 + 1] + z_int) };
	const std::size_t b0{ static_cast<std::size_t>(m_seedValue[x_int + 1] + y_int) };
	const std::size_t b1{ static_cast<std::size_t>(m_seedValue[b0] + z_int) };
	const std::size_t b2{ static_cast<std::size_t>(m_seedValue[b0 + 1] + z_int) };

	return getLerp(w,
		getLerp(v,
			getLerp(u, getGrad(m_seedValue[a1], x_, y_, z_), getGrad(m_seedValue[b1], x_ - 1, y_, z_)),
			getLerp(u, getGrad(m_seedValue[a2], x_, y_ - 1, z_), getGrad(m_seedValue[b2], x_ - 1, y_ - 1, z_))),
		getLerp(v,
			getLerp(u, getGrad(m_seedValue[a1 + 1], x_, y_, z_ - 1), getGrad(m_seedValue[b1 + 1], x_ - 1, y_, z_ - 1)),
			getLerp(u, getGrad(m_seedValue[a2 + 1], x_, y_ - 1, z_ - 1), getGrad(m_seedValue[b2 + 1], x_ - 1, y_ - 1, z_ - 1))));
}

//=============================================================
// [noise] �m�C�Y����
//=============================================================
double noise::getFade(const double t_)
{
	return t_ * t_ * t_ * (t_ * (t_ * 6 - 15) + 10);
}
double noise::getLerp(const double t_, const double a_, const double b_)
{
	return a_ + t_ * (b_ - a_);
}
double noise::makeGrad(const int hash_, const double u_, const double v_)
{
	return (((hash_ & 1) == 0) ? u_ : -u_) + (((hash_ & 2) == 0) ? v_ : -v_);
}
double noise::makeGrad(const int hash_, const double x_, const double y_, const double z_)
{
	return noise::makeGrad(hash_, hash_ < 8 ? x_ : y_, hash_ < 4 ? y_ : hash_ == 12 || hash_ == 14 ? x_ : z_);
}
double noise::getGrad(const int hash_, const double x_, const double y_, const double z_)
{
	return noise::makeGrad(hash_ & 15, x_, y_, z_);
}
