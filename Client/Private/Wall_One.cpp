#include "stdafx.h"
#include "..\Public\Wall_One.h"

#include "GameInstance.h"

CWall_One::CWall_One(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEnviormentObject{ pGraphic_Device }
{
}

CWall_One::CWall_One(const CWall_One& Prototype)
	: CEnviormentObject{ Prototype }
{
}

HRESULT CWall_One::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ�۾��� �����Ѵ�. */
	/* �����κ��� �����͸� �޾ƿ��ų�. ���� ������� ���� �����͸� ���Ѵ�.  */

	return S_OK;
}

HRESULT CWall_One::Initialize(void* pArg)
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

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(60.f));

	/* For.Com_Transform */
	CCollider::COLLIDER_DESC			ColliderDesc{};
	ColliderDesc.center = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ColliderDesc.width = m_pTransformCom->Get_Scaled().x;
	ColliderDesc.height = m_pTransformCom->Get_Scaled().y;
	ColliderDesc.depth = 1.f;
	ColliderDesc.MineGameObject = this;

	//�ݶ��̴� �纻�� ���鶧 Cube ���� �߰������ ��.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	//�ݶ��̴�������Ʈ �߰�
	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_STATIC, this);

	return S_OK;
}

void CWall_One::Priority_Update(_float fTimeDelta)
{
}

void CWall_One::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CWall_One::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CWall_One::Render(_float fTimeDelta)
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

HRESULT CWall_One::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_JUNGLE, TEXT("Prototype_Component_Texture_Wall_One"),
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

CWall_One* CWall_One::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWall_One* pInstance = new CWall_One(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CWall_One"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CWall_One::Clone(void* pArg)
{
	CWall_One* pInstance = new CWall_One(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CWall_One"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWall_One::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	m_pGameInstance->Release_Collider(m_pColliderCom);

	__super::Free();
}
