// Default
#include "pch.h"
#include "ObjectList.h"
#include "StringUtility.h"

// Level, Window
#include "ToolEffectLevel.h"
#include "Inspector.h"
#include "EffectGroupWindow.h"

// Layer
#include "Layer.h"

// GameObject
#include "EffectGroup.h"
#include "ParticleObject.h"
#include "EffectMeshObject.h"
#include "InstanceEffectMeshObject.h"

// Component
#include "ParticleComponent.h"
#include "EffectMeshComponent.h"
#include "InstanceEffectMeshComponent.h"

HRESULT ToolEffect::CObjectList::Initialize()
{
	m_tEffectMeshKeyFrame.m_vScale = Vector3(1.f, 1.f, 1.f);

	return CWindow::Initialize();
}
HRESULT ToolEffect::CObjectList::BeginPlay()
{
	return CWindow::BeginPlay();
}
int32 ToolEffect::CObjectList::PreTick(float _fDeltaSeconds)
{
	if (!m_bObjectList)
	{
		LoadParticlesFromPath();
		LoadEffectMeshsFromPath();
		LoadInstanceEffectMeshsFromPath();

		m_bObjectList = true;
	}

	return CWindow::PreTick(_fDeltaSeconds);
}
int32 ToolEffect::CObjectList::Tick(float _fDeltaSeconds)
{
	if (ImGui::Begin("ObjectList", &m_bObjectList))
	{
		UpdateObjectList();
		ImGui::End();
	}

	return CWindow::Tick(_fDeltaSeconds);
}
int32 ToolEffect::CObjectList::PostTick(float _fDeltaSeconds)
{
	return CWindow::PostTick(_fDeltaSeconds);
}
int32 ToolEffect::CObjectList::Render()
{
	return CWindow::Render();
}
int32 ToolEffect::CObjectList::EndPlay()
{
	return CWindow::EndPlay();
}
void ToolEffect::CObjectList::Release()
{
	CWindow::Release();
}

HRESULT ToolEffect::CObjectList::LoadParticlesFromPath()
{
	for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(m_wstrParticlePath))
	{
		// 파일이 아닌 경우
		if (!filesystem::is_regular_file(fsEntry)) { continue; }

		wstring wstrJsonFileName = fsEntry.path().stem().generic_wstring();
		
		CParticleObject::FParticleObjectDesc tParticleObjectDesc = {};

		tParticleObjectDesc.m_wstrGameObjectName = wstrJsonFileName;
		tParticleObjectDesc.m_eEffectType = EEffectType::PARTICLE;

		shared_ptr<CParticleObject> spParticleObject = CParticleObject::CreateFromJson(make_shared<CParticleObject::FParticleObjectDesc>(tParticleObjectDesc), wstrJsonFileName);
		CHECKF(spParticleObject, L"Failed to Create ParticleObject : CObjectList");

		m_wpToolEffectLevel.lock()->AddGameObject(spParticleObject);
		m_wpToolEffectLevel.lock()->AddGameObjectToLayer(L"Particle", spParticleObject);

		SetCurObject(spParticleObject);
		// m_wpInspector.lock()->SetParticleInfo(dynamic_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->GetParticleInfo());

		spParticleObject->GetParticleCom()->EndLifeTime();
	}

	return S_OK;
}

HRESULT ToolEffect::CObjectList::LoadEffectMeshsFromPath()
{
	for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(m_wstrEffectMeshPath))
	{
		// 파일이 아닌 경우
		if (!filesystem::is_regular_file(fsEntry)) { continue; }

		wstring wstrJsonFileName = fsEntry.path().stem().generic_wstring();

		CEffectMeshObject::FEffectMeshObjectDesc tEffectObjectDesc = {};

		tEffectObjectDesc.m_wstrGameObjectName = wstrJsonFileName;
		tEffectObjectDesc.m_eEffectType = EEffectType::EFFECTMESH;

		tEffectObjectDesc.m_wstrJsonFileName = wstrJsonFileName;
		tEffectObjectDesc.m_wstrModelFilePath = L"";

		tEffectObjectDesc.m_vInitScale = Vector3(1.f, 1.f, 1.f);
		tEffectObjectDesc.m_vInitRotate = Vector3(0.f, 0.f, 0.f);
		tEffectObjectDesc.m_vInitPosition = Vector3(0.f, 0.f, 0.f);

		tEffectObjectDesc.m_matPivot = Matrix::CreateScale(0.01f) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0.f, 1.f, 0.f), XMConvertToRadians(180.f))) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(1.f, 0.f, 0.f), XMConvertToRadians(-90.f)));

		shared_ptr<CEffectMeshObject> spEffectObject = CEffectMeshObject::CreateFromJson(make_shared<CEffectMeshObject::FEffectMeshObjectDesc>(tEffectObjectDesc), wstrJsonFileName);
		CHECKF(spEffectObject, L"Failed to CreateFromJson EffectObject : CInspector");

		m_wpToolEffectLevel.lock()->AddGameObject(spEffectObject);
		m_wpToolEffectLevel.lock()->AddGameObjectToLayer(L"Effect Mesh", spEffectObject);

		SetCurObject(spEffectObject);
		// m_wpInspector.lock()->SetEffectMeshInfo(dynamic_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom()->GetEffectMeshInfo());

		spEffectObject->GetEffectMeshCom()->EndLifeTime();
	}

	return S_OK;
}

HRESULT ToolEffect::CObjectList::LoadInstanceEffectMeshsFromPath()
{
	for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(m_wstrInstanceEffectMeshPath))
	{
		// 파일이 아닌 경우
		if (!filesystem::is_regular_file(fsEntry)) { continue; }

		wstring wstrJsonFileName = fsEntry.path().stem().generic_wstring();

		CInstanceEffectMeshObject::FInstanceEffectMeshObjectDesc tInstanceObjectDesc = {};

		tInstanceObjectDesc.m_wstrGameObjectName = wstrJsonFileName;
		tInstanceObjectDesc.m_eEffectType = EEffectType::InstanceEffectMesh;

		tInstanceObjectDesc.m_wstrJsonFileName = wstrJsonFileName;
		tInstanceObjectDesc.m_wstrModelFilePath = L"";

		tInstanceObjectDesc.m_vInitScale = Vector3(1.f, 1.f, 1.f);
		tInstanceObjectDesc.m_vInitRotate = Vector3(0.f, 0.f, 0.f);
		tInstanceObjectDesc.m_vInitPosition = Vector3(0.f, 0.f, 0.f);

		tInstanceObjectDesc.m_matPivot = Matrix::CreateScale(0.01f) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0.f, 1.f, 0.f), XMConvertToRadians(180.f))) *
			Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(1.f, 0.f, 0.f), XMConvertToRadians(-90.f)));

		shared_ptr<CInstanceEffectMeshObject> spInstanceEffectMeshObject = CInstanceEffectMeshObject::CreateFromJson(make_shared<CInstanceEffectMeshObject::FInstanceEffectMeshObjectDesc>(tInstanceObjectDesc), wstrJsonFileName);
		CHECKF(spInstanceEffectMeshObject, L"Failed to CreateFromJson InstanceEffectMeshObject : CInspector");

		m_wpToolEffectLevel.lock()->AddGameObject(spInstanceEffectMeshObject);
		m_wpToolEffectLevel.lock()->AddGameObjectToLayer(L"Instance Mesh", spInstanceEffectMeshObject);

		SetCurObject(spInstanceEffectMeshObject);

		// m_wpInspector.lock()->SetInstanceEffectMeshEffectMeshInfo(dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->GetEffectMeshInfo());
		// m_wpInspector.lock()->SetInstanceEffectMeshInfo(dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->GetInstanceEffectMeshInfo());

		spInstanceEffectMeshObject->GetInstanceEffectMeshCom()->EndLifeTime();
	}

	return S_OK;
}

