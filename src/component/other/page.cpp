//=============================================================
//
// ページ [page.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "page.h"

//=============================================================
// [Pages] ページの追加
//=============================================================
void Pages::SetNumPage(UINT pages)
{
	m_pages.resize(pages);
}

//=============================================================
// [Pages] オブジェクトを追加する
//=============================================================
void Pages::AddObject(UINT page, GameObject* obj)
{
	if (0 <= page && page < m_pages.size())
	{ // ページ範囲内のとき

		// 既にオブジェクトが登録済みでないかを確認する
		for (auto itr = m_pages[page].begin(); itr != m_pages[page].end(); itr++)
		{
			if (*itr == obj)
			{ // 一致するとき
				return;
			}
		}

		// ページにオブジェクトを追加する
		m_pages[page].push_back(obj);
	}
}

//=============================================================
// [Pages] オブジェクトを削除する
//=============================================================
void Pages::RemoveObject(UINT page, GameObject* obj)
{
	if (0 <= page && page < m_pages.size())
	{ // ページ範囲内のとき
		// 既にオブジェクトが登録済みでないかを確認する
		for (auto itr = m_pages[page].begin(); itr != m_pages[page].end(); itr++)
		{
			if (*itr == obj)
			{ // 一致するとき
				m_pages[page].erase(itr);
				return;
			}
		}
	}
}

//=============================================================
// [Pages] ページを設定する
//=============================================================
void Pages::SetPage(UINT page)
{
	if (0 <= page && page < m_pages.size())
	{ // ページ範囲内のとき
		// 現在のページのオブジェクトを非表示にする
		for (auto itr = m_pages[m_pageIdx].begin(); itr != m_pages[m_pageIdx].end(); itr++)
		{
			(*itr)->SetActive(false);
		}

		// 新しいページのオブジェクトを表示する
		for (auto itr = m_pages[page].begin(); itr != m_pages[page].end(); itr++)
		{
			(*itr)->SetActive(true);
		}
		m_pageIdx = page;
	}
}

//=============================================================
// [Pages] ページに登録されているオブジェクトすべてを非表示にする
//=============================================================
void Pages::AllHideObjects()
{
	for (auto pageItr = m_pages.begin(); pageItr != m_pages.end(); pageItr++)
	{
		for (auto objItr = (*pageItr).begin(); objItr != (*pageItr).end(); objItr++)
		{
			(*objItr)->SetActive(false);
		}
	}
}