#include "stdafx.h"
#include "..\Public\UI_Inventory.h"

#include "UI_ItemTabIcon_Caution.h"
#include "UI_ItemTabIcon_Food.h"
#include "UI_ItemTabIcon_Hat.h"
#include "UI_ItemTabIcon_Leaf.h"
#include "UI_Cursor.h"
#include "UI_Inventory_BackGround.h"
#include "UI_Item.h"
#include "UI_Hat.h"
#include "GameInstance.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CInventory::CInventory(const CInventory& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CInventory::Initialize_Prototype()
{
	/* 원형객체의 초기화작업을 수행한다. */
	/* 서버로부터 데이터를 받아오거나. 파일 입출력을 통해 데이터를 셋한다.  */

	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	// 간이 인벤토리 초기화 (nullptr로 채움)
	m_quickInventory.resize(4, nullptr);

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.0f, 1.f);


	LEVELID currentLevel = (LEVELID)m_pGameInstance->GetLoadingLevelIndex();

	//UI오브젝트 받아서 VECTOR에 넣기
	auto AddUIObject = [&](const TCHAR* prototypeTag, const TCHAR* layerTag, void* pArg = nullptr, const _uint count = 0) -> HRESULT {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, prototypeTag, layerTag, pArg)))
			return E_FAIL;
		CUIObject* pUIObject = static_cast<CUIObject*>(m_pGameInstance->Get_GameObject(currentLevel, layerTag, count));
		if (!pUIObject)
			return E_FAIL;
		Safe_AddRef(pUIObject);
		m_vecUIObject.push_back(pUIObject);
		return S_OK;
		};

	UIDATA slotData{};

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_FadeInOut"), TEXT("Layer_UI_FadeInOut"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Cursor"), TEXT("Layer_UI_ZCursor"))))
		return E_FAIL;


	for (size_t i = 0; i < 2; i++)
	{
		// 각 슬롯에 대한 위치와 크기 설정
		switch (i)
		{
		case 0:
			slotData.position = { 455.f, 130.f };
			slotData.scale = { 320.f, 225.f };
			slotData.alpha = 66.f;
			break;
		case 1:
			slotData.position = { -430.f, 80.f };
			slotData.scale = { 140.f, 145.f };
			slotData.alpha = 104.f;
			break;
		}

		slotData.index = i;

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_BackGround"), TEXT("Layer_UI_Inventory_XBackGround"), &slotData, i)))
			return E_FAIL;
	}

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_DotLine"), TEXT("Layer_UI_Inventory_DotLine"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_Slot"), TEXT("Layer_UI_Inventory_Slot"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_VerticalDotLine"), TEXT("Layer_UI_Inventory_VerticalDotLine"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Caution"), TEXT("Layer_UI_ItemTabIcon_Caution"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Food"), TEXT("Layer_UI_ItemTabIcon_Food"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Hat"), TEXT("Layer_UI_ItemTabIcon_Hat"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_ItemTabIcon_Leaf"), TEXT("Layer_UI_ItemTabIcon_Leaf"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_ItemInfo_TextBox"), TEXT("Layer_UI_Shop_ItemInfo_TextBox"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_PlayerCoin"), TEXT("Layer_UI_Shop_PlayerCoin"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Shop_PriceTag"), TEXT("Layer_UI_Shop_PriceTag"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_SlotBeigeBackground"), TEXT("Layer_UI_Inventory_SlotBeigeBackground"))))
		return E_FAIL;

	if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Inventory_Button"), TEXT("Layer_UI_Inventory_Button"))))
		return E_FAIL;


	const float initialX = -430.0f; // 첫 번째 열의 초기 X 위치
	const float initialY = 85.0f; // 첫 번째 행의 초기 Y 위치
	const float deltaX = 145.0f; // 열 이동 시의 X 위치 증감분
	const float deltaY = -135.0f; // 행 이동 시의 Y 위치 증감분

	for (size_t i = 0; i < m_iInitHatCount; i++)
	{
		// 각 슬롯에 대한 위치와 크기 설정
		int row = (i / 5); // 두 번째 행부터 시작
		int col = i % 5;

		slotData.position = { initialX + col * deltaX, initialY + row * deltaY };
		slotData.scale = { 60.f, 70.f };
		slotData.alpha = 255.f;
		slotData.index = i;

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Hat"), TEXT("Layer_ZUI_Hat"), &slotData, i)))
			return E_FAIL;
	}



	for (size_t i = 0; i < m_iInitItemCount; i++)
	{
		// 각 슬롯에 대한 위치와 크기 설정
		int row = (i / 5); // 두 번째 행부터 시작
		int col = i % 5;

		slotData.position = { initialX + col * deltaX, initialY + row * deltaY };
		slotData.scale = { 60.f, 70.f };
		slotData.alpha = 255.f;
		slotData.index = i;

		if (FAILED(AddUIObject(TEXT("Prototype_GameObject_UI_Item"), TEXT("Layer_ZUI_Item"), &slotData, i)))
			return E_FAIL;
	}

	// 초기 상태 설정
	UpdateAlphaValues();
	return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
}