void ToolEffect::CObjectList::UpdateObjectList()
{
	ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
	if (EEffectType::PARTICLE == m_eCurEffectType)
	{
		UpdateParticleTab();
	}
	else if (EEffectType::EFFECTMESH == m_eCurEffectType)
	{
		UpdateEffectMeshTab();
	}
	else if (EEffectType::InstanceEffectMesh == m_eCurEffectType)
	{
		UpdateInstanceEffectMeshTab();
	}
}
void ToolEffect::CObjectList::UpdateParticleTab()
{
	UpdateParticleList();
	UpdateSelectedParticleKeyFrameList();
	UpdateCurParticleKeyFrame();
	UpdateParticleKeyFrames();
	AddParticleKeyFrame();
	RemoveParticleKeyFrame();
	PauseEffect();
	NonActive();
	AddEffectToGroup();
	Reset();
}
void ToolEffect::CObjectList::UpdateParticleList()
{
	ImGui::SeparatorText("Particle");          

	shared_ptr<CLayer> spParticleLayer = m_wpToolEffectLevel.lock()->FindLayer(L"Particle");
	if (!spParticleLayer) { return; }

	// 파티클 리스트 업데이트
	const unordered_map<wstring, shared_ptr<CGameObject>>& umapParticles = spParticleLayer->GetGameObjects();
	if (umapParticles.empty())
		return;

	UpdateObjectSelector(umapParticles);
}
void ToolEffect::CObjectList::UpdateSelectedParticleKeyFrameList()
{
	if (m_wpSelectedObject.expired()) { return; }
	if (EEffectType::EFFECTMESH == m_wpSelectedObject.lock()->GetEffectType() ||
		EEffectType::InstanceEffectMesh == m_wpSelectedObject.lock()->GetEffectType())
	{ return; }

	vector<FParticleKeyFrame>& vecKeyFrames = dynamic_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetKeyFrames();

	if (vecKeyFrames.empty()) { return; }

	ImGui::SeparatorText("KeyFrame");
	if (ImGui::BeginListBox("Particle KeyFrames", ImVec2(300.f, 100.f)))
	{
		for (uint16 i = 0; i < vecKeyFrames.size(); ++i)
		{
			const bool bSelected = (m_iParticleKeyFrameSelector == vecKeyFrames.size() - 1);
			if (ImGui::Selectable(to_string(i).data(), ImGuiSelectableFlags_None, bSelected))
			{
				m_iParticleColor1EasingType = (int32)vecKeyFrames[i].m_eColor1EasingType;
				m_iParticleColor2EasingType = (int32)vecKeyFrames[i].m_eColor2EasingType;
				m_iParticleColor3EasingType = (int32)vecKeyFrames[i].m_eColor3EasingType;

				m_tParticleKeyFrame = vecKeyFrames[i];

				m_iSelectedKeyFrameIndex = i;
			}
		}

		ImGui::EndListBox();
	}
}
void ToolEffect::CObjectList::UpdateParticleKeyFrames()
{
	ImGui::SeparatorText("Add KeyFrame");

	/* 이펙트 셰이더 패스 인덱스 */
	ImGui::InputInt("Pass Index", &m_tParticleKeyFrame.m_iPassIndex);

	/* 키프레임 시간 */
	ImGui::InputFloat("Time", &m_tParticleKeyFrame.m_fTime);

	/* 렌더 컬 */
	ImGui::Checkbox("Render Culled", &m_tParticleKeyFrame.m_bRenderCulled);

	/* 크기 비율 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Particle KeyFrame Size Ratio -");
	ImGui::InputFloat("Size Ratio", &m_tParticleKeyFrame.m_fSizeRatio);
	UpdateEasingTypeSelector(&m_iParticleSizeEasingType, &m_tParticleKeyFrame.m_eSizeEasingType, 9);

	/* 속도 비율 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Particle KeyFrame Speed Ratio -");
	ImGui::InputFloat("Speed Ratio", &m_tParticleKeyFrame.m_fSpeedRatio);
	UpdateEasingTypeSelector(&m_iParticleSpeedEasingType, &m_tParticleKeyFrame.m_eSpeedEasingType, 10);

	/* 디퓨즈 컬러1 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Particle KeyFrame Color1 -");
	ImGui::ColorEdit4("Particle KeyFrame Color1", &m_tParticleKeyFrame.m_vColor1.x);
	ImGui::InputFloat("Particle KeyFrame Color1 Weight", &m_tParticleKeyFrame.m_fColor1Weight);
	UpdateEasingTypeSelector(&m_iParticleColor1EasingType, &m_tParticleKeyFrame.m_eColor1EasingType, 0);

	/* 디퓨즈 컬러2 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Particle KeyFrame Color2 -");
	ImGui::ColorEdit4("Particle KeyFrame Color2", &m_tParticleKeyFrame.m_vColor2.x);
	ImGui::InputFloat("Particle KeyFrame Color2 Weight", &m_tParticleKeyFrame.m_fColor2Weight);
	UpdateEasingTypeSelector(&m_iParticleColor2EasingType, &m_tParticleKeyFrame.m_eColor2EasingType, 1);

	/* 디퓨즈 컬러3 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Particle KeyFrame Color3 -");
	ImGui::ColorEdit4("Particle KeyFrame Color3", &m_tParticleKeyFrame.m_vColor3.x);
	ImGui::InputFloat("Particle KeyFrame Color3 Weight", &m_tParticleKeyFrame.m_fColor3Weight);
	UpdateEasingTypeSelector(&m_iParticleColor3EasingType, &m_tParticleKeyFrame.m_eColor3EasingType, 2);

	ImGui::Dummy(ImVec2(0.f, 5.f));

	if (m_wpSelectedObject.expired()) { return; }
	if (EEffectType::EFFECTMESH == m_wpSelectedObject.lock()->GetEffectType()) { return; }
}
void ToolEffect::CObjectList::AddParticleKeyFrame()
{
	if (ImGui::Button("Add KeyFrame", ImVec2(120.f, 40.f)))
	{
		if (m_wpSelectedObject.expired())
			return;

		dynamic_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->AddKeyFrame(m_tParticleKeyFrame);
		m_wpInspector.lock()->GetParticleInfo().m_vecKeyFrames.push_back(m_tParticleKeyFrame);
		++m_wpInspector.lock()->GetParticleInfo().m_iNumKeyFrames;

		m_iSelectedKeyFrameIndex = uint16(m_wpInspector.lock()->GetParticleInfo().m_vecKeyFrames.size() - 1);
	}
}
void ToolEffect::CObjectList::RemoveParticle()
{
	if (m_wpSelectedObject.expired())
		return;
}
void ToolEffect::CObjectList::RemoveParticleKeyFrame()
{
	ImGui::SameLine();
	if (ImGui::Button("Remove KeyFrame", ImVec2(120.f, 40.f)))
	{
		if (m_wpSelectedObject.expired()) { return; }
		if (EEffectType::EFFECTMESH == m_wpSelectedObject.lock()->GetEffectType()) { return; }

		vector<FParticleKeyFrame>& vecKeyFrames = dynamic_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetKeyFrames();

		if (vecKeyFrames.empty()) { return; }

		auto iter = vecKeyFrames.begin();

		for (int i = 0; i < m_iSelectedKeyFrameIndex; ++i) { ++iter; }

		vecKeyFrames.erase(iter);
		--dynamic_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->GetParticleInfo().m_iNumKeyFrames;

		m_wpInspector.lock()->RemoveKeyFrame(EEffectType::PARTICLE, m_iSelectedKeyFrameIndex);

		dynamic_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->SetCurKeyFrameIndex(0);
	}
}
void ToolEffect::CObjectList::UpdateCurParticleKeyFrame()
{
	if (m_wpSelectedObject.expired() || EEffectType::EFFECTMESH == m_wpSelectedObject.lock()->GetEffectType()
		|| EEffectType::InstanceEffectMesh == m_wpSelectedObject.lock()->GetEffectType())
		return;

	shared_ptr<CParticleObject> spParticle = static_pointer_cast<CParticleObject>(m_wpSelectedObject.lock());

	vector<FParticleKeyFrame>& vecKeyFrames = spParticle->GetKeyFrames();

	int32 iCurKeyFrameIndex = (int32)spParticle->GetCurKeyFrameIndex();
	if (ImGui::SliderInt("KeyFrame", &iCurKeyFrameIndex, 0, (int)vecKeyFrames.size() - 1))
	{
		
	}
}

void ToolEffect::CObjectList::UpdateEffectMeshTab()
{
	UpdateEffectMeshList();
	UpdateSelectedEffectMeshKeyFrameList();
	UpdateCurEffectMeshKeyFrame();
	UpdateEffectMeshKeyFrames();
	AddEffectMeshKeyFrame();
	RemoveEffectMeshKeyFrame();
	PauseEffect();
	NonActive();
	AddEffectToGroup();
	Reset();
}
void ToolEffect::CObjectList::UpdateEffectMeshList()
{
	ImGui::SeparatorText("Effect Mesh");

	shared_ptr<CLayer> spEffectMeshLayer = m_wpToolEffectLevel.lock()->FindLayer(L"Effect Mesh");
	if (!spEffectMeshLayer) { return; }

	// 이펙트 메시 리스트 업데이트
	const unordered_map<wstring, shared_ptr<CGameObject>>& umapEffectMeshs = spEffectMeshLayer->GetGameObjects();
	if (umapEffectMeshs.empty())
		return;

	UpdateObjectSelector(umapEffectMeshs);
}
void ToolEffect::CObjectList::UpdateSelectedEffectMeshKeyFrameList()
{
	if (m_wpSelectedObject.expired()) { return; }
	if (EEffectType::PARTICLE == m_wpSelectedObject.lock()->GetEffectType() ||
		EEffectType::InstanceEffectMesh == m_wpSelectedObject.lock()->GetEffectType()) { return; }

	const std::vector<FEffectMeshKeyFrame>& vecKeyFrames = dynamic_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetKeyFrames();

	if (vecKeyFrames.empty()) { return; }

	ImGui::SeparatorText("KeyFrame");
	if (ImGui::BeginListBox("EffectMesh KeyFrames", ImVec2(300.f, 100.f)))
	{
		for (uint16 i = 0; i < vecKeyFrames.size(); ++i)
		{
			const bool bSelected = (m_iEffectMeshKeyFrameSelector == vecKeyFrames.size() - 1);
			if (ImGui::Selectable(to_string(i).data(), ImGuiSelectableFlags_None, bSelected))
			{
				m_iEffectMeshColor1EasingType		= (int32)vecKeyFrames[i].m_eColor1EasingType;
				m_iEffectMeshColor2EasingType		= (int32)vecKeyFrames[i].m_eColor2EasingType;
				m_iEffectMeshColor3EasingType		= (int32)vecKeyFrames[i].m_eColor3EasingType;
				m_iEffectMeshScaleEasingType		= (int32)vecKeyFrames[i].m_eScaleEasingType;
				m_iEffectMeshRotationEasingType		= (int32)vecKeyFrames[i].m_eRotationEasingType;
				m_iEffectMeshTranslationEasingType	= (int32)vecKeyFrames[i].m_eTranslationEasingType;

				m_tEffectMeshKeyFrame = vecKeyFrames[i];

				m_iSelectedKeyFrameIndex = i;
			}
		}

		ImGui::EndListBox();
	}
}
void ToolEffect::CObjectList::UpdateEffectMeshKeyFrames()
{
	ImGui::SeparatorText("Add KeyFrame");

	/* 이펙트 셰이더 패스 인덱스 */
	ImGui::InputInt("Pass Index", &m_tEffectMeshKeyFrame.m_iPassIndex);

	/* 키프레임 시간 */
	ImGui::InputFloat("Time", &m_tEffectMeshKeyFrame.m_fTime);

	/* 렌더 컬 */
	ImGui::Checkbox("Render Culled", &m_tEffectMeshKeyFrame.m_bRenderCulled);

	/* Color1 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Effect Mesh KeyFrame Color1 -");
	ImGui::ColorEdit4("Effect Mesh KeyFrame Color1", &m_tEffectMeshKeyFrame.m_vColor1.x);
	ImGui::InputFloat("Effect Mesh KeyFrame Color1 Weight", &m_tEffectMeshKeyFrame.m_fColor1Weight);
	UpdateEasingTypeSelector(&m_iEffectMeshColor1EasingType, &m_tEffectMeshKeyFrame.m_eColor1EasingType, 3);
	if (EEasingType::EASE_IN_SQUARE == m_tEffectMeshKeyFrame.m_eColor1EasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tEffectMeshKeyFrame.m_eColor1EasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tEffectMeshKeyFrame.m_eColor1EasingType)
	{
		ImGui::InputFloat("Effect Mesh KeyFrame Color1 Easing Expo", &m_tEffectMeshKeyFrame.m_fColor1EasingExpo);
	}

	/* Color2 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Effect Mesh KeyFrame Color2 -");
	ImGui::ColorEdit4("Effect Mesh KeyFrame Color2", &m_tEffectMeshKeyFrame.m_vColor2.x);
	ImGui::InputFloat("Effect Mesh KeyFrame Color2 Weight", &m_tEffectMeshKeyFrame.m_fColor2Weight);
	UpdateEasingTypeSelector(&m_iEffectMeshColor2EasingType, &m_tEffectMeshKeyFrame.m_eColor2EasingType, 4);
	if (EEasingType::EASE_IN_SQUARE == m_tEffectMeshKeyFrame.m_eColor2EasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tEffectMeshKeyFrame.m_eColor2EasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tEffectMeshKeyFrame.m_eColor2EasingType)
	{
		ImGui::InputFloat("Effect Mesh KeyFrame Color2 Easing Expo", &m_tEffectMeshKeyFrame.m_fColor2EasingExpo);
	}

	/* Color3 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Effect Mesh KeyFrame Color3 -");
	ImGui::ColorEdit4("Effect Mesh KeyFrame Color3", &m_tEffectMeshKeyFrame.m_vColor3.x);
	ImGui::InputFloat("Effect Mesh KeyFrame Color3 Weight", &m_tEffectMeshKeyFrame.m_fColor3Weight);
	UpdateEasingTypeSelector(&m_iEffectMeshColor3EasingType, &m_tEffectMeshKeyFrame.m_eColor3EasingType, 5);
	if (EEasingType::EASE_IN_SQUARE == m_tEffectMeshKeyFrame.m_eColor3EasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tEffectMeshKeyFrame.m_eColor3EasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tEffectMeshKeyFrame.m_eColor3EasingType)
	{
		ImGui::InputFloat("Effect Mesh KeyFrame Color3 Easing Expo", &m_tEffectMeshKeyFrame.m_fColor3EasingExpo);
	}

	/* Scale */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Scale -");
	ImGui::InputFloat3("Scale", &m_tEffectMeshKeyFrame.m_vScale.x);
	UpdateEasingTypeSelector(&m_iEffectMeshScaleEasingType, &m_tEffectMeshKeyFrame.m_eScaleEasingType, 6);
	if (EEasingType::EASE_IN_SQUARE == m_tEffectMeshKeyFrame.m_eScaleEasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tEffectMeshKeyFrame.m_eScaleEasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tEffectMeshKeyFrame.m_eScaleEasingType)
	{
		ImGui::InputFloat("Effect Mesh KeyFrame Scale Easing Expo", &m_tEffectMeshKeyFrame.m_fScaleEasingExpo);
	}

	/* Rotation */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Rotation -");
	ImGui::InputFloat3("Rotation", &m_tEffectMeshKeyFrame.m_vRotation.x);
	ImGui::InputFloat("Rotate Weight", &m_tEffectMeshKeyFrame.m_fRotateWeight);
	UpdateEasingTypeSelector(&m_iEffectMeshRotationEasingType, &m_tEffectMeshKeyFrame.m_eRotationEasingType, 7);
	if (EEasingType::EASE_IN_SQUARE == m_tEffectMeshKeyFrame.m_eRotationEasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tEffectMeshKeyFrame.m_eRotationEasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tEffectMeshKeyFrame.m_eRotationEasingType)
	{
		ImGui::InputFloat("Effect Mesh KeyFrame Rotation Easing Expo", &m_tEffectMeshKeyFrame.m_fRotationEasingExpo);
	}

	/* Translation */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Translation -");
	ImGui::InputFloat3("Offset", &m_tEffectMeshKeyFrame.m_vTranslation.x);
	UpdateEasingTypeSelector(&m_iEffectMeshTranslationEasingType, &m_tEffectMeshKeyFrame.m_eTranslationEasingType, 8);
	if (EEasingType::EASE_IN_SQUARE == m_tEffectMeshKeyFrame.m_eTranslationEasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tEffectMeshKeyFrame.m_eTranslationEasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tEffectMeshKeyFrame.m_eTranslationEasingType)
	{
		ImGui::InputFloat("Effect Mesh KeyFrame Translation Easing Expo", &m_tEffectMeshKeyFrame.m_fTranslationEasingExpo);
	}
	ImGui::Dummy(ImVec2(0.f, 5.f));
}
void ToolEffect::CObjectList::AddEffectMeshKeyFrame()
{
	if (ImGui::Button("Add KeyFrame", ImVec2(120.f, 40.f)))
	{
		if (m_wpSelectedObject.expired())
			return;

		dynamic_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom()->AddKeyFrame(m_tEffectMeshKeyFrame);
		m_wpInspector.lock()->GetEffectMeshInfo().m_vecKeyFrames.push_back(m_tEffectMeshKeyFrame);
		++m_wpInspector.lock()->GetEffectMeshInfo().m_iNumKeyFrames;

		m_iSelectedKeyFrameIndex = uint16(m_wpInspector.lock()->GetEffectMeshInfo().m_vecKeyFrames.size() - 1);
	}
}
void ToolEffect::CObjectList::RemoveEffectMesh()
{
}
void ToolEffect::CObjectList::RemoveEffectMeshKeyFrame()
{
	ImGui::SameLine();
	if (ImGui::Button("Remove KeyFrame", ImVec2(120.f, 40.f)))
	{
		if (m_wpSelectedObject.expired()) { return; }
		if (EEffectType::PARTICLE == m_wpSelectedObject.lock()->GetEffectType() || EEffectType::InstanceEffectMesh == m_wpSelectedObject.lock()->GetEffectType()) { return; }

		vector<FEffectMeshKeyFrame>& vecKeyFrames = dynamic_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetKeyFrames();

		if (vecKeyFrames.empty()) { return; }

		auto iter = vecKeyFrames.begin();

		for (int i = 0; i < m_iSelectedKeyFrameIndex; ++i) { ++iter; }

		vecKeyFrames.erase(iter);
		--dynamic_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom()->GetEffectMeshInfo().m_iNumKeyFrames;

		m_wpInspector.lock()->RemoveKeyFrame(EEffectType::EFFECTMESH, m_iSelectedKeyFrameIndex);

		dynamic_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom()->SetCurKeyFrameIndex(0);
	}
}
void ToolEffect::CObjectList::UpdateCurEffectMeshKeyFrame()
{
	if (m_wpSelectedObject.expired() ||
		EEffectType::PARTICLE == m_wpSelectedObject.lock()->GetEffectType() ||
		EEffectType::InstanceEffectMesh == m_wpSelectedObject.lock()->GetEffectType())
		return;

	shared_ptr<CEffectMeshObject> spEffect = static_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock());

	const vector<FEffectMeshKeyFrame>& vecKeyFrames = spEffect->GetKeyFrames();

	int32 iCurKeyFrameIndex = (int32)spEffect->GetCurKeyFrameIndex();
	if (ImGui::SliderInt("KeyFrame", &iCurKeyFrameIndex, 0, (int)vecKeyFrames.size() - 1))
	{

	}
}

