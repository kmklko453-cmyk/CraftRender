#include "Renderer.h"
#include "Core/Common.h"
#include "GraphicsContext.h"
#include <d3dcompiler.h>

namespace Craft
{
	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{
		// @Temp: 재사용하는 렌더 커맨드 해제.
		auto& command = renderQueue[0];
		SafeRelease(command.vertexBuffer);
		SafeRelease(command.indexBuffer);
		SafeRelease(command.inputLayout);
		SafeRelease(command.vertexShader);
		SafeRelease(command.pixelShader);
	}

	// 초기화.
	void Renderer::Initialize()
	{
		// @Temp: 프레임워크 구성될 때까지 임시로 재사용할 리소스 새성.

		// 리소스 생성할 장치.
		auto& device = GraphicsContext::Get().GetDevice();

		// 정점 데이터 (3개의 점).
		float vertices[] =
		{
			0.0f, 0.5f, 0.1f,	// 첫 번째 점의 위치(3차원 좌표).
			0.5f, -0.5f, 0.1f,	// 두 번째 점의 위치(3차원 좌표).
			-0.5f, -0.5f, 0.1f,	// 세 번째 점의 위치(3차원 좌표).
		};

		// 버퍼 속성 설정.
		/*
		*   UINT ByteWidth;
			D3D11_USAGE Usage;
			UINT BindFlags;
			UINT CPUAccessFlags;
			UINT MiscFlags;
			UINT StructureByteStride;
		*/
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = sizeof(float) * _countof(vertices);
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		// 서브 리소스 데이터 설정 (실제 데이터 할당).
		/*
		* // pSysMem에 설정된 데이터를 한 번에 전체사용할 때는 아래 두개 0.
		*   const void *pSysMem;
			UINT SysMemPitch;
			UINT SysMemSlicePitch;
		*/
		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = vertices;

		// 정점 버퍼 생성.
		ID3D11Buffer* vertexBuffer = nullptr;
		HRESULT result = device.CreateBuffer(
			&vertexBufferDesc,
			&vertexData,
			&vertexBuffer
		);

		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		// 인덱스 (삼각형 조합할 때 사용할 순서를 지정).
		uint32_t indices[] = { 0, 1, 2 };

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.ByteWidth = sizeof(uint32_t) * _countof(indices);
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// 서브 리소스 데이터 설정 (실제 데이터 할당).
		/*
		* // pSysMem에 설정된 데이터를 한 번에 전체사용할 때는 아래 두개 0.
		*   const void *pSysMem;
			UINT SysMemPitch;
			UINT SysMemSlicePitch;
		*/
		D3D11_SUBRESOURCE_DATA indexData = { };
		indexData.pSysMem = indices;

		// 인덱스 버퍼 생성.
		ID3D11Buffer* indexBuffer = nullptr;
		result = device.CreateBuffer(
			&indexBufferDesc,
			&indexData,
			&indexBuffer
		);

		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		// 셰이더 컴파일.
		// 두 번(VS/PS).
		/*
		*   _In_reads_opt_(_Inexpressible_(pDefines->Name != NULL)) CONST D3D_SHADER_MACRO* pDefines,
		_In_opt_ ID3DInclude* pInclude,
		_In_ LPCSTR pEntrypoint,
		_In_ LPCSTR pTarget,
		_In_ UINT Flags1,
		_In_ UINT Flags2,
		_Out_ ID3DBlob** ppCode,
		_Always_(_Outptr_opt_result_maybenull_) ID3DBlob**
		*/
		// 컴파일된 Object 코드 저장 객체.
		ID3DBlob* vertexShaderObject = nullptr;
		result = D3DCompileFromFile(
			L"HLSLShader/DefaultVS.hlsl",
			nullptr,
			nullptr,
			"main",
			"vs_5_0",
			0,
			0,
			&vertexShaderObject,
			nullptr
		);

		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		// 셰이더 객체 생성.
		ID3D11VertexShader* vertexShader = nullptr;
		result = device.CreateVertexShader(
			vertexShaderObject->GetBufferPointer(),
			vertexShaderObject->GetBufferSize(),
			nullptr,
			&vertexShader
		);

		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		ID3DBlob* pixelShaderObject = nullptr;
		result = D3DCompileFromFile(
			L"HLSLShader/DefaultPS.hlsl",
			nullptr,
			nullptr,
			"main",                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
			"ps_5_0",
			0,
			0,
			&pixelShaderObject,
			nullptr
		);

		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		// 셰이더 객체 생성.
		ID3D11PixelShader* pixelShader = nullptr;
		result = device.CreatePixelShader(
			pixelShaderObject->GetBufferPointer(),
			pixelShaderObject->GetBufferSize(),
			nullptr,
			&pixelShader
		);

		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		// 입력 레이아웃 생성.
		/*
			_In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
			_In_range_(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)  UINT NumElements,
			_In_reads_(BytecodeLength)  const void* pShaderBytecodeWithInputSignature,
			_In_  SIZE_T BytecodeLength,
			_COM_Outptr_opt_  ID3D11InputLayout** ppInputLayout) = 0;
		*/

		/*
		*	LPCSTR SemanticName;
			UINT SemanticIndex;
			DXGI_FORMAT Format;
			UINT InputSlot;
			UINT AlignedByteOffset;
			D3D11_INPUT_CLASSIFICATION InputSlotClass;
			UINT InstanceDataStepRate;
		*/
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// 입력 레이아웃 = 정점 셰이더 입력의 명세서.
		// 따라서 정점 셰이더 정보가 있어야 함.
		ID3D11InputLayout* inputLayout = nullptr;
		result = device.CreateInputLayout(
			inputDesc,
			_countof(inputDesc),
			vertexShaderObject->GetBufferPointer(),
			vertexShaderObject->GetBufferSize(),
			&inputLayout
		);

		// 렌더 큐 추가.
		RenderCommand command;
		command.vertexBuffer = vertexBuffer;
		command.indexBuffer = indexBuffer;
		command.indexCount = _countof(indices);
		command.vertexShader = vertexShader;
		command.pixelShader = pixelShader;
		command.inputLayout = inputLayout;

		renderQueue.emplace_back(command);

		// 사용한 리소스 해제.
		SafeRelease(vertexShaderObject);
		SafeRelease(pixelShaderObject);
	}

	// DrawCall 발생 처리.
	// -> 렌더링 파이프라인 실행(구동).
	void Renderer::DrawScene()
	{
		// 바인딩.
		// -> 셰이더 각 단계에 필요한 정보 전달 및 설정.
		// State 설정.
		auto& context = GraphicsContext::Get().GetDeviceContext();

		//렌더 커맨드 가져오기
		RenderCommand& command = renderQueue[0];

		uint32_t stride = sizeof(float) * 3;
		uint32_t offset = 0;
		context.IASetVertexBuffers(0, 1, &command.vertexBuffer, &stride, &offset);

		context.IASetIndexBuffer(command.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		context.IASetInputLayout(command.inputLayout);

		//점 3개씩 잘라서 읽고 삼각형을 만들어 주는 모드
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//셰이더 설정
		context.VSSetShader(command.vertexShader, nullptr, 0);
		context.PSSetShader(command.pixelShader, nullptr, 0);
		
		// 드로우 콜.
		//렌더링 파이프라인 동작
		context.DrawIndexed(command.indexCount, 0, 0);
	}
}