void CInventory::Update(_float fTimeDelta)
{
	if (!m_bIsOn)
		return;

	bool positionChanged = false;

	if (m_pKeyCom->Key_Down(VK_UP))
	{
		// 위쪽 방향키 입력 처리
		if (m_iCurrentRow > 1) {
			// 두 번째 행 이상일 때 처리
			m_iCurrentRow = (m_iCurrentRow - 1 + m_iMaxRows) % m_iMaxRows;
			m_iCurrentCol = min(m_iCurrentCol, getMaxCols(m_iCurrentRow) - 1);
			positionChanged = true;
		}
		else if (m_iCurrentRow == 1) {
			// 두 번째 행에서 첫 번째 행으로 이동 시
			m_iCurrentRow = 0;
			m_iCurrentCol = 0; // 첫 번째 행의 첫 번째 열로 이동
			positionChanged = true;
		}
	}
	if (m_pKeyCom->Key_Down(VK_DOWN))
	{
		// 아래쪽 방향키 입력 처리
		if (m_iCurrentRow == m_iMaxRows - 1) {
			// 마지막 행에서 아래쪽으로 이동 시 두 번째 행으로 이동
			m_iCurrentRow = 1;
			positionChanged = true;
		}
		else if (m_iCurrentRow < m_iMaxRows - 1) {
			m_iCurrentRow = (m_iCurrentRow + 1) % m_iMaxRows;
			positionChanged = true;
		}
		m_iCurrentCol = min(m_iCurrentCol, getMaxCols(m_iCurrentRow) - 1);
	}
	if (m_pKeyCom->Key_Down(VK_LEFT))
	{
		// 왼쪽 방향키 입력 처리
		int maxCols = getMaxCols(m_iCurrentRow);
		m_iCurrentCol = (m_iCurrentCol - 1 + maxCols) % maxCols;
		positionChanged = true;
	}
	if (m_pKeyCom->Key_Down(VK_RIGHT))
	{
		// 오른쪽 방향키 입력 처리
		int maxCols = getMaxCols(m_iCurrentRow);
		m_iCurrentCol = (m_iCurrentCol + 1) % maxCols;
		positionChanged = true;
	}

	if (m_pKeyCom->Key_Down(VK_RETURN)) // 엔터 키를 눌렀을 때
	{

		if (m_firstRowSelectedCol == 0) {
			EquipHat();
		}
		else if (m_firstRowSelectedCol == 1) {
			EquipItem();
		}
	}

	// 숫자 키 입력 처리 (1, 2, 3, 4)
	if (m_pKeyCom->Key_Down('1')) { AddToQuickInventory(0); }
	if (m_pKeyCom->Key_Down('2')) { AddToQuickInventory(1); }
	if (m_pKeyCom->Key_Down('3')) { AddToQuickInventory(2); }
	if (m_pKeyCom->Key_Down('4')) { AddToQuickInventory(3); }


	if (positionChanged) {
		UpdateAlphaValues();
		m_iPreviousRow = m_iCurrentRow;
		m_iPreviousCol = m_iCurrentCol;
	}

	// 현재 선택된 아이템의 인덱스를 계산
	int selectedIndex = 0;

	if (m_iCurrentRow > 0) {
		// 두 번째 행부터 현재 행까지의 인덱스를 계산
		for (int i = 1; i < m_iCurrentRow; ++i) {
			selectedIndex += getMaxCols(i);
		}
		selectedIndex += m_iCurrentCol;
	}
	else {
		// 첫 번째 행인 경우
		selectedIndex = m_iCurrentCol;
	}

	// 선택된 아이템 처리 (예: 하이라이트 표시)
	// HandleSelection(selectedIndex); // 가정된 함수
}