void ToolEffect::CObjectList::UpdateInstanceEffectMeshTab()
{
	UpdateInstanceEffectMeshList();
	UpdateSelectedInstanceEffectMeshKeyFrameList();
	UpdateCurInstanceEffectMeshKeyFrame();
	UpdateInstanceEffectMeshKeyFrames();
	AddInstanceEffectMeshKeyFrame();
	RemoveInstanceEffectMeshKeyFrame();
	PauseEffect();
	NonActive();
	AddEffectToGroup();
	Reset();
}
void ToolEffect::CObjectList::UpdateInstanceEffectMeshList()
{
	ImGui::SeparatorText("Instance Mesh");

	shared_ptr<CLayer> spInstanceEffectMeshLayer = m_wpToolEffectLevel.lock()->FindLayer(L"Instance Mesh");
	if (!spInstanceEffectMeshLayer) { return; }

	// 인스턴스 메시 리스트 업데이트
	const unordered_map<wstring, shared_ptr<CGameObject>>& umapInstanceEffectMeshs = spInstanceEffectMeshLayer->GetGameObjects();
	if (umapInstanceEffectMeshs.empty())
		return;

	UpdateObjectSelector(umapInstanceEffectMeshs);
}
void ToolEffect::CObjectList::UpdateSelectedInstanceEffectMeshKeyFrameList()
{
	if (m_wpSelectedObject.expired()) { return; }
	if (EEffectType::PARTICLE == m_wpSelectedObject.lock()->GetEffectType() ||
		EEffectType::EFFECTMESH == m_wpSelectedObject.lock()->GetEffectType()) { return; }

	const std::vector<FEffectMeshKeyFrame>& vecKeyFrames = dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetKeyFrames();

	if (vecKeyFrames.empty()) { return; }

	ImGui::SeparatorText("KeyFrame");
	if (ImGui::BeginListBox("Instance Mesh KeyFrames", ImVec2(300.f, 100.f)))
	{
		for (uint16 i = 0; i < vecKeyFrames.size(); ++i)
		{
			const bool bSelected = (m_iInstanceEffectMeshKeyFrameSelector == vecKeyFrames.size() - 1);
			if (ImGui::Selectable(to_string(i).data(), ImGuiSelectableFlags_None, bSelected))
			{
				m_iInstanceEffectMeshColor1EasingType			= (int32)vecKeyFrames[i].m_eColor1EasingType;
				m_iInstanceEffectMeshColor2EasingType			= (int32)vecKeyFrames[i].m_eColor2EasingType;
				m_iInstanceEffectMeshColor3EasingType			= (int32)vecKeyFrames[i].m_eColor3EasingType;
				m_iInstanceEffectMeshScaleEasingType			= (int32)vecKeyFrames[i].m_eScaleEasingType;
				m_iInstanceEffectMeshRotationEasingType		= (int32)vecKeyFrames[i].m_eRotationEasingType;
				m_iInstanceEffectMeshTranslationEasingType	= (int32)vecKeyFrames[i].m_eTranslationEasingType;

				m_tInstanceEffectMeshKeyFrame = vecKeyFrames[i];

				m_iSelectedKeyFrameIndex = i;
			}
		}

		ImGui::EndListBox();
	}
}
void ToolEffect::CObjectList::UpdateInstanceEffectMeshKeyFrames()
{
	ImGui::SeparatorText("Add KeyFrame");

	/* 이펙트 셰이더 패스 인덱스 */
	ImGui::InputInt("Pass Index", &m_tInstanceEffectMeshKeyFrame.m_iPassIndex);

	/* 키프레임 시간 */
	ImGui::InputFloat("Time", &m_tInstanceEffectMeshKeyFrame.m_fTime);

	/* 렌더 컬 */
	ImGui::Checkbox("Render Culled", &m_tInstanceEffectMeshKeyFrame.m_bRenderCulled);

	/* Color1 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Instance Mesh KeyFrame Color1 -");
	ImGui::ColorEdit4("Instance Mesh KeyFrame Color1", &m_tInstanceEffectMeshKeyFrame.m_vColor1.x);
	ImGui::InputFloat("Instance Mesh KeyFrame Color1 Weight", &m_tInstanceEffectMeshKeyFrame.m_fColor1Weight);
	UpdateEasingTypeSelector(&m_iInstanceEffectMeshColor1EasingType, &m_tInstanceEffectMeshKeyFrame.m_eColor1EasingType, 3);
	if (EEasingType::EASE_IN_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eColor1EasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eColor1EasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eColor1EasingType)
	{
		ImGui::InputFloat("Instance Mesh KeyFrame Color1 Easing Expo", &m_tInstanceEffectMeshKeyFrame.m_fColor1EasingExpo);
	}

	/* Color2 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Instance Mesh KeyFrame Color2 -");
	ImGui::ColorEdit4("Instance Mesh KeyFrame Color2", &m_tInstanceEffectMeshKeyFrame.m_vColor2.x);
	ImGui::InputFloat("Instance Mesh KeyFrame Color2 Weight", &m_tInstanceEffectMeshKeyFrame.m_fColor2Weight);
	UpdateEasingTypeSelector(&m_iInstanceEffectMeshColor2EasingType, &m_tInstanceEffectMeshKeyFrame.m_eColor2EasingType, 4);
	if (EEasingType::EASE_IN_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eColor2EasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eColor2EasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eColor2EasingType)
	{
		ImGui::InputFloat("Instance Mesh KeyFrame Color2 Easing Expo", &m_tInstanceEffectMeshKeyFrame.m_fColor2EasingExpo);
	}

	/* Color3 */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Instance Mesh KeyFrame Color3 -");
	ImGui::ColorEdit4("Instance Mesh KeyFrame Color3", &m_tInstanceEffectMeshKeyFrame.m_vColor3.x);
	ImGui::InputFloat("Instance Mesh KeyFrame Color3 Weight", &m_tInstanceEffectMeshKeyFrame.m_fColor3Weight);
	UpdateEasingTypeSelector(&m_iInstanceEffectMeshColor3EasingType, &m_tInstanceEffectMeshKeyFrame.m_eColor3EasingType, 5);
	if (EEasingType::EASE_IN_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eColor3EasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eColor3EasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eColor3EasingType)
	{
		ImGui::InputFloat("Instance Mesh KeyFrame Color3 Easing Expo", &m_tInstanceEffectMeshKeyFrame.m_fColor3EasingExpo);
	}

	/* Scale */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Scale -");
	ImGui::InputFloat3("Scale", &m_tInstanceEffectMeshKeyFrame.m_vScale.x);
	UpdateEasingTypeSelector(&m_iInstanceEffectMeshScaleEasingType, &m_tInstanceEffectMeshKeyFrame.m_eScaleEasingType, 6);
	if (EEasingType::EASE_IN_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eScaleEasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eScaleEasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eScaleEasingType)
	{
		ImGui::InputFloat("Instance Mesh KeyFrame Scale Easing Expo", &m_tInstanceEffectMeshKeyFrame.m_fScaleEasingExpo);
	}

	/* Rotation */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Rotation -");
	ImGui::InputFloat3("Rotation", &m_tInstanceEffectMeshKeyFrame.m_vRotation.x);
	ImGui::InputFloat("Rotate Weight", &m_tInstanceEffectMeshKeyFrame.m_fRotateWeight);
	UpdateEasingTypeSelector(&m_iInstanceEffectMeshRotationEasingType, &m_tInstanceEffectMeshKeyFrame.m_eRotationEasingType, 7);
	if (EEasingType::EASE_IN_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eRotationEasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eRotationEasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eRotationEasingType)
	{
		ImGui::InputFloat("Instance Mesh KeyFrame Rotation Easing Expo", &m_tInstanceEffectMeshKeyFrame.m_fRotationEasingExpo);
	}

	/* Translation */
	ImGui::Dummy(ImVec2(0.f, 5.f));
	ImGui::Text("- Translation -");
	ImGui::InputFloat3("Offset", &m_tInstanceEffectMeshKeyFrame.m_vTranslation.x);
	UpdateEasingTypeSelector(&m_iInstanceEffectMeshTranslationEasingType, &m_tInstanceEffectMeshKeyFrame.m_eTranslationEasingType, 8);
	if (EEasingType::EASE_IN_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eTranslationEasingType ||
		EEasingType::EASE_OUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eTranslationEasingType ||
		EEasingType::EASE_INOUT_SQUARE == m_tInstanceEffectMeshKeyFrame.m_eTranslationEasingType)
	{
		ImGui::InputFloat("Instance Mesh KeyFrame Translation Easing Expo", &m_tInstanceEffectMeshKeyFrame.m_fTranslationEasingExpo);
	}
	ImGui::Dummy(ImVec2(0.f, 5.f));
}
void ToolEffect::CObjectList::AddInstanceEffectMeshKeyFrame()
{
	if (ImGui::Button("Add KeyFrame", ImVec2(120.f, 40.f)))
	{
		if (m_wpSelectedObject.expired()) { return; }

		dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->AddKeyFrame(m_tInstanceEffectMeshKeyFrame);
		m_wpInspector.lock()->GetInstanceEffectMeshEffectMeshInfo().m_vecKeyFrames.push_back(m_tInstanceEffectMeshKeyFrame);
		++m_wpInspector.lock()->GetInstanceEffectMeshEffectMeshInfo().m_iNumKeyFrames;

		m_iSelectedKeyFrameIndex = uint16(m_wpInspector.lock()->GetInstanceEffectMeshEffectMeshInfo().m_vecKeyFrames.size() - 1);
	}
}
void ToolEffect::CObjectList::RemoveInstanceEffectMesh()
{
}
void ToolEffect::CObjectList::RemoveInstanceEffectMeshKeyFrame()
{
	ImGui::SameLine();
	if (ImGui::Button("Remove KeyFrame", ImVec2(120.f, 40.f)))
	{
		if (m_wpSelectedObject.expired()) { return; }
		if (EEffectType::PARTICLE == m_wpSelectedObject.lock()->GetEffectType() || EEffectType::EFFECTMESH == m_wpSelectedObject.lock()->GetEffectType()) { return; }

		vector<FEffectMeshKeyFrame>& vecKeyFrames = dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetKeyFrames();
		if (vecKeyFrames.empty()) { return; }

		auto iter = vecKeyFrames.begin();

		for (int i = 0; i < m_iSelectedKeyFrameIndex; ++i) { ++iter; }

		vecKeyFrames.erase(iter);
		--dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->GetEffectMeshInfo().m_iNumKeyFrames;

		m_wpInspector.lock()->RemoveKeyFrame(EEffectType::InstanceEffectMesh, m_iSelectedKeyFrameIndex);

		dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->SetCurKeyFrameIndex(0);
	}
}
void ToolEffect::CObjectList::UpdateCurInstanceEffectMeshKeyFrame()
{
	if (m_wpSelectedObject.expired() ||
		EEffectType::PARTICLE == m_wpSelectedObject.lock()->GetEffectType() ||
		EEffectType::EFFECTMESH == m_wpSelectedObject.lock()->GetEffectType())
		return;

	shared_ptr<CInstanceEffectMeshObject> spEffect = static_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock());

	const vector<FEffectMeshKeyFrame>& vecKeyFrames = spEffect->GetKeyFrames();

	int32 iCurKeyFrameIndex = (int32)spEffect->GetCurKeyFrameIndex();
	if (ImGui::SliderInt("KeyFrame", &iCurKeyFrameIndex, 0, (int)vecKeyFrames.size() - 1))
	{

	}
}

