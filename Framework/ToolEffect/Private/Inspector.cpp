// Default
#include "pch.h"
#include "Inspector.h"
#include "StringUtility.h"

// Handler
#include "EngineModule.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "ModelHandler.h"
#include "ShaderHandler.h"
#include "TextureHandler.h"

// Level, Window
#include "ToolEffectLevel.h"
#include "ObjectList.h"

// GameObject
#include "ParticleObject.h"
#include "EffectMeshObject.h"
#include "InstanceEffectMeshObject.h"

// Component
#include "CameraComponent.h"
#include "ParticleComponent.h"
#include "EffectMeshComponent.h"
#include "InstanceEffectMeshComponent.h"

HRESULT ToolEffect::CInspector::Initialize()
{
	m_tParticleInfo.m_vRadius = Vector2(0.f, 0.f);

	m_tEffectMeshInfo.m_vScale						= Vector3(1.f, 1.f, 1.f);
	m_tInstanceEffectMeshEffectMeshInfo.m_vScale	= Vector3(1.f, 1.f, 1.f);

	return CWindow::Initialize();
}
HRESULT ToolEffect::CInspector::BeginPlay()
{
	return CWindow::BeginPlay();
}
int32 ToolEffect::CInspector::PreTick(float _fDeltaSeconds)
{
	return CWindow::PreTick(_fDeltaSeconds);
}
int32 ToolEffect::CInspector::Tick(float _fDeltaSeconds)
{
	if (ImGui::Begin("Effect", &m_bInspector))
	{
		EffectTab();

		ImGui::End();
	}

	if (CEngineModule::GetInstance()->GetInputManager()->IsKeyDown(DIK_T))
	{
		CEngineModule::GetInstance()->GetCameraManager()->GetCurCamera()->GetOwnerGameObject()->SetPosition(Vector3(0.f, 0.f, 3.f));
		CEngineModule::GetInstance()->GetCameraManager()->GetCurCamera()->GetOwnerGameObject()->LookAt(Vector3(0.f, 0.f, 0.f));
	}

	return CWindow::Tick(_fDeltaSeconds);
}
int32 ToolEffect::CInspector::PostTick(float _fDeltaSeconds)
{
	return CWindow::PostTick(_fDeltaSeconds);
}
int32 ToolEffect::CInspector::Render()
{
	return CWindow::Render();
}
int32 ToolEffect::CInspector::EndPlay()
{
	return CWindow::EndPlay();
}
void ToolEffect::CInspector::Release()
{
	CWindow::Release();
}

