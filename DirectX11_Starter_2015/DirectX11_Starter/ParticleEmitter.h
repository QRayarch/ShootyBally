#pragma once
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "Vertex.h"
#include "Component.h"

using namespace DirectX;

class ParticleEmitter : public Component
{
public:
	ParticleEmitter();
	ParticleEmitter(
		const XMFLOAT3& startPosition,
		const XMFLOAT3& startVelocity,
		const XMFLOAT4& startColor,
		const XMFLOAT4& midColor,
		const XMFLOAT4& endColor,
		float startSize,
		float midSize,
		float endSize,
		const XMFLOAT3& constantAccel,
		float ageToSpawn,
		float maxLifetime,
		float disableDelay,
		ID3D11ShaderResourceView* texture,
		ID3D11Device* device);
	void Init(
		const XMFLOAT3& startPosition,
		const XMFLOAT3& startVelocity,
		const XMFLOAT4& startColor,
		const XMFLOAT4& midColor,
		const XMFLOAT4& endColor,
		float startSize,
		float midSize,
		float endSize,
		const XMFLOAT3& constantAccel,
		float ageToSpawn,
		float maxLifetime,
		float disableDelay,
		ID3D11ShaderResourceView* texture,
		ID3D11Device* device);
	~ParticleEmitter();

	bool GetEnabled() const { return enabled; }
	float GetDisableTimer() const { return disableTimer; }
	XMFLOAT3 GetConstantAccel() const { return constantAccel; }
	float GetAgeToSpawn() const { return ageToSpawn; }
	float GetMaxLifetime() const { return maxLifetime; }
	ID3D11ShaderResourceView* GetTexture() const { return texture; }
	ID3D11Buffer** GetVertexBufferPointer() { return &vertexBuffer; }
	ID3D11Buffer** GetSoBufferReadPointer() { return &soBufferRead; }
	ID3D11Buffer** GetSoBufferWritePointer() { return &soBufferWrite; }
	int GetFrameCount() const { return frameCount; }
	void SetFrameCount(int frameCount) { this->frameCount = frameCount; }

	void Update() override;

	void Enable();
	void Disable();

	void ParticlesUpdate(float deltaTime);

	// Swaps stream out buffers for ping-ponging.
	void SwapSoBuffers();

private:
	XMFLOAT4 startColor;
	XMFLOAT4 midColor;
	XMFLOAT4 endColor;
	XMFLOAT3 startPosition;
	XMFLOAT3 startVelocity;
	XMFLOAT3 constantAccel;
	float startSize;
	float midSize;
	float endSize;
	float ageToSpawn;
	float maxLifetime;
	float disableDelay;
	float disableTimer;

	ID3D11Device* device;
	ID3D11ShaderResourceView* texture;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* soBufferRead;
	ID3D11Buffer* soBufferWrite;
	int frameCount;
	bool spawnFlip;
	bool enabled;

	void DisableInternal();
};

#endif