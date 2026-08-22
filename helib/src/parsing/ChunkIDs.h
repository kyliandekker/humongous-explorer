#pragma once

#include <string_view>

namespace humongousexplorer::parsing
{
	using chunkid = std::string_view;

	// TALK .HE2
	constexpr chunkid TLKB_CHUNK_ID = "TLKB";
	constexpr chunkid TALK_CHUNK_ID = "TALK";
	constexpr chunkid HSHD_CHUNK_ID = "HSHD";
	constexpr chunkid SBNG_CHUNK_ID = "SBNG";

	// SONG .HE4
	constexpr chunkid SONG_CHUNK_ID = "SONG";
	constexpr chunkid SGEN_CHUNK_ID = "SGEN";
	constexpr chunkid DIGI_CHUNK_ID = "DIGI";
	constexpr chunkid SGHD_CHUNK_ID = "SGHD";

	// GENERAL CHUNKS
	constexpr chunkid SDAT_CHUNK_ID = "SDAT";

	// RESOURCE CHUNKS .(A)
	constexpr chunkid IMHD_CHUNK_ID = "IMHD";
	constexpr chunkid LECF_CHUNK_ID = "LECF";
	constexpr chunkid LFLF_CHUNK_ID = "LFLF";
	constexpr chunkid RMIM_CHUNK_ID = "RMIM";
	constexpr chunkid RMIH_CHUNK_ID = "RMIH";
	constexpr chunkid SCRP_CHUNK_ID = "SCRP";
	constexpr chunkid SMAP_CHUNK_ID = "SMAP";
	constexpr chunkid BMAP_CHUNK_ID = "BMAP";
	constexpr chunkid BOMP_CHUNK_ID = "BOMP";
	constexpr chunkid LOFF_CHUNK_ID = "LOFF";
	constexpr chunkid RMDA_CHUNK_ID = "RMDA";
	constexpr chunkid ROOM_CHUNK_ID = "ROOM";
	constexpr chunkid RMSC_CHUNK_ID = "RMSC";
	constexpr chunkid SOUN_CHUNK_ID = "SOUN";
	constexpr chunkid AKOS_CHUNK_ID = "AKOS";
	constexpr chunkid COST_CHUNK_ID = "COST";
	constexpr chunkid CHAR_CHUNK_ID = "CHAR";
	constexpr chunkid MULT_CHUNK_ID = "MULT";
	constexpr chunkid AWIZ_CHUNK_ID = "AWIZ";
	constexpr chunkid TLKE_CHUNK_ID = "TLKE";
	constexpr chunkid RMHD_CHUNK_ID = "RMHD";
	constexpr chunkid CYCL_CHUNK_ID = "CYCL";
	constexpr chunkid PALS_CHUNK_ID = "PALS";
	constexpr chunkid IMAG_CHUNK_ID = "IMAG";
	constexpr chunkid OBIM_CHUNK_ID = "OBIM";
	constexpr chunkid BOXD_CHUNK_ID = "BOXD";
	constexpr chunkid BOXM_CHUNK_ID = "BOXM";
	constexpr chunkid SCAL_CHUNK_ID = "SCAL";
	constexpr chunkid TRNS_CHUNK_ID = "TRNS";
	constexpr chunkid EPAL_CHUNK_ID = "EPAL";
	constexpr chunkid CLUT_CHUNK_ID = "CLUT";
	constexpr chunkid OBCD_CHUNK_ID = "OBCD";
	constexpr chunkid EXCD_CHUNK_ID = "EXCD";
	constexpr chunkid ENCD_CHUNK_ID = "ENCD";
	constexpr chunkid NLSC_CHUNK_ID = "NLSC";
	constexpr chunkid LSCR_CHUNK_ID = "LSCR";
	constexpr chunkid LSC2_CHUNK_ID = "LSC2";
	constexpr chunkid POLD_CHUNK_ID = "POLD";
	constexpr chunkid OFFS_CHUNK_ID = "OFFS";
	constexpr chunkid APAL_CHUNK_ID = "APAL";
	constexpr chunkid SEQI_CHUNK_ID = "SEQI";
	constexpr chunkid CDHD_CHUNK_ID = "CDHD";
	constexpr chunkid OBNA_CHUNK_ID = "OBNA";
	constexpr chunkid VERB_CHUNK_ID = "VERB";
	constexpr chunkid IM00_CHUNK_ID = "IM00";
	constexpr chunkid IM01_CHUNK_ID = "IM01";
	constexpr chunkid IM02_CHUNK_ID = "IM02";
	constexpr chunkid IM03_CHUNK_ID = "IM03";
	constexpr chunkid IM04_CHUNK_ID = "IM04";
	constexpr chunkid IM05_CHUNK_ID = "IM05";
	constexpr chunkid IM06_CHUNK_ID = "IM06";
	constexpr chunkid IM07_CHUNK_ID = "IM07";
	constexpr chunkid IM08_CHUNK_ID = "IM08";
	constexpr chunkid IM09_CHUNK_ID = "IM09";
	constexpr chunkid IM10_CHUNK_ID = "IM10";
	constexpr chunkid IM11_CHUNK_ID = "IM11";
	constexpr chunkid IM12_CHUNK_ID = "IM12";
	constexpr chunkid IM13_CHUNK_ID = "IM13";
	constexpr chunkid IM14_CHUNK_ID = "IM14";
	constexpr chunkid IM15_CHUNK_ID = "IM15";
	constexpr chunkid IM16_CHUNK_ID = "IM16";
	constexpr chunkid IM17_CHUNK_ID = "IM17";
	constexpr chunkid ZP00_CHUNK_ID = "ZP00";
	constexpr chunkid ZP01_CHUNK_ID = "ZP01";
	constexpr chunkid ZP02_CHUNK_ID = "ZP02";
	constexpr chunkid ZP03_CHUNK_ID = "ZP03";
	constexpr chunkid ZP04_CHUNK_ID = "ZP04";
	constexpr chunkid ZP05_CHUNK_ID = "ZP05";
	constexpr chunkid AKHD_CHUNK_ID = "AKHD";
	constexpr chunkid AKPL_CHUNK_ID = "AKPL";
	constexpr chunkid AKSQ_CHUNK_ID = "AKSQ";
	constexpr chunkid AKOF_CHUNK_ID = "AKOF";
	constexpr chunkid AKCI_CHUNK_ID = "AKCI";
	constexpr chunkid AKCD_CHUNK_ID = "AKCD";
	constexpr chunkid AKLC_CHUNK_ID = "AKLC";
	constexpr chunkid AKST_CHUNK_ID = "AKST";
	constexpr chunkid AKCT_CHUNK_ID = "AKCT";
	constexpr chunkid SP2C_CHUNK_ID = "SP2C";
	constexpr chunkid SPLF_CHUNK_ID = "SPLF";
	constexpr chunkid CLRS_CHUNK_ID = "CLRS";
	constexpr chunkid IMGL_CHUNK_ID = "IMGL";
	constexpr chunkid SQDB_CHUNK_ID = "SQDB";
	constexpr chunkid AKFO_CHUNK_ID = "AKFO";
	constexpr chunkid DROO_CHUNK_ID = "DROO";
	constexpr chunkid DSCR_CHUNK_ID = "DSCR";
	constexpr chunkid DSOU_CHUNK_ID = "DSOU";
	constexpr chunkid DCOS_CHUNK_ID = "DCOS";
	constexpr chunkid DCHR_CHUNK_ID = "DCHR";
	constexpr chunkid RGBS_CHUNK_ID = "RGBS";
	constexpr chunkid AKCH_CHUNK_ID = "AKCH";
	constexpr chunkid CNVS_CHUNK_ID = "CNVS";
	constexpr chunkid WIZH_CHUNK_ID = "WIZH";
	constexpr chunkid WIZD_CHUNK_ID = "WIZD";
	constexpr chunkid SPOT_CHUNK_ID = "SPOT";
	constexpr chunkid RELO_CHUNK_ID = "RELO";
	constexpr chunkid TEXT_CHUNK_ID = "TEXT";
	constexpr chunkid NAME_CHUNK_ID = "NAME";
	constexpr chunkid STOF_CHUNK_ID = "STOF";
	constexpr chunkid SQLC_CHUNK_ID = "SQLC";
	constexpr chunkid SIZE_CHUNK_ID = "SIZE";
	constexpr chunkid SVER_CHUNK_ID = "SVER";
	constexpr chunkid ANAM_CHUNK_ID = "ANAM";
	constexpr chunkid WRAP_CHUNK_ID = "WRAP";
	constexpr chunkid DEFA_CHUNK_ID = "DEFA";
	constexpr chunkid RMAP_CHUNK_ID = "RMAP";
	constexpr chunkid CUSE_CHUNK_ID = "CUSE";
	constexpr chunkid IM0A_CHUNK_ID = "IM0A";
	constexpr chunkid IM0B_CHUNK_ID = "IM0B";
	constexpr chunkid IM0C_CHUNK_ID = "IM0C";
	constexpr chunkid IM0D_CHUNK_ID = "IM0D";
	constexpr chunkid IM0E_CHUNK_ID = "IM0E";
	constexpr chunkid IM0F_CHUNK_ID = "IM0F";
	constexpr chunkid XMAP_CHUNK_ID = "XMAP";