void ToolEffect::CObjectList::PauseEffect()
{
	if (ImGui::Button("Pause", ImVec2(120.f, 40.f)))
	{
		if (m_wpSelectedObject.expired()) { return; }

		switch (m_wpSelectedObject.lock()->GetEffectType())
		{
		case EEffectType::PARTICLE:
		{
			static_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->SetPause();
			break;
		}
		case EEffectType::EFFECTMESH:
		{
			static_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom()->SetPause();
			break;
		}
		case EEffectType::InstanceEffectMesh:
		{
			static_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->SetPause();
		}
		}
	}
}
void ToolEffect::CObjectList::NonActive()
{
	ImGui::SameLine();
	if (ImGui::Button("NonActive", ImVec2(120.f, 40.f)))
	{
		if (m_wpSelectedObject.expired()) { return; }

		switch (m_wpSelectedObject.lock()->GetEffectType())
		{
		case EEffectType::PARTICLE:
		{
			if (m_wpSelectedObject.lock()->IsActive())
			{
				static_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->EndLifeTime();
			}
			else
			{
				static_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->RecycleFromPool();
			}
			break;
		}
		case EEffectType::EFFECTMESH:
		{
			if (m_wpSelectedObject.lock()->IsActive())
			{
				static_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom()->EndLifeTime();
			}
			else
			{
				static_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom()->RecycleFromPool();
			}
			break;
		}
		case EEffectType::InstanceEffectMesh:
		{
			if (m_wpSelectedObject.lock()->IsActive())
			{
				static_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->EndLifeTime();
			}
			else
			{
				static_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->RecycleFromPool();
			}
			break;
		}
		}
	}
}
void ToolEffect::CObjectList::AddEffectToGroup()
{
	if (ImGui::Button("Add to EffectGroup", ImVec2(120.f, 40.f)))
	{
		if (m_wpSelectedObject.expired()) { return; }

		shared_ptr<CEffectGroup> spEffectGroup = m_wpEffectGroupWindow.lock()->GetCurEffectGroup();
		if (!spEffectGroup) { return; }

		// 이펙트를 정보를 복제하여 새로운 객체를 생성한 뒤 이펙트 그룹에 추가합니다.
		switch (m_wpSelectedObject.lock()->GetEffectType())
		{
		case EEffectType::PARTICLE:
		{
			/* 구조체 준비 */
			CParticleObject::FParticleObjectDesc tParticleObjectDesc = {};
			FParticleInfo tParticleInfo = static_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->GetParticleInfo();

			tParticleObjectDesc.m_wstrGameObjectName = m_wpSelectedObject.lock()->GetGameObjectName();
			tParticleObjectDesc.m_eEffectType = EEffectType::PARTICLE;

			/* 객체 생성 */
			shared_ptr<CParticleObject> spParticleObject = CParticleObject::Create(make_shared<CParticleObject::FParticleObjectDesc>(tParticleObjectDesc), tParticleInfo);
			if (!spParticleObject) { return; }

			/* 이펙트 그룹에 추가 */
			spEffectGroup->AddEffect(tParticleObjectDesc.m_wstrGameObjectName, spParticleObject);

			break;
		}
		case EEffectType::EFFECTMESH:
		{
			/* 구조체 준비 */
			CEffectMeshObject::FEffectMeshObjectDesc tEffectMeshObjectDesc = {};
			FEffectMeshInfo tEffectMeshInfo = static_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom()->GetEffectMeshInfo();

			tEffectMeshObjectDesc.m_wstrGameObjectName	= m_wpSelectedObject.lock()->GetGameObjectName();
			tEffectMeshObjectDesc.m_eEffectType			= EEffectType::EFFECTMESH;

			tEffectMeshObjectDesc.m_wstrJsonFileName	= L"";
			tEffectMeshObjectDesc.m_wstrModelFilePath	= tEffectMeshInfo.m_wstrModelFilePath;

			tEffectMeshObjectDesc.m_vInitScale			= tEffectMeshInfo.m_vScale;
			tEffectMeshObjectDesc.m_vInitRotate			= tEffectMeshInfo.m_vRotation;
			tEffectMeshObjectDesc.m_vInitPosition		= tEffectMeshInfo.m_vTranslation;

			tEffectMeshObjectDesc.m_matPivot			= Matrix::CreateScale(0.01f) *
				Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0.f, 1.f, 0.f), XMConvertToRadians(180.f))) *
				Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(1.f, 0.f, 0.f), XMConvertToRadians(-90.f)));

			/* 객체 생성 */
			shared_ptr<CEffectMeshObject> spEffectMeshObject = CEffectMeshObject::Create(make_shared<CEffectMeshObject::FEffectMeshObjectDesc>(tEffectMeshObjectDesc), tEffectMeshInfo);
			if (!spEffectMeshObject) { return; }

			/* 이펙트 그룹에 추가 */
			spEffectGroup->AddEffect(tEffectMeshObjectDesc.m_wstrGameObjectName, spEffectMeshObject);

			break;
		}
		case EEffectType::InstanceEffectMesh:
		{
			/* 구조체 준비 */
			CInstanceEffectMeshObject::FInstanceEffectMeshObjectDesc tInstanceEffectMeshObjectDesc = {};

			FEffectMeshInfo		tEffectMeshInfo		= static_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->GetEffectMeshInfo();
			FInstanceEffectMeshInfo	tInstanceEffectMeshInfo	= static_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->GetInstanceEffectMeshInfo();

			tInstanceEffectMeshObjectDesc.m_wstrGameObjectName	= m_wpSelectedObject.lock()->GetGameObjectName();
			tInstanceEffectMeshObjectDesc.m_eEffectType			= EEffectType::InstanceEffectMesh;

			tInstanceEffectMeshObjectDesc.m_wstrJsonFileName		= L"";
			tInstanceEffectMeshObjectDesc.m_wstrModelFilePath		= tEffectMeshInfo.m_wstrModelFilePath;

			tInstanceEffectMeshObjectDesc.m_vInitScale				= tEffectMeshInfo.m_vScale;
			tInstanceEffectMeshObjectDesc.m_vInitRotate				= tEffectMeshInfo.m_vRotation;
			tInstanceEffectMeshObjectDesc.m_vInitPosition			= tEffectMeshInfo.m_vTranslation;

			tInstanceEffectMeshObjectDesc.m_matPivot = Matrix::CreateScale(0.01f) *
				Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0.f, 1.f, 0.f), XMConvertToRadians(180.f))) *
				Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(1.f, 0.f, 0.f), XMConvertToRadians(-90.f)));

			/* 객체 생성 */
			shared_ptr<CInstanceEffectMeshObject> spInstanceEffectMeshObject = CInstanceEffectMeshObject::Create(make_shared<CInstanceEffectMeshObject::FInstanceEffectMeshObjectDesc>(tInstanceEffectMeshObjectDesc), tEffectMeshInfo, tInstanceEffectMeshInfo);
			if (!spInstanceEffectMeshObject) { return; }

			/* 이펙트 그룹에 추가 */
			spEffectGroup->AddEffect(tInstanceEffectMeshObjectDesc.m_wstrGameObjectName, spInstanceEffectMeshObject);

			break;
		}
		}
	}
}
void ToolEffect::CObjectList::Reset()
{
	ImGui::SameLine();
	if (ImGui::Button("Reset", ImVec2(120.f, 40.f)))
	{
		if (m_wpSelectedObject.expired()) { return; }

		m_wpSelectedObject.lock()->SetPosition(Vector3(0.f, 0.f, 0.f));

		switch (m_wpSelectedObject.lock()->GetEffectType())
		{
		case EEffectType::PARTICLE:
		{
			shared_ptr<CParticleComponent> spParticleCom = static_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom();
			if (!spParticleCom) { return; }

			spParticleCom->RecycleFromPool();
			break;
		}
		case EEffectType::EFFECTMESH:
		{
			shared_ptr<CEffectMeshComponent> spEffectMeshCom = static_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom();
			if (!spEffectMeshCom) { return; }

			spEffectMeshCom->RecycleFromPool();
			break;
		}
		case EEffectType::InstanceEffectMesh:
		{
			shared_ptr<CInstanceEffectMeshComponent> spInstanceEffectMeshCom = static_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom();
			if (!spInstanceEffectMeshCom) { return; }

			spInstanceEffectMeshCom->RecycleFromPool();
			break;
		}
		}
	}
}

