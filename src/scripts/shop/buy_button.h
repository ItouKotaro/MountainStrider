//=============================================================
//
// �w���{�^�� [buy_button.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _BUY_BUTTON_H_
#define _BUY_BUTTON_H_

#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "shop_items.h"

// �w���{�^��
class BuyButtonUI : public Component
{
public:
	BuyButtonUI(ShopItem* shopItem) { m_shopItem = shopItem; }
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	static const float EDGE_BOLD;		// ���̑���
	static const float WIDTH;
	static const float HEIGHT;
	static const float ITEM_SIZE;
	static const float PRICEICON_SIZE;
	static const D3DXCOLOR ITEM_COLOR;
	static const D3DXCOLOR ITEM_COLOR_BOUGHT;
	static const D3DXCOLOR PERK_COLOR;
	static const D3DXCOLOR PERK_COLOR_BOUGHT;
private:
	ShopItem* m_shopItem;
	float m_progress;

	SingleComponent<CPolygon>* m_edge;			// ��
	SingleComponent<CPolygon>* m_bought;		// �w����
	SingleComponent<CPolygon>* m_inside;		// �w���O
	SingleComponent<CPolygon>* m_mask;			// �}�X�N�i�i�s�\���p�j
	SingleComponent<CPolygon>* m_image;		// �C���[�W�摜
	SingleComponent<CText>* m_itemName;		// �A�C�e����
	SingleComponent<CPolygon>* m_priceIcon	;	// ���i�A�C�R��
	SingleComponent<CText>* m_priceText;			// ���i�e�L�X�g
	SingleComponent<CPolygon>* m_splitLine;		// �d������
	SingleComponent<CText>* m_description;		// ����
};

#endif // !_BUY_BUTTON_H_
