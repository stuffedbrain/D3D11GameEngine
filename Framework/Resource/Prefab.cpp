#include "Framework.h"
#include "Prefab.h"

Prefab::Prefab(Context * context) :
	Resource<Prefab>(context)
{
}

void Prefab::LoadFromFile(const std::string & path)
{
}
