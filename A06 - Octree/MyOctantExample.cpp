#include "MyOctantExample.h"

using namespace Simplex;

Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{

}

Simplex::MyOctant::MyOctant(vector3 a_v3center, float a_fsize)
{
	m_v3Center = a_v3center; // root v3 = 0.0f
	m_fSize = a_fsize; // root cube size = 36.0f
	m_v3Min = m_v3Center - m_fSize;
	m_v3Max = m_v3Center + m_fSize;
}

vector3 Simplex::MyOctant::GetMin(void)
{
	return m_v3Min;
}

void Simplex::MyOctant::SetMin(vector3 min)
{
	m_v3Min = min;
}
vector3 Simplex::MyOctant::GetMax(void)
{
	return m_v3Max;
}

void Simplex::MyOctant::SetMax(vector3 max)
{
	m_v3Max = max;
}
vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

void Simplex::MyOctant::SetCenterGlobal(vector3 center)
{
	m_v3Center = center;
}

float Simplex::MyOctant::GetSize(void)
{
	return m_fSize;
}

void Simplex::MyOctant::SetSize(float size)
{
	m_fSize = size;
}

MyOctant* Simplex::MyOctant::GetOctantContainingEntity(MyRigidBody* rigidbody) 
{
	if (m_lChild.empty()) {
		if (HasPoint(rigidbody->GetCenterGlobal())) {
			return this;
		}
		else {
			return nullptr;
		}
	}
	else {
		for (MyOctant* octant : m_lChild) {
			MyOctant* oct = octant->GetOctantContainingEntity(rigidbody);
			if (oct != nullptr) {
				return oct;
			}
		}
	}
	return nullptr;
}

bool Simplex::MyOctant::Contains(MyRigidBody* rigidbody)
{
	vector3 center = rigidbody->GetCenterGlobal();
	return HasPoint(center);
}

bool Simplex::MyOctant::HasPoint(vector3 v3Position)
{
	if (m_lChild.empty()) {
		if (v3Position.x > m_v3Min.x && v3Position.y > m_v3Min.y && v3Position.z > m_v3Min.z) { // Check greater than minimum
			if (v3Position.x < m_v3Max.x && v3Position.y < m_v3Max.y && v3Position.z < m_v3Max.z) { // check less than maximum
				return true;
			}
		}
	}
	else {
		for (MyOctant* nOctant : m_lChild) 
		{
			if (nOctant->HasPoint(v3Position)) 
			{
				return true;
			}
		}
	}
	return false;
}

void Simplex::MyOctant::AddEntityID(uint uID)
{
	m_EntityList.push_back(uID);
}

bool Simplex::MyOctant::HasEntity(uint uID)
{
	if (m_uChildren != 0){
		for (int i = 0; i < 8; i++)
			HasEntity(uID);
	}
	else {
		for (uint x : m_EntityList) {
			if (uID == x) 
				return true;
		}
	}
}

void Simplex::MyOctant::Populate(uint uID)
{
	m_EntityList.push_back(uID);
}

std::vector<uint> Simplex::MyOctant::GetEntityList()
{
	return m_EntityList;
}

void Simplex::MyOctant::SetEntityMngr(MyEntityManager * entityMngr)
{
	m_pEntityMngr = entityMngr;
}