void ToolEffect::CObjectList::UpdateObjectSelector(const unordered_map<wstring, shared_ptr<CGameObject>>& _umapObjects)
{
	map<wstring, shared_ptr<CGameObject>> mapObjects;

	for (auto& pair : _umapObjects)
	{
		mapObjects.emplace(pair.first, pair.second);
	}

	if (ImGui::BeginListBox("Effects", ImVec2(300.f, 200.f)))
	{
		for (auto& pair : mapObjects)
		{
			string strFilePath = CStringUtility::ToString(pair.first);
			char szFileName[MAX_PATH] = "";
			_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

			const bool bSelected = (m_iObjectSelector == mapObjects.size() - 1);
			if (ImGui::Selectable(szFileName, ImGuiSelectableFlags_None, bSelected))
			{
				m_wpSelectedObject = static_pointer_cast<CEffectBase>(pair.second);

				switch (m_wpSelectedObject.lock()->GetEffectType())
				{
				case EEffectType::PARTICLE:
				{
					m_wpInspector.lock()->SetParticleInfo(dynamic_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->GetParticleInfo());
					m_wpInspector.lock()->SetParticleRenderType(dynamic_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->GetParticleInfo().m_eRenderType);
					m_wpInspector.lock()->SetParticleType(dynamic_pointer_cast<CParticleObject>(m_wpSelectedObject.lock())->GetParticleCom()->GetParticleInfo().m_eParticleType);
					break;
				}
				case EEffectType::EFFECTMESH:
				{
					m_wpInspector.lock()->SetEffectMeshInfo(dynamic_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom()->GetEffectMeshInfo());
					m_wpInspector.lock()->SetEffectMeshRenderType(dynamic_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock())->GetEffectMeshCom()->GetEffectMeshInfo().m_eRenderType);
					break;
				}
				case EEffectType::InstanceEffectMesh:
				{
					m_wpInspector.lock()->SetInstanceEffectMeshEffectMeshInfo(dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->GetEffectMeshInfo());
					m_wpInspector.lock()->SetInstanceEffectMeshInfo(dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->GetInstanceEffectMeshInfo());
					m_wpInspector.lock()->SetInstanceEffectMeshRenderType(dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock())->GetInstanceEffectMeshCom()->GetEffectMeshInfo().m_eRenderType);
					break;
				}
				}
			}
		}

		ImGui::EndListBox();
	}
}
void ToolEffect::CObjectList::UpdateEasingTypeSelector(int32* _pSelector, EEasingType* _pEasingType, int32 iIndex)
{
	/* 렌더타입 */// -> 추후에 렌더타입이 추가될 경우 수정될 수 있습니다.
	array<const char*, (int16)EEasingType::ENUM_END> arrEasingType = { "NONE", "EASE_LINEAR", "EASE_IN_SINE", "EASE_OUT_SINE", "EASE_INOUT_SINE", "EASE_IN_SQUARE", "EASE_OUT_SQUARE",
																	   "EASE_INOUT_SQUARE", "EASE_IN_CIRC", "EASE_OUT_CIRC", "EASE_INOUT_CIRC", "EASE_IN_BACK", "EASE_OUT_BACK",
																	   "EASE_INOUT_BACK", "EASE_IN_ELASTIC", "EASE_OUT_ELASTIC", "EASE_INOUT_ELASTIC", "EASE_IN_BOUNCE", "EASE_OUT_BOUNCE",
																	   "EASE_INOUT_BOUNCE" };

	switch (*_pSelector)
	{
	case 0: *_pEasingType	= EEasingType::EASE_NONE;			break;
	case 1: *_pEasingType	= EEasingType::EASE_LINEAR;			break;
	case 2: *_pEasingType	= EEasingType::EASE_IN_SINE;		break;
	case 3: *_pEasingType	= EEasingType::EASE_OUT_SINE;		break;
	case 4: *_pEasingType	= EEasingType::EASE_INOUT_SINE;		break;
	case 5: *_pEasingType	= EEasingType::EASE_IN_SQUARE;		break;
	case 6: *_pEasingType	= EEasingType::EASE_OUT_SQUARE;		break;
	case 7: *_pEasingType	= EEasingType::EASE_INOUT_SQUARE;	break;
	case 8: *_pEasingType	= EEasingType::EASE_IN_CIRC;		break;
	case 9: *_pEasingType	= EEasingType::EASE_OUT_CIRC;		break;
	case 10: *_pEasingType	= EEasingType::EASE_INOUT_CIRC;		break;
	case 11: *_pEasingType	= EEasingType::EASE_IN_BACK;		break;
	case 12: *_pEasingType	= EEasingType::EASE_OUT_BACK;		break;
	case 13: *_pEasingType	= EEasingType::EASE_INOUT_BACK;		break;
	case 14: *_pEasingType	= EEasingType::EASE_IN_ELASTIC;		break;
	case 15: *_pEasingType	= EEasingType::EASE_OUT_ELASTIC;	break;
	case 16: *_pEasingType	= EEasingType::EASE_INOUT_ELASTIC;	break;
	case 17: *_pEasingType	= EEasingType::EASE_IN_BOUNCE;		break;
	case 18: *_pEasingType	= EEasingType::EASE_OUT_BOUNCE;		break;
	case 19: *_pEasingType	= EEasingType::EASE_INOUT_BOUNCE;	break;
	default: break;
	}

	string strComboBoxName = "Easing Type " + to_string(iIndex);

	ImGui::Combo(strComboBoxName.c_str(), _pSelector, arrEasingType.data(), (int16)arrEasingType.size());
}

