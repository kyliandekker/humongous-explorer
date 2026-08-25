#include "XMLCreator.h"

#include <tinyxml/tinyxml2.h>

#include <helib/archive/Archive.h>
#include <helib/parsing/Chunk.h>
#include <helib/xml/XMLStruct.h>

namespace humongousexplorer::xml
{
	//---------------------------------------------------------------------
	bool CreateXMLFromArchive(const archive::Archive& a_Archive, tinyxml2::XMLDocument& a_Document, const XMLStruct& a_XMLInfo)
	{
		// <?xml version="1.0" encoding="UTF-8"?>
		a_Document.InsertFirstChild(a_Document.NewDeclaration());

		tinyxml2::XMLElement* root = a_Document.NewElement("Archive");
		a_Document.InsertEndChild(root);

		size_t offset = 0;
		CreateXMLElementFromChunk(a_Archive.GetRoot(), *root, offset, a_XMLInfo);

		return true;
	}

	//---------------------------------------------------------------------
	bool CreateXMLElementFromChunk(const parsing::Chunk& a_Chunk, tinyxml2::XMLElement& a_Element, size_t& a_iOffset, const XMLStruct& a_XMLInfo, int a_iCurrentDepth)
	{
		for (const std::unique_ptr<parsing::Chunk>& chunk : a_Chunk.GetChildren())
		{
			if (!chunk)
			{
				continue;
			}

			std::string tag = chunk->GetTag();
			tinyxml2::XMLElement* element = a_Element.GetDocument()->NewElement(tag.c_str());

			a_Element.InsertEndChild(element);

			element->SetAttribute("offset", a_iOffset);
			element->SetAttribute("dataSize", chunk->ChunkSize());
			element->SetAttribute("chunkSize", chunk->WholeChunkSize());

			a_iOffset += parsing::CHUNK_HEADER_SIZE;

			if (a_XMLInfo.m_iMaxDepth < 0 || a_iCurrentDepth < a_XMLInfo.m_iMaxDepth)
			{
				CreateXMLElementFromChunk(*chunk, *element, a_iOffset, a_XMLInfo, a_iCurrentDepth + 1);
			}

			a_iOffset += chunk->ChunkSize();
		}

		return true;
	}
}
