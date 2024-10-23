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
	void SetSeed(const int& seed) {m_seed = seed;}

	// �T�C�Y�ݒ�
	void SetSize(const int& x, const int& y);

	// ���ʂ��擾����
	int Get(const int& x, const int& y);

	// �T�C�Y�̎擾
	int GetSizeX() { return static_cast<int>(m_value.size()); }
	int GetSizeY() { return static_cast<int>(m_value[0].size()); }

protected:
	// �V�[�h�l�Ɋ�Â��ď����l��������
	void InitValue();

	int m_seed;	// �V�[�h�l
	std::vector<std::vector<int>> m_value;
};

// �p�[�����m�C�Y
class PerlinNoise : public Noise
{
public:
	void Generate();
private:

};

#endif // !_NOISE_H_