void CInventory::UpdateAlphaValues()
{
	if (m_iCurrentRow == 0) {
		Control_FirstRow();
	}
	else {
		Control_OtherRow();
	}
}

void CInventory::ShowHats()
{
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Hat))
			iter->m_fAlpha = 255.f; // Hat 객체를 보이게 설정
		else if (typeid(*iter) == typeid(CUI_Item))
			iter->m_fAlpha = 0.f; // Item 객체를 숨김
	}
}

void CInventory::ShowItems()
{
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Hat))
			iter->m_fAlpha = 0.f; // Hat 객체를 보이게 설정
		else if (typeid(*iter) == typeid(CUI_Item))
			iter->m_fAlpha = 255.f; // Item 객체를 숨김
	}
}

void CInventory::EquipHat()
{
	int selectedIndex = 0;

	if (m_iCurrentRow > 0) {
		// 두 번째 행부터 현재 행까지의 인덱스를 계산
		for (int i = 1; i < m_iCurrentRow; ++i) {
			selectedIndex += getMaxCols(i);
		}
		selectedIndex += m_iCurrentCol;

		// 인덱스를 통해 현재 선택된 Hat을 찾음
		if (selectedIndex >= 0 && selectedIndex < m_vecUIObject.size()) {
			for (auto& iter : m_vecUIObject) {
				if (iter->m_iIndex == selectedIndex && typeid(*iter) == typeid(CUI_Hat)) {
					m_currentEquipHat = iter;
					break;
				}
			}
		}
	}
}

void CInventory::EquipItem()
{
	int selectedIndex = 0;

	if (m_iCurrentRow > 0) {
		// 두 번째 행부터 현재 행까지의 인덱스를 계산
		for (int i = 1; i < m_iCurrentRow; ++i) {
			selectedIndex += getMaxCols(i);
		}
		selectedIndex += m_iCurrentCol;

		// 인덱스를 통해 현재 선택된 Item을 찾음
		if (selectedIndex >= 0 && selectedIndex < m_vecUIObject.size()) {
			for (auto& iter : m_vecUIObject) {
				if (iter->m_iIndex == selectedIndex && typeid(*iter) == typeid(CUI_Item)) {
					m_currentEquipItem = iter;
					break;
				}
			}
		}
	}
}
void CInventory::AddToQuickInventory(_uint slot)
{
	if (slot < 0 || slot >= 4)
		return;

	UIDATA slotData{};
	LEVELID currentLevel = (LEVELID)m_pGameInstance->GetCurrentLevelIndex();

	// 각 슬롯에 대한 위치와 크기 설정
	switch (slot)
	{
	case 0:
		slotData.position = { -530.f, 250.f };
		slotData.scale = { 40.f, 40.f };
		break;
	case 1:
		slotData.position = { -480.f, 250.f };
		slotData.scale = { 40.f, 40.f };
		break;
	case 2:
		slotData.position = { -430.f, 250.f };
		slotData.scale = { 40.f, 40.f };
		break;
	case 3:
		slotData.position = { -380.f, 250.f };
		slotData.scale = { 40.f, 40.f };
		break;
	default:
		return; // 기본값 (예외 처리)
	}

	slotData.alpha = 200.f;

	// 기존 슬롯에 있는 오브젝트를 제거
	if (m_quickInventory[slot] != nullptr) {
		Safe_Release(m_quickInventory[slot]);
	}

	if (m_firstRowSelectedCol == 0 && m_currentEquipHat != nullptr) {
		// Hat을 간이 인벤토리에 추가
		slotData.index = m_currentEquipHat->m_iIndex;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_Hat"), TEXT("Layer_ZUI_Hat_QuickInventory"), &slotData))) {
			return;
		}

		CGameObject* gameobject = m_pGameInstance->GetObjectByIndex(currentLevel, TEXT("Layer_ZUI_Hat_QuickInventory"), m_currentEquipHat->m_iIndex);
		CUIObject* uiobject = static_cast<CUIObject*>(gameobject);
		m_quickInventory[slot] = uiobject;
		m_currentEquipHat = nullptr;
	}
	else if (m_firstRowSelectedCol == 1 && m_currentEquipItem != nullptr) {
		// Item을 간이 인벤토리에 추가
		slotData.index = m_currentEquipItem->m_iIndex;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, TEXT("Prototype_GameObject_UI_Item"), TEXT("Layer_ZUI_Item_QuickInventory"), &slotData))) {
			return;
		}
		CGameObject* gameobject = m_pGameInstance->GetObjectByIndex(currentLevel, TEXT("Layer_ZUI_Item_QuickInventory"), m_currentEquipItem->m_iIndex);
		CUIObject* uiobject = static_cast<CUIObject*>(gameobject);

		m_quickInventory[slot] = uiobject;
		m_currentEquipItem = nullptr;
	}
}




