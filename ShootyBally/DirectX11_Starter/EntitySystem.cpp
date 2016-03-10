#include "EntitySystem.h"
#include "Entity.h"
#include "Component.h"

EntitySystem::EntitySystem(const int newMaxNumberOfEntsCanHold)
{
	maxNumberOfEntsCanHold = newMaxNumberOfEntsCanHold;
	ents = new Entity[maxNumberOfEntsCanHold];
	numEnts = 0;
}

EntitySystem::~EntitySystem()
{
	if (ents != nullptr) {
		delete[] ents;
		ents = nullptr;
	}
}

void EntitySystem::Update()
{
	for (int e = 0; e < numEnabledEnts; e++) {
		ents[e].Update();
	}
}

// 'Adds' an entity to the active list. Also resets the entity.
Entity* EntitySystem::AddEntity()
{
	Entity* addedEnt = EnableEntity(numEnabledEnts);
	if (addedEnt != nullptr) {
		//addedEnt->Reset();
	}
	return addedEnt;
}

Entity* EntitySystem::EnableEntity(int index)
{
	//Index is not in the bounds, we don't care about it
	if (!IsEntityIndexValid(index)) return nullptr;
	//We are already active do nothing
	if (IsEntityActive(index)) return &ents[index];
	if (index >= numEnabledEnts && index < numEnts) {
		//Entity temp = ents[index];
		std::swap(ents[index], ents[numEnabledEnts]);
		numEnabledEnts++;
		return &ents[index];
	}
	else if (index >= numEnts && index < maxNumberOfEntsCanHold) {
		std::swap(ents[index], ents[numEnabledEnts]);
		std::swap(ents[index], ents[numEnts]);
		numEnts++;
		numEnabledEnts++;
		return &ents[index];
	}
	//I fell like I convered all cases, but I could be wrong, this is likely to change
	return nullptr;
}

void EntitySystem::AddComponentToEntity(int index, Component * addedComponent)
{
	//Index is not in the bounds, we don't care about it
	if (!IsEntityIndexValid(index)) return;
	//Add the component
	ents[index].AddComponent(addedComponent);
}

void EntitySystem::AddComponentToEntity(Component * addedComponent)
{
	AddComponentToEntity(numEnabledEnts - 1, addedComponent);
}

Entity * EntitySystem::GetEntity(int index)
{
	//Index is not in the bounds, we don't care about it
	if (!IsEntityIndexValid(index)) return nullptr;
	//Return the actual entity
	return &ents[index];
}

bool EntitySystem::IsEntityIndexValid(int index)
{
	return index >= 0 && index < maxNumberOfEntsCanHold;
}

bool EntitySystem::IsEntityActive(int index)
{
	return IsEntityIndexValid(index) && index < numEnabledEnts;
}