void Simplex::MyOctant::Subdivide(void)
{
	vector3 newMax = ZERO_V3; // store new max positions 
	vector3 newMin = ZERO_V3; // store new min positions 
	vector3 newCenter = ZERO_V3; // store new center positions
	float newSize = m_fSize / 2.0f; // calculate new size
	for (int i = 1; i < 9; i++)
	{
			MyOctant* octant = new MyOctant(); // create new octant
			// std::cout << "NEW OCTANT LIST SIZE:::" << octant->GetEntityList().size() << ":::";
			newMax = vector3(0.0f);
			newMin = vector3(0.0f);
			newCenter = vector3(0.0f);
			if (i == 1) // back right top
			{
				newMax = m_v3Max;
				newMin = m_v3Center;
			}
			if (i == 2) // back left top
			{
				newMax = vector3(m_v3Center.x, m_v3Max.y, m_v3Max.z);
				newMin = vector3(m_v3Min.x, m_v3Center.y, m_v3Center.z);
			}
			if (i == 3) // back right bottom
			{
				newMax = vector3(m_v3Max.x, m_v3Center.y, m_v3Max.z);
				newMin = vector3(m_v3Center.x, m_v3Min.y, m_v3Center.z);
			}
			if (i == 4) // back left bottom
			{
				newMax = vector3(m_v3Center.x, m_v3Center.y, m_v3Max.z);
				newMin = vector3(m_v3Min.x, m_v3Min.y, m_v3Center.z);
			}
			if (i == 5) // front right top
			{
				newMax = vector3(m_v3Max.x, m_v3Max.y, m_v3Center.z);
				newMin = vector3(m_v3Center.x, m_v3Center.y, m_v3Min.z);
			}
			if (i == 6) // front left top
			{
				newMax = vector3(m_v3Center.x, m_v3Max.y, m_v3Center.z);
				newMin = vector3(m_v3Min.x, m_v3Center.y, m_v3Min.z);
			}
			if (i == 7) // front right bottom
			{
				newMax = vector3(m_v3Max.x, m_v3Center.y, m_v3Center.z);
				newMin = vector3(m_v3Center.x, m_v3Min.y, m_v3Min.z);
			}
			if (i == 8) // front left bottom
			{
				newMax = m_v3Center;
				newMin = m_v3Min;
			}

			newCenter = (newMax - m_fSize) + (m_fSize / 2.0f); // calculate new center for child octant
			octant->SetSize(newSize); // set new size for octant
			octant->SetCenterGlobal(newCenter); // set new center for octant
			octant->SetMin(newMin); // set the new min position for the octant
			octant->SetMax(newMax); // set the new max position for the octant

			m_lChild.push_back(octant);

			for (uint id : m_EntityList) {
				MyRigidBody* rigidbody = m_pEntityMngr->GetRigidBody(id);
				vector3 center = rigidbody->GetCenterGlobal();

				// assign id for each entity found in this octant
				if (octant->HasPoint(center)) {
					octant->AddEntityID(id);
				}

			}
			
			// TEST octant entity list count
			std::cout << "COUNT" << i << "---" << octant->GetEntityList().size();
	}

	m_EntityList.clear(); // VITAL

}

void Simplex::MyOctant::Update(void)
{
	//Clear all collisions
	for (uint i = 0; i < m_EntityList.size(); i++)
	{
		MyEntity* entity = m_pEntityMngr->GetEntity(m_EntityList[i]);
		entity->ClearCollisionList();
	}

	for (uint i=0; i < m_EntityList.size(); i++)
	{
		MyEntity* entity = m_pEntityMngr->GetEntity(m_EntityList[i]);
		MyRigidBody* rb = entity->GetRigidBody();

		MyOctant* oct = GetOctantContainingEntity(rb);
		if (oct != nullptr) 
		{
			for (uint j = i + 1; j < m_EntityList.size(); j++)
			{ 
				MyEntity* otherEntity = m_pEntityMngr->GetEntity(m_EntityList[j]);
				MyRigidBody* otherRB = otherEntity->GetRigidBody();

				if (oct->HasPoint(otherRB->GetCenterGlobal())) 
				{
					rb->IsColliding(otherRB);
					//std::cout << "collision?";
				}
			}
		}
	}
}

void Simplex::MyOctant::UpdateTwo(void)
{
	//Clear all collisions
	for (uint i = 0; i < m_EntityList.size(); i++)
	{
		MyEntity* entity = m_pEntityMngr->GetEntity(m_EntityList[i]);
		entity->ClearCollisionList();
	}

	for (uint i = 0; i < m_EntityList.size(); i++)
	{
		MyEntity* entity = m_pEntityMngr->GetEntity(m_EntityList[i]);
		MyRigidBody* rb = entity->GetRigidBody();

		MyOctant* oct = GetOctantContainingEntity(rb);
		if (oct != nullptr)
		{
			for (uint j = i + 1; j < m_EntityList.size(); j++)
			{
				MyEntity* otherEntity = m_pEntityMngr->GetEntity(m_EntityList[j]);
				MyRigidBody* otherRB = otherEntity->GetRigidBody();

				if (oct->HasPoint(otherRB->GetCenterGlobal()))
				{
					rb->IsColliding(otherRB);
					//std::cout << "collision?";
				}
			}
		}
	}
}

void Simplex::MyOctant::Display()
{
	
}
