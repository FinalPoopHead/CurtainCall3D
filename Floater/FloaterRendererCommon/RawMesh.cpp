#include "./include/RawMesh.h"

flt::RawMesh* flt::RawMeshBuilder::build() const
{
	RawMesh* pMesh = new RawMesh();
	pMesh->vertices = std::move(vertices);
	pMesh->indices = std::move(indices);
	return pMesh;
}
