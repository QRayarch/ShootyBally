#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition - You will eventually ADD TO this!
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Position;	    // The position of the vertex
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 Tangent;
};

struct ParticleVertex
{
	int Type;
	float Age;
	DirectX::XMFLOAT3 StartPosition;
	DirectX::XMFLOAT3 StartVelocity;
	DirectX::XMFLOAT4 StartColor;
	DirectX::XMFLOAT4 MidColor;
	DirectX::XMFLOAT4 EndColor;
	DirectX::XMFLOAT3 StartMidEndSize;
};
