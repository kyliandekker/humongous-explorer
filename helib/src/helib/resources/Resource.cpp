#include "./Resource.h"

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	// Resource
	//---------------------------------------------------------------------
	ResourceType Resource::GetResourceType() const
	{
		return m_eResourceType;
	}
}