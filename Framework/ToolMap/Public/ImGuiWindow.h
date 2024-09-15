#pragma once

namespace Engine
{
	class CCameraManager;
	class CPickManager;
	class CInputManager;
}

namespace ToolMap
{
	class CImGuiManager;
	class CToolMapLevel;
	class CToolMapObject;

	class CImGuiWindow final
	{
	public:
		explicit CImGuiWindow() = default;

		virtual ~CImGuiWindow() = default;

	/********************
		Events
	********************/
	public:
		static std::shared_ptr<CImGuiWindow> Create();

		HRESULT Initialize();

		HRESULT BeginPlay();

		int32 Tick();

		void Release();


	/********************
		Methods
	********************/
	public:
		void SetFastPickingMode(bool _bFastPickingMode) { m_bFastPickingMode = _bFastPickingMode; }

	/********************
		Methods
	********************/
	public:
		std::wstring ExtractFileNameFromFilePath(uint32 _iIndex, std::vector <std::wstring> _vecModelFilePaths);

		std::wstring ExtractWstringWithoutNumber(std::wstring _wstr);
		
		void SetGameObjectID(uint32& _iGameObjectID) { m_iGameObjectID = _iGameObjectID; }

		float GetRandomRotationAngle();

		float GetRandomScale();

	/********************
		Methods
	********************/
	private: 
		/* Left Window */
		/* Install */
		void TickInstall();
			void InstallGridMapBar();
			void InstallObjectBar();
			void InstallPalBar();
			void InstallLightBar();
			void InstallMapCameraBar();

	private:
		/* Right Window */
		/* Function */
		void TickFunction();
			void FunctionGridMapBar();
			void FunctionObjectBar();
			void FunctionPalBar();
			void FunctionLightBar();
			void FunctionMapCameraBar();

	private:
		void AddObjectToFunctionTab();

		// Save & Load
		void SaveDataInto64Json();
		void LoadDataFrom64Json();

		void SaveDataInto512Json();
		void LoadDataFrom512Json();
		
		// Create & Delete
		void CreateObject(const wstring& _wstrModelFilePath);
		void DeleteLatestObject();
		void DeleteAllObject();

		// Gizmo
		void EditTransform(const uint32& _iGameObjectID);

	private:
		void LoadMapMeshResources();
		
	/********************
		Data Members
	********************/
	private: 
		std::weak_ptr<CToolMapLevel> m_wpToolMapLevel;
		std::weak_ptr<CCameraManager> m_wpCameraManager;
		std::weak_ptr<CPickManager> m_wpPickManager;
		std::weak_ptr<CInputManager> m_wpInputManager;

		// ��ü�� ���� ID
		uint32 m_iGameObjectID = 0;

		// ī�޶�� ��ŷ��带 �����ϱ� ���� ������� �Դϴ�.
		bool m_bPickMode = false;
		bool m_bLockCameraViewDir = false;
		bool m_bFastPickingMode = false;

		// Left Panel �� ��� ��ǥ ���������
		std::array<float, 3> m_arrCurCameraPos = { 0.0f, 0.0f, 0.0f };
		std::array<float, 3> m_arrPickedPos = { 0.0f, 0.0f, 0.0f };

		// ���ҽ� ���ϰ�� �� ���ϸ��� �����ϴ� �����̳� �Դϴ�.
		std::vector <std::wstring> m_vecGridLandscapeModelFilePaths;
		std::vector <std::wstring> m_vecGridEnvModelFilePaths;
		std::vector <std::wstring> m_vecPlaceableTreeModelFilePaths;
		std::vector <std::wstring> m_vecPlaceableRockModelFilePaths;
		std::vector <std::wstring> m_vecPlaceableWoodModelFilePaths;
		std::vector <std::wstring> m_vecPlaceableStoneModelFilePaths;
		std::vector <std::wstring> m_vecPlaceablePalModelFilePaths;
		std::vector <std::wstring> m_vecPlaceableArchitectureModelFilePaths;
		std::vector <std::wstring> m_vecPlaceableSpotLightModelFilePaths;
		std::vector <std::wstring> m_vecPlaceablePointLightModelFilePaths;
		std::vector <std::wstring> m_vecPlaceableDirectionalLightModelFilePaths;
		std::vector <std::wstring> m_vecPlaceableCameraModelFilePaths;
		


		// ī�޶��� �̸��� ��ġ�� �����ϱ� ���� ������� �Դϴ�.
		char m_chCameraName[MAX_PATH] = "";
		std::string m_strCameraName;
		std::wstring m_wstrCameraName;
		std::array<float, 3> m_arrPreCameraPos = { 0.0f, 0.0f, 0.0f };
		std::vector<std::array<float, 3>> m_vecCamerasPos; // �߰��� ī�޶���� ��ġ�� ������ �ִ� ���� �����̳� �Դϴ�.

		// �ľſ� �ؽ�Ʈ�� ������ ��������Դϴ�.
		char m_chSaveLoadFileName[MAX_PATH] = "";
		std::string m_strSaveLoadFileName;
		std::wstring m_wstrSaveLoadFileName;

