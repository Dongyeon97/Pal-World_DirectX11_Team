// Default
#include "pch.h"
#include "EffectGroupWindow.h"
#include "StringUtility.h"

// Level
#include "ToolEffectLevel.h"

// GameObject
#include "EffectGroup.h"
#include "ParticleObject.h"
#include "EffectMeshObject.h"
#include "InstanceEffectMeshObject.h"

// Component
#include "ParticleComponent.h"
#include "EffectMeshComponent.h"
#include "InstanceEffectMeshComponent.h"
#include "SphereComponent.h"
#include "BoxComponent.h"

HRESULT ToolEffect::CEffectGroupWindow::Initialize()
{
	LoadEffectGroupNamesFromPath();

	return CWindow::Initialize();
}

HRESULT ToolEffect::CEffectGroupWindow::LoadEffectGroupNamesFromPath()
{
	for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(m_wstrEffectGroupPath))
	{
		// 파일이 아닌 경우
		if (!filesystem::is_regular_file(fsEntry)) { continue; }

		wstring wstrJsonFileName = fsEntry.path().stem().generic_wstring();
		m_vecEffectGroupNames.push_back(wstrJsonFileName);
	}

	return S_OK;
}

HRESULT ToolEffect::CEffectGroupWindow::ParseEffectGroupToJSON(const wstring& _wstrJsonFileName)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	writer.StartObject();

	writer.Key("Effect Group Name");
	writer.String(CStringUtility::ToString(_wstrJsonFileName).c_str());
	writer.Key("Duration");
	writer.Double(m_spCurEffectGroup->GetDuration());
	writer.Key("Loop");
	writer.Bool(m_spCurEffectGroup->GetLoop());

	writer.Key("Is Collider");
	writer.Bool(m_spCurEffectGroup->GetColliderInfo().m_bCollider);
	
	// 사운드
	writer.Key("Sound Name");
	writer.String(CStringUtility::ToString(m_spCurEffectGroup->GetSoundName()).c_str());
	writer.Key("Sound Loop");
	writer.Bool(m_spCurEffectGroup->GetSoundLoop());
	writer.Key("Sound Value");
	writer.Double(m_spCurEffectGroup->GetSoundValue());

	writer.Key("Collider Type");
	writer.Uint((uint32)m_spCurEffectGroup->GetColliderInfo().m_eColliderType);
	writer.Key("Collision Flag");
	writer.Uint((uint32)m_spCurEffectGroup->GetColliderInfo().m_eCollisionFlag);
	writer.Key("Collision Layer");
	writer.Uint((uint32)m_spCurEffectGroup->GetColliderInfo().m_eCollisionLayer);

	writer.Key("Sphere Collider Radius");
	writer.Double(m_spCurEffectGroup->GetColliderInfo().m_fRadius);

	writer.Key("Box Collider Extents");
	writer.StartArray();
	writer.Double(m_spCurEffectGroup->GetColliderInfo().m_vExtents.x);
	writer.Double(m_spCurEffectGroup->GetColliderInfo().m_vExtents.y);
	writer.Double(m_spCurEffectGroup->GetColliderInfo().m_vExtents.z);
	writer.EndArray();

	writer.Key("Box Collider LocalPosition");
	writer.StartArray();
	writer.Double(m_spCurEffectGroup->GetColliderInfo().m_vLocalPosition.x);
	writer.Double(m_spCurEffectGroup->GetColliderInfo().m_vLocalPosition.y);
	writer.Double(m_spCurEffectGroup->GetColliderInfo().m_vLocalPosition.z);
	writer.EndArray();

	writer.Key("Box Collider LocalRotate");
	writer.StartArray();
	writer.Double(m_spCurEffectGroup->GetColliderInfo().m_vLocalRotate.x);
	writer.Double(m_spCurEffectGroup->GetColliderInfo().m_vLocalRotate.y);
	writer.Double(m_spCurEffectGroup->GetColliderInfo().m_vLocalRotate.z);
	writer.EndArray();

	writer.Key("Effects");
	writer.StartArray();
	{
		uint16 iIndex = 0;
		for (auto& pair : m_spCurEffectGroup->GetEffects())
		{
			string strName = "";

			switch (pair.second->GetEffectType())
			{
			case EEffectType::PARTICLE:
			{
				static_pointer_cast<CParticleObject>(pair.second)->GetParticleCom()->WriteToJsonFile(_wstrJsonFileName, buffer, writer);
				break;
			}
			case EEffectType::EFFECTMESH:
			{
				static_pointer_cast<CEffectMeshObject>(pair.second)->GetEffectMeshCom()->WriteToJsonFile(_wstrJsonFileName, buffer, writer);
				break;
			}
			case EEffectType::InstanceEffectMesh:
			{
				static_pointer_cast<CInstanceEffectMeshObject>(pair.second)->GetInstanceEffectMeshCom()->WriteToJsonFile(_wstrJsonFileName, buffer, writer);
				break;
			}
			}

			++iIndex;
		}
	}
	writer.EndArray();

	writer.EndObject();

	ofstream osJsonpath(L"../../Resource/Effects/EffectGroup/" + _wstrJsonFileName + L".json");
	if (!osJsonpath.is_open())
	{
		throw runtime_error("failed to open json file : CEffectMeshComponent");
	}

	osJsonpath << buffer.GetString();
	osJsonpath.close();

	return S_OK;
}

