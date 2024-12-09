//=============================================================
//
// ページ [page.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _PAGE_H_
#define _PAGE_H_

#include "component.h"

// ページコンポーネント
class Pages : public Component
{
public:
	Pages() : m_pageIdx(0){}

	// ページを追加する
	void SetNumPage(UINT pages);

	// オブジェクトを登録する
	void AddObject(UINT page, GameObject* obj);

	// オブジェクトの登録を解除する
	void RemoveObject(UINT page, GameObject* obj);

	// ページを設定する
	void SetPage(UINT page);

	// 現在のページ数を取得する
	UINT GetPage() { return m_pageIdx; }

	// ページに登録されているすべてのオブジェクトを非表示にする
	void AllHideObjects();
private:
	UINT m_pageIdx;
	std::vector<std::vector<GameObject*>> m_pages;
};

#endif // !_PAGE_H_
