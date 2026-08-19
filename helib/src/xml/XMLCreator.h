#pragma once

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
	bool CreateXMLFromArchive(const archive::Archive& a_Archive, tinyxml2::XMLDocument& a_Document);
	bool CreateXMLElementFromChunk(const parsing::Chunk& a_Chunk, tinyxml2::XMLElement& a_Element, size_t& a_iOffset);
}