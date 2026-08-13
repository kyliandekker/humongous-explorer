#include "HEParser.h"

#include <cstring>
#include <unordered_map>
#include <string>

#include "humongous/ChunkIDs.h"
#include "core/Memory.h"

namespace humongousexplorer::parsing
{
	//---------------------------------------------------------------------
	static uint32_t ReadBE32(const uint8_t* p)
	{
		return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) |
			   (uint32_t(p[2]) << 8)  |  uint32_t(p[3]);
	}

#define NO_CHILD {}
#define IMXX { SMAP_CHUNK_ID, BMAP_CHUNK_ID, BOMP_CHUNK_ID, ZP00_CHUNK_ID, ZP01_CHUNK_ID, ZP02_CHUNK_ID, ZP03_CHUNK_ID, ZP04_CHUNK_ID, ZP05_CHUNK_ID }

	//---------------------------------------------------------------------
	const std::unordered_map<std::string, std::vector<std::string>> SCHEMA =
	{
		{ LECF_CHUNK_ID, {
				LOFF_CHUNK_ID,
				LFLF_CHUNK_ID
			}
		},
		{ LOFF_CHUNK_ID, NO_CHILD },
		{ LFLF_CHUNK_ID, {
				RMIM_CHUNK_ID,
				RMDA_CHUNK_ID,
				ROOM_CHUNK_ID,
				RMSC_CHUNK_ID,
				SCRP_CHUNK_ID,
				SOUN_CHUNK_ID,
				AKOS_CHUNK_ID,
				COST_CHUNK_ID,
				CHAR_CHUNK_ID,
				DIGI_CHUNK_ID,
				MULT_CHUNK_ID,
				AWIZ_CHUNK_ID,
				TALK_CHUNK_ID,
				TLKE_CHUNK_ID,
			}
		},
		{ ROOM_CHUNK_ID, {
				RMHD_CHUNK_ID,
				CYCL_CHUNK_ID,
				PALS_CHUNK_ID,
				IMAG_CHUNK_ID,
				OBIM_CHUNK_ID,
				BOXD_CHUNK_ID,
				BOXM_CHUNK_ID,
				SCAL_CHUNK_ID,
				RMSC_CHUNK_ID,
				TRNS_CHUNK_ID,
				EPAL_CHUNK_ID,
				CLUT_CHUNK_ID,
				RMIM_CHUNK_ID,
				OBCD_CHUNK_ID,
				EXCD_CHUNK_ID,
				ENCD_CHUNK_ID,
				NLSC_CHUNK_ID,
				LSCR_CHUNK_ID,
			}
		},
		{ RMDA_CHUNK_ID, {
				RMHD_CHUNK_ID,
				CYCL_CHUNK_ID,
				TRNS_CHUNK_ID,
				PALS_CHUNK_ID,
				OBIM_CHUNK_ID,
				OBCD_CHUNK_ID,
				EXCD_CHUNK_ID,
				ENCD_CHUNK_ID,
				NLSC_CHUNK_ID,
				LSC2_CHUNK_ID,
				LSCR_CHUNK_ID,
				POLD_CHUNK_ID,
			}
		},
		{ RMHD_CHUNK_ID, NO_CHILD },
		{ RMIM_CHUNK_ID, {
				RMIH_CHUNK_ID,
				IM00_CHUNK_ID,
			}
		},
		{ TRNS_CHUNK_ID, NO_CHILD },
		{ EPAL_CHUNK_ID, NO_CHILD },
		{ CYCL_CHUNK_ID, NO_CHILD },
		{ PALS_CHUNK_ID, {
				WRAP_CHUNK_ID,
			}
		},
		{ OFFS_CHUNK_ID, NO_CHILD },
		{ APAL_CHUNK_ID, NO_CHILD },
		{ WRAP_CHUNK_ID, {
				OFFS_CHUNK_ID,
				APAL_CHUNK_ID,
				SMAP_CHUNK_ID,
				BOMP_CHUNK_ID,
				AWIZ_CHUNK_ID,
				SEQI_CHUNK_ID,
			}
		},
		{ IMAG_CHUNK_ID, {
				WRAP_CHUNK_ID,
			}
		},
		{ OBIM_CHUNK_ID, {
				IMHD_CHUNK_ID,
				IMAG_CHUNK_ID,
				IM00_CHUNK_ID,
				IM01_CHUNK_ID,
				IM02_CHUNK_ID,
				IM03_CHUNK_ID,
				IM04_CHUNK_ID,
				IM05_CHUNK_ID,
				IM06_CHUNK_ID,
				IM07_CHUNK_ID,
				IM08_CHUNK_ID,
				IM09_CHUNK_ID,
				IM10_CHUNK_ID,
				IM11_CHUNK_ID,
				IM12_CHUNK_ID,
				IM13_CHUNK_ID,
				IM14_CHUNK_ID,
				IM15_CHUNK_ID,
				IM16_CHUNK_ID,
				IM17_CHUNK_ID,
			}
		},
		{ IM00_CHUNK_ID, IMXX },
		{ IM01_CHUNK_ID, IMXX },
		{ IM02_CHUNK_ID, IMXX },
		{ IM03_CHUNK_ID, IMXX },
		{ IM04_CHUNK_ID, IMXX },
		{ IM05_CHUNK_ID, IMXX },
		{ IM06_CHUNK_ID, IMXX },
		{ IM07_CHUNK_ID, IMXX },
		{ IM08_CHUNK_ID, IMXX },
		{ IM09_CHUNK_ID, IMXX },
		{ IM10_CHUNK_ID, IMXX },
		{ IM11_CHUNK_ID, IMXX },
		{ IM12_CHUNK_ID, IMXX },
		{ IM13_CHUNK_ID, IMXX },
		{ IM14_CHUNK_ID, IMXX },
		{ IM15_CHUNK_ID, IMXX },
		{ IM16_CHUNK_ID, IMXX },
		{ IM17_CHUNK_ID, IMXX },
		{ ZP00_CHUNK_ID, NO_CHILD },
		{ ZP01_CHUNK_ID, NO_CHILD },
		{ ZP02_CHUNK_ID, NO_CHILD },
		{ ZP03_CHUNK_ID, NO_CHILD },
		{ ZP04_CHUNK_ID, NO_CHILD },
		{ ZP05_CHUNK_ID, NO_CHILD },
		{ RMSC_CHUNK_ID, {
				ENCD_CHUNK_ID,
				EXCD_CHUNK_ID,
				OBCD_CHUNK_ID,
				LSCR_CHUNK_ID,
			}
		},
		{ OBCD_CHUNK_ID, {
				CDHD_CHUNK_ID,
				OBNA_CHUNK_ID,
				VERB_CHUNK_ID,
			}
		},
		{ BOXD_CHUNK_ID, NO_CHILD },
		{ BOXM_CHUNK_ID, NO_CHILD },
		{ CLUT_CHUNK_ID, NO_CHILD },
		{ SCAL_CHUNK_ID, NO_CHILD },
		{ RMIH_CHUNK_ID, NO_CHILD },
		{ AKOS_CHUNK_ID, {
				AKHD_CHUNK_ID,
				AKPL_CHUNK_ID,
				RGBS_CHUNK_ID,
				AKSQ_CHUNK_ID,
				AKCH_CHUNK_ID,
				AKOF_CHUNK_ID,
				AKCI_CHUNK_ID,
				AKCD_CHUNK_ID,
				AKLC_CHUNK_ID,
				AKST_CHUNK_ID,
				AKCT_CHUNK_ID,
				SP2C_CHUNK_ID,
				SPLF_CHUNK_ID,
				CLRS_CHUNK_ID,
				IMGL_CHUNK_ID,
				SQDB_CHUNK_ID,
				AKFO_CHUNK_ID,
			}
		},
		{ SMAP_CHUNK_ID, NO_CHILD },
		{ IMHD_CHUNK_ID, NO_CHILD },
		{ CDHD_CHUNK_ID, NO_CHILD },
		{ VERB_CHUNK_ID, NO_CHILD },
		{ OBNA_CHUNK_ID, NO_CHILD },
		{ EXCD_CHUNK_ID, NO_CHILD },
		{ ENCD_CHUNK_ID, NO_CHILD },
		{ NLSC_CHUNK_ID, NO_CHILD },
		{ LSCR_CHUNK_ID, NO_CHILD },
		{ CHAR_CHUNK_ID, NO_CHILD },
		{ SCRP_CHUNK_ID, NO_CHILD },
		{ COST_CHUNK_ID, NO_CHILD },
		{ SOUN_CHUNK_ID, NO_CHILD },
		{ BOMP_CHUNK_ID, NO_CHILD },
		{ RNAM_CHUNK_ID, NO_CHILD },
		{ MAXS_CHUNK_ID, NO_CHILD },
		{ DROO_CHUNK_ID, NO_CHILD },
		{ DSCR_CHUNK_ID, NO_CHILD },
		{ DSOU_CHUNK_ID, NO_CHILD },
		{ DCOS_CHUNK_ID, NO_CHILD },
		{ DCHR_CHUNK_ID, NO_CHILD },
		{ DOBJ_CHUNK_ID, NO_CHILD },
		{ BMAP_CHUNK_ID, NO_CHILD },
		{ LSC2_CHUNK_ID, NO_CHILD },
		{ DIGI_CHUNK_ID, {
				HSHD_CHUNK_ID,
				SBNG_CHUNK_ID,
				SDAT_CHUNK_ID,
			}
		},
		{ HSHD_CHUNK_ID, NO_CHILD },
		{ SDAT_CHUNK_ID, NO_CHILD },
		{ AKHD_CHUNK_ID, NO_CHILD },
		{ AKPL_CHUNK_ID, NO_CHILD },
		{ RGBS_CHUNK_ID, NO_CHILD },
		{ AKSQ_CHUNK_ID, NO_CHILD },
		{ AKCH_CHUNK_ID, NO_CHILD },
		{ AKOF_CHUNK_ID, NO_CHILD },
		{ AKCI_CHUNK_ID, NO_CHILD },
		{ AKCD_CHUNK_ID, NO_CHILD },
		{ AKLC_CHUNK_ID, NO_CHILD },
		{ AKST_CHUNK_ID, NO_CHILD },
		{ AKCT_CHUNK_ID, NO_CHILD },
		{ AKFO_CHUNK_ID, NO_CHILD },
		{ RMAP_CHUNK_ID, NO_CHILD },
		{ CUSE_CHUNK_ID, NO_CHILD },
		{ XMAP_CHUNK_ID, NO_CHILD },
		{ MULT_CHUNK_ID, {
				DEFA_CHUNK_ID,
				WRAP_CHUNK_ID,
			}
		},
		{ DEFA_CHUNK_ID, {
				RGBS_CHUNK_ID,
				CNVS_CHUNK_ID,
				RMAP_CHUNK_ID,
				CUSE_CHUNK_ID,
				CUSE_CHUNK_ID,
			}
		},
		{ AWIZ_CHUNK_ID, {
				WIZH_CHUNK_ID,
				WIZD_CHUNK_ID,
				CNVS_CHUNK_ID,
				SPOT_CHUNK_ID,
				RELO_CHUNK_ID,
				RGBS_CHUNK_ID,
				XMAP_CHUNK_ID,
				TRNS_CHUNK_ID,
				TRNS_CHUNK_ID,
			}
		},
		{ TLKE_CHUNK_ID, {
				TEXT_CHUNK_ID,
			}
		},
		{ TEXT_CHUNK_ID, NO_CHILD },
		{ WIZH_CHUNK_ID, NO_CHILD },
		{ WIZD_CHUNK_ID, NO_CHILD },
		{ CNVS_CHUNK_ID, NO_CHILD },
		{ SPOT_CHUNK_ID, NO_CHILD },
		{ RELO_CHUNK_ID, NO_CHILD },
		{ POLD_CHUNK_ID, NO_CHILD },
		{ SP2C_CHUNK_ID, NO_CHILD },
		{ SPLF_CHUNK_ID, NO_CHILD },
		{ CLRS_CHUNK_ID, NO_CHILD },
		{ IMGL_CHUNK_ID, NO_CHILD },
		{ SQDB_CHUNK_ID, {
				WRAP_CHUNK_ID,
			}
		},
		{ SEQI_CHUNK_ID, {
				NAME_CHUNK_ID,
				STOF_CHUNK_ID,
				SQLC_CHUNK_ID,
				SIZE_CHUNK_ID,
			}
		},
		{ NAME_CHUNK_ID, NO_CHILD },
		{ STOF_CHUNK_ID, NO_CHILD },
		{ SQLC_CHUNK_ID, NO_CHILD },
		{ SIZE_CHUNK_ID, NO_CHILD },
		{ SBNG_CHUNK_ID, NO_CHILD },
		{ TALK_CHUNK_ID, {
				HSHD_CHUNK_ID,
				SBNG_CHUNK_ID,
				SDAT_CHUNK_ID,
			}
		},
		{ DIRI_CHUNK_ID, NO_CHILD },
		{ DIRR_CHUNK_ID, NO_CHILD },
		{ DIRS_CHUNK_ID, NO_CHILD },
		{ DIRN_CHUNK_ID, NO_CHILD },
		{ DIRC_CHUNK_ID, NO_CHILD },
		{ DIRF_CHUNK_ID, NO_CHILD },
		{ DIRM_CHUNK_ID, NO_CHILD },
		{ DIRT_CHUNK_ID, NO_CHILD },
		{ DLFL_CHUNK_ID, NO_CHILD },
		{ DISK_CHUNK_ID, NO_CHILD },
		{ SVER_CHUNK_ID, NO_CHILD },
		{ AARY_CHUNK_ID, NO_CHILD },
		{ INIB_CHUNK_ID, {
				NOTE_CHUNK_ID,
			}
		},
		{ NOTE_CHUNK_ID, NO_CHILD },
		{ TLKB_CHUNK_ID, {
				TALK_CHUNK_ID,
			}
		},
		{ SONG_CHUNK_ID, {
				SGHD_CHUNK_ID,
				SGEN_CHUNK_ID,
				DIGI_CHUNK_ID,
			}
		},
		{ SGHD_CHUNK_ID, NO_CHILD },
		{ SGEN_CHUNK_ID , NO_CHILD },
		{ ANAM_CHUNK_ID , NO_CHILD },
		{ IM0A_CHUNK_ID, {
				SMAP_CHUNK_ID,
			}
		},
		{ IM0B_CHUNK_ID, {
				SMAP_CHUNK_ID,
			}
		},
		{ IM0C_CHUNK_ID, {
				SMAP_CHUNK_ID,
			}
		},
		{ IM0D_CHUNK_ID, {
				SMAP_CHUNK_ID,
			}
		},
		{ IM0E_CHUNK_ID, {
				SMAP_CHUNK_ID,
			}
		},
		{ IM0F_CHUNK_ID, {
				SMAP_CHUNK_ID,
			}
		},
		{ TMSK_CHUNK_ID, {} },
	};

	//---------------------------------------------------------------------
	size_t Chunk::ChunkSize() const
	{
		if (m_aChildren.empty())
		{
			return m_Data.size();
		}
		
		size_t childrenSize = 0;
		for (const std::unique_ptr<Chunk>& chunk : m_aChildren)
		{
			childrenSize += chunk->ChunkSize();
		}
		return childrenSize;
	}

	//---------------------------------------------------------------------
	size_t Chunk::WholeChunkSize() const
	{
		if (m_aChildren.empty())
		{
			return m_Data.size() + HEADER_SIZE;
		}

		size_t childrenSize = 0;
		for (const std::unique_ptr<Chunk>& chunk : m_aChildren)
		{
			childrenSize += chunk->WholeChunkSize();
		}
		return childrenSize + HEADER_SIZE;
	}

	//---------------------------------------------------------------------
	void Chunk::FixParents(Chunk& a_Chunk)
	{
		for (auto& child : a_Chunk.m_aChildren)
		{
			child->m_pParent = &a_Chunk;
			FixParents(*child);
		}
	}

	//---------------------------------------------------------------------
	Chunk* Chunk::TryFindChild(const std::string& a_sChunkID)
	{
		Chunk* found = nullptr;
		for (std::unique_ptr<Chunk>& chunk : m_aChildren)
		{
			if (core::chunkcmp(chunk->m_sTag, a_sChunkID.c_str()) == 0)
			{
				return chunk.get();
			}

			if (found = chunk->TryFindChild(a_sChunkID))
			{
				return found;
			}
		}
		return found;
	}

	//---------------------------------------------------------------------
	Chunk* Chunk::FindChunkAt(size_t a_iTarget, size_t a_iBase)
	{
		size_t totalSize = WholeChunkSize();
		if (a_iTarget < a_iBase || a_iTarget >= a_iBase + totalSize)
		{
			return nullptr;
		}

		if (a_iTarget < a_iBase + 8)
		{
			return this;
		}

		size_t childPos = a_iBase + 8;
		for (auto& child : m_aChildren)
		{
			size_t childTotal = child->WholeChunkSize();
			if (a_iTarget < childPos + childTotal)
			{
				return child->FindChunkAt(a_iTarget, childPos);
			}
			childPos += childTotal;
		}
		return this;
	}

	//---------------------------------------------------------------------
	Chunk* Chunk::GetRoot()
	{
		Chunk* parent = m_pParent;
		while (parent && parent->m_pParent)
		{
			parent = parent->m_pParent; 
		}
		return parent;
	}

	//---------------------------------------------------------------------
	bool ParseChunks(Chunk& a_Out, const core::Data& a_Buf, size_t a_iPos /* = 0 */)
	{
		const auto* data = a_Buf.dataAs<unsigned char>();

		// A chunk always needs an 8-byte header.
		if (a_iPos > a_Buf.size() || a_Buf.size() - a_iPos < 8)
			return false;

		memcpy(a_Out.m_sTag, data + a_iPos, 4);

		const size_t size = ReadBE32(data + a_iPos + 4);

		// Size includes the 8-byte header.
		if (size < 8)
			return false;

		// Entire chunk must fit inside the buffer.
		if (size > a_Buf.size() - a_iPos)
			return false;

		const size_t endPos = a_iPos + size;

		std::string tag(a_Out.m_sTag, CHUNK_ID_SIZE);
		auto it = SCHEMA.find(tag);
		const bool isContainer =
			it != SCHEMA.end() && !it->second.empty();

		if (isContainer)
		{
			size_t childPos = a_iPos + 8;

			while (childPos < endPos)
			{
				// Remaining bytes aren't enough for a child header.
				if (endPos - childPos < 8)
					return false;

				const size_t childSize =
					ReadBE32(data + childPos + 4);

				// Child must have a header and fit inside parent.
				if (childSize < 8 || childSize > endPos - childPos)
					return false;

				auto child = std::make_unique<Chunk>();
				child->m_pParent = &a_Out;

				if (!ParseChunks(*child, a_Buf, childPos))
					return false;

				a_Out.m_aChildren.emplace_back(std::move(child));

				childPos += childSize;
			}
		}
		else
		{
			a_Out.m_Data = core::Data(
				a_Buf.dataAs<char>() + a_iPos + 8,
				size - 8
			);
		}

		return true;
	}
}
