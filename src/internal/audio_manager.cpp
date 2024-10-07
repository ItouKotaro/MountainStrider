//=============================================================
//
// オーディオ管理 [audio_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "audio_manager.h"
#include "manager.h"
#include <iostream>
#include <fstream>

class WaveFile {
public:
	~WaveFile() {
		m_fp.close();
	}
	bool Open(const char* path);
	int Read(void* out, size_t size);
	bool IsEnd()const { return (0 == m_LoadedSize) || (m_DataSize == m_LoadedSize); }
	ALuint CreateBuffer(ALuint buffer);

private:
	unsigned short m_ChannelQuantity;
	unsigned short m_BlockSize;
	unsigned short m_BitPerSample;
	unsigned int m_Bps;
	unsigned int m_SamplingRate;
	unsigned int m_Length;

	std::ifstream m_fp;
	std::ifstream::pos_type m_DataHead;
	size_t m_DataSize;
	size_t m_LoadedSize;
};
bool WaveFile::Open(const char* path) {
	m_fp.open(path, std::ios::binary);

	// "RIFF" の読み込み
	unsigned int riff;
	m_fp.read((char*)&riff, 4);

	// データサイズを取得
	m_fp.read((char*)&m_DataSize, 4);

	// WAVEの読み込み
	unsigned int wave;
	m_fp.read((char*)&wave, 4);

	// PCM 情報とデータの先頭の取得
	for (int i = 0; i < 2; ++i) {
		unsigned int res, size;
		m_fp.read((char*)&res, 4);
		m_fp.read((char*)&size, 4);
		if (0x20746d66u == res) 
		{
			// PCM 情報の取得
			unsigned short res16;
			m_fp.read((char*)&res16, 2);
			if (1 != res16) {// 非対応フォーマット
				return false;
			}

			// モノラル(1), ステレオ(2)
			m_fp.read((char*)&m_ChannelQuantity, 2);
			if (2 < m_ChannelQuantity) {
				return false;
			}

			// サンプリングレート
			m_fp.read((char*)&m_SamplingRate, 4);

			// 1秒あたりのバイト数(byte/sec)
			m_fp.read((char*)&m_Bps, 4);

			// ブロックサイズ(byte/sample)
			m_fp.read((char*)&m_BlockSize, 2);

			// サンプルあたりのビット数(bit/sample)
			m_fp.read((char*)&m_BitPerSample, 2);
		}
		else if (0x61746164u == res) 
		{
			// データの開始位置を保存
			m_DataHead = m_fp.tellg();
			m_DataSize = size;

			// データを読み飛ばす
			m_fp.seekg(size, std::ios::cur);
		}
	}

	// データの開始位置までシーク
	m_fp.seekg(m_DataHead);
	m_LoadedSize = 0;
	m_DataSize -= m_DataSize % 100;
	m_Length = m_DataSize / m_Bps;
	return true;
}

int WaveFile::Read(void* out, size_t size) {
	if (!out) 
	{
		return 0;
	}

	// データサイズの調整
	if (m_LoadedSize + size > m_DataSize) {
		size = m_DataSize - m_LoadedSize;
	}

	// データを読み出し書き込む
	m_fp.read((char*)out, size);
	m_LoadedSize += size;
	if (m_LoadedSize == m_DataSize) {
		m_LoadedSize = 0;
		m_fp.seekg(m_DataHead);
	}
	return size;
}

ALuint WaveFile::CreateBuffer(ALuint buffer) {
	// 新規にバッファを作成
	if (!buffer)
	{
		alGenBuffers(1, &buffer);
	}
	std::vector<char> wavData(m_DataSize);
	int read = Read(&wavData[0], m_DataSize);
	ALenum format;
	if (1 == m_ChannelQuantity) 
	{ // モノラル
		format = (8 == m_BitPerSample) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
	}
	else 
	{ // ステレオ
		format = (8 == m_BitPerSample) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	}

	// OpenAL のバッファにデータの書き込み
	alBufferData(buffer, format, &wavData[0], read, m_SamplingRate);
	return buffer;
}


//=============================================================
// [CAudioManager] 初期化
//=============================================================
HRESULT CAudioManager::Init()
{
	// デバイスの作成
	m_device = alcOpenDevice(nullptr);
	if (m_device)
	{
		// コンテキストの作成
		m_context = alcCreateContext(m_device, nullptr);
		alcMakeContextCurrent(m_context);
	}

	alGetError();

	return S_OK;
}

//=============================================================
// [CAudioManager] 終了
//=============================================================
void CAudioManager::Uninit()
{
	// コンテキストを破棄する
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(m_context);

	// デバイスを破棄する
	alcCloseDevice(m_device);
}

//=============================================================
// [CAudioManager] 更新
//=============================================================
void CAudioManager::Update()
{

}