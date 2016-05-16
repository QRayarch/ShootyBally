#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
	: ParticleEmitter(
		nullptr,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		0.0f,
		0.0f,
		0.0f,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		0,
		0.0f,
		0.0f,
		0.0f,
		nullptr,
		nullptr)
{
}

ParticleEmitter::ParticleEmitter(
	Transform* sourceTransform,
	const XMFLOAT3& startPosition,
	const XMFLOAT3& startVelocity,
	const XMFLOAT4& startColor,
	const XMFLOAT4& midColor,
	const XMFLOAT4& endColor,
	float startSize,
	float midSize,
	float endSize,
	const XMFLOAT3& constantAccel,
	int numSpawn,
	float ageToSpawn,
	float maxLifetime,
	float disableDelay,
	ID3D11ShaderResourceView* texture,
	ID3D11Device* device)
	: Component(),
	startColor(startColor),
	midColor(midColor),
	endColor(endColor),
	startPosition(startPosition),
	startVelocity(startVelocity),
	constantAccel(constantAccel),
	startSize(startSize),
	midSize(midSize),
	endSize(endSize),
	ageToSpawn(ageToSpawn),
	maxLifetime(maxLifetime),
	disableDelay(disableDelay),
	disableTimer(-1.0f),
	numSpawn(numSpawn),
	sourceTransform(sourceTransform),
	device(device),
	texture(texture),
	vertexBuffer(nullptr),
	soBufferRead(nullptr),
	soBufferWrite(nullptr),
	frameCount(0),
	spawnFlip(false),
	enabled(false)
{
}

void ParticleEmitter::Init(
	Transform* sourceTransform,
	const XMFLOAT3& startPosition,
	const XMFLOAT3& startVelocity,
	const XMFLOAT4& startColor,
	const XMFLOAT4& midColor,
	const XMFLOAT4& endColor,
	float startSize,
	float midSize,
	float endSize,
	const XMFLOAT3& constantAccel,
	int numSpawn,
	float ageToSpawn,
	float maxLifetime,
	float disableDelay,
	ID3D11ShaderResourceView* texture,
	ID3D11Device* device)
{
	this->startColor = startColor;
	this->midColor = midColor;
	this->endColor = endColor;
	this->startPosition = startPosition;
	this->startVelocity = startVelocity;
	this->constantAccel = constantAccel;
	this->startSize = startSize;
	this->midSize = midSize;
	this->endSize = endSize;
	this->ageToSpawn = ageToSpawn;
	this->maxLifetime = maxLifetime;
	this->disableDelay = disableDelay;
	this->numSpawn = numSpawn;
	this->sourceTransform = sourceTransform;
	this->device = device;
	this->texture = texture;

	UpdateTransform();
}

ParticleEmitter::~ParticleEmitter()
{
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(soBufferRead);
	ReleaseMacro(soBufferWrite);
}

void ParticleEmitter::Update(float dt)
{
	Component::Update(dt);
}

void ParticleEmitter::Enable()
{
	if (enabled)
	{
		DisableInternal();
	}

	UpdateTransform();

	// Set up the root vertex to put into the vertex buffer.
	ParticleVertex vertices[1];
	vertices[0].Type = 0;
	vertices[0].Age = 0.0f;
	vertices[0].StartPosition = startPosition;
	vertices[0].StartVelocity = startVelocity;
	vertices[0].StartColor = startColor;
	vertices[0].MidColor = midColor;
	vertices[0].EndColor = endColor;
	vertices[0].StartMidEndSize = XMFLOAT3(startSize, midSize, endSize);

	// Create the vertex buffer.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(ParticleVertex) * 1;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	HR(device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer));

	enabled = true;
}

void ParticleEmitter::Disable()
{
	if (disableDelay == 0.0f)
	{
		DisableInternal();
	}
	else
	{
		disableTimer = 0.0f;
	}
}

void ParticleEmitter::DisableInternal()
{
	ReleaseMacro(vertexBuffer);
	disableTimer = -1.0f;
	frameCount = 0;
	spawnFlip = false;
	enabled = false;
}

void ParticleEmitter::ParticlesUpdate(float deltaTime)
{
	if (enabled)
	{
		if (disableTimer == -1.0f)
		{
			UpdateTransform();
		}
		else
		{
			disableTimer += deltaTime;

			if (disableTimer >= disableDelay)
			{
				DisableInternal();
			}
		}
	}
}

void ParticleEmitter::UpdateTransform()
{
	if (sourceTransform != nullptr)
	{
		Transform& transform = GetTransform();
		transform.SetPosition(sourceTransform->GetPosition());
		transform.SetRotation(sourceTransform->GetRotation());
		transform.SetScale(sourceTransform->GetScale());
	}
}

void ParticleEmitter::SwapSoBuffers()
{
	ID3D11Buffer* temp = soBufferRead;
	soBufferRead = soBufferWrite;
	soBufferWrite = temp;
}