void ToolEffect::CInspector::EffectTab()
{
	ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("EffectTabBar", tabBarFlags))
	{
		if (ImGui::BeginTabItem("Particle"))
		{
			m_wpObjectList.lock()->SetCurEffectType(EEffectType::PARTICLE);
			ParticleTab();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("EffectMesh"))
		{
			m_wpObjectList.lock()->SetCurEffectType(EEffectType::EFFECTMESH);
			EffectMeshTab();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("InstanceEffectMesh"))
		{
			m_wpObjectList.lock()->SetCurEffectType(EEffectType::InstanceEffectMesh);
			InstanceEffectMeshTab();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

void ToolEffect::CInspector::ParticleTab()
{
	ParticleInfo();
	CreateParticle();
	SaveParticleToJSON();
	CreateParticleFromJson();
}
void ToolEffect::CInspector::ParticleInfo()
{
	{
		/* 이펙트 셰이더 */
		ImGui::SeparatorText("Shader");
		SelectEffectShader(EEffectType::PARTICLE);
		ImGui::Text(CStringUtility::ToString(m_tParticleInfo.m_wstrEffectShaderName).c_str());
	}

	ImGui::SeparatorText("Type");
	{
		/* 렌더타입 */// -> 추후에 렌더타입이 추가될 경우 수정될 수 있습니다.
		array<const char*, (int8)ERenderType::ENUM_END> arrRenderType = { "NONE",
			"RENDER_PRE",
			"RENDER_NBLEND",
			"RENDER_LBLEND",
			"RENDER_PBR",
			"RENDER_BLEND",
			"RENDER_UI",
			"RENDER_POST",
			"RENDER_DEBUG",
			"RENDER_GLOW",
			"RENDER_DISTORTIN"};

		switch (m_iParticleRenderType)
		{
		case 0: m_tParticleInfo.m_eRenderType = ERenderType::NONE; break;
		case 1: m_tParticleInfo.m_eRenderType = ERenderType::RENDER_PRE; break;
		case 2: m_tParticleInfo.m_eRenderType = ERenderType::RENDER_NBLEND; break;
		case 3: m_tParticleInfo.m_eRenderType = ERenderType::RENDER_LBLEND; break;
		case 4: m_tParticleInfo.m_eRenderType = ERenderType::RENDER_PBR; break;
		case 5: m_tParticleInfo.m_eRenderType = ERenderType::RENDER_BLEND; break;
		case 6: m_tParticleInfo.m_eRenderType = ERenderType::RENDER_UI; break;
		case 7: m_tParticleInfo.m_eRenderType = ERenderType::RENDER_POST; break;
		case 8: m_tParticleInfo.m_eRenderType = ERenderType::RENDER_DEBUG; break;
		case 9: m_tParticleInfo.m_eRenderType = ERenderType::RENDER_GLOW; break;
		case 10: m_tParticleInfo.m_eRenderType = ERenderType::RENDER_DISTORTION; break;
		default: break;
		}

		ImGui::Combo("Particle RenderType", &m_iParticleRenderType, arrRenderType.data(), (int32)arrRenderType.size());
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 파티클 타입 */
		array<const char*, (int8)EParticleType::ENUM_END> arrParticleType = { "DROP", "SPREAD", "ERUPTION", "CHARGE", "ORTHOGONAL", "REVOLUTION", "FOLLOW", "TONADO", "GROUNDED", "ALPHADECREASE", "SPRAY" };
		ImGui::Combo("Particle Type", &m_iParticleType, arrParticleType.data(), (int32)arrParticleType.size());

		switch (m_iParticleType)
		{
		case 0: m_tParticleInfo.m_eParticleType = EParticleType::DROP; break;
		case 1: m_tParticleInfo.m_eParticleType = EParticleType::SPREAD; break;
		case 2: m_tParticleInfo.m_eParticleType = EParticleType::ERUPTION; break;
		case 3: m_tParticleInfo.m_eParticleType = EParticleType::CHARGE; break;
		case 4: m_tParticleInfo.m_eParticleType = EParticleType::ORTHOGONAL; break;
		case 5: m_tParticleInfo.m_eParticleType = EParticleType::REVOLUTION; break;
		case 6: m_tParticleInfo.m_eParticleType = EParticleType::FOLLOW; break;
		case 7: m_tParticleInfo.m_eParticleType = EParticleType::TONADO; break;
		case 8: m_tParticleInfo.m_eParticleType = EParticleType::GROUNDED; break;
		case 9: m_tParticleInfo.m_eParticleType = EParticleType::ALPHADECREASE; break;
		case 10:m_tParticleInfo.m_eParticleType = EParticleType::SPRAY; break;
		default: break;
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));
	}


	ImGui::SeparatorText("Texture");
	{
		/* 디퓨즈 텍스처 */
		ImGui::Checkbox("Use Diffuse", &m_tParticleInfo.m_bDiffuse);
		if(m_tParticleInfo.m_bDiffuse)
		{
			SelectDiffuseTexture(EEffectType::PARTICLE);
		}
		else
		{
			ImGui::ColorEdit4("Particle Color1", &m_tParticleInfo.m_vColor1.x);
			ImGui::ColorEdit4("Particle Color2", &m_tParticleInfo.m_vColor2.x);
			ImGui::ColorEdit4("Particle Color3", &m_tParticleInfo.m_vColor3.x);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 마스크 텍스처 */
		ImGui::Checkbox("Use Mask", &m_tParticleInfo.m_bMask);
		if (m_tParticleInfo.m_bMask)
		{
			SelectMaskTexture(EEffectType::PARTICLE);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));
	}


	ImGui::SeparatorText("Base Info");
	{
		/* 루프 */
		ImGui::Checkbox("Loop", &m_tParticleInfo.m_bLoop);
		ImGui::SameLine();

		/* 빌보드 */
		ImGui::Checkbox("Billboard", &m_tParticleInfo.m_bBillboard);
		ImGui::SameLine();

		/* 방향성 */
		ImGui::Checkbox("Directional", &m_tParticleInfo.m_bDirectional);
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 스프라이트 모드 */
		ImGui::Checkbox("Sprite Mode", &m_tParticleInfo.m_bSprite);
		if (m_tParticleInfo.m_bSprite)
		{
			ImGui::InputInt("Row", &m_tParticleInfo.m_iSpriteRow);
			if (1 > m_tParticleInfo.m_iSpriteRow)
				m_tParticleInfo.m_iSpriteRow = 1;

			ImGui::InputInt("Col", &m_tParticleInfo.m_iSpriteCol);
			if (1 > m_tParticleInfo.m_iSpriteCol)
				m_tParticleInfo.m_iSpriteCol = 1;

			ImGui::InputFloat("Sprite Speed", &m_tParticleInfo.m_fSpriteSpeed);
			if (0.f > m_tParticleInfo.m_fSpriteSpeed)
				m_tParticleInfo.m_fSpriteSpeed = 0.f;
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 중력 */
		ImGui::Checkbox("Gravity", &m_tParticleInfo.m_bGravity);
		if (m_tParticleInfo.m_bGravity)
		{
			ImGui::SameLine();
			ImGui::InputFloat("GravityValue", &m_tParticleInfo.m_fGravityValue);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 글로우 */
		ImGui::Checkbox("Glow", &m_tParticleInfo.m_bGlow);
		if (m_tParticleInfo.m_bGlow)
		{
			ImGui::SameLine();
			ImGui::InputFloat("GlowWeight", &m_tParticleInfo.m_fGlowWeight);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 블룸 */
		ImGui::Checkbox("Bloom", &m_tParticleInfo.m_bBloom);
		if (m_tParticleInfo.m_bBloom)
		{
			ImGui::SameLine();
			ImGui::InputFloat("BloomWeight", &m_tParticleInfo.m_fBloomWeight);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 알파 감쇄 */
		ImGui::Checkbox("AlphaAttenuation", &m_tParticleInfo.m_bAlphaAttenuation);
		if (m_tParticleInfo.m_bAlphaAttenuation)
		{
			ImGui::SameLine();
			ImGui::InputFloat("AlphaAttenuationWeight", &m_tParticleInfo.m_fAlphaAttenuationWeight);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 인스턴스 개수 */
		ImGui::InputInt("NumInstance", &m_tParticleInfo.m_iNumInstance);
		if (0 >= m_tParticleInfo.m_iNumInstance)
			m_tParticleInfo.m_iNumInstance = 1;

		ImGui::Checkbox("Random Life Time", &m_tParticleInfo.m_bRandomLifeTime);

		/* 라이프 타임*/
		ImGui::InputFloat2("Life Time", &m_tParticleInfo.m_vLifeTime.x);

		/* 크기 */
		ImGui::InputFloat2("Size", &m_tParticleInfo.m_vSize.x);

		/* 진행 속도 */
		ImGui::InputFloat2("Speed", &m_tParticleInfo.m_vSpeed.x);

		/* 주체로부터 떨어질 위치 */
		ImGui::InputFloat3("Offset", &m_tParticleInfo.m_vOffset.x);

		/* 생성될 위치 */
		// ImGui::InputFloat3("Center", &m_tParticleInfo.m_vCenter.x);

		/* 피벗 */
		ImGui::InputFloat3("Pivot", &m_tParticleInfo.m_vPivot.x);

		/* 생성 범위 x,y,z */
		ImGui::InputFloat3("Range", &m_tParticleInfo.m_vRange.x);

		/* 초당 회전 정도 */
		ImGui::InputFloat3("Rotation", &m_tParticleInfo.m_vRotation.x);

		/* 최소, 최대 생성반경 */
		ImGui::InputFloat2("Radius", &m_tParticleInfo.m_vRadius.x);

		/* 이름 갱신 */
		m_strParticleObjectName = m_chParticleObjectName;
	}

	/* 정보 갱신 */
	UpdateCurParticleInfo();

	ImGui::Dummy(ImVec2(0.f, 5.f));
}
void ToolEffect::CInspector::UpdateCurParticleInfo()
{
	shared_ptr<CParticleObject> spParticle = m_wpObjectList.lock()->GetParticleObject();
	if (!spParticle) { return; }

	// ParticleInfo에서 실시간으로 변하면 안되는 값들을 저장합니다.
	int32 iPrevNumInstance = spParticle->GetParticleCom()->GetParticleInfo().m_iNumInstance;

	// ParticleInfo를 갱신해줍니다.
	spParticle->GetParticleCom()->SetParticleInfo(m_tParticleInfo);

	// 갱신한 값에서 변하면 안되는 값들을 되돌려줍니다.
	spParticle->GetParticleCom()->GetParticleInfo().m_iNumInstance = iPrevNumInstance;

	// 셰이더와 텍스처를 갱신해줍니다.
	spParticle->GetParticleCom()->SetRenderType(m_tParticleInfo.m_eRenderType);
	spParticle->GetParticleCom()->SetEffectShader(m_tParticleInfo.m_wstrEffectShaderName);
	spParticle->GetParticleCom()->SetTexture(m_tParticleInfo.m_wstrDiffuseTextureName, ETextureType::DIFFUSE);
	spParticle->GetParticleCom()->SetTexture(m_tParticleInfo.m_wstrMaskTextureName, ETextureType::MASK);
}
void ToolEffect::CInspector::CreateParticle()
{
	/* 이름 */
	ImGui::SeparatorText("Object Name");
	ImGui::InputTextWithHint("ObjectName", "ObjectName", m_chParticleObjectName, MAX_PATH);

	if (ImGui::Button("Create Particle", ImVec2(120.f, 40.f)))
	{
		CParticleObject::FParticleObjectDesc tParticleObjectDesc = {};

		tParticleObjectDesc.m_wstrGameObjectName		= CStringUtility::ToWideString(m_chParticleObjectName);
		tParticleObjectDesc.m_eEffectType				= EEffectType::PARTICLE;

		shared_ptr<CParticleObject> spParticleObject	= CParticleObject::Create(make_shared<CParticleObject::FParticleObjectDesc>(tParticleObjectDesc), m_tParticleInfo);
		CHECKF(spParticleObject, L"Failed to Create ParticleObject : CInspector");

		m_wpToolEffectLevel.lock()->AddGameObject(spParticleObject);
		m_wpToolEffectLevel.lock()->AddGameObjectToLayer(L"Particle", spParticleObject);

		m_wpObjectList.lock()->SetCurObject(spParticleObject);

		m_tParticleInfo = spParticleObject->GetParticleCom()->GetParticleInfo();
	}
	ImGui::SameLine();
}
void ToolEffect::CInspector::CreateParticleFromJson()
{
	ImGui::SeparatorText("Json");
	ImGui::InputTextWithHint("Json File", "Particle Json File Name", m_chParticleJsonFileName, MAX_PATH);

	if (ImGui::Button("Create From Json", ImVec2(120.f, 40.f)))
	{
		CParticleObject::FParticleObjectDesc tParticleObjectDesc = {};

		tParticleObjectDesc.m_wstrGameObjectName		= CStringUtility::ToWideString(m_chParticleJsonFileName);
		tParticleObjectDesc.m_eEffectType				= EEffectType::PARTICLE;

		shared_ptr<CParticleObject> spParticleObject	= CParticleObject::CreateFromJson(make_shared<CParticleObject::FParticleObjectDesc>(tParticleObjectDesc), CStringUtility::ToWideString(m_chParticleJsonFileName));
		CHECKF(spParticleObject, L"Failed to Create ParticleObject : CInspector");

		m_wpToolEffectLevel.lock()->AddGameObject(spParticleObject);
		m_wpToolEffectLevel.lock()->AddGameObjectToLayer(L"Particle", spParticleObject);

		m_wpObjectList.lock()->SetCurObject(spParticleObject);

		m_tParticleInfo = spParticleObject->GetParticleCom()->GetParticleInfo();
	}
}

void ToolEffect::CInspector::EffectMeshTab()
{
	EffectMeshInfo();
	CreateEffectMesh();
	SaveEffectMeshToJSON(m_tEffectMeshInfo, m_chEffectMeshObjectName);
	CreateEffectMeshFromJson();
}
void ToolEffect::CInspector::EffectMeshInfo()
{
	{
		/* 모델 파일 경로 */
		ImGui::SeparatorText("Mesh");
		SelectEffectMesh(EEffectType::EFFECTMESH);

		/* 이펙트 셰이더 */
		ImGui::SeparatorText("Shader");
		SelectEffectShader(EEffectType::EFFECTMESH);
		ImGui::Text(CStringUtility::ToString(m_tEffectMeshInfo.m_wstrEffectShaderName).c_str());

		/* S R T */
		ImGui::SeparatorText("Local Transform");

		ImGui::InputFloat3("Scale", &m_tEffectMeshInfo.m_vScale.x);
		ImGui::InputFloat3("Rotate", &m_tEffectMeshInfo.m_vRotation.x);
		ImGui::InputFloat3("Translate", &m_tEffectMeshInfo.m_vTranslation.x);
	}

	ImGui::SeparatorText("Type");
	{
		/* 렌더타입 */// -> 추후에 렌더타입이 추가될 경우 수정될 수 있습니다.
		array<const char*, (int8)ERenderType::ENUM_END> arrRenderType = { "NONE",
			"RENDER_PRE",
			"RENDER_NBLEND",
			"RENDER_LBLEND",
			"RENDER_PBR",
			"RENDER_BLEND",
			"RENDER_UI",
			"RENDER_POST", 
			"RENDER_DEBUG",
			"RENDER_GLOW",
			"RENDER_DISTORTION"};

		switch (m_iEffectMeshRenderType)
		{
		case 0: m_tEffectMeshInfo.m_eRenderType = ERenderType::NONE; break;
		case 1: m_tEffectMeshInfo.m_eRenderType = ERenderType::RENDER_PRE; break;
		case 2: m_tEffectMeshInfo.m_eRenderType = ERenderType::RENDER_NBLEND; break;
		case 3: m_tEffectMeshInfo.m_eRenderType = ERenderType::RENDER_LBLEND; break;
		case 4: m_tEffectMeshInfo.m_eRenderType = ERenderType::RENDER_PBR; break;
		case 5: m_tEffectMeshInfo.m_eRenderType = ERenderType::RENDER_BLEND; break;
		case 6: m_tEffectMeshInfo.m_eRenderType = ERenderType::RENDER_UI; break;
		case 7: m_tEffectMeshInfo.m_eRenderType = ERenderType::RENDER_POST; break;
		case 8: m_tEffectMeshInfo.m_eRenderType = ERenderType::RENDER_DEBUG; break;
		case 9: m_tEffectMeshInfo.m_eRenderType = ERenderType::RENDER_GLOW; break;
		case 10: m_tEffectMeshInfo.m_eRenderType = ERenderType::RENDER_DISTORTION; break;
		default: break;
		}

		ImGui::Combo("EffectMesh RenderType", &m_iEffectMeshRenderType, arrRenderType.data(), (int32)arrRenderType.size());
		ImGui::Dummy(ImVec2(0.f, 5.f));
	}

	ImGui::Checkbox("Mesh Trail", &m_tEffectMeshInfo.m_bTrail);
	if(m_tEffectMeshInfo.m_bTrail)
	{
		ImGui::InputInt("Num Trails", &m_tEffectMeshInfo.m_iNumTrails);
		ImGui::InputFloat("Trail Term", &m_tEffectMeshInfo.m_fTrailTerm);
	}

	ImGui::SeparatorText("Texture");
	{
		/* 디퓨즈 텍스처 */
		ImGui::Checkbox("Diffuse", &m_tEffectMeshInfo.m_bDiffuse);
		if (m_tEffectMeshInfo.m_bDiffuse)
		{
			SelectDiffuseTexture(EEffectType::EFFECTMESH);
			ImGui::Text(CStringUtility::ToString(m_tEffectMeshInfo.m_wstrDiffuseTextureName).c_str());
		}
		else
		{
			ImGui::ColorEdit4("Effect Mesh Color1", &m_tEffectMeshInfo.m_vDiffuseColor1.x);
			ImGui::ColorEdit4("Effect Mesh Color2", &m_tEffectMeshInfo.m_vDiffuseColor2.x);
			ImGui::ColorEdit4("Effect Mesh Color3", &m_tEffectMeshInfo.m_vDiffuseColor3.x);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 노이즈 텍스처 */
		ImGui::Checkbox("Noise", &m_tEffectMeshInfo.m_bNoise);
		if (m_tEffectMeshInfo.m_bNoise)
		{
			SelectNoiseTexture(EEffectType::EFFECTMESH);
			ImGui::Text(CStringUtility::ToString(m_tEffectMeshInfo.m_wstrNoiseTextureName).c_str());
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 마스크 텍스처 */
		ImGui::Checkbox("Mask", &m_tEffectMeshInfo.m_bMask);
		if (m_tEffectMeshInfo.m_bMask)
		{
			SelectMaskTexture(EEffectType::EFFECTMESH);
			ImGui::Text(CStringUtility::ToString(m_tEffectMeshInfo.m_wstrMaskTextureName).c_str());
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 디스토션 */
		ImGui::Checkbox("Distortion", &m_tEffectMeshInfo.m_bDistortion);
		if (m_tEffectMeshInfo.m_bDistortion)
		{
			SelectDistortionTexture(EEffectType::EFFECTMESH);
			ImGui::Text(CStringUtility::ToString(m_tEffectMeshInfo.m_wstrDistortionTextureName).c_str());
			ImGui::InputFloat("Distortion Weight", &m_tEffectMeshInfo.m_fDistortionWeight);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 디졸브 */
		ImGui::Checkbox("Dissolve", &m_tEffectMeshInfo.m_bDissolve);
		if (m_tEffectMeshInfo.m_bDissolve)
		{
			SelectDissolveTexture(EEffectType::EFFECTMESH);
			ImGui::Text(CStringUtility::ToString(m_tEffectMeshInfo.m_wstrDissolveTextureName).c_str());
			ImGui::InputFloat("Dissolve Weight", &m_tEffectMeshInfo.m_fDissolveWeight);
		}
	}

	ImGui::SeparatorText("Option");
	{
		/* 애님 여부 */
		ImGui::Checkbox("Anim", &m_tEffectMeshInfo.m_bAnimation);

		/* 루프 여부 */
		ImGui::Checkbox("Loop", &m_tEffectMeshInfo.m_bLoop);

		ImGui::SeparatorText("Base Info");
		/* 라이프 타임 */
		ImGui::InputFloat2("LifeTime", &m_tEffectMeshInfo.m_vLifeTime.x);

		/* 스프라이트 모드 */
		ImGui::Checkbox("Sprite Mode", &m_tEffectMeshInfo.m_bSprite);
		if (m_tEffectMeshInfo.m_bSprite)
		{
			ImGui::InputInt("Row", &m_tEffectMeshInfo.m_iSpriteRow);
			if (1 > m_tEffectMeshInfo.m_iSpriteRow)
				m_tEffectMeshInfo.m_iSpriteRow = 1;

			ImGui::InputInt("Col", &m_tEffectMeshInfo.m_iSpriteCol);
			if (1 > m_tEffectMeshInfo.m_iSpriteCol)
				m_tEffectMeshInfo.m_iSpriteCol = 1;

			ImGui::InputFloat("Sprite Speed", &m_tEffectMeshInfo.m_fSpriteSpeed);
			if (0.f > m_tEffectMeshInfo.m_fSpriteSpeed)
				m_tEffectMeshInfo.m_fSpriteSpeed = 0.f;
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* UVMove */
		ImGui::Checkbox("UVMove", &m_tEffectMeshInfo.m_bUVMoveLoop);
		ImGui::InputFloat2("UVMoveSpeed", &m_tEffectMeshInfo.m_vUVMoveSpeed.x);

		/* 글로우 */
		ImGui::Checkbox("Glow", &m_tEffectMeshInfo.m_bGlow);
		ImGui::InputFloat("Glow Weight", &m_tEffectMeshInfo.m_fGlowWeight);

		/* 블룸 */
		ImGui::Checkbox("Bloom", &m_tEffectMeshInfo.m_bBloom);
		ImGui::InputFloat("Bloom Weight", &m_tEffectMeshInfo.m_fBloomWeight);

		/* 알파 감쇄 */
		ImGui::Checkbox("Alpha Attenuation", &m_tEffectMeshInfo.m_bAlphaAttenuation);
		ImGui::InputFloat("Alpha Attenuation Weight", &m_tEffectMeshInfo.m_fAlphaAttenuationWeight);

		/* 알파 자르는 기준 값 */
		ImGui::InputFloat("Alpha Discard Value", &m_tEffectMeshInfo.m_fAlphaDiscardValue);

		/* 이름 갱신 */
		m_strEffectMeshObjectName = m_chEffectMeshObjectName;
	}

	/* 정보 갱신 */
	UpdateCurEffectMeshInfo();

	ImGui::Dummy(ImVec2(0.f, 5.f));
}
void ToolEffect::CInspector::UpdateCurEffectMeshInfo()
{
	if (nullptr == m_wpObjectList.lock()->GetEffectBase()) { return; }

	shared_ptr<CEffectMeshObject> spEffect = m_wpObjectList.lock()->GetEffectMeshObject();
	if (!spEffect) { return; }

	// EffectMeshInfo 갱신
	spEffect->GetEffectMeshCom()->SetEffectMeshInfo(m_tEffectMeshInfo);

	// S / R / T 갱신
	spEffect->SetScale(m_tEffectMeshInfo.m_vScale);
	spEffect->SetRotate(m_tEffectMeshInfo.m_vRotation);
	// spEffect->SetRotate(Vector3(0.f, 1.f, 0.f), m_tEffectMeshInfo.m_vRotation.y);
	// spEffect->SetRotate(Vector3(0.f, 0.f, 1.f), m_tEffectMeshInfo.m_vRotation.z);
	spEffect->SetPosition(m_tEffectMeshInfo.m_vTranslation);

	// 셰이더, 텍스처 갱신
	spEffect->GetEffectMeshCom()->SetRenderType(m_tEffectMeshInfo.m_eRenderType);
	spEffect->GetEffectMeshCom()->SetEffectShader(m_tEffectMeshInfo.m_wstrEffectShaderName);
	spEffect->GetEffectMeshCom()->SetTexture(m_tEffectMeshInfo.m_wstrDiffuseTextureName, ETextureType::DIFFUSE);
	spEffect->GetEffectMeshCom()->SetTexture(m_tEffectMeshInfo.m_wstrNoiseTextureName, ETextureType::NOISE);
	spEffect->GetEffectMeshCom()->SetTexture(m_tEffectMeshInfo.m_wstrMaskTextureName, ETextureType::MASK);
	spEffect->GetEffectMeshCom()->SetTexture(m_tEffectMeshInfo.m_wstrDistortionTextureName, ETextureType::DISTORTION);
	spEffect->GetEffectMeshCom()->SetTexture(m_tEffectMeshInfo.m_wstrDissolveTextureName, ETextureType::DISSOLVE);
}
void ToolEffect::CInspector::CreateEffectMesh()
{
	/* 이름 */
	ImGui::SeparatorText("Object Name");
	ImGui::InputTextWithHint("#ObjectName", "ObjectName", m_chEffectMeshObjectName, MAX_PATH);

	if (ImGui::Button("Create Effect", ImVec2(100.f, 40.f)))
	{
		CEffectMeshObject::FEffectMeshObjectDesc tEffectObjectDesc = {};

		tEffectObjectDesc.m_wstrGameObjectName = CStringUtility::ToWideString(m_chEffectMeshObjectName);
		tEffectObjectDesc.m_eEffectType = EEffectType::EFFECTMESH;

		tEffectObjectDesc.m_wstrJsonFileName = L"";
		tEffectObjectDesc.m_wstrModelFilePath = m_tEffectMeshInfo.m_wstrModelFilePath;

		tEffectObjectDesc.m_vInitScale = m_tEffectMeshInfo.m_vScale;
		tEffectObjectDesc.m_vInitRotate = m_tEffectMeshInfo.m_vRotation;
		tEffectObjectDesc.m_vInitPosition = m_tEffectMeshInfo.m_vTranslation;

		tEffectObjectDesc.m_matPivot = Matrix::CreateScale(0.01f) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0.f, 1.f, 0.f), XMConvertToRadians(180.f))) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(1.f, 0.f, 0.f), XMConvertToRadians(-90.f)));

		shared_ptr<CEffectMeshObject> spEffectObject = CEffectMeshObject::Create(make_shared<CEffectMeshObject::FEffectMeshObjectDesc>(tEffectObjectDesc), m_tEffectMeshInfo);
		CHECKF(spEffectObject, L"Failed to Create EffectObject : CInspector");

		m_wpToolEffectLevel.lock()->AddGameObject(spEffectObject);
		m_wpToolEffectLevel.lock()->AddGameObjectToLayer(L"Effect Mesh", spEffectObject);

		m_tEffectMeshInfo = spEffectObject->GetEffectMeshCom()->GetEffectMeshInfo();

		m_wpObjectList.lock()->SetCurObject(spEffectObject);
	}
}
void ToolEffect::CInspector::CreateEffectMeshFromJson()
{
	ImGui::SeparatorText("Json");
	ImGui::InputTextWithHint("Json File Name", "Effect Mesh Json File Name", m_chEffectMeshJsonFileName, MAX_PATH);

	if (ImGui::Button("Create From Json", ImVec2(100.f, 40.f)))
	{
		CEffectMeshObject::FEffectMeshObjectDesc tEffectObjectDesc = {};

		tEffectObjectDesc.m_wstrGameObjectName = CStringUtility::ToWideString(m_chEffectMeshJsonFileName);
		tEffectObjectDesc.m_eEffectType = EEffectType::EFFECTMESH;

		tEffectObjectDesc.m_wstrJsonFileName = CStringUtility::ToWideString(m_chEffectMeshJsonFileName);
		tEffectObjectDesc.m_wstrModelFilePath = L"";

		tEffectObjectDesc.m_vInitScale = Vector3(1.f, 1.f, 1.f);
		tEffectObjectDesc.m_vInitRotate = Vector3(0.f, 0.f, 0.f);
		tEffectObjectDesc.m_vInitPosition = Vector3(0.f, 0.f, 0.f);

		tEffectObjectDesc.m_matPivot = Matrix::CreateScale(0.01f) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0.f, 1.f, 0.f), XMConvertToRadians(180.f))) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(1.f, 0.f, 0.f), XMConvertToRadians(-90.f)));

		shared_ptr<CEffectMeshObject> spEffectObject = CEffectMeshObject::CreateFromJson(make_shared<CEffectMeshObject::FEffectMeshObjectDesc>(tEffectObjectDesc), CStringUtility::ToWideString(m_chEffectMeshJsonFileName));
		CHECKF(spEffectObject, L"Failed to CreateFromJson EffectObject : CInspector");

		m_wpToolEffectLevel.lock()->AddGameObject(spEffectObject);
		m_wpToolEffectLevel.lock()->AddGameObjectToLayer(L"Effect Mesh", spEffectObject);

		m_tEffectMeshInfo = spEffectObject->GetEffectMeshCom()->GetEffectMeshInfo();

		m_wpObjectList.lock()->SetCurObject(spEffectObject);
	}
}

void ToolEffect::CInspector::InstanceEffectMeshTab()
{
	InstanceEffectMeshsEffectMeshInfo();
	CreateInstanceEffectMesh();
	SaveInstanceEffectMeshToJSON(m_tInstanceEffectMeshEffectMeshInfo, m_tInstanceEffectMeshInfo, m_chInstanceEffectMeshObjectName);
	CreateInstanceEffectMeshFromJson();
}
void ToolEffect::CInspector::InstanceEffectMeshsEffectMeshInfo()
{
	{
		/* 모델 파일 경로 */
		ImGui::SeparatorText("Mesh");
		SelectEffectMesh(EEffectType::InstanceEffectMesh);
		ImGui::Text(CStringUtility::ToString(m_tInstanceEffectMeshEffectMeshInfo.m_wstrModelFilePath).c_str());

		/* 이펙트 셰이더 */
		ImGui::SeparatorText("Shader");
		SelectEffectShader(EEffectType::InstanceEffectMesh);
		ImGui::Text(CStringUtility::ToString(m_tInstanceEffectMeshEffectMeshInfo.m_wstrEffectShaderName).c_str());

		/* S R T */
		ImGui::SeparatorText("Local Transform");

		ImGui::InputFloat3("Scale", &m_tInstanceEffectMeshEffectMeshInfo.m_vScale.x);
		ImGui::InputFloat3("Rotate", &m_tInstanceEffectMeshEffectMeshInfo.m_vRotation.x);
		ImGui::InputFloat3("Translate", &m_tInstanceEffectMeshEffectMeshInfo.m_vTranslation.x);
	}

	ImGui::SeparatorText("Type");
	{
		/* 렌더타입 */
		array<const char*, (int8)ERenderType::ENUM_END> arrRenderType = { "NONE",
			"RENDER_PRE",
			"RENDER_NBLEND",
			"RENDER_LBLEND",
			"RENDER_PBR",
			"RENDER_BLEND",
			"RENDER_UI",
			"RENDER_POST",
			"RENDER_DEBUG",
			"RENDER_GLOW",
			"RENDER_DISTORTION"};

		switch (m_iInstanceEffectMeshRenderType)
		{
		case 0: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::NONE; break;
		case 1: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::RENDER_PRE; break;
		case 2: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::RENDER_NBLEND; break;
		case 3: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::RENDER_LBLEND; break;
		case 4: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::RENDER_PBR; break;
		case 5: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::RENDER_BLEND; break;
		case 6: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::RENDER_UI; break;
		case 7: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::RENDER_POST; break;
		case 8: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::RENDER_DEBUG; break;
		case 9: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::RENDER_GLOW; break;
		case 10: m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType = ERenderType::RENDER_DISTORTION; break;
		default: break;
		}

		ImGui::Combo("InstanceEffectMesh RenderType", &m_iInstanceEffectMeshRenderType, arrRenderType.data(), (int32)arrRenderType.size());
		ImGui::Dummy(ImVec2(0.f, 5.f));
	}


	ImGui::SeparatorText("Texture");
	{
		/* 디퓨즈 텍스처 */
		ImGui::Checkbox("Diffuse", &m_tInstanceEffectMeshEffectMeshInfo.m_bDiffuse);
		if (m_tInstanceEffectMeshEffectMeshInfo.m_bDiffuse)
		{
			SelectDiffuseTexture(EEffectType::InstanceEffectMesh);
			ImGui::Text(CStringUtility::ToString(m_tInstanceEffectMeshEffectMeshInfo.m_wstrDiffuseTextureName).c_str());
		}
		else
		{
			ImGui::ColorEdit4("Instance Mesh Color1", &m_tInstanceEffectMeshEffectMeshInfo.m_vDiffuseColor1.x);
			ImGui::ColorEdit4("Instance Mesh Color2", &m_tInstanceEffectMeshEffectMeshInfo.m_vDiffuseColor2.x);
			ImGui::ColorEdit4("Instance Mesh Color3", &m_tInstanceEffectMeshEffectMeshInfo.m_vDiffuseColor3.x);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 노이즈 텍스처 */
		ImGui::Checkbox("Noise", &m_tInstanceEffectMeshEffectMeshInfo.m_bNoise);
		if (m_tInstanceEffectMeshEffectMeshInfo.m_bNoise)
		{
			SelectNoiseTexture(EEffectType::InstanceEffectMesh);
			ImGui::Text(CStringUtility::ToString(m_tInstanceEffectMeshEffectMeshInfo.m_wstrNoiseTextureName).c_str());
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 마스크 텍스처 */
		ImGui::Checkbox("Mask", &m_tInstanceEffectMeshEffectMeshInfo.m_bMask);
		if (m_tInstanceEffectMeshEffectMeshInfo.m_bMask)
		{
			SelectMaskTexture(EEffectType::InstanceEffectMesh);
			ImGui::Text(CStringUtility::ToString(m_tInstanceEffectMeshEffectMeshInfo.m_wstrMaskTextureName).c_str());
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 디스토션 */
		ImGui::Checkbox("Distortion", &m_tInstanceEffectMeshEffectMeshInfo.m_bDistortion);
		if (m_tInstanceEffectMeshEffectMeshInfo.m_bDistortion)
		{
			SelectDistortionTexture(EEffectType::InstanceEffectMesh);
			ImGui::Text(CStringUtility::ToString(m_tInstanceEffectMeshEffectMeshInfo.m_wstrDistortionTextureName).c_str());
			ImGui::InputFloat("Distortion Weight", &m_tInstanceEffectMeshEffectMeshInfo.m_fDistortionWeight);
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* 디졸브 */
		ImGui::Checkbox("Dissolve", &m_tInstanceEffectMeshEffectMeshInfo.m_bDissolve);
		if (m_tInstanceEffectMeshEffectMeshInfo.m_bDissolve)
		{
			SelectDissolveTexture(EEffectType::InstanceEffectMesh);
			ImGui::Text(CStringUtility::ToString(m_tInstanceEffectMeshEffectMeshInfo.m_wstrDissolveTextureName).c_str());
			ImGui::InputFloat("Dissolve Weight", &m_tInstanceEffectMeshEffectMeshInfo.m_fDissolveWeight);
		}
	}

	ImGui::SeparatorText("Option");
	{
		/* 인스턴스 메시 Info */
		InstanceEffectMeshInfo();

		/* 애님 여부 */
		ImGui::Checkbox("Anim", &m_tInstanceEffectMeshEffectMeshInfo.m_bAnimation);

		/* 루프 여부 */
		ImGui::Checkbox("Loop", &m_tInstanceEffectMeshEffectMeshInfo.m_bLoop);

		ImGui::SeparatorText("Base Info");
		/* 라이프 타임 */
		ImGui::InputFloat2("LifeTime", &m_tInstanceEffectMeshEffectMeshInfo.m_vLifeTime.x);

		/* 스프라이트 모드 */
		ImGui::Checkbox("Sprite Mode", &m_tInstanceEffectMeshEffectMeshInfo.m_bSprite);
		if (m_tInstanceEffectMeshEffectMeshInfo.m_bSprite)
		{
			ImGui::InputInt("Row", &m_tInstanceEffectMeshEffectMeshInfo.m_iSpriteRow);
			if (1 > m_tInstanceEffectMeshEffectMeshInfo.m_iSpriteRow)
				m_tInstanceEffectMeshEffectMeshInfo.m_iSpriteRow = 1;

			ImGui::InputInt("Col", &m_tInstanceEffectMeshEffectMeshInfo.m_iSpriteCol);
			if (1 > m_tInstanceEffectMeshEffectMeshInfo.m_iSpriteCol)
				m_tInstanceEffectMeshEffectMeshInfo.m_iSpriteCol = 1;

			ImGui::InputFloat("Sprite Speed", &m_tInstanceEffectMeshEffectMeshInfo.m_fSpriteSpeed);
			if (0.f > m_tInstanceEffectMeshEffectMeshInfo.m_fSpriteSpeed)
				m_tInstanceEffectMeshEffectMeshInfo.m_fSpriteSpeed = 0.f;
		}
		ImGui::Dummy(ImVec2(0.f, 5.f));

		/* UVMove */
		ImGui::Checkbox("UVMove", &m_tInstanceEffectMeshEffectMeshInfo.m_bUVMoveLoop);
		ImGui::InputFloat2("UVMoveSpeed", &m_tInstanceEffectMeshEffectMeshInfo.m_vUVMoveSpeed.x);

		/* 글로우 */
		ImGui::Checkbox("Glow", &m_tInstanceEffectMeshEffectMeshInfo.m_bGlow);
		ImGui::InputFloat("Glow Weight", &m_tInstanceEffectMeshEffectMeshInfo.m_fGlowWeight);

		/* 블룸 */
		ImGui::Checkbox("Bloom", &m_tInstanceEffectMeshEffectMeshInfo.m_bBloom);
		ImGui::InputFloat("Bloom Weight", &m_tInstanceEffectMeshEffectMeshInfo.m_fBloomWeight);

		/* 알파 감쇄 */
		ImGui::Checkbox("Alpha Attenuation", &m_tInstanceEffectMeshEffectMeshInfo.m_bAlphaAttenuation);
		ImGui::InputFloat("Alpha Attenuation Weight", &m_tInstanceEffectMeshEffectMeshInfo.m_fAlphaAttenuationWeight);

		/* 알파 자르는 기준 값 */
		ImGui::InputFloat("Alpha Discard Value", &m_tInstanceEffectMeshEffectMeshInfo.m_fAlphaDiscardValue);

		/* 이름 갱신 */
		m_strInstanceEffectMeshObjectName = m_chInstanceEffectMeshObjectName;
	}

	/* 정보 갱신 */
	UpdateCurInstanceEffectMeshInfo();

	ImGui::Dummy(ImVec2(0.f, 5.f));
}
void ToolEffect::CInspector::InstanceEffectMeshInfo()
{
	ImGui::SeparatorText("Instance Info");

	/* 파티클 타입 */
	array<const char*, (int8)EParticleType::ENUM_END> arrParticleType = { "DROP", "SPREAD", "ERUPTION", "CHARGE", "ORTHOGONAL", "REVOLUTION", "FOLLOW", "TONADO", "GROUNDED", "ALPHADECREASE", "SPRAY" };
	ImGui::Combo("Particle Type", &m_iParticleType, arrParticleType.data(), (int32)arrParticleType.size());

	switch (m_iParticleType)
	{
	case 0: m_tParticleInfo.m_eParticleType = EParticleType::DROP; break;
	case 1: m_tParticleInfo.m_eParticleType = EParticleType::SPREAD; break;
	case 2: m_tParticleInfo.m_eParticleType = EParticleType::ERUPTION; break;
	case 3: m_tParticleInfo.m_eParticleType = EParticleType::CHARGE; break;
	case 4: m_tParticleInfo.m_eParticleType = EParticleType::ORTHOGONAL; break;
	case 5: m_tParticleInfo.m_eParticleType = EParticleType::REVOLUTION; break;
	case 6: m_tParticleInfo.m_eParticleType = EParticleType::FOLLOW; break;
	case 7: m_tParticleInfo.m_eParticleType = EParticleType::TONADO; break;
	case 8: m_tParticleInfo.m_eParticleType = EParticleType::GROUNDED; break;
	case 9: m_tParticleInfo.m_eParticleType = EParticleType::ALPHADECREASE; break;
	case 10:m_tParticleInfo.m_eParticleType = EParticleType::SPRAY; break;
	default: break;
	}
	ImGui::Dummy(ImVec2(0.f, 5.f));

	/* 인스턴스 개수 */
	ImGui::InputInt("Mesh Num", &m_tInstanceEffectMeshInfo.m_iNumInstance);
	ImGui::Dummy(ImVec2(0.f, 5.f));

	/* 중력 */
	ImGui::Checkbox("Mesh Gravity", &m_tInstanceEffectMeshInfo.m_bGravity);
	if (m_tInstanceEffectMeshInfo.m_bGravity)
		ImGui::InputFloat("Mesh Gravity Value", &m_tInstanceEffectMeshInfo.m_fGravityValue);
	ImGui::Dummy(ImVec2(0.f, 5.f));

	/* 크기 */
	ImGui::InputFloat2("Mesh Size", &m_tInstanceEffectMeshInfo.m_vSize.x);

	/* 속도 */
	ImGui::InputFloat2("Mesh Speed", &m_tInstanceEffectMeshInfo.m_vSpeed.x);

	/* 생성 범위 */
	ImGui::InputFloat3("Create Range", &m_tInstanceEffectMeshInfo.m_vRange.x);

	/* 생성 반경 */
	ImGui::InputFloat2("Create Radius", &m_tInstanceEffectMeshInfo.m_vRadius.x);

	/* 초당 회전 */
	ImGui::InputFloat3("Mesh Turn Ratio", &m_tInstanceEffectMeshInfo.m_vRotation.x);
}
void ToolEffect::CInspector::UpdateCurInstanceEffectMeshInfo()
{
	if (nullptr == m_wpObjectList.lock()->GetEffectBase()) { return; }

	shared_ptr<CInstanceEffectMeshObject> spEffect = m_wpObjectList.lock()->GetInstanceEffectMeshObject();
	if (!spEffect) { return; }

	// ParticleInfo에서 실시간으로 변하면 안되는 값들을 저장합니다.
	int32 iPrevNumInstance = spEffect->GetInstanceEffectMeshCom()->GetInstanceEffectMeshInfo().m_iNumInstance;

	// EffectMeshInfo, InstanceEffectMeshInfo 갱신
	spEffect->GetInstanceEffectMeshCom()->SetEffectMeshInfo(m_tInstanceEffectMeshEffectMeshInfo);
	spEffect->GetInstanceEffectMeshCom()->SetInstanceEffectMeshInfo(m_tInstanceEffectMeshInfo);

	// 갱신한 값에서 변하면 안되는 값들을 되돌려줍니다.
	spEffect->GetInstanceEffectMeshCom()->GetInstanceEffectMeshInfo().m_iNumInstance = iPrevNumInstance;

	// S / R / T 갱신
	spEffect->SetScale(m_tInstanceEffectMeshEffectMeshInfo.m_vScale);
	spEffect->SetRotate(Vector3(1.f, 0.f, 0.f), m_tInstanceEffectMeshEffectMeshInfo.m_vRotation.x);
	spEffect->SetRotate(Vector3(0.f, 1.f, 0.f), m_tInstanceEffectMeshEffectMeshInfo.m_vRotation.y);
	spEffect->SetRotate(Vector3(0.f, 0.f, 1.f), m_tInstanceEffectMeshEffectMeshInfo.m_vRotation.z);
	spEffect->SetPosition(m_tInstanceEffectMeshEffectMeshInfo.m_vTranslation);

	// 셰이더, 텍스처 갱신
	spEffect->GetInstanceEffectMeshCom()->SetRenderType(m_tInstanceEffectMeshEffectMeshInfo.m_eRenderType);
	spEffect->GetInstanceEffectMeshCom()->SetEffectShader(m_tInstanceEffectMeshEffectMeshInfo.m_wstrEffectShaderName);
	spEffect->GetInstanceEffectMeshCom()->SetTexture(m_tInstanceEffectMeshEffectMeshInfo.m_wstrDiffuseTextureName, ETextureType::DIFFUSE);
	spEffect->GetInstanceEffectMeshCom()->SetTexture(m_tInstanceEffectMeshEffectMeshInfo.m_wstrNoiseTextureName, ETextureType::NOISE);
	spEffect->GetInstanceEffectMeshCom()->SetTexture(m_tInstanceEffectMeshEffectMeshInfo.m_wstrMaskTextureName, ETextureType::MASK);
	spEffect->GetInstanceEffectMeshCom()->SetTexture(m_tInstanceEffectMeshEffectMeshInfo.m_wstrDistortionTextureName, ETextureType::DISTORTION);
	spEffect->GetInstanceEffectMeshCom()->SetTexture(m_tInstanceEffectMeshEffectMeshInfo.m_wstrDissolveTextureName, ETextureType::DISSOLVE);
}
void ToolEffect::CInspector::CreateInstanceEffectMesh()
{
	/* 이름 */
	ImGui::SeparatorText("Object Name");
	ImGui::InputTextWithHint("#ObjectName", "ObjectName", m_chInstanceEffectMeshObjectName, MAX_PATH);

	if (ImGui::Button("Create Effect", ImVec2(100.f, 40.f)))
	{
		CInstanceEffectMeshObject::FInstanceEffectMeshObjectDesc tInstanceEffectMeshObjectDesc = {};

		tInstanceEffectMeshObjectDesc.m_wstrGameObjectName = CStringUtility::ToWideString(m_chInstanceEffectMeshObjectName);
		tInstanceEffectMeshObjectDesc.m_eEffectType = EEffectType::InstanceEffectMesh;

		tInstanceEffectMeshObjectDesc.m_wstrJsonFileName = L"";
		tInstanceEffectMeshObjectDesc.m_wstrModelFilePath = m_tInstanceEffectMeshEffectMeshInfo.m_wstrModelFilePath;

		tInstanceEffectMeshObjectDesc.m_vInitScale = m_tInstanceEffectMeshEffectMeshInfo.m_vScale;
		tInstanceEffectMeshObjectDesc.m_vInitRotate = m_tInstanceEffectMeshEffectMeshInfo.m_vRotation;
		tInstanceEffectMeshObjectDesc.m_vInitPosition = m_tInstanceEffectMeshEffectMeshInfo.m_vTranslation;

		tInstanceEffectMeshObjectDesc.m_matPivot = Matrix::CreateScale(0.01f) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0.f, 1.f, 0.f), XMConvertToRadians(180.f))) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(1.f, 0.f, 0.f), XMConvertToRadians(-90.f)));

		shared_ptr<CInstanceEffectMeshObject> spInstanceEffectMeshObject = CInstanceEffectMeshObject::Create(make_shared<CInstanceEffectMeshObject::FInstanceEffectMeshObjectDesc>(tInstanceEffectMeshObjectDesc), m_tInstanceEffectMeshEffectMeshInfo, m_tInstanceEffectMeshInfo);
		CHECKF(spInstanceEffectMeshObject, L"Failed to Create InstanceEffectMeshObject : CInspector");

		m_wpToolEffectLevel.lock()->AddGameObject(spInstanceEffectMeshObject);
		m_wpToolEffectLevel.lock()->AddGameObjectToLayer(L"Instance Mesh", spInstanceEffectMeshObject);

		m_tInstanceEffectMeshEffectMeshInfo = spInstanceEffectMeshObject->GetInstanceEffectMeshCom()->GetEffectMeshInfo();
		m_tInstanceEffectMeshInfo = spInstanceEffectMeshObject->GetInstanceEffectMeshCom()->GetInstanceEffectMeshInfo();

		m_wpObjectList.lock()->SetCurObject(spInstanceEffectMeshObject);
	}
}
void ToolEffect::CInspector::CreateInstanceEffectMeshFromJson()
{
	ImGui::SeparatorText("Json");
	ImGui::InputTextWithHint("Json File Name", "Instance Mesh Json File Name", m_chInstanceEffectMeshJsonFileName, MAX_PATH);

	if (ImGui::Button("Create From Json", ImVec2(100.f, 40.f)))
	{
		CInstanceEffectMeshObject::FInstanceEffectMeshObjectDesc tInstanceObjectDesc = {};

		tInstanceObjectDesc.m_wstrGameObjectName = CStringUtility::ToWideString(m_chInstanceEffectMeshJsonFileName);
		tInstanceObjectDesc.m_eEffectType = EEffectType::InstanceEffectMesh;

		tInstanceObjectDesc.m_wstrJsonFileName = CStringUtility::ToWideString(m_chInstanceEffectMeshJsonFileName);
		tInstanceObjectDesc.m_wstrModelFilePath = L"";

		tInstanceObjectDesc.m_vInitScale = Vector3(1.f, 1.f, 1.f);
		tInstanceObjectDesc.m_vInitRotate = Vector3(0.f, 0.f, 0.f);
		tInstanceObjectDesc.m_vInitPosition = Vector3(0.f, 0.f, 0.f);

		tInstanceObjectDesc.m_matPivot = Matrix::CreateScale(0.01f) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0.f, 1.f, 0.f), XMConvertToRadians(180.f))) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(1.f, 0.f, 0.f), XMConvertToRadians(-90.f)));

		shared_ptr<CInstanceEffectMeshObject> spInstanceEffectMeshObject = CInstanceEffectMeshObject::CreateFromJson(make_shared<CInstanceEffectMeshObject::FInstanceEffectMeshObjectDesc>(tInstanceObjectDesc), CStringUtility::ToWideString(m_chInstanceEffectMeshJsonFileName));
		CHECKF(spInstanceEffectMeshObject, L"Failed to CreateFromJson InstanceEffectMeshObject : CInspector");

		m_wpToolEffectLevel.lock()->AddGameObject(spInstanceEffectMeshObject);
		m_wpToolEffectLevel.lock()->AddGameObjectToLayer(L"Instance Mesh", spInstanceEffectMeshObject);

		m_tInstanceEffectMeshEffectMeshInfo	= spInstanceEffectMeshObject->GetInstanceEffectMeshCom()->GetEffectMeshInfo();
		m_tInstanceEffectMeshInfo				= spInstanceEffectMeshObject->GetInstanceEffectMeshCom()->GetInstanceEffectMeshInfo();

		m_wpObjectList.lock()->SetCurObject(spInstanceEffectMeshObject);
	}
}