std::shared_ptr<CEffectBase> ToolEffect::CObjectList::GetEffectBase()
{
	if (m_wpSelectedObject.expired()) { return nullptr; }

	return m_wpSelectedObject.lock();
}
std::shared_ptr<CParticleObject> ToolEffect::CObjectList::GetParticleObject()
{
	if (m_wpSelectedObject.expired()) { return nullptr; }
	if (EEffectType::EFFECTMESH == m_wpSelectedObject.lock()->GetEffectType() ||
		EEffectType::InstanceEffectMesh == m_wpSelectedObject.lock()->GetEffectType())
	{
		return nullptr;
	}

	return dynamic_pointer_cast<CParticleObject>(m_wpSelectedObject.lock());
}
std::shared_ptr<CEffectMeshObject> ToolEffect::CObjectList::GetEffectMeshObject()
{
	if (m_wpSelectedObject.expired()) { return nullptr; }
	if (EEffectType::PARTICLE == m_wpSelectedObject.lock()->GetEffectType() ||
		EEffectType::InstanceEffectMesh == m_wpSelectedObject.lock()->GetEffectType()) { return nullptr; }

	return dynamic_pointer_cast<CEffectMeshObject>(m_wpSelectedObject.lock());
}
std::shared_ptr<CInstanceEffectMeshObject> ToolEffect::CObjectList::GetInstanceEffectMeshObject()
{
	if (m_wpSelectedObject.expired()) { return nullptr; }
	if (EEffectType::PARTICLE == m_wpSelectedObject.lock()->GetEffectType() ||
		EEffectType::EFFECTMESH == m_wpSelectedObject.lock()->GetEffectType())
	{
		return nullptr;
	}

	return dynamic_pointer_cast<CInstanceEffectMeshObject>(m_wpSelectedObject.lock());
}
