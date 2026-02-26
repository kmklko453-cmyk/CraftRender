#pragma once


//D3D 쪽의 리소스 해제할 때 사용할 함수
template <typename T>
void SafeRelease(T*& resourse)
{
	if (resourse)
	{
		resourse->Release();
		resourse = nullptr;
	}
}
