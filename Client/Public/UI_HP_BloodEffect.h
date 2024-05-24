#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_HP_BloodEffect final : public CUIObject
{	
private:
	CUI_HP_BloodEffect(LPDIRECT3DDEVICE9 pGraphic_Device); /* �������� �� */
	CUI_HP_BloodEffect(const CUI_HP_BloodEffect& Prototype); /* �纻���� �� */
	virtual ~CUI_HP_BloodEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	HRESULT Ready_Components();
private:	
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };

private:
	float m_fAlpha = { 0.f };               // Current alpha value
	bool m_bIncreasingAlpha = { true };     // Is alpha value increasing
	float m_fElapsedTime = { 0.f };       // Elapsed time for alpha animation
	float m_fAlphaAnimationDuration = { 0.5f }; // Duration for alpha increase/decrease
	float m_fMinAlpha = { 0.f };           // Minimum alpha value
	float m_fMaxAlpha = { 30.f };         // Maximum alpha value

public:
	/* ������ü�� �����Ѵ�. */
	static CUI_HP_BloodEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	/* ������ü�� ������ �纻��ü�� �����Ѵ�.(�� ���ӳ����� ���� �����ϱ����� ��ü��) */
	virtual CGameObject* Clone(void* pArg = nullptr ) override;
	virtual void Free() override;
};

END