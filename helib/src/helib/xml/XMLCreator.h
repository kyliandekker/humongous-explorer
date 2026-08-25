#pragma once

#include <cstddef>

namespace tinyxml2
{
	class XMLElement;
	class XMLDocument;
}
namespace humongousexplorer::archive
{
	class Archive;
}
namespace humongousexplorer::parsing
{
	class Chunk;
}
namespace humongousexplorer::xml
{
	struct XMLStruct;

	bool CreateXMLFromArchive(const archive::Archive& a_Archive, tinyxml2::XMLDocument& a_Document, const XMLStruct& a_XMLInfo);
	bool CreateXMLElementFromChunk(const parsing::Chunk& a_Chunk, tinyxml2::XMLElement& a_Element, size_t& a_iOffset, const XMLStruct& a_XMLInfo, int a_iCurrentDepth = 0);
}
