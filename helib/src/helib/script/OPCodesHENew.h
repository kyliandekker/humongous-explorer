#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace humongousexplorer::script
{
	enum class ScrArgumentType;
	struct ArgInfo
	{
		ArgInfo(size_t a_iSize, ScrArgumentType a_eArgumentType);

		size_t m_iSize;
		ScrArgumentType m_eArgumentType;
	};

	std::vector<ArgInfo> default_func(uint8_t a_iByte, const unsigned char* a_pData);
	class bytecode
	{
	public:
		~bytecode();
		bytecode() = default;
		bytecode(const std::string& a_sName);
		bytecode(const std::string& a_sName, std::function<std::vector<ArgInfo>(uint8_t, const unsigned char*)> a_fnSize);

		const std::string& GetName() const;
		std::function<std::vector<ArgInfo>(uint8_t, const unsigned char*)> GetSizeFn() const;
	private:
		std::string m_sName;
		std::function<std::vector<ArgInfo>(uint8_t, const unsigned char*)> m_fnSize = default_func;
	};

	using OPCodeMap = std::unordered_map<uint8_t, bytecode>;

	std::vector<ArgInfo> extended_b_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_w_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_ww_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_dw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_ddw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_bw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_bdw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> jump_cmd(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> djump_cmd(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> msg_cmd(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> msg_cmd_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> msg_cmd_he100(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> msg_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> msg_op_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> actor_ops_v6(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> actor_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> actor_ops_he60(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> verb_ops_v6(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> verb_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> array_ops_v6(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> array_ops(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> array_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> array_ops_he100(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> wait_ops(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> wait_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> wait_ops_he100(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> room_ops_he60(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> dmsg_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> sys_msg(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> ini_op_v71(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> file_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> file_op_he100(uint8_t a_iByte, const unsigned char* a_pData);

	//---------------------------------------------------------------------
	inline void GetV6codes(OPCodeMap& a_mOPCodes)
	{
		a_mOPCodes[0x00] = bytecode("o6_pushByte", extended_b_op );
		a_mOPCodes[0x01] = bytecode("o6_pushWord", extended_w_op );
		a_mOPCodes[0x02] = bytecode("o6_pushByteVar", extended_b_op );
		a_mOPCodes[0x03] = bytecode("o6_pushWordVar", extended_w_op );
		// TODO: a_mOPCodes[0x06] = bytecode("o6_byteArrayRead"};
		a_mOPCodes[0x07] = bytecode("o6_wordArrayRead", extended_w_op );
		a_mOPCodes[0x0A] = bytecode("o6_byteArrayIndexedRead", extended_b_op );
		a_mOPCodes[0x0B] = bytecode("o6_wordArrayIndexedRead", extended_w_op );
		a_mOPCodes[0x0C] = bytecode("o6_dup");
		a_mOPCodes[0x0D] = bytecode("o6_not");
		a_mOPCodes[0x0E] = bytecode("o6_eq" );
		a_mOPCodes[0x0F] = bytecode("o6_neq" );
		a_mOPCodes[0x10] = bytecode("o6_gt" );
		a_mOPCodes[0x11] = bytecode("o6_lt" );
		a_mOPCodes[0x12] = bytecode("o6_le" );
		a_mOPCodes[0x13] = bytecode("o6_ge" );
		a_mOPCodes[0x14] = bytecode("o6_add" );
		a_mOPCodes[0x15] = bytecode("o6_sub" );
		a_mOPCodes[0x16] = bytecode("o6_mul" );
		a_mOPCodes[0x17] = bytecode("o6_div" );
		a_mOPCodes[0x18] = bytecode("o6_land" );  // logical and
		a_mOPCodes[0x19] = bytecode("o6_lor" );  // logical or
		a_mOPCodes[0x1A] = bytecode("o6_pop" );
		// TODO: a_mOPCodes[0x42] = bytecode("o6_writeByteVar"};
		a_mOPCodes[0x43] = bytecode("o6_writeWordVar", extended_w_op );
		// TODO: a_mOPCodes[0x46] = bytecode("o6_byteArrayWrite"};
		a_mOPCodes[0x47] = bytecode("o6_wordArrayWrite", extended_w_op );
		// TODO: a_mOPCodes[0x4a] = bytecode("o6_byteArrayIndexedWrite"};
		a_mOPCodes[0x4B] = bytecode("o6_wordArrayIndexedWrite", extended_w_op );
		// TODO: a_mOPCodes[0x4e] = bytecode("o6_byteVarInc"};
		a_mOPCodes[0x4F] = bytecode("o6_wordVarInc", extended_w_op );
		// TODO: a_mOPCodes[0x52] = bytecode("o6_byteArrayInc"};
		a_mOPCodes[0x53] = bytecode("o6_wordArrayInc", extended_w_op );
		// TODO: a_mOPCodes[0x56] = bytecode("o6_byteVarDec"};
		a_mOPCodes[0x57] = bytecode("o6_wordVarDec", extended_w_op );
		// TODO: a_mOPCodes[0x5a] = bytecode("o6_byteArrayDec"};
		a_mOPCodes[0x5B] = bytecode("o6_wordArrayDec", extended_w_op );
		a_mOPCodes[0x5C] = bytecode("o6_if", jump_cmd );  // jump if
		a_mOPCodes[0x5D] = bytecode("o6_ifNot", jump_cmd );  // jump if not
		a_mOPCodes[0x5E] = bytecode("o6_startScript" );
		a_mOPCodes[0x5F] = bytecode("o6_startScriptQuick" );
		a_mOPCodes[0x60] = bytecode("o6_startObject" );
		a_mOPCodes[0x61] = bytecode("o6_drawObject" );
		a_mOPCodes[0x62] = bytecode("o6_drawObjectAt" );
		a_mOPCodes[0x63] = bytecode("o6_drawBlastObject" );
		a_mOPCodes[0x64] = bytecode("o6_setBlastObjectWindow" );
		a_mOPCodes[0x65] = bytecode("o6_stopObjectCodeObject" );  // o6_stopObjectCode
		a_mOPCodes[0x66] = bytecode("o6_stopObjectCodeScript" );  // o6_stopObjectCode
		a_mOPCodes[0x67] = bytecode("o6_endCutscene" );
		a_mOPCodes[0x68] = bytecode("o6_cutscene" );
		// TODO: a_mOPCodes[0x69] = bytecode("o6_stopMusic"};
		a_mOPCodes[0x6A] = bytecode("o6_freezeUnfreeze" );
		a_mOPCodes[0x6B] = bytecode("o6_cursorCommand", extended_b_op );
		a_mOPCodes[0x6C] = bytecode("o6_breakHere" );
		a_mOPCodes[0x6D] = bytecode("o6_ifClassOfIs" );
		a_mOPCodes[0x6E] = bytecode("o6_setClass" );
		a_mOPCodes[0x6F] = bytecode("o6_getState" );
		a_mOPCodes[0x70] = bytecode("o6_setState" );
		a_mOPCodes[0x71] = bytecode("o6_setOwner" );
		a_mOPCodes[0x72] = bytecode("o6_getOwner" );
		a_mOPCodes[0x73] = bytecode("o6_jump", jump_cmd );
		a_mOPCodes[0x74] = bytecode("o6_startSound" );
		a_mOPCodes[0x75] = bytecode("o6_stopSound" );
		// TODO: a_mOPCodes[0x76] = bytecode("o6_startMusic"};
		a_mOPCodes[0x77] = bytecode("o6_stopObjectScript" );
		a_mOPCodes[0x78] = bytecode("o6_panCameraTo" );
		a_mOPCodes[0x79] = bytecode("o6_actorFollowCamera" );
		a_mOPCodes[0x7A] = bytecode("o6_setCameraAt" );
		a_mOPCodes[0x7B] = bytecode("o6_loadRoom" );
		a_mOPCodes[0x7C] = bytecode("o6_stopScript" );
		a_mOPCodes[0x7D] = bytecode("o6_walkActorToObj" );
		a_mOPCodes[0x7E] = bytecode("o6_walkActorTo" );
		a_mOPCodes[0x7F] = bytecode("o6_putActorAtXY" );
		a_mOPCodes[0x80] = bytecode("o6_putActorAtObject" );
		a_mOPCodes[0x81] = bytecode("o6_faceActor" );
		a_mOPCodes[0x82] = bytecode("o6_animateActor" );
		a_mOPCodes[0x83] = bytecode("o6_doSentence" );
		a_mOPCodes[0x84] = bytecode("o6_pickupObject" );
		a_mOPCodes[0x85] = bytecode("o6_loadRoomWithEgo" );
		a_mOPCodes[0x87] = bytecode("o6_getRandomNumber" );
		a_mOPCodes[0x88] = bytecode("o6_getRandomNumberRange" );
		a_mOPCodes[0x8A] = bytecode("o6_getActorMoving" );
		a_mOPCodes[0x8B] = bytecode("o6_isScriptRunning" );
		a_mOPCodes[0x8C] = bytecode("o6_getActorRoom" );
		a_mOPCodes[0x8D] = bytecode("o6_getObjectX" );
		a_mOPCodes[0x8E] = bytecode("o6_getObjectY" );
		a_mOPCodes[0x8F] = bytecode("o6_getObjectOldDir" );
		a_mOPCodes[0x90] = bytecode("o6_getActorWalkBox" );
		a_mOPCodes[0x91] = bytecode("o6_getActorCostume" );
		a_mOPCodes[0x92] = bytecode("o6_findInventory" );
		a_mOPCodes[0x93] = bytecode("o6_getInventoryCount" );
		a_mOPCodes[0x94] = bytecode("o6_getVerbFromXY" );
		a_mOPCodes[0x95] = bytecode("o6_beginOverride" );
		a_mOPCodes[0x96] = bytecode("o6_endOverride" );
		a_mOPCodes[0x97] = bytecode("o6_setObjectName", msg_op );
		a_mOPCodes[0x98] = bytecode("o6_isSoundRunning" );
		a_mOPCodes[0x99] = bytecode("o6_setBoxFlags" );
		a_mOPCodes[0x9A] = bytecode("o6_createBoxMatrix" );
		a_mOPCodes[0x9B] = bytecode("o6_resourceRoutines", extended_b_op );
		a_mOPCodes[0x9C] = bytecode("o6_roomOps", extended_b_op );
		a_mOPCodes[0x9D] = bytecode("o6_actorOps", actor_ops_v6 );
		a_mOPCodes[0x9E] = bytecode("o6_verbOps", verb_ops_v6 );
		a_mOPCodes[0x9F] = bytecode("o6_getActorFromXY" );
		a_mOPCodes[0xA0] = bytecode("o6_findObject" );
		a_mOPCodes[0xA1] = bytecode("o6_pseudoRoom" );
		a_mOPCodes[0xA2] = bytecode("o6_getActorElevation" );
		a_mOPCodes[0xA3] = bytecode("o6_getVerbEntrypoint" );
		a_mOPCodes[0xA4] = bytecode("o6_arrayOps", array_ops_v6 );
		a_mOPCodes[0xA5] = bytecode("o6_saveRestoreVerbs", extended_b_op );
		a_mOPCodes[0xA6] = bytecode("o6_drawBox" );
		a_mOPCodes[0xA7] = bytecode("o6_pop" );
		a_mOPCodes[0xA8] = bytecode("o6_getActorWidth" );
		a_mOPCodes[0xA9] = bytecode("o6_wait", wait_ops );
		a_mOPCodes[0xAA] = bytecode("o6_getActorScaleX" );
		a_mOPCodes[0xAB] = bytecode("o6_getActorAnimCounter" );
		a_mOPCodes[0xAC] = bytecode("o6_soundKludge" );
		a_mOPCodes[0xAD] = bytecode("o6_isAnyOf" );
		a_mOPCodes[0xAE] = bytecode("o6_systemOps", extended_b_op );
		a_mOPCodes[0xAF] = bytecode("o6_isActorInBox" );
		a_mOPCodes[0xB0] = bytecode("o6_delay" );
		a_mOPCodes[0xB1] = bytecode("o6_delaySeconds" );
		a_mOPCodes[0xB2] = bytecode("o6_delayMinutes" );
		a_mOPCodes[0xB3] = bytecode("o6_stopSentence" );
		a_mOPCodes[0xB4] = bytecode("o6_printLine", msg_cmd );
		a_mOPCodes[0xB5] = bytecode("o6_printText", msg_cmd );
		a_mOPCodes[0xB6] = bytecode("o6_printDebug", msg_cmd );
		a_mOPCodes[0xB7] = bytecode("o6_printSystem", msg_cmd );
		a_mOPCodes[0xB8] = bytecode("o6_printActor", msg_cmd );
		a_mOPCodes[0xB9] = bytecode("o6_printEgo", msg_cmd );
		a_mOPCodes[0xBA] = bytecode("o6_talkActor", msg_op );
		a_mOPCodes[0xBB] = bytecode("o6_talkEgo", msg_op );
		a_mOPCodes[0xBC] = bytecode("o6_dimArray", extended_bw_op );
		a_mOPCodes[0xBD] = bytecode("o6_dummy" );
		a_mOPCodes[0xBE] = bytecode("o6_startObjectQuick" );
		a_mOPCodes[0xBF] = bytecode("o6_startScriptQuick2" );
		a_mOPCodes[0xC0] = bytecode("o6_dim2dimArray", extended_bw_op );
		a_mOPCodes[0xC4] = bytecode("o6_abs" );
		a_mOPCodes[0xC5] = bytecode("o6_distObjectObject" );
		// TODO: a_mOPCodes[0xc6] = bytecode("o6_distObjectPt"};
		a_mOPCodes[0xC7] = bytecode("o6_distPtPt" );
		a_mOPCodes[0xC8] = bytecode("o6_kernelGetFunctions" );
		a_mOPCodes[0xC9] = bytecode("o6_kernelSetFunctions" );
		a_mOPCodes[0xCA] = bytecode("o6_delayFrames" );
		a_mOPCodes[0xCB] = bytecode("o6_pickOneOf" );
		a_mOPCodes[0xCC] = bytecode("o6_pickOneOfDefault" );
		a_mOPCodes[0xCD] = bytecode("o6_stampObject" );
		a_mOPCodes[0xD0] = bytecode("o6_getDateTime" );
		a_mOPCodes[0xD1] = bytecode("o6_stopTalking" );
		a_mOPCodes[0xD2] = bytecode("o6_getAnimateVariable" );
		a_mOPCodes[0xD4] = bytecode("o6_shuffle", extended_w_op );
		a_mOPCodes[0xD5] = bytecode("o6_jumpToScript" );
		a_mOPCodes[0xD6] = bytecode("o6_band" );  // bitwise and
		a_mOPCodes[0xD7] = bytecode("o6_bor" );  // bitwise or
		a_mOPCodes[0xD8] = bytecode("o6_isRoomScriptRunning" );
		a_mOPCodes[0xDD] = bytecode("o6_findAllObjects" );
		a_mOPCodes[0xE1] = bytecode("o6_getPixel" );
		a_mOPCodes[0xE3] = bytecode("o6_pickVarRandom", extended_w_op );
		a_mOPCodes[0xE4] = bytecode("o6_setBoxSet", extended_b_op );
		a_mOPCodes[0xEC] = bytecode("o6_getActorLayer" );
		a_mOPCodes[0xED] = bytecode("o6_getObjectNewDir" );
	}

	//---------------------------------------------------------------------
	inline void GetHE60codes(OPCodeMap& a_mOPCodes)
	{
		GetV6codes(a_mOPCodes);

		a_mOPCodes[0x63] = bytecode();
		a_mOPCodes[0x64] = bytecode();
		a_mOPCodes[0x70] = bytecode("o60_setState" );
		a_mOPCodes[0x9A] = bytecode();
		a_mOPCodes[0x9C] = bytecode("o60_roomOps", room_ops_he60 );
		a_mOPCodes[0x9D] = bytecode("o60_actorOps", actor_ops_he60 );
		a_mOPCodes[0xAC] = bytecode();
		a_mOPCodes[0xBD] = bytecode("o6_stopObjectCodeReturn" );
		a_mOPCodes[0xC8] = bytecode("o60_kernelGetFunctions" );
		a_mOPCodes[0xC9] = bytecode("o60_kernelSetFunctions" );
		a_mOPCodes[0xD9] = bytecode("o60_closeFile" );
		a_mOPCodes[0xDA] = bytecode("o60_openFile", msg_op );
		a_mOPCodes[0xDB] = bytecode("o60_readFile" );
		a_mOPCodes[0xDC] = bytecode("o60_writeFile" );
		a_mOPCodes[0xDE] = bytecode("o60_deleteFile", msg_op );
		a_mOPCodes[0xDF] = bytecode("o60_rename", dmsg_op );
		a_mOPCodes[0xE0] = bytecode("o60_soundOps", extended_b_op );
		a_mOPCodes[0xE2] = bytecode("o60_localizeArrayToScript" );
		a_mOPCodes[0xE9] = bytecode("o60_seekFilePos" );
		a_mOPCodes[0xEA] = bytecode("o60_redimArray", extended_bw_op );
		a_mOPCodes[0xEB] = bytecode("o60_readFilePos" );
		a_mOPCodes[0xEC] = bytecode();
		a_mOPCodes[0xED] = bytecode();
	}

	//---------------------------------------------------------------------
	inline void GetHE70codes(OPCodeMap& a_mOPCodes)
	{
		GetHE60codes(a_mOPCodes);

		a_mOPCodes[0x74] = bytecode("o70_soundOps", extended_b_op );
		a_mOPCodes[0x84] = bytecode("o70_pickupObject" );
		a_mOPCodes[0x8C] = bytecode("o70_getActorRoom" );
		a_mOPCodes[0x9B] = bytecode("o70_resourceRoutines", extended_b_op );
		a_mOPCodes[0xAE] = bytecode("o70_systemOps", extended_b_op );
		a_mOPCodes[0xEE] = bytecode("o70_getStringLen" );
		a_mOPCodes[0xF2] = bytecode("o70_isResourceLoaded", extended_b_op );
		a_mOPCodes[0xF3] = bytecode("o70_readINI", msg_op );
		a_mOPCodes[0xF4] = bytecode("o70_writeINI", ini_op_v71 );
		a_mOPCodes[0xF9] = bytecode("o70_createDirectory", msg_op );
		a_mOPCodes[0xFA] = bytecode("o70_setSystemMessage", sys_msg );
	}

	//---------------------------------------------------------------------
	inline void GetHE71codes(OPCodeMap& a_mOPCodes)
	{
		GetHE70codes(a_mOPCodes);

		a_mOPCodes[0xC9] = bytecode("o71_kernelSetFunctions" );
		a_mOPCodes[0xEC] = bytecode("o71_copyString" );
		a_mOPCodes[0xED] = bytecode("o71_getStringWidth" );
		a_mOPCodes[0xEF] = bytecode("o71_appendString" );
		a_mOPCodes[0xF0] = bytecode("o71_concatString" );
		a_mOPCodes[0xF1] = bytecode("o71_compareString" );
		a_mOPCodes[0xF5] = bytecode("o71_getStringLenForWidth" );
		a_mOPCodes[0xF6] = bytecode("o71_getCharIndexInString" );
		a_mOPCodes[0xF7] = bytecode("o71_findBox" );
		a_mOPCodes[0xFB] = bytecode("o71_polygonOps", extended_b_op );
		a_mOPCodes[0xFC] = bytecode("o71_polygonHit" );
	}

	//---------------------------------------------------------------------
	inline void GetHE72codes(OPCodeMap& a_mOPCodes)
	{
		GetHE71codes(a_mOPCodes);

		a_mOPCodes[0x02] = bytecode("o72_pushDWord", extended_dw_op );
		a_mOPCodes[0x04] = bytecode("o72_getScriptString", msg_op );
		a_mOPCodes[0x0A] = bytecode();
		a_mOPCodes[0x1B] = bytecode("o72_isAnyOf" );
		a_mOPCodes[0x42] = bytecode();
		a_mOPCodes[0x46] = bytecode();
		a_mOPCodes[0x4A] = bytecode();
		a_mOPCodes[0x4E] = bytecode();
		a_mOPCodes[0x50] = bytecode("o72_resetCutscene" );
		a_mOPCodes[0x51] = bytecode("o72_getHeap", extended_b_op );
		a_mOPCodes[0x52] = bytecode("o72_findObjectWithClassOf" );
		a_mOPCodes[0x54] = bytecode("o72_getObjectImageX" );
		a_mOPCodes[0x55] = bytecode("o72_getObjectImageY" );
		a_mOPCodes[0x56] = bytecode("o72_captureWizImage" );
		a_mOPCodes[0x58] = bytecode("o72_getTimer", extended_b_op );
		a_mOPCodes[0x59] = bytecode("o72_setTimer", extended_b_op );
		a_mOPCodes[0x5A] = bytecode("o72_getSoundPosition" );
		a_mOPCodes[0x5E] = bytecode("o72_startScript", extended_b_op );
		a_mOPCodes[0x60] = bytecode("o72_startObject", extended_b_op );
		a_mOPCodes[0x61] = bytecode("o72_drawObject", extended_b_op );
		a_mOPCodes[0x62] = bytecode("o72_printWizImage" );
		a_mOPCodes[0x63] = bytecode("o72_getArrayDimSize", extended_bw_op );
		a_mOPCodes[0x64] = bytecode("o72_getNumFreeArrays" );
		a_mOPCodes[0x97] = bytecode();
		a_mOPCodes[0x9C] = bytecode("o72_roomOps", extended_b_op );
		a_mOPCodes[0x9D] = bytecode("o72_actorOps", extended_b_op );
		a_mOPCodes[0x9E] = bytecode("o72_verbOps", extended_b_op );
		// TODO: a_mOPCodes[0xa0] = bytecode("o72_findObject" );
		a_mOPCodes[0xA4] = bytecode("o72_arrayOps", array_ops );
		a_mOPCodes[0xAE] = bytecode("o72_systemOps", extended_b_op );
		a_mOPCodes[0xBA] = bytecode("o72_talkActor", msg_op );
		a_mOPCodes[0xBB] = bytecode("o72_talkEgo", msg_op );
		a_mOPCodes[0xBC] = bytecode("o72_dimArray", extended_bw_op );
		a_mOPCodes[0xC0] = bytecode("o72_dim2dimArray", extended_bw_op );
		a_mOPCodes[0xC1] = bytecode("o72_traceStatus" );
		a_mOPCodes[0xC8] = bytecode("o72_kernelGetFunctions" );
		a_mOPCodes[0xCE] = bytecode("o72_drawWizImage" );
		a_mOPCodes[0xCF] = bytecode("o72_debugInput" );
		a_mOPCodes[0xD5] = bytecode("o72_jumpToScript", extended_b_op );
		a_mOPCodes[0xDA] = bytecode("o72_openFile" );
		a_mOPCodes[0xDB] = bytecode("o72_readFile", file_op );
		a_mOPCodes[0xDC] = bytecode("o72_writeFile", file_op );
		a_mOPCodes[0xDD] = bytecode("o72_findAllObjects" );
		a_mOPCodes[0xDE] = bytecode("o72_deleteFile" );
		a_mOPCodes[0xDF] = bytecode("o72_rename" );
		a_mOPCodes[0xE1] = bytecode("o72_getPixel", extended_b_op );
		// TODO: a_mOPCodes[0xe3] = bytecode("o72_pickVarRandom" );
		a_mOPCodes[0xEA] = bytecode("o72_redimArray", extended_bw_op );
		a_mOPCodes[0xF3] = bytecode("o72_readINI", extended_b_op );
		a_mOPCodes[0xF4] = bytecode("o72_writeINI", extended_b_op );
		a_mOPCodes[0xF8] = bytecode("o72_getResourceSize" );
		a_mOPCodes[0xF9] = bytecode("o72_createDirectory" );
		a_mOPCodes[0xFA] = bytecode("o72_setSystemMessage", extended_b_op );
	}

	//---------------------------------------------------------------------
	inline void GetHE73codes(OPCodeMap& a_mOPCodes)
	{
		GetHE72codes(a_mOPCodes);

		a_mOPCodes[0xF8] = bytecode("o73_getResourceSize", extended_b_op );
	}

	//---------------------------------------------------------------------
	inline void GetHE80codes(OPCodeMap& a_mOPCodes)
	{
		GetHE73codes(a_mOPCodes);

		a_mOPCodes[0x45] = bytecode("o80_createSound", extended_b_op );
		a_mOPCodes[0x46] = bytecode("o80_getFileSize" );
		a_mOPCodes[0x48] = bytecode("o80_stringToInt" );
		a_mOPCodes[0x49] = bytecode("o80_getSoundVar" );
		a_mOPCodes[0x4A] = bytecode("o80_localizeArrayToRoom" );
		// TODO: a_mOPCodes[0x4C] = bytecode("o80_sourceDebug" );
		a_mOPCodes[0x4D] = bytecode("o80_readConfigFile", extended_b_op );
		a_mOPCodes[0x4E] = bytecode("o80_writeConfigFile", extended_b_op );
		a_mOPCodes[0x69] = bytecode();
		a_mOPCodes[0x6B] = bytecode("o80_cursorCommand", extended_b_op );
		a_mOPCodes[0x70] = bytecode("o80_setState" );
		a_mOPCodes[0x76] = bytecode();
		a_mOPCodes[0x94] = bytecode();
		a_mOPCodes[0x9E] = bytecode();
		a_mOPCodes[0xA5] = bytecode();
		a_mOPCodes[0xAC] = bytecode("o80_drawWizPolygon" );
		a_mOPCodes[0xE0] = bytecode("o80_drawLine", extended_b_op );
		a_mOPCodes[0xE3] = bytecode("o80_pickVarRandom", extended_w_op );
	}

	//---------------------------------------------------------------------
	inline void GetHE90codes(OPCodeMap& a_mOPCodes)
	{
		GetHE80codes(a_mOPCodes);

		a_mOPCodes[0x0A] = bytecode("o90_dup_n", extended_w_op );
		a_mOPCodes[0x1C] = bytecode("o90_wizImageOps", extended_b_op );
		a_mOPCodes[0x1D] = bytecode("o90_min" );
		a_mOPCodes[0x1E] = bytecode("o90_max" );
		a_mOPCodes[0x1F] = bytecode("o90_sin" );
		a_mOPCodes[0x20] = bytecode("o90_cos" );
		a_mOPCodes[0x21] = bytecode("o90_sqrt" );
		a_mOPCodes[0x22] = bytecode("o90_atan2" );
		a_mOPCodes[0x23] = bytecode("o90_getSegmentAngle" );
		a_mOPCodes[0x24] = bytecode("o90_getDistanceBetweenPoints", extended_b_op );
		a_mOPCodes[0x25] = bytecode("o90_getSpriteInfo", extended_b_op );
		a_mOPCodes[0x26] = bytecode("o90_setSpriteInfo", extended_b_op );
		a_mOPCodes[0x27] = bytecode("o90_getSpriteGroupInfo", extended_b_op );
		a_mOPCodes[0x28] = bytecode("o90_setSpriteGroupInfo", extended_b_op );
		a_mOPCodes[0x29] = bytecode("o90_getWizData", extended_b_op );
		a_mOPCodes[0x2A] = bytecode("o90_getActorData" );
		a_mOPCodes[0x2B] = bytecode("o90_startScriptUnk", extended_b_op );
		a_mOPCodes[0x2C] = bytecode("o90_jumpToScriptUnk", extended_b_op );
		a_mOPCodes[0x2D] = bytecode("o90_videoOps", extended_b_op );
		a_mOPCodes[0x2E] = bytecode("o90_getVideoData", extended_b_op );
		a_mOPCodes[0x2F] = bytecode("o90_floodFill", extended_b_op );
		a_mOPCodes[0x30] = bytecode("o90_mod" );
		a_mOPCodes[0x31] = bytecode("o90_shl" );
		a_mOPCodes[0x32] = bytecode("o90_shr" );
		a_mOPCodes[0x33] = bytecode("o90_xor" );
		a_mOPCodes[0x34] = bytecode("o90_findAllObjectsWithClassOf" );
		a_mOPCodes[0x35] = bytecode("o90_getPolygonOverlap" );
		a_mOPCodes[0x36] = bytecode("o90_cond" );
		a_mOPCodes[0x37] = bytecode("o90_dim2dim2Array", extended_bw_op );
		a_mOPCodes[0x38] = bytecode("o90_redim2dimArray", extended_bw_op );
		a_mOPCodes[0x39] = bytecode("o90_getLinesIntersectionPoint", extended_ww_op );
		a_mOPCodes[0x3A] = bytecode("o90_sortArray", extended_bw_op );
		a_mOPCodes[0x44] = bytecode("o90_getObjectData", extended_b_op );
		a_mOPCodes[0x69] = bytecode("o90_disabled_windowOps", extended_b_op );
		a_mOPCodes[0x94] = bytecode("o90_getPaletteData", extended_b_op );
		a_mOPCodes[0x9E] = bytecode("o90_paletteOps", extended_b_op );
		a_mOPCodes[0xA5] = bytecode("o90_fontEnum", extended_b_op );
		// TODO: a_mOPCodes[0xab] = bytecode("o90_getActorAnimProgress" );
		// TODO: a_mOPCodes[0xc8] = bytecode("o90_kernelGetFunctions" );
		// TODO: a_mOPCodes[0xc9] = bytecode("o90_kernelSetFunctions" );
	}

	//---------------------------------------------------------------------
	inline void GetHE100codes(OPCodeMap& a_mOPCodes)
	{
		a_mOPCodes[0x00] = bytecode("o100_actorOps", extended_b_op );
		a_mOPCodes[0x01] = bytecode("o6_add" );
		a_mOPCodes[0x02] = bytecode("o6_faceActor" );
		a_mOPCodes[0x03] = bytecode("o90_sortArray", extended_bw_op );
		a_mOPCodes[0x04] = bytecode("o100_arrayOps", array_ops_he100 );
		a_mOPCodes[0x05] = bytecode("o6_band" );
		a_mOPCodes[0x06] = bytecode("o6_bor" );
		a_mOPCodes[0x07] = bytecode("o6_breakHere" );
		a_mOPCodes[0x08] = bytecode("o6_delayFrames" );
		a_mOPCodes[0x09] = bytecode("o90_shl" );
		a_mOPCodes[0x0A] = bytecode("o90_shr" );
		a_mOPCodes[0x0B] = bytecode("o90_xor" );
		a_mOPCodes[0x0C] = bytecode("o6_setCameraAt" );
		a_mOPCodes[0x0D] = bytecode("o6_actorFollowCamera" );
		a_mOPCodes[0x0E] = bytecode("o6_loadRoom" );
		// TODO: a_mOPCodes[0x0f] = bytecode("o6_panCameraTo" );
		// TODO: a_mOPCodes[0x10] = bytecode("o72_captureWizImage" );
		a_mOPCodes[0x11] = bytecode("o100_jumpToScript", extended_b_op );
		a_mOPCodes[0x12] = bytecode("o6_setClass" );
		a_mOPCodes[0x13] = bytecode("o60_closeFile" );
		// TODO: a_mOPCodes[0x14] = bytecode("o6_loadRoomWithEgo" );
		a_mOPCodes[0x16] = bytecode("o72_createDirectory" );
		a_mOPCodes[0x17] = bytecode("o100_createSound", extended_b_op );
		// TODO: a_mOPCodes[0x18] = bytecode("o6_cutscene" );
		a_mOPCodes[0x19] = bytecode("o6_pop" );
		a_mOPCodes[0x1A] = bytecode("o72_traceStatus" );
		a_mOPCodes[0x1B] = bytecode("o6_wordVarDec", extended_w_op );
		a_mOPCodes[0x1C] = bytecode("o6_wordArrayDec", extended_w_op );
		a_mOPCodes[0x1D] = bytecode("o72_deleteFile" );
		a_mOPCodes[0x1E] = bytecode("o100_dim2dimArray", extended_bw_op );
		a_mOPCodes[0x1F] = bytecode("o100_dimArray", extended_bw_op );
		a_mOPCodes[0x20] = bytecode("o6_div" );
		a_mOPCodes[0x21] = bytecode("o6_animateActor" );
		// TODO: a_mOPCodes[0x22] = bytecode("o6_doSentence" );
		a_mOPCodes[0x23] = bytecode("o6_drawBox" );
		// TODO: a_mOPCodes[0x24] = bytecode("o72_drawWizImage" );
		// TODO: a_mOPCodes[0x25] = bytecode("o80_drawWizPolygon" );
		a_mOPCodes[0x26] = bytecode("o100_drawLine", extended_b_op );
		a_mOPCodes[0x27] = bytecode("o100_drawObject", extended_b_op );
		a_mOPCodes[0x28] = bytecode("o6_dup" );
		a_mOPCodes[0x29] = bytecode("o90_dup_n", extended_w_op );
		// TODO: a_mOPCodes[0x2a] = bytecode("o6_endCutscene" );
		a_mOPCodes[0x2B] = bytecode("o6_stopObjectCodeObject" );  // o6_stopObjectCode
		a_mOPCodes[0x2C] = bytecode("o6_stopObjectCodeScript" );  // o6_stopObjectCode
		a_mOPCodes[0x2D] = bytecode("o6_eq" );
		// TODO: a_mOPCodes[0x2e] = bytecode("o100_floodFill" );
		// TODO: a_mOPCodes[0x2f] = bytecode("o6_freezeUnfreeze" );
		a_mOPCodes[0x30] = bytecode("o6_ge" );
		a_mOPCodes[0x31] = bytecode("o6_getDateTime" );
		a_mOPCodes[0x32] = bytecode("o100_setSpriteGroupInfo", extended_b_op );
		a_mOPCodes[0x33] = bytecode("o6_gt" );
		a_mOPCodes[0x34] = bytecode("o100_resourceRoutines", extended_b_op );
		a_mOPCodes[0x35] = bytecode("o6_if", jump_cmd );
		a_mOPCodes[0x36] = bytecode("o6_ifNot", jump_cmd );
		a_mOPCodes[0x37] = bytecode("o100_wizImageOps", extended_b_op );
		a_mOPCodes[0x38] = bytecode("o72_isAnyOf" );
		a_mOPCodes[0x39] = bytecode("o6_wordVarInc", extended_w_op );
		a_mOPCodes[0x3A] = bytecode("o6_wordArrayInc", extended_w_op );
		a_mOPCodes[0x3B] = bytecode("o6_jump", jump_cmd );
		a_mOPCodes[0x3C] = bytecode("o90_kernelSetFunctions" );
		a_mOPCodes[0x3D] = bytecode("o6_land" );
		a_mOPCodes[0x3E] = bytecode("o6_le" );
		a_mOPCodes[0x3F] = bytecode("o60_localizeArrayToScript" );
		a_mOPCodes[0x40] = bytecode("o6_wordArrayRead", extended_w_op );
		a_mOPCodes[0x41] = bytecode("o6_wordArrayIndexedRead", extended_w_op );
		a_mOPCodes[0x42] = bytecode("o6_lor" );
		a_mOPCodes[0x43] = bytecode("o6_lt" );
		a_mOPCodes[0x44] = bytecode("o90_mod" );
		a_mOPCodes[0x45] = bytecode("o6_mul" );
		a_mOPCodes[0x46] = bytecode("o6_neq" );
		a_mOPCodes[0x47] = bytecode("o100_dim2dim2Array", extended_bw_op );
		a_mOPCodes[0x49] = bytecode("o100_redim2dimArray", extended_bw_op );
		a_mOPCodes[0x4A] = bytecode("o6_not" );
		a_mOPCodes[0x4C] = bytecode("o6_beginOverride" );
		a_mOPCodes[0x4D] = bytecode("o6_endOverride" );
		a_mOPCodes[0x4E] = bytecode("o72_resetCutscene" );
		a_mOPCodes[0x4F] = bytecode("o6_setOwner" );
		a_mOPCodes[0x50] = bytecode("o100_paletteOps", extended_b_op );
		a_mOPCodes[0x51] = bytecode("o70_pickupObject" );
		a_mOPCodes[0x52] = bytecode("o100_polygonOps", extended_b_op );  // o71_polygonOps
		a_mOPCodes[0x53] = bytecode("o6_pop" );
		a_mOPCodes[0x54] = bytecode("o100_printDebug", msg_cmd_he100 );  // o6_printDebug
		a_mOPCodes[0x55] = bytecode("o72_printWizImage" );
		a_mOPCodes[0x56] = bytecode("o100_printLine", msg_cmd_he100 );  // o6_printLine
		a_mOPCodes[0x57] = bytecode("o100_printSystem", msg_cmd_he100 );  // o6_printSystem
		a_mOPCodes[0x58] = bytecode("o100_printText", msg_cmd_he100 );  // o6_printText
		// TODO: a_mOPCodes[0x59] = bytecode("o100_jumpToScriptUnk" );
		a_mOPCodes[0x5A] = bytecode("o100_startScriptUnk", extended_b_op );
		// TODO: a_mOPCodes[0x5b] = bytecode("o6_pseudoRoom" );
		a_mOPCodes[0x5C] = bytecode("o6_pushByte", extended_b_op );
		a_mOPCodes[0x5D] = bytecode("o72_pushDWord", extended_dw_op );
		a_mOPCodes[0x5E] = bytecode("o72_getScriptString", msg_op );
		a_mOPCodes[0x5F] = bytecode("o6_pushWord", extended_w_op );
		a_mOPCodes[0x60] = bytecode("o6_pushWordVar", extended_w_op );
		a_mOPCodes[0x61] = bytecode("o6_putActorAtObject" );
		a_mOPCodes[0x62] = bytecode("o6_putActorAtXY" );
		a_mOPCodes[0x64] = bytecode("o100_redimArray", extended_bw_op );
		a_mOPCodes[0x65] = bytecode("o72_rename" );
		a_mOPCodes[0x66] = bytecode("o6_stopObjectCodeReturn" );  // o6_stopObjectCode
		// TODO: a_mOPCodes[0x67] = bytecode("o80_localizeArrayToRoom" );
		a_mOPCodes[0x68] = bytecode("o100_roomOps", extended_b_op );
		a_mOPCodes[0x69] = bytecode("o100_printActor", msg_cmd_he100 );  // o6_printActor
		a_mOPCodes[0x6A] = bytecode("o100_printEgo", msg_cmd_he100 );  // o6_printEgo
		a_mOPCodes[0x6B] = bytecode("o72_talkActor", msg_op );
		a_mOPCodes[0x6C] = bytecode("o72_talkEgo", msg_op );
		a_mOPCodes[0x6E] = bytecode("o60_seekFilePos" );
		a_mOPCodes[0x6F] = bytecode("o6_setBoxFlags" );
		// TODO: a_mOPCodes[0x71] = bytecode("o6_setBoxSet" );
		a_mOPCodes[0x72] = bytecode("o100_setSystemMessage", extended_b_op );
		a_mOPCodes[0x73] = bytecode("o6_shuffle", extended_w_op );
		a_mOPCodes[0x74] = bytecode("o6_delay" );
		// TODO: a_mOPCodes[0x75] = bytecode("o6_delayMinutes" );
		a_mOPCodes[0x76] = bytecode("o6_delaySeconds" );
		a_mOPCodes[0x77] = bytecode("o100_soundOps", extended_b_op );
		a_mOPCodes[0x78] = bytecode("o80_sourceDebug", extended_ddw_op );
		a_mOPCodes[0x79] = bytecode("o100_setSpriteInfo", extended_b_op );
		a_mOPCodes[0x7A] = bytecode("o6_stampObject" );
		a_mOPCodes[0x7B] = bytecode("o72_startObject", extended_b_op );
		a_mOPCodes[0x7C] = bytecode("o100_startScript", extended_b_op );
		// TODO: a_mOPCodes[0x7d] = bytecode("o6_startScriptQuick" );
		a_mOPCodes[0x7E] = bytecode("o80_setState" );
		a_mOPCodes[0x7F] = bytecode("o6_stopObjectScript" );
		a_mOPCodes[0x80] = bytecode("o6_stopScript" );
		a_mOPCodes[0x81] = bytecode("o6_stopSentence" );
		a_mOPCodes[0x82] = bytecode("o6_stopSound" );
		a_mOPCodes[0x83] = bytecode("o6_stopTalking" );
		a_mOPCodes[0x84] = bytecode("o6_writeWordVar", extended_w_op );
		a_mOPCodes[0x85] = bytecode("o6_wordArrayWrite", extended_w_op );
		a_mOPCodes[0x86] = bytecode("o6_wordArrayIndexedWrite", extended_w_op );
		a_mOPCodes[0x87] = bytecode("o6_sub" );
		a_mOPCodes[0x88] = bytecode("o100_systemOps", extended_b_op );
		a_mOPCodes[0x8A] = bytecode("o72_setTimer", extended_b_op );
		a_mOPCodes[0x8B] = bytecode("o100_cursorCommand", extended_b_op );
		a_mOPCodes[0x8C] = bytecode("o100_videoOps", extended_b_op );
		a_mOPCodes[0x8D] = bytecode("o100_wait", wait_ops_he100 );
		// TODO: a_mOPCodes[0x8e] = bytecode("o6_walkActorToObj" );
		a_mOPCodes[0x8F] = bytecode("o6_walkActorTo" );
		a_mOPCodes[0x89] = bytecode("o100_disabled_windowOps", extended_b_op );
		a_mOPCodes[0x90] = bytecode("o100_writeFile", file_op_he100 );
		a_mOPCodes[0x91] = bytecode("o72_writeINI", extended_b_op );
		a_mOPCodes[0x92] = bytecode("o80_writeConfigFile", extended_b_op );
		a_mOPCodes[0x93] = bytecode("o6_abs" );
		// TODO: a_mOPCodes[0x94] = bytecode("o6_getActorWalkBox" );
		a_mOPCodes[0x95] = bytecode("o6_getActorCostume" );
		a_mOPCodes[0x96] = bytecode("o6_getActorElevation" );
		a_mOPCodes[0x97] = bytecode("o6_getObjectOldDir" );
		a_mOPCodes[0x98] = bytecode("o6_getActorMoving" );
		a_mOPCodes[0x99] = bytecode("o90_getActorData" );
		a_mOPCodes[0x9A] = bytecode("o6_getActorRoom" );
		a_mOPCodes[0x9B] = bytecode("o6_getActorScaleX" );
		a_mOPCodes[0x9C] = bytecode("o6_getAnimateVariable" );
		// TODO: a_mOPCodes[0x9d] = bytecode("o6_getActorWidth" );
		a_mOPCodes[0x9E] = bytecode("o6_getObjectX" );
		a_mOPCodes[0x9F] = bytecode("o6_getObjectY" );
		a_mOPCodes[0xA0] = bytecode("o90_atan2" );
		a_mOPCodes[0xA1] = bytecode("o90_getSegmentAngle" );
		// TODO: a_mOPCodes[0xa2] = bytecode("o90_getActorAnimProgress" );
		a_mOPCodes[0xA3] = bytecode("o90_getDistanceBetweenPoints", extended_b_op );
		a_mOPCodes[0xA4] = bytecode("o6_ifClassOfIs" );
		a_mOPCodes[0xA6] = bytecode("o90_cond" );
		a_mOPCodes[0xA7] = bytecode("o90_cos" );
		a_mOPCodes[0xA8] = bytecode("o100_debugInput", extended_b_op );
		a_mOPCodes[0xA9] = bytecode("o80_getFileSize" );
		a_mOPCodes[0xAA] = bytecode("o6_getActorFromXY" );
		a_mOPCodes[0xAB] = bytecode("o72_findAllObjects" );
		a_mOPCodes[0xAC] = bytecode("o90_findAllObjectsWithClassOf" );
		// TODO: a_mOPCodes[0xad] = bytecode("o71_findBox" );
		// TODO: a_mOPCodes[0xae] = bytecode("o6_findInventory" );
		a_mOPCodes[0xAF] = bytecode("o72_findObject" );
		// TODO: a_mOPCodes[0xb0] = bytecode("o72_findObjectWithClassOf" );
		a_mOPCodes[0xB1] = bytecode("o71_polygonHit" );
		// TODO: a_mOPCodes[0xb2] = bytecode("o90_getLinesIntersectionPoint" );
		a_mOPCodes[0xB3] = bytecode("o90_fontEnum", extended_b_op );
		a_mOPCodes[0xB4] = bytecode("o72_getNumFreeArrays" );
		a_mOPCodes[0xB5] = bytecode("o72_getArrayDimSize", extended_bw_op );
		a_mOPCodes[0xB6] = bytecode("o100_isResourceLoaded", extended_b_op );
		a_mOPCodes[0xB7] = bytecode("o100_getResourceSize", extended_b_op );
		a_mOPCodes[0xB8] = bytecode("o100_getSpriteGroupInfo", extended_b_op );
		a_mOPCodes[0xB9] = bytecode("o100_getHeap", extended_b_op );
		a_mOPCodes[0xBA] = bytecode("o100_getWizData", extended_b_op );
		// TODO: a_mOPCodes[0xbb] = bytecode("o6_isActorInBox" );
		a_mOPCodes[0xBC] = bytecode("o6_isAnyOf" );
		// TODO: a_mOPCodes[0xbd] = bytecode("o6_getInventoryCount" );
		a_mOPCodes[0xBE] = bytecode("o90_kernelGetFunctions" );
		a_mOPCodes[0xBF] = bytecode("o90_max" );
		a_mOPCodes[0xC0] = bytecode("o90_min" );
		a_mOPCodes[0xC1] = bytecode("o72_getObjectImageX" );
		a_mOPCodes[0xC2] = bytecode("o72_getObjectImageY" );
		a_mOPCodes[0xC3] = bytecode("o6_isRoomScriptRunning" );
		// TODO: a_mOPCodes[0xc4] = bytecode("o90_getObjectData" );
		a_mOPCodes[0xC5] = bytecode("o72_openFile" );
		a_mOPCodes[0xC6] = bytecode("o90_getPolygonOverlap" );
		a_mOPCodes[0xC7] = bytecode("o6_getOwner" );
		a_mOPCodes[0xC8] = bytecode("o100_getPaletteData", extended_b_op );
		a_mOPCodes[0xC9] = bytecode("o6_pickOneOf" );
		a_mOPCodes[0xCA] = bytecode("o6_pickOneOfDefault" );
		a_mOPCodes[0xCB] = bytecode("o80_pickVarRandom", extended_w_op );
		// TODO: a_mOPCodes[0xcc] = bytecode("o72_getPixel" );
		// TODO: a_mOPCodes[0xcd] = bytecode("o6_distObjectObject" );
		// TODO: a_mOPCodes[0xce] = bytecode("o6_distObjectPt" );
		// TODO: a_mOPCodes[0xcf] = bytecode("o6_distPtPt" );
		a_mOPCodes[0xD0] = bytecode("o6_getRandomNumber" );
		a_mOPCodes[0xD1] = bytecode("o6_getRandomNumberRange" );
		a_mOPCodes[0xD3] = bytecode("o100_readFile", file_op_he100 );
		a_mOPCodes[0xD4] = bytecode("o72_readINI", extended_b_op );
		a_mOPCodes[0xD5] = bytecode("o80_readConfigFile", extended_b_op );
		a_mOPCodes[0xD6] = bytecode("o6_isScriptRunning" );
		a_mOPCodes[0xD7] = bytecode("o90_sin" );
		a_mOPCodes[0xD8] = bytecode("o72_getSoundPosition" );
		a_mOPCodes[0xD9] = bytecode("o6_isSoundRunning" );
		// TODO: a_mOPCodes[0xda] = bytecode("o80_getSoundVar" );
		a_mOPCodes[0xDB] = bytecode("o100_getSpriteInfo", extended_b_op );
		a_mOPCodes[0xDC] = bytecode("o90_sqrt" );
		a_mOPCodes[0xDD] = bytecode("o6_startObjectQuick" );
		a_mOPCodes[0xDE] = bytecode("o6_startScriptQuick2" );
		a_mOPCodes[0xDF] = bytecode("o6_getState" );
		a_mOPCodes[0xE0] = bytecode("o71_compareString" );
		a_mOPCodes[0xE1] = bytecode("o71_copyString" );
		a_mOPCodes[0xE2] = bytecode("o71_appendString" );
		// TODO: a_mOPCodes[0xe3] = bytecode("o71_concatString" );
		a_mOPCodes[0xE4] = bytecode("o70_getStringLen" );
		a_mOPCodes[0xE5] = bytecode("o71_getStringLenForWidth" );
		a_mOPCodes[0xE6] = bytecode("o80_stringToInt" );
		a_mOPCodes[0xE7] = bytecode("o71_getCharIndexInString" );
		a_mOPCodes[0xE8] = bytecode("o71_getStringWidth" );
		a_mOPCodes[0xE9] = bytecode("o60_readFilePos" );
		a_mOPCodes[0xEA] = bytecode("o72_getTimer", extended_b_op );
		a_mOPCodes[0xEB] = bytecode("o6_getVerbEntrypoint" );
		a_mOPCodes[0xEC] = bytecode("o100_getVideoData", extended_b_op );
	}

	//---------------------------------------------------------------------
	inline void GetHE101codes(OPCodeMap& a_mOPCodes)
	{
		GetHE100codes(a_mOPCodes);

		a_mOPCodes[0xA8] = bytecode("o72_debugInput" );
	}

	//---------------------------------------------------------------------
	inline void GetV8codes(OPCodeMap& a_mOPCodes)
	{
		a_mOPCodes[0x01] = bytecode("o6_pushWord", extended_dw_op );
		a_mOPCodes[0x02] = bytecode("o6_pushWordVar", extended_dw_op );
		a_mOPCodes[0x03] = bytecode("o6_wordArrayRead", extended_dw_op );
		a_mOPCodes[0x04] = bytecode("o6_wordArrayIndexedRead", extended_dw_op );
		a_mOPCodes[0x05] = bytecode("o6_dup" );
		a_mOPCodes[0x06] = bytecode("o6_pop" );
		a_mOPCodes[0x07] = bytecode("o6_not" );
		a_mOPCodes[0x08] = bytecode("o6_eq" );
		a_mOPCodes[0x09] = bytecode("o6_neq" );
		a_mOPCodes[0x0A] = bytecode("o6_gt" );
		a_mOPCodes[0x0B] = bytecode("o6_lt" );
		a_mOPCodes[0x0C] = bytecode("o6_le" );
		a_mOPCodes[0x0D] = bytecode("o6_ge" );
		a_mOPCodes[0x0E] = bytecode("o6_add" );
		a_mOPCodes[0x0F] = bytecode("o6_sub" );
		a_mOPCodes[0x10] = bytecode("o6_mul" );
		a_mOPCodes[0x11] = bytecode("o6_div" );
		a_mOPCodes[0x12] = bytecode("o6_land" );
		a_mOPCodes[0x13] = bytecode("o6_lor" );
		a_mOPCodes[0x14] = bytecode("o6_band" );
		a_mOPCodes[0x15] = bytecode("o6_bor" );
		a_mOPCodes[0x16] = bytecode("o8_mod" );
		a_mOPCodes[0x64] = bytecode("o6_if" );
		a_mOPCodes[0x65] = bytecode("o6_ifNot", djump_cmd );
		a_mOPCodes[0x66] = bytecode("o6_jump", djump_cmd );
		a_mOPCodes[0x67] = bytecode("o6_breakHere" );
		a_mOPCodes[0x68] = bytecode("o6_delayFrames" );
		a_mOPCodes[0x69] = bytecode("o8_wait", wait_ops_v8 );
		a_mOPCodes[0x6A] = bytecode("o6_delay" );
		a_mOPCodes[0x6B] = bytecode("o6_delaySeconds" );
		a_mOPCodes[0x6C] = bytecode("o6_delayMinutes" );
		a_mOPCodes[0x6D] = bytecode("o6_writeWordVar", extended_dw_op );
		a_mOPCodes[0x6E] = bytecode("o6_wordVarInc", extended_dw_op );
		a_mOPCodes[0x6F] = bytecode("o6_wordVarDec", extended_dw_op );
		a_mOPCodes[0x70] = bytecode("o8_dimArray", extended_bdw_op );
		a_mOPCodes[0x71] = bytecode("o6_wordArrayWrite", extended_dw_op );
		a_mOPCodes[0x72] = bytecode("o6_wordArrayInc", extended_dw_op );
		a_mOPCodes[0x73] = bytecode("o6_wordArrayDec", extended_dw_op );
		a_mOPCodes[0x74] = bytecode("o8_dim2dimArray", extended_bdw_op );
		a_mOPCodes[0x75] = bytecode("o6_wordArrayIndexedWrite", extended_dw_op );
		a_mOPCodes[0x76] = bytecode("o8_arrayOps", array_ops_v8 );
		a_mOPCodes[0x79] = bytecode("o6_startScript" );
		a_mOPCodes[0x7A] = bytecode("o6_startScriptQuick" );
		a_mOPCodes[0x7B] = bytecode("o6_stopObjectCodeScript" );  // o6_stopObjectCode
		a_mOPCodes[0x7C] = bytecode("o6_stopScript" );
		a_mOPCodes[0x7D] = bytecode("o6_jumpToScript" );
		a_mOPCodes[0x7E] = bytecode("o6_dummy" );
		a_mOPCodes[0x7F] = bytecode("o6_startObject" );
		a_mOPCodes[0x80] = bytecode("o6_stopObjectScript" );
		a_mOPCodes[0x81] = bytecode("o6_cutscene" );
		a_mOPCodes[0x82] = bytecode("o6_endCutscene" );
		a_mOPCodes[0x83] = bytecode("o6_freezeUnfreeze" );
		a_mOPCodes[0x84] = bytecode("o6_beginOverride" );
		a_mOPCodes[0x85] = bytecode("o6_endOverride" );
		a_mOPCodes[0x86] = bytecode("o6_stopSentence" );
		a_mOPCodes[0x87] = bytecode("o8_debug" );
		a_mOPCodes[0x89] = bytecode("o6_setClass" );
		a_mOPCodes[0x8A] = bytecode("o6_setState" );
		a_mOPCodes[0x8B] = bytecode("o6_setOwner" );
		a_mOPCodes[0x8C] = bytecode("o6_panCameraTo" );
		a_mOPCodes[0x8D] = bytecode("o6_actorFollowCamera" );
		a_mOPCodes[0x8E] = bytecode("o6_setCameraAt" );
		a_mOPCodes[0x8F] = bytecode("o8_printActor", msg_cmd_v8 );  // o6_printActor
		a_mOPCodes[0x90] = bytecode("o8_printEgo", msg_cmd_v8 );  // o6_printEgo
		a_mOPCodes[0x91] = bytecode("o8_talkActor", msg_op_v8 );  // o6_talkActor
		a_mOPCodes[0x92] = bytecode("o8_talkEgo", msg_op_v8 );  // o6_talkEgo
		a_mOPCodes[0x93] = bytecode("o8_printLine", msg_cmd_v8 );  // o6_printLine
		a_mOPCodes[0x94] = bytecode("o8_printText", msg_cmd_v8 );  // o6_printText
		a_mOPCodes[0x95] = bytecode("o8_printDebug", msg_cmd_v8 );  // o6_printDebug
		a_mOPCodes[0x96] = bytecode("o8_printSystem", msg_cmd_v8 );  // o6_printSystem
		a_mOPCodes[0x97] = bytecode("o8_blastText", msg_cmd_v8 );
		a_mOPCodes[0x98] = bytecode("o8_drawObject" );
		a_mOPCodes[0x9C] = bytecode("o8_cursorCommand", extended_b_op );
		a_mOPCodes[0x9D] = bytecode("o6_loadRoom" );
		a_mOPCodes[0x9E] = bytecode("o6_loadRoomWithEgo" );
		a_mOPCodes[0x9F] = bytecode("o6_walkActorToObj" );
		a_mOPCodes[0xA0] = bytecode("o6_walkActorTo" );
		a_mOPCodes[0xA1] = bytecode("o6_putActorAtXY" );
		a_mOPCodes[0xA2] = bytecode("o6_putActorAtObject" );
		a_mOPCodes[0xA3] = bytecode("o6_faceActor" );
		a_mOPCodes[0xA4] = bytecode("o6_animateActor" );
		a_mOPCodes[0xA5] = bytecode("o8_doSentence" );  // o6_doSentence
		a_mOPCodes[0xA6] = bytecode("o6_pickupObject" );
		a_mOPCodes[0xA7] = bytecode("o6_setBoxFlags" );
		a_mOPCodes[0xA8] = bytecode("o6_createBoxMatrix" );
		a_mOPCodes[0xAA] = bytecode("o8_resourceRoutines", extended_b_op );
		a_mOPCodes[0xAB] = bytecode("o8_roomOps", extended_b_op );
		a_mOPCodes[0xAC] = bytecode("o8_actorOps", actor_ops_v8 );
		a_mOPCodes[0xAD] = bytecode("o8_cameraOps", extended_b_op );
		a_mOPCodes[0xAE] = bytecode("o8_verbOps", verb_ops_v8 );
		a_mOPCodes[0xAF] = bytecode("o6_startSound" );
		a_mOPCodes[0xB0] = bytecode("o6_startMusic" );
		a_mOPCodes[0xB1] = bytecode("o6_stopSound" );
		a_mOPCodes[0xB2] = bytecode("o6_soundKludge" );
		a_mOPCodes[0xB3] = bytecode("o8_systemOps", extended_b_op );
		a_mOPCodes[0xB4] = bytecode("o6_saveRestoreVerbs", extended_b_op );
		a_mOPCodes[0xB5] = bytecode("o6_setObjectName", msg_op_v8 );
		a_mOPCodes[0xB6] = bytecode("o6_getDateTime" );
		a_mOPCodes[0xB7] = bytecode("o6_drawBox" );
		a_mOPCodes[0xB9] = bytecode("o8_startVideo", msg_op_v8 );
		a_mOPCodes[0xBA] = bytecode("o8_kernelSetFunctions" );
		a_mOPCodes[0xC8] = bytecode("o6_startScriptQuick2" );
		a_mOPCodes[0xC9] = bytecode("o6_startObjectQuick" );
		a_mOPCodes[0xCA] = bytecode("o6_pickOneOf" );
		a_mOPCodes[0xCB] = bytecode("o6_pickOneOfDefault" );
		a_mOPCodes[0xCD] = bytecode("o6_isAnyOf" );
		a_mOPCodes[0xCE] = bytecode("o6_getRandomNumber" );
		a_mOPCodes[0xCF] = bytecode("o6_getRandomNumberRange" );
		a_mOPCodes[0xD0] = bytecode("o6_ifClassOfIs" );
		a_mOPCodes[0xD1] = bytecode("o6_getState" );
		a_mOPCodes[0xD2] = bytecode("o6_getOwner" );
		a_mOPCodes[0xD3] = bytecode("o6_isScriptRunning" );
		a_mOPCodes[0xD5] = bytecode("o6_isSoundRunning" );
		a_mOPCodes[0xD6] = bytecode("o6_abs" );
		a_mOPCodes[0xD8] = bytecode("o8_kernelGetFunctions" );
		a_mOPCodes[0xD9] = bytecode("o6_isActorInBox" );
		a_mOPCodes[0xDA] = bytecode("o6_getVerbEntrypoint" );
		a_mOPCodes[0xDB] = bytecode("o6_getActorFromXY" );
		a_mOPCodes[0xDC] = bytecode("o6_findObject" );
		a_mOPCodes[0xDD] = bytecode("o6_getVerbFromXY" );
		a_mOPCodes[0xDF] = bytecode("o6_findInventory" );
		a_mOPCodes[0xE0] = bytecode("o6_getInventoryCount" );
		a_mOPCodes[0xE1] = bytecode("o6_getAnimateVariable" );
		a_mOPCodes[0xE2] = bytecode("o6_getActorRoom" );
		a_mOPCodes[0xE3] = bytecode("o6_getActorWalkBox" );
		a_mOPCodes[0xE4] = bytecode("o6_getActorMoving" );
		a_mOPCodes[0xE5] = bytecode("o6_getActorCostume" );
		a_mOPCodes[0xE6] = bytecode("o6_getActorScaleX" );
		a_mOPCodes[0xE7] = bytecode("o6_getActorLayer" );
		a_mOPCodes[0xE8] = bytecode("o6_getActorElevation" );
		a_mOPCodes[0xE9] = bytecode("o6_getActorWidth" );
		a_mOPCodes[0xEA] = bytecode("o6_getObjectNewDir" );
		a_mOPCodes[0xEB] = bytecode("o6_getObjectX" );
		a_mOPCodes[0xEC] = bytecode("o6_getObjectY" );
		a_mOPCodes[0xED] = bytecode("o8_getActorChore" );
		a_mOPCodes[0xEE] = bytecode("o6_distObjectObject" );
		a_mOPCodes[0xEF] = bytecode("o6_distPtPt" );
		a_mOPCodes[0xF0] = bytecode("o8_getObjectImageX" );
		a_mOPCodes[0xF1] = bytecode("o8_getObjectImageY" );
		a_mOPCodes[0xF2] = bytecode("o8_getObjectImageWidth" );
		a_mOPCodes[0xF3] = bytecode("o8_getObjectImageHeight" );
		a_mOPCodes[0xF6] = bytecode("o8_getStringWidth", msg_op_v8 );
		a_mOPCodes[0xF7] = bytecode("o8_getActorZPlane" );
	}

	//---------------------------------------------------------------------
	inline void GetOPCodeTable(OPCodeMap& a_mMap, int version, int heVersion)
	{
		if (heVersion >= 101)
		{
			GetHE101codes(a_mMap);
		}
		else if (heVersion >= 100)
		{
			GetHE100codes(a_mMap);
		}
		else if (heVersion >= 90)
		{
			GetHE90codes(a_mMap);
		}
		else if (heVersion >= 80)
		{
			GetHE80codes(a_mMap);
		}
		else if (heVersion >= 73)
		{
			GetHE73codes(a_mMap);
		}
		else if (heVersion >= 72)
		{
			GetHE72codes(a_mMap);
		}
		else if (heVersion >= 71)
		{
			GetHE71codes(a_mMap);
		}
		else if (heVersion >= 70)
		{
			GetHE70codes(a_mMap);
		}
		else if (heVersion >= 60)
		{
			GetHE60codes(a_mMap);
		}
		else if (version >= 8)
		{
			GetV8codes(a_mMap);
		}
		else if (version >= 6)
		{
			GetV6codes(a_mMap);
		}
		else
		{
			GetV6codes(a_mMap);
		}
	}
}