void ToolEffect::CInspector::RemoveKeyFrame(EEffectType _eEffectType, int32 _iIndex)
{
	switch (_eEffectType)
	{
	case EEffectType::PARTICLE:
	{
		auto iter = m_tParticleInfo.m_vecKeyFrames.begin();
		for (int32 i = 0; i < _iIndex; ++i) { ++iter; }

		m_tParticleInfo.m_vecKeyFrames.erase(iter);
		--m_tParticleInfo.m_iNumKeyFrames;
		break;
	}
	case EEffectType::EFFECTMESH:
	{
		auto iter = m_tEffectMeshInfo.m_vecKeyFrames.begin();
		for (int32 i = 0; i < _iIndex; ++i) { ++iter; }

		m_tEffectMeshInfo.m_vecKeyFrames.erase(iter);
		--m_tEffectMeshInfo.m_iNumKeyFrames;
		break;
	}
	case EEffectType::InstanceEffectMesh:
	{
		auto iter = m_tInstanceEffectMeshEffectMeshInfo.m_vecKeyFrames.begin();
		for (int32 i = 0; i < _iIndex; ++i) { ++iter; }

		m_tInstanceEffectMeshEffectMeshInfo.m_vecKeyFrames.erase(iter);
		--m_tInstanceEffectMeshEffectMeshInfo.m_iNumKeyFrames;
		break;
	}
	}
}
void ToolEffect::CInspector::SetParticleKeyFrameValue(const FParticleKeyFrame& _tParticleKeyFrame, int32 _iIndex)
{
	if (m_tParticleInfo.m_vecKeyFrames.empty() || _iIndex > (int32)m_tParticleInfo.m_vecKeyFrames.size() - 1) { return; }

	m_tParticleInfo.m_vecKeyFrames[_iIndex] = _tParticleKeyFrame;
}
void ToolEffect::CInspector::SetEffectMeshKeyFrameValue(const FEffectMeshKeyFrame& _tEffectMeshKeyFrame, int32 _iIndex)
{
	if (m_tEffectMeshInfo.m_vecKeyFrames.empty() || _iIndex > (int32)m_tEffectMeshInfo.m_vecKeyFrames.size() - 1) { return; }

	m_tEffectMeshInfo.m_vecKeyFrames[_iIndex] = _tEffectMeshKeyFrame;
}
void ToolEffect::CInspector::SetInstanceEffectMeshKeyFrameValue(const FEffectMeshKeyFrame& _tEffectMeshKeyFrame, int32 _iIndex)
{
	if (m_tInstanceEffectMeshEffectMeshInfo.m_vecKeyFrames.empty() || _iIndex > (int32)m_tInstanceEffectMeshEffectMeshInfo.m_vecKeyFrames.size() - 1) { return; }

	m_tInstanceEffectMeshEffectMeshInfo.m_vecKeyFrames[_iIndex] = _tEffectMeshKeyFrame;
}

