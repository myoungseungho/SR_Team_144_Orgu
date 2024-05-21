#include "stdafx.h"
#include "..\Public\Outside_TelePhonePole.h"

#include "GameInstance.h"

COutside_TelePhonePole::COutside_TelePhonePole(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

COutside_TelePhonePole::COutside_TelePhonePole(const COutside_TelePhonePole& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT COutside_TelePhonePole::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT COutside_TelePhonePole::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (m_bIsPasingObject)
	{
		FILEDATA* fileData = static_cast<FILEDATA*>(pArg);
		m_pTransformCom->Set_Scaled(_float3(fileData->scale.x, fileData->scale.y, fileData->scale.z));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(fileData->position.x, fileData->position.y, fileData->position.z));
	}

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 1.f;
	ColliderDesc.MineGameObject = this;

	//�ݶ��̴� �纻�� ���鶧 Cube ���� �߰������ ��.
	if (FAILED(__super::Add_Component(LEVEL_TACHO, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//�ݶ��̴�������Ʈ �߰�
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_STATIC, this);

	return S_OK;
}

void COutside_TelePhonePole::Priority_Update(_float fTimeDelta)
{
}

void COutside_TelePhonePole::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void COutside_TelePhonePole::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT COutside_TelePhonePole::Render(_float fTimeDelta)
{
	__super::Begin_RenderState();

	/* �簢������ �ø��� ���� �ױ��ĸ� �̸� ��ġ�� ���ε��Ѵ�.  */
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End_RenderState();

	return S_OK;
}

HRESULT COutside_TelePhonePole::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_TACHO, TEXT("Prototype_Component_Texture_Sprite_Outside_TelephonePole"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC			TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

COutside_TelePhonePole* COutside_TelePhonePole::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	COutside_TelePhonePole* pInstance = new COutside_TelePhonePole(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : COutside_TelePhonePole"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* COutside_TelePhonePole::Clone(void* pArg)
{
	COutside_TelePhonePole* pInstance = new COutside_TelePhonePole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : COutside_TelePhonePole"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COutside_TelePhonePole::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}