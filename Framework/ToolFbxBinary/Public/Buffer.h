#pragma once

namespace FbxBinary
{
	class CBuffer abstract
	{
		/********************
			Ctor/Dtor
		********************/
	public:
		explicit CBuffer() = default;
		virtual ~CBuffer() = default;

		/********************
			Events
		********************/
	public:
		virtual HRESULT Initialize();
		virtual HRESULT Render();
		virtual void Release();

	protected:
		virtual HRESULT Bind_Buffers();

		/********************
			Methods
		********************/
	protected:
		HRESULT CreateBuffer(ID3D11Buffer** _ppBuffer);

		/********************
			Data Members
		********************/

	protected:
		ComPtr<ID3D11Device>		m_cpDevice = nullptr;
		ComPtr<ID3D11DeviceContext> m_cpDeviceContext = nullptr;

		ComPtr<ID3D11Buffer>		m_cpVertexBuffer = nullptr;
		ComPtr<ID3D11Buffer>		m_cpIndexBuffer = nullptr;

	protected:
		std::vector<Vector3>		m_vecVertexPositions;			// 정점 포지션들
		std::vector<UINT32>			m_vecIndices;					// 정점 인덱스들

	protected:
		D3D11_BUFFER_DESC			m_tBufferDesc;
		D3D11_SUBRESOURCE_DATA		m_tInitialData;
		uint32						m_iVertexStride = 0;			// 정점 구조체의 바이트크기
		uint32						m_iNumVertices = 0;				// 정점의 개수
		uint32						m_iIndexStride = 0;				// 인덱스 자료형 바이트크기 2 or 4
		uint32						m_iNumIndices = 0;				// 인덱스 개수
		uint32						m_iNumVertexBuffers = 0;		// 정점버퍼의 개수(인스턴싱)
		DXGI_FORMAT					m_eIndexFormat;					// 인덱스 자료형
		D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitiveTopology;			// 그리기 형식

	};
}


