//=============================================================
//
// フラグ [flag.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _FLAG_H_
#define _FLAG_H_

// フラグ
// - 位置を管理します
class Flag
{
public:
	// すべてのフラグを取得する
	static std::vector<Flag>& GetAllFlags() { return m_allFlags; }
private:
	// 名前
	std::string m_name;

	// 位置
	D3DXVECTOR3 m_pos;

	// すべてのフラグ
	static std::vector<Flag> m_allFlags;
};

#endif // !_FLAG_H_
