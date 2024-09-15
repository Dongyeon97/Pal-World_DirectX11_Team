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
		std::vector<Vector3>		m_vecVertexPositions;			// ���� �����ǵ�
		std::vector<UINT32>			m_vecIndices;					// ���� �ε�����

	protected:
		D3D11_BUFFER_DESC			m_tBufferDesc;
		D3D11_SUBRESOURCE_DATA		m_tInitialData;
		uint32						m_iVertexStride = 0;			// ���� ����ü�� ����Ʈũ��
		uint32						m_iNumVertices = 0;				// ������ ����
		uint32						m_iIndexStride = 0;				// �ε��� �ڷ��� ����Ʈũ�� 2 or 4
		uint32						m_iNumIndices = 0;				// �ε��� ����
		uint32						m_iNumVertexBuffers = 0;		// ���������� ����(�ν��Ͻ�)
		DXGI_FORMAT					m_eIndexFormat;					// �ε��� �ڷ���
		D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitiveTopology;			// �׸��� ����

	};
}