void CInventory::Control_FirstRow()
{
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_ItemTabIcon_Hat)) {
			iter->m_fAlpha = (m_iCurrentCol == 0) ? 255.f : 150.f;
		}
		else if (typeid(*iter) == typeid(CUI_ItemTabIcon_Food)) {
			iter->m_fAlpha = (m_iCurrentCol == 1) ? 255.f : 150.f;
		}
		else if (typeid(*iter) == typeid(CUI_ItemTabIcon_Leaf)) {
			iter->m_fAlpha = (m_iCurrentCol == 2) ? 255.f : 150.f;
		}
		else if (typeid(*iter) == typeid(CUI_ItemTabIcon_Caution)) {
			iter->m_fAlpha = (m_iCurrentCol == 3) ? 255.f : 150.f;
		}
	}

	// 첫 번째 행일 때 CUI_Cursor를 숨김
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Cursor))
		{
			iter->m_fAlpha = 0.f;
		}
		else if (typeid(*iter) == typeid(CUI_Inventory_BackGround))
		{
			// 특정 조건에 맞는 CUI_Inventory_BackGround 객체를 선택
			if (iter->m_iIndex == 1) {
				iter->m_fAlpha = 0.f;
			}
		}
	}

	// 첫 번째 행에서 선택된 열 위치를 기억
	m_firstRowSelectedCol = m_iCurrentCol;

	// 두 번째 행 이후의 객체를 변경
	if (m_iCurrentCol == 0)
	{
		// Hat 객체를 보여줌
		ShowHats();
	}
	else if (m_iCurrentCol == 1)
	{
		// Item 객체를 보여줌
		ShowItems();
	}
}

void CInventory::Control_OtherRow()
{
	// 첫 번째 행의 아이템들의 알파값을 150.f로 설정
	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_ItemTabIcon_Hat) ||
			typeid(*iter) == typeid(CUI_ItemTabIcon_Food) ||
			typeid(*iter) == typeid(CUI_ItemTabIcon_Leaf) ||
			typeid(*iter) == typeid(CUI_ItemTabIcon_Caution)) {
			iter->m_fAlpha = 150.f;
		}
	}

	// CUI_Cursor 객체의 위치를 업데이트
	const float initialX = -430.0f; // 첫 번째 열의 초기 X 위치
	const float initialY = 85.0f; // 첫 번째 행의 초기 Y 위치
	const float deltaX = 145.0f; // 열 이동 시의 X 위치 증감분
	const float deltaY = -135.0f; // 행 이동 시의 Y 위치 증감분

	for (auto& iter : m_vecUIObject)
	{
		if (typeid(*iter) == typeid(CUI_Cursor))
		{
			iter->m_fX = initialX + m_iCurrentCol * deltaX;
			iter->m_fY = initialY + (m_iCurrentRow - 1) * deltaY; // 첫 번째 행 제외
			iter->m_fAlpha = 255.f; // 두 번째 행부터는 커서가 보이게 설정
		}
	}
}

void CInventory::Late_Update(_float fTimeDelta)
{

}

HRESULT CInventory::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CInventory::Ready_Components()
{
	/* For.Com_KeyState */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		TEXT("Com_KeyState"), reinterpret_cast<CComponent**>(&m_pKeyCom))))
		return E_FAIL;

	return S_OK;
}


CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInventory* pInstance = new CInventory(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Inventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUI_Inventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	for (auto& pUIObject : m_vecUIObject)
	{
		Safe_Release(pUIObject);
	}
	m_vecUIObject.clear();

	for (auto& pUIObject : m_quickInventory)
	{
		Safe_Release(pUIObject);
	}

	m_quickInventory.clear();
	Safe_Release(m_pKeyCom);
	__super::Free();
}

