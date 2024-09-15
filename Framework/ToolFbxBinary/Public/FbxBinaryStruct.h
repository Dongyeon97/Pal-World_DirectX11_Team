#pragma once

namespace FbxBinary
{
	/********************
		Input Layout
	********************/
	struct FPosition final
	{
		XMFLOAT3 m_vPosition;

		const static UINT iNumInputElementDescs = 1;
		const static D3D11_INPUT_ELEMENT_DESC m_tInputElementDescs[1];
	};

	struct FPositionSize final
	{
		XMFLOAT3 m_vPosition;
		XMFLOAT2 m_vPSize;

		const static UINT iNumInputElementDescs = 2;
		const static D3D11_INPUT_ELEMENT_DESC m_tInputElementDescs[2];
	};

	struct FPositionColor final
	{
		XMFLOAT3 m_vPosition;
		XMFLOAT3 m_vColor;

		const static UINT iNumInputElementDescs = 2;
		const static D3D11_INPUT_ELEMENT_DESC m_tInputElementDescs[2];
	};

	struct FPositionTexCoord2 final
	{
		XMFLOAT3 m_vPosition;
		XMFLOAT2 m_vTexCoord2;

		const static UINT iNumInputElementDescs = 2;
		const static D3D11_INPUT_ELEMENT_DESC m_tInputElementDescs[2];
	};

	struct FPositionTexCoord3 final
	{
		XMFLOAT3 m_vPosition;
		XMFLOAT3 m_vTexCoord3;

		const static UINT iNumInputElementDescs = 2;
		const static D3D11_INPUT_ELEMENT_DESC m_tInputElementDescs[2];
	};

	struct FPositionColorTexCoord2 final
	{
		XMFLOAT3 m_vPosition;
		XMFLOAT3 m_vColor;
		XMFLOAT2 m_vTexCoord2;

		const static UINT iNumInputElementDescs = 3;
		const static D3D11_INPUT_ELEMENT_DESC m_tInputElementDescs[3];
	};

	struct FPositionNormalTexCoord2 final
	{
		XMFLOAT3 m_vPosition;
		XMFLOAT3 m_vNormal;
		XMFLOAT2 m_vTexCoord2;

		const static UINT iNumInputElementDescs = 3;
		const static D3D11_INPUT_ELEMENT_DESC m_tInputElementDescs[3];
	};

	struct FPositionNormalTexCoord2Tangent final
	{
		XMFLOAT3 m_vPosition;
		XMFLOAT3 m_vNormal;
		XMFLOAT2 m_vTexCoord2;
		XMFLOAT3 m_vTangent;

		const static UINT iNumInputElementDescs = 4;
		const static D3D11_INPUT_ELEMENT_DESC m_tInputElementDescs[4];
	};

	struct FPositionNormalTexCoord2TexCoord2Tangent final
	{
		XMFLOAT3 m_vPosition;
		XMFLOAT3 m_vNormal;
		XMFLOAT2 m_vTexCoord21;		// 일반 텍스처 맵핑용 UV
		XMFLOAT2 m_vTexCoord22;		// 어떤 텍스처를 혼합할 건지 ( Landscape Layer ) UV
		XMFLOAT3 m_vTangent;

		const static UINT iNumInputElementDescs = 5;
		const static D3D11_INPUT_ELEMENT_DESC m_tInputElementDescs[5];
	};

	struct FPositionNormalTexCoord2TangentBlendIndexBlendWeight final
	{
		XMFLOAT3 m_vPosition;
		XMFLOAT3 m_vNormal;
		XMFLOAT2 m_vTexCoord2;
		XMFLOAT3 m_vTangent;
		XMUINT4 m_vBlendIndices;
		XMFLOAT4 m_vBlendWeights;

		const static UINT iNumInputElementDescs = 6;
		const static D3D11_INPUT_ELEMENT_DESC m_tInputElementDescs[6];
	};

	/********************
		Cbuffer
	********************/
	// Matrix
	struct FConstantBuffer { };

	struct FOneMatrixConstantBuffer final : public FConstantBuffer
	{
		Matrix m_matMatrix_0 = Matrix::Identity;
	};

	struct FTwoMatrixConstantBuffer final : public FConstantBuffer
	{
		Matrix m_matMatrix_0 = Matrix::Identity;
		Matrix m_matMatrix_1 = Matrix::Identity;
	};

	struct FThreeMatrixConstantBuffer final : public FConstantBuffer
	{
		Matrix m_matMatrix_0 = Matrix::Identity;
		Matrix m_matMatrix_1 = Matrix::Identity;
		Matrix m_matMatrix_2 = Matrix::Identity;
	};

