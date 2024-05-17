#include "Animator.h"

#include"Texture.h"
#include"GameInstance.h"

CAnimator::CAnimator(LPDIRECT3DDEVICE9 pGrpahic_Device)
	:CComponent { pGrpahic_Device }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

CAnimator::CAnimator(const CAnimator& pPrototype)
	:CComponent{ pPrototype }
	, m_pGameInstance{ pPrototype.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CAnimator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimator::Initialize(void* pArg)
{
	return S_OK;
}



HRESULT CAnimator::Add_Animator(_uint iLevel, const wstring& strComponentTag, const wstring& strTextureTag)
{ 
	CTexture* pTexture = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(iLevel, strComponentTag));

	if (pTexture == nullptr)
		return E_FAIL;

	m_pTexture.emplace(strTextureTag, pTexture);

	return S_OK;
}

HRESULT CAnimator::Play_Animator(const wstring& strTextureTag, _float fFrame , _float fTimeDelta , _bool isLoop)
{
	CTexture* pTexture = Find_Texture(strTextureTag);
	
	if (pTexture == nullptr)
		return E_FAIL;

	Move_Frame(fFrame , pTexture->m_iNumTextures,fTimeDelta , isLoop);

	return	pTexture->Bind_Anim(m_iIndex);
}

void CAnimator::Move_Frame(_float fFrame, _uint iNumTextures , _float fTimeDelta, _bool isLoop)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc >= (_float)(fFrame / iNumTextures))
	{
		m_fTimeAcc = 0.f;
		m_iIndex++;
	}

	if (m_iIndex >= iNumTextures && isLoop)
		m_iIndex = 0;
	
	else if (!isLoop)
		m_iIndex = iNumTextures - 1;
	
}

CTexture* CAnimator::Find_Texture(const wstring& strTextureTag)
{
	auto	iter = m_pTexture.find(strTextureTag);

	if(iter == m_pTexture.end())
		return nullptr;

	return iter->second;
}

CAnimator* CAnimator::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAnimator* pInstance = new CAnimator(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : Animator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAnimator::Clone(void* pArg)
{
	CAnimator* pInstance = new CAnimator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : Animator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimator::Free()
{
	__super::Free();

	for (auto& Pair : m_pTexture)
		Safe_Release(Pair.second);

	m_pTexture.clear();

	Safe_Release(m_pGameInstance);
}