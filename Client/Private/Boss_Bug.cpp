#include "stdafx.h"

#include "Boss_Bug.h"
#include "GameInstance.h"
#include "Mon_Turtle.h"


CBoss_Bug::CBoss_Bug(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster{ pGraphic_Device }
{
}

CBoss_Bug::CBoss_Bug(const CBoss_Bug& Prototype)
	:CMonster{ Prototype }
{
}

HRESULT CBoss_Bug::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Bug::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BOSS_BUG_DESC* pDesc = static_cast<BOSS_BUG_DESC*>(pArg);

	m_pTargetTransform = pDesc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);
	m_tMonsterDesc.iHp =  pDesc->iHp;
	m_tMonsterDesc.iAttack= pDesc->iAttack;

 	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(0.0f, 3.f, 10.f));
	m_pTransformCom->LookAt(m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

void CBoss_Bug::Priority_Update(_float fTimeDelta)
{

}

void CBoss_Bug::Update(_float fTimeDelta)
{
	m_fAngle++;

	if (m_fAngle > 360.f)
		m_fAngle = 0.f;

	if (m_pKeyCom->Key_Down('1'))
		Bullet_Create();

	if (m_pKeyCom->Key_Down('2') || m_isDesh)
	{
		m_isDesh = true;
		Skill_Dash(fTimeDelta);
	}
	if (m_pKeyCom->Key_Down('3'))
		Turtle_Create();

	if (m_pKeyCom->Key_Down('4') || m_isUp)
	{
		m_isUp = true;
		Fly(fTimeDelta);
	}

	if (m_pKeyCom->Key_Down('5') || m_iLand)
	{
		m_iLand = true;
		Land(0, 0, fTimeDelta);
	}

	if(m_pKeyCom->Key_Down('6'))
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(0.0f, 3.f, 10.f));

	Hp_State(fTimeDelta);

	
}

void CBoss_Bug::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBoss_Bug::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	_float4x4		ViewMatrix, ProjMatrix;

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;
	  
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CBoss_Bug::Ready_Components()
{
	/* For.Com_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Key"),
		TEXT("Com_Key"), reinterpret_cast<CComponent**>(&m_pKeyCom))))
		return E_FAIL;

	/* For.Com_Timer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Timer"),
		TEXT("Com_Timer"), reinterpret_cast<CComponent**>(&m_pTimerCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster"),
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

void CBoss_Bug::Warf(_int iPosX, _int iPosZ,_float fDistance, _float fAngle)
{
	_float WarfPosX = iPosX +  fDistance * cos(fAngle * (D3DX_PI / 180.f));
	_float WarfPosZ = iPosZ -   fDistance * sin(fAngle * (D3DX_PI / 180.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3(WarfPosX,  3.f, WarfPosZ));
	m_pTransformCom->LookAt(m_pTargetTransform->Get_State(CTransform::STATE_POSITION));
	
}

void CBoss_Bug::Skill_Dash(_float fTimeDelta)
{
	auto iter = dynamic_cast<CMon_Turtle*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster_Turtle")));
	
	if (iter)
	{
		if (m_pTimerCom->Time_Limit(fTimeDelta, 5.f))
		{
			Warf(0, 0, 10.f, m_fAngle);
		}
		else
			m_pTransformCom->Go_Straight(fTimeDelta * 5.f);
	}
	
}

void CBoss_Bug::Fly(_float fTimeDelta)
{
	if (m_pTimerCom->Time_Limit(fTimeDelta, 2.f, 5.f))
		m_pTransformCom->Go_Up(fTimeDelta);
	
}

void CBoss_Bug::Land(_int iPosX, _int iPosZ, _float fTimeDelta)
{
	if (!m_iCon)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (&_float3(iPosX, 5.f, iPosZ)));
		m_iCon = true;
	}
	if(m_pTimerCom->Time_Limit(fTimeDelta , 2.f, 5.f))
		m_pTransformCom->Go_Down(fTimeDelta);
}

HRESULT CBoss_Bug::Desh_Stop(_float fTimeDelta) 
{
	auto iter = dynamic_cast<CMon_Turtle*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster_Turtle")));

	if (iter != nullptr)
	{
		return E_FAIL;
	}
	else
	{
		if (m_pTimerCom->Time_Limit(fTimeDelta, 2.f,5.f))
			Fly(fTimeDelta); 

		return S_OK;
	}
}

HRESULT CBoss_Bug::Turtle_Create()
{
	CMon_Turtle::MON_TURTLE_DESC	 Desc{};

	Desc.iHp = 10;
	Desc.iAttack = 1;

	for (int i = 0; (i < 3) && !m_isTurtleDead ; ++i)
	{
		Desc.m_iColor = i;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Turtle"), TEXT("Layer_Monster_Turtle"), &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CBoss_Bug::Bullet_Create()
{
	CSkill_Bug_Bullet::SKILL_BUG_BULLET_DESC	SkillDesc{};
	SkillDesc.pTargetTransform = m_pTransformCom;

	for (int i = 1; i <= 5; ++i)
	{
		SkillDesc.iBulletCnt = i;
		if(FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Bug_Bullet"), TEXT("Layer_Skill_Bug_Bullet"), &SkillDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void CBoss_Bug::Hp_State(_float fTimeDelta)
{
	switch (m_eMon_State)
	{
	case MON_STATE::SKILL_STATEA:
		if (m_pTimerCom->Time_Limit(fTimeDelta, 3.f))
			Bullet_Create();
		break;

	case MON_STATE::SKILL_STATEB:
		Skill_Dash(fTimeDelta);
		break;

	default:
		break;
	}
}

CBoss_Bug* CBoss_Bug::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss_Bug* pInstance = new CBoss_Bug(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Boss_Bug"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Bug::Clone(void* pArg)
{
	CBoss_Bug* pInstance = new CBoss_Bug(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Boss_Bug"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Bug::Free()
{
	__super::Free();

	Safe_Release(m_pKeyCom);
	Safe_Release(m_pTimerCom);
	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