		// ��ü ����
		uint32 m_iSelectedFileIndex = 0; // ��ü ���� �Ҷ� ������ ���� �ε����� ���� ���� ��� �� ���ϸ��� ã������ �Դϴ�.
		std::wstring m_wstrObjectName; // "Wood", "Stone", "Tree"...
		std::wstring m_wstrModelFileName; // ��ü�� �����ϱ� ���� ���ϸ� �Դϴ�.
		std::wstring m_wstrModelFilePath;

		/////////////����//////////////////

		//// ���� â ��ü ����Ʈ���� �߰� �� ��ü���� ������ �ִ� �����̳� �Դϴ�.
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapGridMapListBox;
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapTreeListBox;
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapRockListBox;
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapWoodListBox;
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapStoneListBox;
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapPalListBox;
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapArchitectureListBox;
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapSpotLightListBox;
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapPointLightListBox;
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapDirectionalLightListBox;
		std::unordered_map<std::wstring, std::pair<std::wstring, std::shared_ptr<CToolMapObject>>> m_umapCameraListBox;

		// ���� â ��ü���� �̸��� ��ġ�� �ʰ� �ϱ� ���ؼ� �Դϴ�.
		uint32 m_iGridMapCount = 0;
		uint32 m_iTreeCount = 0;
		uint32 m_iRockCount = 0;
		uint32 m_iWoodCount = 0;
		uint32 m_iStoneCount = 0;
		uint32 m_iPalCount = 0;
		uint32 m_iArchitectureCount = 0;
		uint32 m_iSpotLightCount = 0;
		uint32 m_iPointLightCount = 0;
		uint32 m_iDirectionalLightCount = 0;
		uint32 m_iCameraCount = 0;

		// MapObject�� �Ӽ����� ���õ� ������� �Դϴ�.
		int32 m_iModelType = 0; // Static, Dynamic
		int32 m_iObjectType = 0; // WOOD, STONE, TREE...
		int32 m_iColliderType = 0; // Collider, Trigger
		int32 m_iStaticObjectIndex = 0; // Dynamic��ü�� �߿��� � ��ü����(switch���� ����ϱ� ���� ����)
		int32 m_iDynamicObjectIndex = 0; // Dynamic��ü�� �߿��� � ��ü����(switch���� ����ϱ� ���� ����)

		std::array<float, 3> m_arrObjectScale = { 0.0f, 0.0f, 0.0f };
		std::array<float, 3> m_arrObjectRotation = { 0.0f, 0.0f, 0.0f };
		std::array<float, 3> m_arrObjectTranslation = { 0.0f, 0.0f, 0.0f };

		// ���� â ��ü ����Ʈ ���ð� ���õ� �κ�
		std::wstring m_wstrSelectedListBoxObjectName;

		std::array<int32, 3> m_arrOctreeIndex = { 0, 0, 0 };


		// Test 
		std::vector<FHabitatTreeOrRockCache>		m_vecHabitatTreeOrRockCaches;
		std::vector<FHabitatBranchOrStoneCache>		m_vecHabitatBranchOrStoneCaches;
		std::vector<FHabitatPalCache>				m_vecHabitatPalCaches;
		std::vector<FHabitatArchitectureCache>		m_vecHabitatArchitectureCaches;
		std::vector<FHabitatSpotLightCache>			m_vecHabitatSpotLightCaches;
		std::vector<FHabitatPointLightCache>		m_vecHabitatPointLightCaches;
		std::vector<FHabitatDirectionalLightCache>  m_vecHabitatDirectionalLightCaches;
		std::vector<FHabitatCameraFrustumCache>		m_vecHabitatCameraFrustumCaches;

		/////////////////////////////�Ʒ��� �ʿ��� �ֵ鸸 �̾Ƽ� ����� ������ ��� �������Դϴ�////////////////////////////


		float m_fDegree = 0.0f;

		/* Function */
		int32 m_iInstalledObjectType = 0;
		int32 m_iInstalledNonAnimIndex = 0;
		int32 m_iInstalledWoodIndex = 0;
		int32 m_iInstalledChestIndex = 0;


		std::list <wstring> m_NonAnimObjectList;

		std::weak_ptr<CToolMapObject> m_wpToolMapObject;

		

		//Camera 
		int32 m_iCameraIndex = 0;
		char m_chNextCameraName[MAX_PATH] = "";

		
		std::wstring m_strNextCameraName;
		std::vector<std::shared_ptr<wstring>> m_vecCameraName;
		std::unordered_map<int32, std::wstring> m_umapCameras;

		


		//Camera ��ġ
		Vector3 m_vSelectedCameraPos;

		EToolMapObjectType m_eToolMapObject = {};


	private:
		std::unordered_map<std::wstring, std::weak_ptr<CToolMapObject>> m_umapChests;
		std::unordered_map<std::wstring, std::weak_ptr<CToolMapObject>> m_umapWoods;

		char m_chMapObjectFileName[MAX_PATH]{ NULL };
	};
}

