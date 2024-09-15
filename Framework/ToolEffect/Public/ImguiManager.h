#pragma once
#include "Manager.h"

namespace Engine
{
	class CLevel;
}

namespace ToolEffect
{
	class CInspector;
	class CObjectList;
	class CEffectGroupWindow;

	class CImguiManager final : public CManager
	{
		DECLARE_SINGLETON(CImguiManager)

	public:
		HRESULT Initialize();
		void UpdateBegin(float _fDeltaSeconds);
		void Update(float _fDeltaSeconds);
		void UpdateEnd(float _fDeltaSeconds);
		void Render();
		void Release();

	public:
		void SetCurLevelToInspector(shared_ptr<CLevel> _spLevel);
		void SetCurLevelToObjectList(shared_ptr<CLevel> _spLevel);
		void SetCurLevelToEffectGroupWindow(shared_ptr<CLevel> _spLevel);
		void SetMouseHold();

	private:
	#pragma region ÄÄ °´Ã¼
		ComPtr<ID3D11Device> m_pDevice = nullptr;

		ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
	#pragma endregion

	private:
		float* m_vBackGroundColor;

		bool m_bMouseHold = false;

		std::shared_ptr<CInspector> m_spInspector = nullptr;

		std::shared_ptr<CObjectList> m_spObjectList = nullptr;

		std::shared_ptr<CEffectGroupWindow> m_spEffectGroupWindow = nullptr;
	};

}
