#pragma once

#include "Client_Defines.h"
#include "Level.h"

#include "LandObject.h"

BEGIN(Client)

class CLevel_Tacho final : public CLevel
{
private:
	CLevel_Tacho(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Tacho() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	/* 내 게임내에 배경을 구성하는 객체들을 추가한다. */
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Sky(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag, CLandObject::LANDOBJECT_DESC& Desc);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);

	HRESULT Ready_LandObjects();

	HRESULT ParseInitialize();

public:
	static CLevel_Tacho* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END