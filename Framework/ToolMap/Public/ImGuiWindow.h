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

		// 객체의 고유 ID
		uint32 m_iGameObjectID = 0;

		// 카메라와 피킹모드를 제어하기 위한 멤버변수 입니다.
		bool m_bPickMode = false;
		bool m_bLockCameraViewDir = false;
		bool m_bFastPickingMode = false;

		// Left Panel 좌 상단 좌표 멤버변수들
		std::array<float, 3> m_arrCurCameraPos = { 0.0f, 0.0f, 0.0f };
		std::array<float, 3> m_arrPickedPos = { 0.0f, 0.0f, 0.0f };

		// 리소스 파일경로 및 파일명을 저장하는 컨테이너 입니다.
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
		


		// 카메라의 이름과 위치를 저장하기 위한 멤버변수 입니다.
		char m_chCameraName[MAX_PATH] = "";
		std::string m_strCameraName;
		std::wstring m_wstrCameraName;
		std::array<float, 3> m_arrPreCameraPos = { 0.0f, 0.0f, 0.0f };
		std::vector<std::array<float, 3>> m_vecCamerasPos; // 추가한 카메라들의 위치를 가지고 있는 벡터 컨테이너 입니다.

		// 파씽용 텍스트를 저장할 멤버변수입니다.
		char m_chSaveLoadFileName[MAX_PATH] = "";
		std::string m_strSaveLoadFileName;
		std::wstring m_wstrSaveLoadFileName;

		// 객체 생성
		uint32 m_iSelectedFileIndex = 0; // 객체 생성 할때 선택한 파일 인덱스를 통해 파일 경로 및 파일명을 찾기위함 입니다.
		std::wstring m_wstrObjectName; // "Wood", "Stone", "Tree"...
		std::wstring m_wstrModelFileName; // 객체를 생성하기 위한 파일명 입니다.
		std::wstring m_wstrModelFilePath;

		/////////////우측//////////////////

		//// 우측 창 객체 리스트란에 추가 될 객체들을 가지고 있는 컨테이너 입니다.
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

		// 우측 창 객체들의 이름이 겹치지 않게 하기 위해서 입니다.
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

		// MapObject의 속성값과 관련된 멤버변수 입니다.
		int32 m_iModelType = 0; // Static, Dynamic
		int32 m_iObjectType = 0; // WOOD, STONE, TREE...
		int32 m_iColliderType = 0; // Collider, Trigger
		int32 m_iStaticObjectIndex = 0; // Dynamic객체들 중에서 어떤 객체인지(switch문을 사용하기 위한 변수)
		int32 m_iDynamicObjectIndex = 0; // Dynamic객체들 중에서 어떤 객체인지(switch문을 사용하기 위한 변수)

		std::array<float, 3> m_arrObjectScale = { 0.0f, 0.0f, 0.0f };
		std::array<float, 3> m_arrObjectRotation = { 0.0f, 0.0f, 0.0f };
		std::array<float, 3> m_arrObjectTranslation = { 0.0f, 0.0f, 0.0f };

		// 우측 창 객체 리스트 선택과 관련된 부분
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

		/////////////////////////////아래는 필요한 애들만 뽑아서 사용할 예정인 멤버 변수들입니다////////////////////////////


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

		


		//Camera 위치
		Vector3 m_vSelectedCameraPos;

		EToolMapObjectType m_eToolMapObject = {};


	private:
		std::unordered_map<std::wstring, std::weak_ptr<CToolMapObject>> m_umapChests;
		std::unordered_map<std::wstring, std::weak_ptr<CToolMapObject>> m_umapWoods;

		char m_chMapObjectFileName[MAX_PATH]{ NULL };
	};
}