void ToolEffect::CInspector::SelectEffectMesh(EEffectType _eEffectType)
{
	if (ImGui::BeginListBox("Effect Meshs", ImVec2(350.f, 100.f)))
	{
		const vector<wstring>& vecEffectMeshs = CModelHandler::GetInstance()->GetEffectMeshPaths();

		for (auto& iter : vecEffectMeshs)
		{
			string strFilePath = CStringUtility::ToString(iter);
			char szFileName[MAX_PATH] = "";
			_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

			const bool bRectSelected = (m_iEffectShaderSelector == vecEffectMeshs.size() - 1);
			if (ImGui::Selectable(szFileName, ImGuiSelectableFlags_None, bRectSelected))
			{
				switch (_eEffectType)
				{
				case EEffectType::EFFECTMESH:
				{
					m_tEffectMeshInfo.m_wstrModelFilePath = iter;
					break;
				}
				case EEffectType::InstanceEffectMesh:
				{
					m_tInstanceEffectMeshEffectMeshInfo.m_wstrModelFilePath = iter;
					break;
				}
				}
			}
		}

		ImGui::EndListBox();
	}
}
void ToolEffect::CInspector::SelectEffectShader(EEffectType _eEffectType)
{
	if (ImGui::BeginListBox("Shader Files", ImVec2(350.f, 100.f)))
	{
		const unordered_map<wstring, shared_ptr<CEffectShader>>& umapEffectShaders = CShaderHandler::GetInstance()->GetEffectShaders();

		for (auto& pair : umapEffectShaders)
		{
			string strFilePath = CStringUtility::ToString(pair.first);
			char szFileName[MAX_PATH] = "";
			_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

			const bool bRectSelected = (m_iEffectShaderSelector == umapEffectShaders.size() - 1);
			if (ImGui::Selectable(szFileName, ImGuiSelectableFlags_None, bRectSelected))
			{
				switch (_eEffectType)
				{
				case EEffectType::PARTICLE:
				{
					m_tParticleInfo.m_wstrEffectShaderName = pair.first;
					break;
				}
				case EEffectType::EFFECTMESH:
				{
					m_tEffectMeshInfo.m_wstrEffectShaderName = pair.first;
					break;
				}
				case EEffectType::InstanceEffectMesh:
				{
					m_tInstanceEffectMeshEffectMeshInfo.m_wstrEffectShaderName = pair.first;
					break;
				}
				}
			}
		}

		ImGui::EndListBox();
	}
}
void ToolEffect::CInspector::SelectDiffuseTexture(EEffectType _eEffectType)
{
	if (ImGui::BeginListBox("Diffuse Textures", ImVec2(250.f, 200.f)))
	{
		const map<wstring, ComPtr<ID3D11ShaderResourceView>>& mapDiffuses = CTextureHandler::GetInstance()->GetDiffuseTextures();
		m_iDiffuseTextureSelector = 0;

		for (auto& pair : mapDiffuses)
		{
			string strTextureId = "Diffuse" + to_string(m_iDiffuseTextureSelector);

			if (ImGui::ImageButton(strTextureId.c_str(), pair.second.Get(), ImVec2(30.f, 30.f)))
			{
				string strFilePath = CStringUtility::ToString(pair.first);
				char szFileName[MAX_PATH] = "";
				_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

				switch (_eEffectType)
				{
				case EEffectType::PARTICLE:
				{
					m_tParticleInfo.m_wstrDiffuseTextureName = pair.first;
					break;
				}
				case EEffectType::EFFECTMESH:
				{
					m_tEffectMeshInfo.m_wstrDiffuseTextureName = pair.first;
					break;
				}
				case EEffectType::InstanceEffectMesh:
				{
					m_tInstanceEffectMeshEffectMeshInfo.m_wstrDiffuseTextureName = pair.first;
					break;
				}
				}

			}
			++m_iDiffuseTextureSelector;

			if (0 != m_iDiffuseTextureSelector % 5)
			{
				ImGui::SameLine();
			}
		}

		ImGui::EndListBox();
	}
}
void ToolEffect::CInspector::SelectNoiseTexture(EEffectType _eEffectType)
{
	if (ImGui::BeginListBox("Noise Textures", ImVec2(250.f, 200.f)))
	{
		const map<wstring, ComPtr<ID3D11ShaderResourceView>>& mapNoises = CTextureHandler::GetInstance()->GetNoiseTextures();
		m_iNoiseTextureSelector = 0;

		for (auto& pair : mapNoises)
		{
			string strTextureId = "Noise" + to_string(m_iNoiseTextureSelector);

			if (ImGui::ImageButton(strTextureId.c_str(), pair.second.Get(), ImVec2(30.f, 30.f)))
			{
				string strFilePath = CStringUtility::ToString(pair.first);
				char szFileName[MAX_PATH] = "";
				_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

				switch (_eEffectType)
				{
				case EEffectType::PARTICLE:
				{
					m_tParticleInfo.m_wstrDiffuseTextureName = pair.first;
					break;
				}
				case EEffectType::EFFECTMESH:
				{
					m_tEffectMeshInfo.m_wstrNoiseTextureName = pair.first;
					break;
				}
				case EEffectType::InstanceEffectMesh:
				{
					m_tInstanceEffectMeshEffectMeshInfo.m_wstrNoiseTextureName = pair.first;
					break;
				}
				}

			}
			++m_iNoiseTextureSelector;

			if (0 != m_iNoiseTextureSelector % 5)
			{
				ImGui::SameLine();
			}
		}

		ImGui::EndListBox();
	}
}
void ToolEffect::CInspector::SelectMaskTexture(EEffectType _eEffectType)
{
	if (ImGui::BeginListBox("Mask Textures", ImVec2(250.f, 200.f)))
	{
		const map<wstring, ComPtr<ID3D11ShaderResourceView>>& mapMasks = CTextureHandler::GetInstance()->GetMaskTextures();
		m_iMaskTextureSelector = 0;

		for (auto& pair : mapMasks)
		{
			string strTextureId = "Mask" + to_string(m_iMaskTextureSelector);

			if (ImGui::ImageButton(strTextureId.c_str(), pair.second.Get(), ImVec2(30.f, 30.f)))
			{
				string strFilePath = CStringUtility::ToString(pair.first);
				char szFileName[MAX_PATH] = "";
				_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

				switch (_eEffectType)
				{
				case EEffectType::PARTICLE:
				{
					m_tParticleInfo.m_wstrMaskTextureName = pair.first;
					break;
				}
				case EEffectType::EFFECTMESH:
				{
					m_tEffectMeshInfo.m_wstrMaskTextureName = pair.first;
					break;
				}
				case EEffectType::InstanceEffectMesh:
				{
					m_tInstanceEffectMeshEffectMeshInfo.m_wstrMaskTextureName = pair.first;
					break;
				}
				}

			}
			++m_iMaskTextureSelector;

			if (0 != m_iMaskTextureSelector % 5)
			{
				ImGui::SameLine();
			}
		}

		ImGui::EndListBox();
	}
}
void ToolEffect::CInspector::SelectDistortionTexture(EEffectType _eEffectType)
{
	if (ImGui::BeginListBox("Distortion Textures", ImVec2(250.f, 200.f)))
	{
		const map<wstring, ComPtr<ID3D11ShaderResourceView>>& mapDistortions = CTextureHandler::GetInstance()->GetDistortionTextures();
		m_iDistortionTextureSelector = 0;

		for (auto& pair : mapDistortions)
		{
			string strTextureId = "Distortion" + to_string(m_iDistortionTextureSelector);

			if (ImGui::ImageButton(strTextureId.c_str(), pair.second.Get(), ImVec2(30.f, 30.f)))
			{
				string strFilePath = CStringUtility::ToString(pair.first);
				char szFileName[MAX_PATH] = "";
				_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

				switch (_eEffectType)
				{
				case EEffectType::PARTICLE:
				{
					// m_tParticleInfo.m_wstrDiffuseTextureName = pair.first;
					break;
				}
				case EEffectType::EFFECTMESH:
				{
					m_tEffectMeshInfo.m_wstrDistortionTextureName = pair.first;
					break;
				}
				case EEffectType::InstanceEffectMesh:
				{
					m_tInstanceEffectMeshEffectMeshInfo.m_wstrDistortionTextureName = pair.first;
					break;
				}
				}
			}
			++m_iDistortionTextureSelector;

			if (0 != m_iDistortionTextureSelector % 5)
			{
				ImGui::SameLine();
			}
		}

		ImGui::EndListBox();
	}
}
void ToolEffect::CInspector::SelectDissolveTexture(EEffectType _eEffectType)
{
	if (ImGui::BeginListBox("Dissolve Textures", ImVec2(250.f, 200.f)))
	{
		const map<wstring, ComPtr<ID3D11ShaderResourceView>>& mapDissolves = CTextureHandler::GetInstance()->GetDissolveTextures();
		m_iDissolveTextureSelector = 0;

		for (auto& pair : mapDissolves)
		{
			string strTextureId = "Dissolve" + to_string(m_iDissolveTextureSelector);

			if (ImGui::ImageButton(strTextureId.c_str(), pair.second.Get(), ImVec2(30.f, 30.f)))
			{
				string strFilePath = CStringUtility::ToString(pair.first);
				char szFileName[MAX_PATH] = "";
				_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

				switch (_eEffectType)
				{
				case EEffectType::PARTICLE:
				{
					// m_tParticleInfo.m_wstrDiffuseTextureName = pair.first;
					break;
				}
				case EEffectType::EFFECTMESH:
				{
					m_tEffectMeshInfo.m_wstrDissolveTextureName = pair.first;
					break;
				}
				case EEffectType::InstanceEffectMesh:
				{
					m_tInstanceEffectMeshEffectMeshInfo.m_wstrDissolveTextureName = pair.first;
					break;
				}
				}
			}
			++m_iDissolveTextureSelector;

			if (0 != m_iDissolveTextureSelector % 5)
			{
				ImGui::SameLine();
			}
		}

		ImGui::EndListBox();
	}
}