	struct FFourMatrixConstantBuffer final : public FConstantBuffer
	{
		Matrix m_matMatrix_0 = Matrix::Identity;
		Matrix m_matMatrix_1 = Matrix::Identity;
		Matrix m_matMatrix_2 = Matrix::Identity;
		Matrix m_matMatrix_3 = Matrix::Identity;
	};

	struct FMatrix256ConstantBuffer final : public FConstantBuffer
	{
		Matrix m_matMatrix[256]{ Matrix::Identity };
	};


	// Int
	struct FOneIntConstantBuffer final : public FConstantBuffer
	{
		int32 m_iInt_0 = 0;

	private:
		int32 m_iDummies[3]{ 0 };
	};

	struct FTwoIntConstantBuffer final : public FConstantBuffer
	{
		int32 m_iInt_0 = 0;
		int32 m_iInt_1 = 0;

	private:
		int32 m_iDummies[2]{ 0 };
	};

	struct FThreeIntConstantBuffer final : public FConstantBuffer
	{
		int32 m_iInt_0 = 0;
		int32 m_iInt_1 = 0;
		int32 m_iInt_2 = 0;

	private:
		int32 m_iDummies[1]{ 0 };
	};

	struct FFourIntConstantBuffer final : public FConstantBuffer
	{
		int32 m_iInt_0 = 0;
		int32 m_iInt_1 = 0;
		int32 m_iInt_2 = 0;
		int32 m_iInt_3 = 0;
	};


	// Float
	struct FOneFloatConstantBuffer final : public FConstantBuffer
	{
		float m_fFloat_0 = 0.0f;

	private:
		float m_fDummies[3]{ 0.0f };
	};

	struct FTwoFloatConstantBuffer final : public FConstantBuffer
	{
		float m_fFloat_0 = 0.0f;
		float m_fFloat_1 = 0.0f;

	private:
		float m_fDummies[2]{ 0.0f };
	};

	struct FThreeFloatConstantBuffer final : public FConstantBuffer
	{
		float m_fFloat_0 = 0.0f;
		float m_fFloat_1 = 0.0f;
		float m_fFloat_2 = 0.0f;

	private:
		float m_fDummies[1]{ 0.0f };
	};

	struct FFourFloatConstantBuffer final : public FConstantBuffer
	{
		float m_fFloat_0 = 0.0f;
		float m_fFloat_1 = 0.0f;
		float m_fFloat_2 = 0.0f;
		float m_fFloat_3 = 0.0f;
	};


	// Vector3
	struct FOneVector3ConstantBuffer final : public FConstantBuffer
	{
		Vector3 m_vVector3_0 = Vector3::Zero;

	private:
		float m_fDummies[1]{ 0.0f };
	};

	struct FTwoVector3ConstantBuffer final : public FConstantBuffer
	{
		Vector3 m_vVector3_0 = Vector3::Zero;
		Vector3 m_vVector3_1 = Vector3::Zero;

	private:
		float m_fDummies[2]{ 0.0f };
	};

	struct FThreeVector3ConstantBuffer final : public FConstantBuffer
	{
		Vector3 m_vVector3_0 = Vector3::Zero;
		Vector3 m_vVector3_1 = Vector3::Zero;
		Vector3 m_vVector3_2 = Vector3::Zero;

	private:
		float m_fDummies[3]{ 0.0f };
	};

	struct FFourVector3ConstantBuffer final : public FConstantBuffer
	{
		Vector3 m_vVector3_0 = Vector3::Zero;
		Vector3 m_vVector3_1 = Vector3::Zero;
		Vector3 m_vVector3_2 = Vector3::Zero;
		Vector3 m_vVector3_3 = Vector3::Zero;
	};


	// Vector4
	struct FOneVector4ConstantBuffer final : public FConstantBuffer
	{
		Vector4 m_vVector4_0 = Vector4::Zero;
	};

	struct FTwoVector4ConstantBuffer final : public FConstantBuffer
	{
		Vector4 m_vVector4_0 = Vector4::Zero;
		Vector4 m_vVector4_1 = Vector4::Zero;
	};

	struct FThreeVector4ConstantBuffer final : public FConstantBuffer
	{
		Vector4 m_vVector4_0 = Vector4::Zero;
		Vector4 m_vVector4_1 = Vector4::Zero;
		Vector4 m_vVector4_2 = Vector4::Zero;
	};

	struct FFourVector4ConstantBuffer final : public FConstantBuffer
	{
		Vector4 m_vVector4_0 = Vector4::Zero;
		Vector4 m_vVector4_1 = Vector4::Zero;
		Vector4 m_vVector4_2 = Vector4::Zero;
		Vector4 m_vVector4_3 = Vector4::Zero;
	};
}