	// INDEX CHUNKS .HE0
	constexpr chunkid MAXS_CHUNK_ID = "MAXS";
	constexpr chunkid DIRI_CHUNK_ID = "DIRI";
	constexpr chunkid DIRR_CHUNK_ID = "DIRR";
	constexpr chunkid DIRS_CHUNK_ID = "DIRS";
	constexpr chunkid DIRN_CHUNK_ID = "DIRN";
	constexpr chunkid DIRC_CHUNK_ID = "DIRC";
	constexpr chunkid DIRF_CHUNK_ID = "DIRF";
	constexpr chunkid DIRM_CHUNK_ID = "DIRM";
	constexpr chunkid DIRT_CHUNK_ID = "DIRT";
	constexpr chunkid DLFL_CHUNK_ID = "DLFL";
	constexpr chunkid DISK_CHUNK_ID = "DISK";
	constexpr chunkid RNAM_CHUNK_ID = "RNAM";
	constexpr chunkid DOBJ_CHUNK_ID = "DOBJ";
	constexpr chunkid AARY_CHUNK_ID = "AARY";
	constexpr chunkid INIB_CHUNK_ID = "INIB";
	constexpr chunkid NOTE_CHUNK_ID = "NOTE";

	constexpr chunkid WSOU_CHUNK_ID = "WSOU";
	constexpr chunkid REMP_CHUNK_ID = "REMP";
}