HRESULT ToolEffect::CEffectGroupWindow::BeginPlay()
{
	return CWindow::BeginPlay();
}

int32 ToolEffect::CEffectGroupWindow::PreTick(float _fDeltaSeconds)
{
	return CWindow::PreTick(_fDeltaSeconds);
}

int32 ToolEffect::CEffectGroupWindow::Tick(float _fDeltaSeconds)
{
	if (ImGui::Begin("Effect Groups", &m_bEffectGroupWindow))
	{
		if(ImGui::BeginTabBar("Effect Group"))
		{
			/* 이펙트 그룹 파일 */
			if (ImGui::BeginTabItem("File"))
			{
				UpdateEffectGroupNames();
				CreateEffectGroupFromFile();
				ImGui::EndTabItem();
			}
			/* 현재 레벨의 이펙트 그룹 업데이트 */
			if (ImGui::BeginTabItem("Level EffectGroups"))
			{
				UpdateLevelEffectGroups();
				CreateEffectGroup();
				RemoveEffectGroup();
				PauseEffectGroup();
				ResetEffectGroup();
				SaveEffectGroup();
				LoopEffectGroup();
				PrintTrackPosition();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Collider"))
			{
				UpdateAttachingColliderTab();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		
		ImGui::End();
	}

	return CWindow::Tick(_fDeltaSeconds);
}

int32 ToolEffect::CEffectGroupWindow::PostTick(float _fDeltaSeconds)
{
	return CWindow::PostTick(_fDeltaSeconds);
}

int32 ToolEffect::CEffectGroupWindow::Render()
{
	return CWindow::Render();
}

int32 ToolEffect::CEffectGroupWindow::EndPlay()
{
	return CWindow::EndPlay();
}

void ToolEffect::CEffectGroupWindow::Release()
{
	CWindow::Release();
}

void ToolEffect::CEffectGroupWindow::UpdateEffectGroupNames()
{
	if (ImGui::BeginListBox("Files", ImVec2(300.f, 150.f)))
	{
		for (size_t i = 0; i < m_vecEffectGroupNames.size(); ++i)
		{
			const bool bSelected = (m_iEffectGroupFileSelector == m_vecEffectGroupNames.size() - 1);
			if (ImGui::Selectable(CStringUtility::ToString(m_vecEffectGroupNames[i]).c_str(), ImGuiSelectableFlags_None, bSelected))
			{
				m_iEffectGroupFileSelector = (uint16)i;
			}
		}

		ImGui::EndListBox();
	}
}

void ToolEffect::CEffectGroupWindow::CreateEffectGroupFromFile()
{
	if (ImGui::Button("Create From Json", ImVec2(150.f, 40.f)))
	{
		/* 파일이름 리스트에서 현재 선택된 JSON파일 이름을 가져옵니다. */
		wstring wstrJsonFileName = m_vecEffectGroupNames[m_iEffectGroupFileSelector];
		if (wstrJsonFileName.empty()) { return; }

		/* FEffectGroupDesc */
		CEffectGroup::FEffectGroupDesc tEffectGroupDesc;
		tEffectGroupDesc.m_wstrEffectGroupName = wstrJsonFileName;

		/* JSON파일로부터 객체를 생성합니다. */
		shared_ptr<CEffectGroup> spEffectGroup = CEffectGroup::CreateFromJson(make_shared<CEffectGroup::FEffectGroupDesc>(tEffectGroupDesc), wstrJsonFileName);
		CHECKF(spEffectGroup, L"Failed to CreateFromJson : CEffectGroupWindow");

		m_mapEffectGroups.emplace(wstrJsonFileName, spEffectGroup);
		m_spCurEffectGroup = spEffectGroup;
		m_wstrCurEffectGroupName = wstrJsonFileName;

		m_wpToolEffectLevel.lock()->AddGameObject(spEffectGroup);
	}
}

// ========================================================================================

void ToolEffect::CEffectGroupWindow::UpdateLevelEffectGroups()
{
	if (ImGui::BeginListBox("Groups", ImVec2(300.f, 150.f)))
	{
		for (auto& pair : m_mapEffectGroups)
		{
			const bool bSelected = (m_iCurEffectGroupSelector == m_mapEffectGroups.size() - 1);
			if (ImGui::Selectable(CStringUtility::ToString(pair.first).c_str(), ImGuiSelectableFlags_None, bSelected))
			{
				m_wstrCurEffectGroupName = pair.first;
				m_spCurEffectGroup = pair.second;
			}
		}

		ImGui::EndListBox();
	}
}

void ToolEffect::CEffectGroupWindow::CreateEffectGroup()
{
	ImGui::InputTextWithHint("Name", "Effect Group Name", m_szEffectGroupName, MAX_PATH);
	ImGui::SameLine();
	ImGui::InputFloat("Duration", &m_fEffectGroupDuration);

	if (ImGui::Button("Create", ImVec2(100.f, 40.f)))
	{
		/* 파일이름 리스트에서 현재 선택된 JSON파일 이름을 가져옵니다. */
		wstring wstrEffectGroupName = CStringUtility::ToWideString(m_szEffectGroupName);
		if (wstrEffectGroupName.empty()) { return; }

		/* FEffectGroupDesc */
		CEffectGroup::FEffectGroupDesc tEffectGroupDesc;
		tEffectGroupDesc.m_wstrEffectGroupName = wstrEffectGroupName;
		tEffectGroupDesc.m_fDuration = m_fEffectGroupDuration;

		/* EffectGroup 객체를 생성합니다. */
		shared_ptr<CEffectGroup> spEffectGroup = CEffectGroup::Create(make_shared<CEffectGroup::FEffectGroupDesc>(tEffectGroupDesc));
		CHECKF(spEffectGroup, L"Failed to CreateFromJson : CEffectGroupWindow");

		m_mapEffectGroups.emplace(wstrEffectGroupName, spEffectGroup);
		m_spCurEffectGroup = spEffectGroup;
		m_wstrCurEffectGroupName = wstrEffectGroupName;

		m_wpToolEffectLevel.lock()->AddGameObject(spEffectGroup);
		spEffectGroup->SetOwnerLevel(m_wpToolEffectLevel.lock());
		// m_wpToolEffectLevel.lock()->AddGameObjectToLayer(L"Effect Group", spEffectGroup);
	}
}

void ToolEffect::CEffectGroupWindow::RemoveEffectGroup()
{
	ImGui::SameLine();
	if (ImGui::Button("Remove", ImVec2(100.f, 40.f)))
	{
		RemoveEffectGroup(m_wstrCurEffectGroupName);
	}
}

void ToolEffect::CEffectGroupWindow::PauseEffectGroup()
{
	ImGui::SameLine();
	if (ImGui::Button("Pause", ImVec2(100.f, 40.f)))
	{
		if (!m_spCurEffectGroup) { return; }

		m_spCurEffectGroup->SetPause();
	}
}

void ToolEffect::CEffectGroupWindow::ResetEffectGroup()
{
	ImGui::SameLine();
	if (ImGui::Button("Reset", ImVec2(100.f, 40.f)))
	{
		if (!m_spCurEffectGroup) { return; }

		// m_spCurEffectGroup->SetPosition(Vector3(0.f, 0.f, 0.f));
		m_spCurEffectGroup->Reset();
	}
}

void ToolEffect::CEffectGroupWindow::SaveEffectGroup()
{
	ImGui::SameLine();
	if (ImGui::Button("Save", ImVec2(100.f, 40.f)))
	{
		if (!m_spCurEffectGroup) { return; }

		ParseEffectGroupToJSON(m_wstrCurEffectGroupName);
	}
}

void ToolEffect::CEffectGroupWindow::LoopEffectGroup()
{
	ImGui::SameLine();
	if (ImGui::Button("Loop", ImVec2(100.f, 40.f)))
	{
		if (!m_spCurEffectGroup) { return; }

		m_spCurEffectGroup->SetLoop();
	}
}

void ToolEffect::CEffectGroupWindow::PrintTrackPosition()
{
	ImGui::Dummy(ImVec2(0.f, 5.f));

	if (!m_spCurEffectGroup) { return; }

	float fDuration = m_spCurEffectGroup->GetDuration();
	float fTrackPosition = m_spCurEffectGroup->GetTrackPosition();

	string strGroupName = "[ Group - " + CStringUtility::ToString(m_wstrCurEffectGroupName) + " ]";
	string strDuration = "Duration = " + to_string(fDuration);

	ImGui::Text(strGroupName.c_str());
	ImGui::Text(strDuration.c_str());
	ImGui::SliderFloat("Group TrackPosition", &fTrackPosition, 0.f, fDuration);

	for (auto& pair : m_spCurEffectGroup->GetEffects())
	{
		float fLifeTimeX = 0.f;
		float fLifeTimeY = 0.f;
		float fTrackPosition = 0.f;

		string strEffectName = "";
		string strLifeTime = "";
		switch (pair.second->GetEffectType())
		{
		case EEffectType::PARTICLE:
		{
			strEffectName = "[Particle - " + CStringUtility::ToString(pair.first) + "]";

			fLifeTimeX		= static_pointer_cast<CParticleObject>(pair.second)->GetParticleCom()->GetParticleInfo().m_vLifeTime.x;
			fLifeTimeY		= static_pointer_cast<CParticleObject>(pair.second)->GetParticleCom()->GetParticleInfo().m_vLifeTime.y;
			fTrackPosition	= static_pointer_cast<CParticleObject>(pair.second)->GetParticleCom()->GetTrackPosition();
			break;
		}
		case EEffectType::EFFECTMESH:
		{
			strEffectName = "[Effect Mesh - " + CStringUtility::ToString(pair.first) + "]";

			fLifeTimeX		= static_pointer_cast<CEffectMeshObject>(pair.second)->GetEffectMeshCom()->GetEffectMeshInfo().m_vLifeTime.x;
			fLifeTimeY		= static_pointer_cast<CEffectMeshObject>(pair.second)->GetEffectMeshCom()->GetEffectMeshInfo().m_vLifeTime.y;
			fTrackPosition	= static_pointer_cast<CEffectMeshObject>(pair.second)->GetEffectMeshCom()->GetTrackPosition();
			break;
		}
		case EEffectType::InstanceEffectMesh:
		{
			strEffectName = "[Instance Mesh - " + CStringUtility::ToString(pair.first) + "]";

			fLifeTimeX = static_pointer_cast<CInstanceEffectMeshObject>(pair.second)->GetInstanceEffectMeshCom()->GetEffectMeshInfo().m_vLifeTime.x;
			fLifeTimeY = static_pointer_cast<CInstanceEffectMeshObject>(pair.second)->GetInstanceEffectMeshCom()->GetEffectMeshInfo().m_vLifeTime.y;
			fTrackPosition = static_pointer_cast<CInstanceEffectMeshObject>(pair.second)->GetInstanceEffectMeshCom()->GetTrackPosition();
			break;
		}
		}

		strLifeTime = "Life Time = " + to_string(fLifeTimeX) + " " + to_string(fLifeTimeY);

		ImGui::SeparatorText(strEffectName.c_str());
		ImGui::Text(strLifeTime.c_str());
		ImGui::SliderFloat("Effect TrackPosition", &fTrackPosition, fLifeTimeX, fLifeTimeY);
	}
}

void ToolEffect::CEffectGroupWindow::UpdateAttachingColliderTab()
{
	UpdateColliderInfo();
	UpdateAddButton();
}

void ToolEffect::CEffectGroupWindow::UpdateColliderInfo()
{
	// 선택된 이펙트 그룹 이름
	ImGui::Text(CStringUtility::ToString(m_wstrCurEffectGroupName).c_str());

	// 콜라이더 여부
	ImGui::Checkbox("Is Collider", &m_bCollider);

	// 콜라이더 타입
	array<const char*, 2> arrColliderType =
	{
		"SPHERE",
		"BOX"
	};
	ImGui::Combo("Collider Type", &m_iColliderTypeSelector, arrColliderType.data(), (int32)arrColliderType.size());
	ImGui::Dummy(ImVec2(0.f, 5.f));

	switch (m_iColliderTypeSelector)
	{
	case 0: m_eColliderType = EColliderType::SPHERE; break;
	case 1: m_eColliderType = EColliderType::BOX; break;
	default: break;
	}

	// 콜리전 레이어
	array<const char*, 18> arrCollisionLayer =
	{
		"PLAYER",
		"BOSS",
		"ALLY",
		"MONSTER",
		"PLAYER_WEAPON",
		"ALLY_WEAPON",
		"MONSTER_WEAPON",
		"PLAYER_PROJECTILE",
		"ALLY_PROJECTILE",
		"MONSTER_PROJECTILE",
		"ITEM",
		"ENVIRONMENT",
		"BUILDING",
		"NATURE",
		"INTERACTION",
		"BASE_CAMP",
		"NPC",
		"ENUM_END"
	};

	ImGui::Combo("Collision Layer", &m_iCollisionLayerSelector, arrCollisionLayer.data(), (int32)arrCollisionLayer.size());
	ImGui::Dummy(ImVec2(0.f, 5.f));

	switch (m_iCollisionLayerSelector)
	{
	case 0: m_eCollisionLayer = ECollisionLayer::PLAYER; break;
	case 1: m_eCollisionLayer = ECollisionLayer::BOSS; break;
	case 2: m_eCollisionLayer = ECollisionLayer::ALLY; break;
	case 3: m_eCollisionLayer = ECollisionLayer::MONSTER; break;
	case 4: m_eCollisionLayer = ECollisionLayer::PLAYER_WEAPON; break;
	case 5: m_eCollisionLayer = ECollisionLayer::ALLY_WEAPON; break;
	case 6: m_eCollisionLayer = ECollisionLayer::MONSTER_WEAPON; break;
	case 7: m_eCollisionLayer = ECollisionLayer::PLAYER_PROJECTILE; break;
	case 8: m_eCollisionLayer = ECollisionLayer::ALLY_PROJECTILE; break;
	case 9: m_eCollisionLayer = ECollisionLayer::MONSTER_PROJECTILE; break;
	case 10: m_eCollisionLayer = ECollisionLayer::ITEM; break;
	case 11: m_eCollisionLayer = ECollisionLayer::ENVIRONMENT; break;
	case 12: m_eCollisionLayer = ECollisionLayer::BUILDING; break;
	case 13: m_eCollisionLayer = ECollisionLayer::NATURE; break;
	case 14: m_eCollisionLayer = ECollisionLayer::INTERACTION; break;
	case 15: m_eCollisionLayer = ECollisionLayer::BASE_CAMP; break;
	case 16: m_eCollisionLayer = ECollisionLayer::NPC; break;
	case 17: m_eCollisionLayer = ECollisionLayer::ENUM_END; break;
	default: break;
	}

	// 콜리전 플래그
	array<const char*, 18> arrCollisionFlag =
	{
		"PLAYER",
		"BOSS",
		"ALLY",
		"MONSTER",
		"PLAYER_WEAPON",
		"ALLY_WEAPON",
		"MONSTER_WEAPON",
		"PLAYER_PROJECTILE",
		"ALLY_PROJECTILE",
		"MONSTER_PROJECTILE",
		"ITEM",
		"ENVIRONMENT",
		"BUILDING",
		"NATURE",
		"INTERACTION",
		"BASE_CAMP",
		"NPC",
		"ENUM_END"
	};

	ImGui::Combo("Collision Flag", &m_iCollisionFlagSelector, arrCollisionFlag.data(), (int32)arrCollisionFlag.size());
	ImGui::Dummy(ImVec2(0.f, 5.f));

	switch (m_iCollisionFlagSelector)
	{
	case 0: m_eCollisionFlag = ECollisionFlag::PLAYER; break;
	case 1: m_eCollisionFlag = ECollisionFlag::BOSS; break;
	case 2: m_eCollisionFlag = ECollisionFlag::ALLY; break;
	case 3: m_eCollisionFlag = ECollisionFlag::MONSTER; break;
	case 4: m_eCollisionFlag = ECollisionFlag::PLAYER_WEAPON; break;
	case 5: m_eCollisionFlag = ECollisionFlag::ALLY_WEAPON; break;
	case 6: m_eCollisionFlag = ECollisionFlag::MONSTER_WEAPON; break;
	case 7: m_eCollisionFlag = ECollisionFlag::PLAYER_PROJECTILE; break;
	case 8: m_eCollisionFlag = ECollisionFlag::ALLY_PROJECTILE; break;
	case 9: m_eCollisionFlag = ECollisionFlag::MONSTER_PROJECTILE; break;
	case 10: m_eCollisionFlag = ECollisionFlag::ITEM; break;
	case 11: m_eCollisionFlag = ECollisionFlag::ENVIRONMENT; break;
	case 12: m_eCollisionFlag = ECollisionFlag::BUILDING; break;
	case 13: m_eCollisionFlag = ECollisionFlag::NATURE; break;
	case 14: m_eCollisionFlag = ECollisionFlag::INTERACTION; break;
	case 15: m_eCollisionFlag = ECollisionFlag::BASE_CAMP; break;
	case 16: m_eCollisionFlag = ECollisionFlag::NPC; break;
	case 17: m_eCollisionFlag = ECollisionFlag::ENUM_END; break;
	default: break;
	}

	// Sphere
	ImGui::SeparatorText("Sphere");
	ImGui::SliderFloat("Radius", &m_fRadius, 0.f, 100.f);

	// Box
	ImGui::SeparatorText("Box");
	ImGui::SliderFloat3("Extents", &m_vExtents.x, 0.f, 100.f);
	ImGui::SliderFloat3("LocalPosition", &m_vLocalPosition.x, 0.f, 100.f);
	// ImGui::SliderFloat3("LocalRotate", &m_vLocalRotate.x, 0.f, 360.f);

	FColliderInfo tColliderInfo;
	tColliderInfo.m_eColliderType	= m_eColliderType;
	tColliderInfo.m_eCollisionFlag	= m_eCollisionFlag;
	tColliderInfo.m_eCollisionLayer = m_eCollisionLayer;
	tColliderInfo.m_fRadius			= m_fRadius;
	tColliderInfo.m_vExtents		= m_vExtents;
	tColliderInfo.m_vLocalPosition	= m_vLocalPosition;
	tColliderInfo.m_vLocalRotate	= m_vLocalRotate;

	if (m_spCurEffectGroup)
	{
		m_spCurEffectGroup->UpdateColliderInfo(tColliderInfo);
	}
}

void ToolEffect::CEffectGroupWindow::UpdateAddButton()
{
	if (ImGui::Button("Add Collider", ImVec2(150.f, 50.f)))
	{
		if (!m_spCurEffectGroup) { return; }

		FColliderInfo tColliderInfo;
		tColliderInfo.m_bCollider		= m_bCollider;
		tColliderInfo.m_eColliderType	= m_eColliderType;
		tColliderInfo.m_eCollisionFlag	= m_eCollisionFlag;
		tColliderInfo.m_eCollisionLayer = m_eCollisionLayer;
		tColliderInfo.m_fRadius			= m_fRadius;
		tColliderInfo.m_vExtents		= m_vExtents;
		tColliderInfo.m_vLocalPosition	= m_vLocalPosition;
		tColliderInfo.m_vLocalRotate	= m_vLocalRotate;

		m_spCurEffectGroup->InitializeCollider(tColliderInfo);
	}
}

HRESULT ToolEffect::CEffectGroupWindow::AddEffectGroup(const std::wstring& _wstrEffectGroupName, std::shared_ptr<CEffectGroup> _spEffectGroup)
{
	if (m_mapEffectGroups.contains(_wstrEffectGroupName)) { return E_FAIL; }

	m_mapEffectGroups.emplace(_wstrEffectGroupName, _spEffectGroup);

	return S_OK;
}

std::shared_ptr<CEffectGroup> ToolEffect::CEffectGroupWindow::FindEffectGroup(const std::wstring& _wstrEffectGroupName)
{
	if (m_mapEffectGroups.contains(_wstrEffectGroupName)) { return m_mapEffectGroups[_wstrEffectGroupName]; }

	return nullptr;
}

HRESULT ToolEffect::CEffectGroupWindow::RemoveEffectGroup(const std::wstring& _wstrEffectGroupName)
{
	if (!m_mapEffectGroups.contains(_wstrEffectGroupName)) { return E_FAIL; }

	m_mapEffectGroups[_wstrEffectGroupName]->Release();
	m_mapEffectGroups[_wstrEffectGroupName].reset();

	m_mapEffectGroups.erase(_wstrEffectGroupName);

	return S_OK;
}

std::shared_ptr<CEffectGroup> ToolEffect::CEffectGroupWindow::GetCurEffectGroup()
{
	if (m_spCurEffectGroup) { return m_spCurEffectGroup; }

	return nullptr;
}