void ToolEffect::CInspector::SaveParticleToJSON()
{
	if (ImGui::Button("Save", ImVec2(120.f, 40.f)))
	{
		shared_ptr<CParticleObject> spParticleObject = m_wpObjectList.lock()->GetParticleObject();
		if (!spParticleObject)
			return;

		const FParticleInfo& tParticleInfo = spParticleObject->GetParticleCom()->GetParticleInfo();

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		writer.StartObject();

		writer.Key("RenderType");
		writer.Int((int32)tParticleInfo.m_eRenderType);
		writer.Key("EffectType");
		writer.Int((int32)tParticleInfo.m_eEffectType);
		writer.Key("ParticleType");
		writer.Int((int32)tParticleInfo.m_eParticleType);
		writer.Key("Effect Shader Name");
		writer.String(CStringUtility::ToString(tParticleInfo.m_wstrEffectShaderName).c_str());

		writer.Key("Use Diffuse Texture");
		writer.Bool(tParticleInfo.m_bDiffuse);
		writer.Key("Diffuse Texture Name");
		writer.String(CStringUtility::ToString(tParticleInfo.m_wstrDiffuseTextureName).c_str());
		writer.Key("Use Mask Texture");
		writer.Bool(tParticleInfo.m_bMask);
		writer.Key("Mask Texture Name");
		writer.String(CStringUtility::ToString(tParticleInfo.m_wstrMaskTextureName).c_str());

		writer.Key("Is Sprite");
		writer.Bool(tParticleInfo.m_bSprite);
		writer.Key("Sprite Row");
		writer.Int(tParticleInfo.m_iSpriteRow);
		writer.Key("Sprite Col");
		writer.Int(tParticleInfo.m_iSpriteCol);
		writer.Key("Sprite Speed");
		writer.Double(tParticleInfo.m_fSpriteSpeed);

		writer.Key("Is Loop");
		writer.Bool(tParticleInfo.m_bLoop);
		writer.Key("Is Billboard");
		writer.Bool(tParticleInfo.m_bBillboard);
		writer.Key("Is Directional");
		writer.Bool(tParticleInfo.m_bDirectional);
		writer.Key("Is Gravity");
		writer.Bool(tParticleInfo.m_bGravity);
		writer.Key("Is Glow");
		writer.Bool(tParticleInfo.m_bGlow);
		writer.Key("Is Bloom");
		writer.Bool(tParticleInfo.m_bBloom);
		writer.Key("Is AlphaAttenuation");
		writer.Bool(tParticleInfo.m_bAlphaAttenuation);

		writer.Key("Gravity Value");
		writer.Double(tParticleInfo.m_fGravityValue);
		writer.Key("Glow Weight");
		writer.Double(tParticleInfo.m_fGlowWeight);
		writer.Key("Bloom Weight");
		writer.Double(tParticleInfo.m_fBloomWeight);
		writer.Key("AlphaAttenuation Weight");
		writer.Double(tParticleInfo.m_fAlphaAttenuationWeight);

		writer.Key("Random Life Time");
		writer.Bool(m_tParticleInfo.m_bRandomLifeTime);

		writer.Key("Life Time");
		writer.StartArray();
		writer.Double(tParticleInfo.m_vLifeTime.x);
		writer.Double(tParticleInfo.m_vLifeTime.y);
		writer.EndArray();

		writer.Key("Num Instance");
		writer.Int(tParticleInfo.m_iNumInstance);

		writer.Key("Size");
		writer.StartArray();
		writer.Double(tParticleInfo.m_vSize.x);
		writer.Double(tParticleInfo.m_vSize.y);
		writer.EndArray();

		writer.Key("Color1");
		writer.StartArray();
		writer.Double(tParticleInfo.m_vColor1.x);
		writer.Double(tParticleInfo.m_vColor1.y);
		writer.Double(tParticleInfo.m_vColor1.z);
		writer.Double(tParticleInfo.m_vColor1.w);
		writer.EndArray();

		writer.Key("Color2");
		writer.StartArray();
		writer.Double(tParticleInfo.m_vColor2.x);
		writer.Double(tParticleInfo.m_vColor2.y);
		writer.Double(tParticleInfo.m_vColor2.z);
		writer.Double(tParticleInfo.m_vColor2.w);
		writer.EndArray();

		writer.Key("Color3");
		writer.StartArray();
		writer.Double(tParticleInfo.m_vColor3.x);
		writer.Double(tParticleInfo.m_vColor3.y);
		writer.Double(tParticleInfo.m_vColor3.z);
		writer.Double(tParticleInfo.m_vColor3.w);
		writer.EndArray();

		writer.Key("Speed");
		writer.StartArray();
		writer.Double(tParticleInfo.m_vSpeed.x);
		writer.Double(tParticleInfo.m_vSpeed.y);
		writer.EndArray();

		writer.Key("Offset");
		writer.StartArray();
		writer.Double(tParticleInfo.m_vOffset.x);
		writer.Double(tParticleInfo.m_vOffset.y);
		writer.Double(tParticleInfo.m_vOffset.z);
		writer.EndArray();

		writer.Key("Pivot");
		writer.StartArray();
		writer.Double(m_tParticleInfo.m_vPivot.x);
		writer.Double(m_tParticleInfo.m_vPivot.y);
		writer.Double(m_tParticleInfo.m_vPivot.z);
		writer.EndArray();

		writer.Key("Range");
		writer.StartArray();
		writer.Double(tParticleInfo.m_vRange.x);
		writer.Double(tParticleInfo.m_vRange.y);
		writer.Double(tParticleInfo.m_vRange.z);
		writer.EndArray();

		writer.Key("Rotation Per Second");
		writer.StartArray();
		writer.Double(tParticleInfo.m_vRotation.x);
		writer.Double(tParticleInfo.m_vRotation.y);
		writer.Double(tParticleInfo.m_vRotation.z);
		writer.EndArray();

		writer.Key("Radius");
		writer.StartArray();
		writer.Double(tParticleInfo.m_vRadius.x);
		writer.Double(tParticleInfo.m_vRadius.y);
		writer.EndArray();

		writer.Key("Num KeyFrames");
		writer.Int(tParticleInfo.m_iNumKeyFrames);

		writer.Key("KeyFrames");
		writer.StartArray();

		const vector<FParticleKeyFrame> vecParticleKeyFrames = spParticleObject->GetKeyFrames();

		for (size_t i = 0; i < vecParticleKeyFrames.size(); ++i)
		{
			writer.StartObject();

			string strKeyFrameIndex = "KeyFrame" + to_string(i);
			writer.Key("Index");
			writer.String(strKeyFrameIndex.data());

			writer.Key("Effect Shader Pass Index");
			writer.Uint(vecParticleKeyFrames[i].m_iPassIndex);

			writer.Key("Time");
			writer.Double(vecParticleKeyFrames[i].m_fTime);

			writer.Key("Render Culled");
			writer.Bool(vecParticleKeyFrames[i].m_bRenderCulled);

			writer.Key("Size Ratio");
			writer.Double(vecParticleKeyFrames[i].m_fSizeRatio);
			writer.Key("Size Easing Type");
			writer.Uint((uint32)vecParticleKeyFrames[i].m_eSizeEasingType);
			writer.Key("Size Easing Expo");
			writer.Double(vecParticleKeyFrames[i].m_fSizeEasingExpo);

			writer.Key("Speed Ratio");
			writer.Double(vecParticleKeyFrames[i].m_fSpeedRatio);
			writer.Key("Speed Easing Type");
			writer.Uint((uint32)vecParticleKeyFrames[i].m_eSpeedEasingType);
			writer.Key("Speed Easing Expo");
			writer.Double(vecParticleKeyFrames[i].m_fSpeedEasingExpo);

			writer.Key("Color1");
			writer.StartArray();
			writer.Double(vecParticleKeyFrames[i].m_vColor1.x);
			writer.Double(vecParticleKeyFrames[i].m_vColor1.y);
			writer.Double(vecParticleKeyFrames[i].m_vColor1.z);
			writer.Double(vecParticleKeyFrames[i].m_vColor1.w);
			writer.EndArray();
			writer.Key("Color1 Weight");
			writer.Double(vecParticleKeyFrames[i].m_fColor1Weight);
			writer.Key("Color1 Easing Type");
			writer.Uint((uint32)vecParticleKeyFrames[i].m_eColor1EasingType);
			writer.Key("Color1 Easing Expo");
			writer.Double(vecParticleKeyFrames[i].m_fColor1EasingExpo);

			writer.Key("Color2");
			writer.StartArray();
			writer.Double(vecParticleKeyFrames[i].m_vColor2.x);
			writer.Double(vecParticleKeyFrames[i].m_vColor2.y);
			writer.Double(vecParticleKeyFrames[i].m_vColor2.z);
			writer.Double(vecParticleKeyFrames[i].m_vColor2.w);
			writer.EndArray();
			writer.Key("Color2 Weight");
			writer.Double(vecParticleKeyFrames[i].m_fColor2Weight);
			writer.Key("Color2 Easing Type");
			writer.Uint((uint32)vecParticleKeyFrames[i].m_eColor2EasingType);
			writer.Key("Color2 Easing Expo");
			writer.Double(vecParticleKeyFrames[i].m_fColor2EasingExpo);

			writer.Key("Color3");
			writer.StartArray();
			writer.Double(vecParticleKeyFrames[i].m_vColor3.x);
			writer.Double(vecParticleKeyFrames[i].m_vColor3.y);
			writer.Double(vecParticleKeyFrames[i].m_vColor3.z);
			writer.Double(vecParticleKeyFrames[i].m_vColor3.w);
			writer.EndArray();
			writer.Key("Color3 Weight");
			writer.Double(vecParticleKeyFrames[i].m_fColor3Weight);
			writer.Key("Color3 Easing Type");
			writer.Uint((uint32)vecParticleKeyFrames[i].m_eColor3EasingType);
			writer.Key("Color3 Easing Expo");
			writer.Double(vecParticleKeyFrames[i].m_fColor3EasingExpo);

			writer.EndObject();
		}
		writer.EndArray();

		writer.EndObject();

		ofstream ofs("../../Resource/Effects/Particle/" + m_strParticleObjectName + ".json");
		if (!ofs.is_open())
		{
			return;
		}

		ofs << buffer.GetString();
		ofs.close();
	}
}
void ToolEffect::CInspector::SaveEffectMeshToJSON(const FEffectMeshInfo& _tEffectMeshInfo, const string& _strObjectName)
{
	if (ImGui::Button("Save", ImVec2(100.f, 40.f)))
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		writer.StartObject();

		writer.Key("RenderType");
		writer.Int((int32)_tEffectMeshInfo.m_eRenderType);
		writer.Key("EffectType");
		writer.Int((int32)_tEffectMeshInfo.m_eEffectType);
		writer.Key("Model File Path");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrModelFilePath).c_str());
		writer.Key("Effect Shader Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrEffectShaderName).c_str());

		writer.Key("Is Trail");
		writer.Bool(_tEffectMeshInfo.m_bTrail);
		writer.Key("Num Trails");
		writer.Int(_tEffectMeshInfo.m_iNumTrails);
		writer.Key("Trail Term");
		writer.Double(_tEffectMeshInfo.m_fTrailTerm);

		writer.Key("Scale");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vScale.x);
		writer.Double(_tEffectMeshInfo.m_vScale.y);
		writer.Double(_tEffectMeshInfo.m_vScale.z);
		writer.EndArray();

		writer.Key("Rotation");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vRotation.x);
		writer.Double(_tEffectMeshInfo.m_vRotation.y);
		writer.Double(_tEffectMeshInfo.m_vRotation.z);
		writer.EndArray();

		writer.Key("Translation");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vTranslation.x);
		writer.Double(_tEffectMeshInfo.m_vTranslation.y);
		writer.Double(_tEffectMeshInfo.m_vTranslation.z);
		writer.EndArray();

		writer.Key("Diffuse Color1");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor1.x);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor1.y);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor1.z);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor1.w);
		writer.EndArray();

		writer.Key("Diffuse Color2");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor2.x);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor2.y);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor2.z);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor2.w);
		writer.EndArray();

		writer.Key("Diffuse Color3");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor3.x);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor3.y);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor3.z);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor3.w);
		writer.EndArray();

		writer.Key("Diffuse Texture Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrDiffuseTextureName).c_str());
		writer.Key("Noise Texture Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrNoiseTextureName).c_str());
		writer.Key("Mask Texture Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrMaskTextureName).c_str());
		writer.Key("Distortion Texture Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrDistortionTextureName).c_str());
		writer.Key("Dissolve Texture Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrDissolveTextureName).c_str());

		writer.Key("Is Sprite");
		writer.Bool(_tEffectMeshInfo.m_bSprite);
		writer.Key("Sprite Row");
		writer.Int(_tEffectMeshInfo.m_iSpriteRow);
		writer.Key("Sprite Col");
		writer.Int(_tEffectMeshInfo.m_iSpriteCol);
		writer.Key("Sprite Speed");
		writer.Double(_tEffectMeshInfo.m_fSpriteSpeed);

		writer.Key("Is Anim");
		writer.Bool(_tEffectMeshInfo.m_bAnimation);
		writer.Key("Is Loop");
		writer.Bool(_tEffectMeshInfo.m_bLoop);
		writer.Key("Is UVMove Loop");
		writer.Bool(_tEffectMeshInfo.m_bUVMoveLoop);
		writer.Key("Is Glow");
		writer.Bool(_tEffectMeshInfo.m_bGlow);
		writer.Key("Is Bloom");
		writer.Bool(_tEffectMeshInfo.m_bBloom);
		writer.Key("Is AlphaAttenuation");
		writer.Bool(_tEffectMeshInfo.m_bAlphaAttenuation);
		writer.Key("Use Diffuse Texture");
		writer.Bool(_tEffectMeshInfo.m_bDiffuse);
		writer.Key("Use Noise Texture");
		writer.Bool(_tEffectMeshInfo.m_bNoise);
		writer.Key("Use Mask Texture");
		writer.Bool(_tEffectMeshInfo.m_bMask);
		writer.Key("Use Distortion Texture");
		writer.Bool(_tEffectMeshInfo.m_bDistortion);
		writer.Key("Use Dissolve Texture");
		writer.Bool(_tEffectMeshInfo.m_bDissolve);

		writer.Key("Distortion Weight");
		writer.Double(_tEffectMeshInfo.m_fDistortionWeight);
		writer.Key("Dissolve Weight");
		writer.Double(_tEffectMeshInfo.m_fDissolveWeight);
		writer.Key("Glow Weight");
		writer.Double(_tEffectMeshInfo.m_fGlowWeight);
		writer.Key("Bloom Weight");
		writer.Double(_tEffectMeshInfo.m_fBloomWeight);
		writer.Key("AlphaAttenuation Weight");
		writer.Double(_tEffectMeshInfo.m_fAlphaAttenuationWeight);

		writer.Key("Life Time");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vLifeTime.x);
		writer.Double(_tEffectMeshInfo.m_vLifeTime.y);
		writer.EndArray();

		writer.Key("UVMove Speed");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vUVMoveSpeed.x);
		writer.Double(_tEffectMeshInfo.m_vUVMoveSpeed.y);
		writer.EndArray();

		writer.Key("Num KeyFrames");
		writer.Int(_tEffectMeshInfo.m_iNumKeyFrames);

		writer.Key("KeyFrames");
		writer.StartArray();

		shared_ptr<CEffectMeshObject> spEffectObject = m_wpObjectList.lock()->GetEffectMeshObject();
		if (spEffectObject)
		{
			const vector<FEffectMeshKeyFrame> vecEffectMeshKeyFrames = spEffectObject->GetKeyFrames();

			for (size_t i = 0; i < vecEffectMeshKeyFrames.size(); ++i)
			{
				writer.StartObject();

				string strKeyFrameIndex = "KeyFrame" + to_string(i);
				writer.Key("Index");
				writer.String(strKeyFrameIndex.data());

				writer.Key("Effect Shader Pass Index");
				writer.Uint(vecEffectMeshKeyFrames[i].m_iPassIndex);

				writer.Key("Time");
				writer.Double(vecEffectMeshKeyFrames[i].m_fTime);

				writer.Key("Color1");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor1.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor1.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor1.z);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor1.w);
				writer.EndArray();
				writer.Key("Color1 Weight");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor1Weight);
				writer.Key("Color1 Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eColor1EasingType);
				writer.Key("Color1 Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor1EasingExpo);

				writer.Key("Color2");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor2.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor2.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor2.z);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor2.w);
				writer.EndArray();
				writer.Key("Color2 Weight");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor2Weight);
				writer.Key("Color2 Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eColor2EasingType);
				writer.Key("Color2 Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor2EasingExpo);

				writer.Key("Color3");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor3.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor3.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor3.z);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor3.w);
				writer.EndArray();
				writer.Key("Color3 Weight");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor3Weight);
				writer.Key("Color3 Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eColor3EasingType);
				writer.Key("Color3 Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor3EasingExpo);

				writer.Key("Scale");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vScale.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vScale.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vScale.z);
				writer.EndArray();
				writer.Key("Scale Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eScaleEasingType);
				writer.Key("Scale Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fScaleEasingExpo);

				writer.Key("Rotation");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vRotation.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vRotation.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vRotation.z);
				writer.EndArray();
				writer.Key("Rotation Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eRotationEasingType);
				writer.Key("Rotation Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fRotationEasingExpo);

				writer.Key("Translation");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vTranslation.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vTranslation.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vTranslation.z);
				writer.EndArray();
				writer.Key("Translation Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eTranslationEasingType);
				writer.Key("Translation Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fTranslationEasingExpo);

				writer.Key("Rotate Weight");
				writer.Double(vecEffectMeshKeyFrames[i].m_fRotateWeight);

				writer.EndObject();
			}
		}
		writer.EndArray();

		writer.EndObject();

		ofstream ofs("../../Resource/Effects/EffectMesh/" + _strObjectName + ".json");
		if (!ofs.is_open())
		{
			return;
		}

		ofs << buffer.GetString();
		ofs.close();
	}
}
void ToolEffect::CInspector::SaveInstanceEffectMeshToJSON(const FEffectMeshInfo& _tEffectMeshInfo, const FInstanceEffectMeshInfo& _tInstanceEffectMeshInfo, const string& _strObjectName)
{
	if (ImGui::Button("Save", ImVec2(100.f, 40.f)))
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		writer.StartObject();

		/* EffectMeshInfo */
		writer.Key("RenderType");
		writer.Int((int32)_tEffectMeshInfo.m_eRenderType);
		writer.Key("EffectType");
		writer.Int((int32)_tEffectMeshInfo.m_eEffectType);
		writer.Key("Model File Path");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrModelFilePath).c_str());
		writer.Key("Effect Shader Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrEffectShaderName).c_str());

		writer.Key("Scale");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vScale.x);
		writer.Double(_tEffectMeshInfo.m_vScale.y);
		writer.Double(_tEffectMeshInfo.m_vScale.z);
		writer.EndArray();

		writer.Key("Rotation");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vRotation.x);
		writer.Double(_tEffectMeshInfo.m_vRotation.y);
		writer.Double(_tEffectMeshInfo.m_vRotation.z);
		writer.EndArray();

		writer.Key("Translation");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vTranslation.x);
		writer.Double(_tEffectMeshInfo.m_vTranslation.y);
		writer.Double(_tEffectMeshInfo.m_vTranslation.z);
		writer.EndArray();

		writer.Key("Diffuse Color1");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor1.x);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor1.y);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor1.z);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor1.w);
		writer.EndArray();

		writer.Key("Diffuse Color2");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor2.x);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor2.y);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor2.z);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor2.w);
		writer.EndArray();

		writer.Key("Diffuse Color3");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor3.x);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor3.y);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor3.z);
		writer.Double(_tEffectMeshInfo.m_vDiffuseColor3.w);
		writer.EndArray();

		writer.Key("Diffuse Texture Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrDiffuseTextureName).c_str());
		writer.Key("Noise Texture Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrNoiseTextureName).c_str());
		writer.Key("Mask Texture Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrMaskTextureName).c_str());
		writer.Key("Distortion Texture Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrDistortionTextureName).c_str());
		writer.Key("Dissolve Texture Name");
		writer.String(CStringUtility::ToString(_tEffectMeshInfo.m_wstrDissolveTextureName).c_str());

		writer.Key("Is Sprite");
		writer.Bool(_tEffectMeshInfo.m_bSprite);
		writer.Key("Sprite Row");
		writer.Int(_tEffectMeshInfo.m_iSpriteRow);
		writer.Key("Sprite Col");
		writer.Int(_tEffectMeshInfo.m_iSpriteCol);
		writer.Key("Sprite Speed");
		writer.Double(_tEffectMeshInfo.m_fSpriteSpeed);

		writer.Key("Is Anim");
		writer.Bool(_tEffectMeshInfo.m_bAnimation);
		writer.Key("Is Loop");
		writer.Bool(_tEffectMeshInfo.m_bLoop);
		writer.Key("Is UVMove Loop");
		writer.Bool(_tEffectMeshInfo.m_bUVMoveLoop);
		writer.Key("Is Glow");
		writer.Bool(_tEffectMeshInfo.m_bGlow);
		writer.Key("Is Bloom");
		writer.Bool(_tEffectMeshInfo.m_bBloom);
		writer.Key("Is AlphaAttenuation");
		writer.Bool(_tEffectMeshInfo.m_bAlphaAttenuation);
		writer.Key("Use Diffuse Texture");
		writer.Bool(_tEffectMeshInfo.m_bDiffuse);
		writer.Key("Use Noise Texture");
		writer.Bool(_tEffectMeshInfo.m_bNoise);
		writer.Key("Use Mask Texture");
		writer.Bool(_tEffectMeshInfo.m_bMask);
		writer.Key("Use Distortion Texture");
		writer.Bool(_tEffectMeshInfo.m_bDistortion);
		writer.Key("Use Dissolve Texture");
		writer.Bool(_tEffectMeshInfo.m_bDissolve);

		writer.Key("Distortion Weight");
		writer.Double(_tEffectMeshInfo.m_fDistortionWeight);
		writer.Key("Dissolve Weight");
		writer.Double(_tEffectMeshInfo.m_fDissolveWeight);
		writer.Key("Glow Weight");
		writer.Double(_tEffectMeshInfo.m_fGlowWeight);
		writer.Key("Bloom Weight");
		writer.Double(_tEffectMeshInfo.m_fBloomWeight);
		writer.Key("AlphaAttenuation Weight");
		writer.Double(_tEffectMeshInfo.m_fAlphaAttenuationWeight);

		writer.Key("Life Time");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vLifeTime.x);
		writer.Double(_tEffectMeshInfo.m_vLifeTime.y);
		writer.EndArray();

		writer.Key("UVMove Speed");
		writer.StartArray();
		writer.Double(_tEffectMeshInfo.m_vUVMoveSpeed.x);
		writer.Double(_tEffectMeshInfo.m_vUVMoveSpeed.y);
		writer.EndArray();

		/* InstanceEffectMeshInfo */
		writer.Key("Instance Mesh Particle Type");
		writer.Int((int32)_tInstanceEffectMeshInfo.m_eParticleType);
		writer.Key("Num Mesh Instance");
		writer.Int(_tInstanceEffectMeshInfo.m_iNumInstance);

		writer.Key("Is Gravity");
		writer.Bool(_tInstanceEffectMeshInfo.m_bGravity);
		writer.Key("Gravity Value");
		writer.Double(_tInstanceEffectMeshInfo.m_fGravityValue);

		writer.Key("Size");
		writer.StartArray();
		writer.Double(_tInstanceEffectMeshInfo.m_vSize.x);
		writer.Double(_tInstanceEffectMeshInfo.m_vSize.y);
		writer.EndArray();

		writer.Key("Speed");
		writer.StartArray();
		writer.Double(_tInstanceEffectMeshInfo.m_vSpeed.x);
		writer.Double(_tInstanceEffectMeshInfo.m_vSpeed.y);
		writer.EndArray();

		writer.Key("Range");
		writer.StartArray();
		writer.Double(_tInstanceEffectMeshInfo.m_vRange.x);
		writer.Double(_tInstanceEffectMeshInfo.m_vRange.y);
		writer.Double(_tInstanceEffectMeshInfo.m_vRange.z);
		writer.EndArray();

		writer.Key("Radius");
		writer.StartArray();
		writer.Double(_tInstanceEffectMeshInfo.m_vRadius.x);
		writer.Double(_tInstanceEffectMeshInfo.m_vRadius.y);
		writer.EndArray();

		writer.Key("Rotation Per Seconds");
		writer.StartArray();
		writer.Double(_tInstanceEffectMeshInfo.m_vRotation.x);
		writer.Double(_tInstanceEffectMeshInfo.m_vRotation.y);
		writer.Double(_tInstanceEffectMeshInfo.m_vRotation.z);
		writer.EndArray();

		/* EffectMesh KeyFrame */
		writer.Key("Num KeyFrames");
		writer.Int(_tEffectMeshInfo.m_iNumKeyFrames);

		writer.Key("KeyFrames");
		writer.StartArray();

		shared_ptr<CInstanceEffectMeshObject> spInstanceEffectMeshObject = m_wpObjectList.lock()->GetInstanceEffectMeshObject();
		if (spInstanceEffectMeshObject)
		{
			const vector<FEffectMeshKeyFrame> vecEffectMeshKeyFrames = spInstanceEffectMeshObject->GetKeyFrames();

			for (size_t i = 0; i < vecEffectMeshKeyFrames.size(); ++i)
			{
				writer.StartObject();

				string strKeyFrameIndex = "KeyFrame" + to_string(i);
				writer.Key("Index");
				writer.String(strKeyFrameIndex.data());

				writer.Key("Effect Shader Pass Index");
				writer.Uint(vecEffectMeshKeyFrames[i].m_iPassIndex);

				writer.Key("Time");
				writer.Double(vecEffectMeshKeyFrames[i].m_fTime);

				writer.Key("Color1");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor1.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor1.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor1.z);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor1.w);
				writer.EndArray();
				writer.Key("Color1 Weight");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor1Weight);
				writer.Key("Color1 Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eColor1EasingType);
				writer.Key("Color1 Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor1EasingExpo);

				writer.Key("Color2");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor2.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor2.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor2.z);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor2.w);
				writer.EndArray();
				writer.Key("Color2 Weight");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor2Weight);
				writer.Key("Color2 Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eColor2EasingType);
				writer.Key("Color2 Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor2EasingExpo);

				writer.Key("Color3");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor3.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor3.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor3.z);
				writer.Double(vecEffectMeshKeyFrames[i].m_vColor3.w);
				writer.EndArray();
				writer.Key("Color3 Weight");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor3Weight);
				writer.Key("Color3 Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eColor3EasingType);
				writer.Key("Color3 Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fColor3EasingExpo);

				writer.Key("Scale");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vScale.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vScale.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vScale.z);
				writer.EndArray();
				writer.Key("Scale Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eScaleEasingType);
				writer.Key("Scale Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fScaleEasingExpo);

				writer.Key("Rotation");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vRotation.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vRotation.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vRotation.z);
				writer.EndArray();
				writer.Key("Rotation Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eRotationEasingType);
				writer.Key("Rotation Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fRotationEasingExpo);

				writer.Key("Translation");
				writer.StartArray();
				writer.Double(vecEffectMeshKeyFrames[i].m_vTranslation.x);
				writer.Double(vecEffectMeshKeyFrames[i].m_vTranslation.y);
				writer.Double(vecEffectMeshKeyFrames[i].m_vTranslation.z);
				writer.EndArray();
				writer.Key("Translation Easing Type");
				writer.Uint((uint32)vecEffectMeshKeyFrames[i].m_eTranslationEasingType);
				writer.Key("Translation Easing Expo");
				writer.Double(vecEffectMeshKeyFrames[i].m_fTranslationEasingExpo);

				writer.Key("Rotate Weight");
				writer.Double(vecEffectMeshKeyFrames[i].m_fRotateWeight);

				writer.EndObject();
			}
		}
		writer.EndArray();

		writer.EndObject();

		ofstream ofs("../../Resource/Effects/InstanceEffectMesh/" + _strObjectName + ".json");
		if (!ofs.is_open())
		{
			return;
		}

		ofs << buffer.GetString();
		ofs.close();
	}
}