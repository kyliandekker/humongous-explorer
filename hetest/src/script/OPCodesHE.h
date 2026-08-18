#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "script/ScrInstruction.h"

namespace humongousexplorer::script
{
	std::unique_ptr<ScrInstruction> default_func(uint8_t a_iByte, const unsigned char* a_pData);
	struct bytecode
	{
		std::string m_sName;
		std::function<std::unique_ptr<ScrInstruction>(uint8_t, const unsigned char*)> m_fnSize = default_func;
	};

	std::unique_ptr<ScrInstruction> extended_b_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> extended_w_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> extended_ww_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> extended_dw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> extended_ddw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> extended_bw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> extended_bdw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> jump_cmd(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> djump_cmd(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> msg_cmd(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> msg_cmd_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> msg_cmd_he100(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> msg_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> msg_op_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> actor_ops_v6(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> actor_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> actor_ops_he60(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> verb_ops_v6(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> verb_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> array_ops_v6(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> array_ops(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> array_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> array_ops_he100(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> wait_ops(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> wait_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> wait_ops_he100(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> room_ops_he60(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> dmsg_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> sys_msg(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> ini_op_v71(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> file_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::unique_ptr<ScrInstruction> file_op_he100(uint8_t a_iByte, const unsigned char* a_pData);

	// ============================================================================
// V6
// ============================================================================
	using OPCodeMap = std::unordered_map<uint8_t, bytecode>;

	//---------------------------------------------------------------------
	inline void GetV6codes(OPCodeMap& a_mOPCodes)
	{
		a_mOPCodes[0x00] = { "o6_pushByte", extended_b_op };
		a_mOPCodes[0x01] = { "o6_pushWord", extended_w_op };
		a_mOPCodes[0x02] = { "o6_pushByteVar", extended_b_op };
		a_mOPCodes[0x03] = { "o6_pushWordVar", extended_w_op };
		// TODO: a_mOPCodes[0x06] = { "o6_byteArrayRead"};
		a_mOPCodes[0x07] = { "o6_wordArrayRead", extended_w_op };
		a_mOPCodes[0x0A] = { "o6_byteArrayIndexedRead", extended_b_op };
		a_mOPCodes[0x0B] = { "o6_wordArrayIndexedRead", extended_w_op };
		a_mOPCodes[0x0C] = { "o6_dup" };
		a_mOPCodes[0x0D] = { "o6_not" };
		a_mOPCodes[0x0E] = { "o6_eq" };
		a_mOPCodes[0x0F] = { "o6_neq" };
		a_mOPCodes[0x10] = { "o6_gt" };
		a_mOPCodes[0x11] = { "o6_lt" };
		a_mOPCodes[0x12] = { "o6_le" };
		a_mOPCodes[0x13] = { "o6_ge" };
		a_mOPCodes[0x14] = { "o6_add" };
		a_mOPCodes[0x15] = { "o6_sub" };
		a_mOPCodes[0x16] = { "o6_mul" };
		a_mOPCodes[0x17] = { "o6_div" };
		a_mOPCodes[0x18] = { "o6_land" };  // logical and
		a_mOPCodes[0x19] = { "o6_lor" };  // logical or
		a_mOPCodes[0x1A] = { "o6_pop" };
		// TODO: a_mOPCodes[0x42] = { "o6_writeByteVar"};
		a_mOPCodes[0x43] = { "o6_writeWordVar", extended_w_op };
		// TODO: a_mOPCodes[0x46] = { "o6_byteArrayWrite"};
		a_mOPCodes[0x47] = { "o6_wordArrayWrite", extended_w_op };
		// TODO: a_mOPCodes[0x4a] = { "o6_byteArrayIndexedWrite"};
		a_mOPCodes[0x4B] = { "o6_wordArrayIndexedWrite", extended_w_op };
		// TODO: a_mOPCodes[0x4e] = { "o6_byteVarInc"};
		a_mOPCodes[0x4F] = { "o6_wordVarInc", extended_w_op };
		// TODO: a_mOPCodes[0x52] = { "o6_byteArrayInc"};
		a_mOPCodes[0x53] = { "o6_wordArrayInc", extended_w_op };
		// TODO: a_mOPCodes[0x56] = { "o6_byteVarDec"};
		a_mOPCodes[0x57] = { "o6_wordVarDec", extended_w_op };
		// TODO: a_mOPCodes[0x5a] = { "o6_byteArrayDec"};
		a_mOPCodes[0x5B] = { "o6_wordArrayDec", extended_w_op };
		a_mOPCodes[0x5C] = { "o6_if", jump_cmd };  // jump if
		a_mOPCodes[0x5D] = { "o6_ifNot", jump_cmd };  // jump if not
		a_mOPCodes[0x5E] = { "o6_startScript" };
		a_mOPCodes[0x5F] = { "o6_startScriptQuick" };
		a_mOPCodes[0x60] = { "o6_startObject" };
		a_mOPCodes[0x61] = { "o6_drawObject" };
		a_mOPCodes[0x62] = { "o6_drawObjectAt" };
		a_mOPCodes[0x63] = { "o6_drawBlastObject" };
		a_mOPCodes[0x64] = { "o6_setBlastObjectWindow" };
		a_mOPCodes[0x65] = { "o6_stopObjectCodeObject" };  // o6_stopObjectCode
		a_mOPCodes[0x66] = { "o6_stopObjectCodeScript" };  // o6_stopObjectCode
		a_mOPCodes[0x67] = { "o6_endCutscene" };
		a_mOPCodes[0x68] = { "o6_cutscene" };
		// TODO: a_mOPCodes[0x69] = { "o6_stopMusic"};
		a_mOPCodes[0x6A] = { "o6_freezeUnfreeze" };
		a_mOPCodes[0x6B] = { "o6_cursorCommand", extended_b_op };
		a_mOPCodes[0x6C] = { "o6_breakHere" };
		a_mOPCodes[0x6D] = { "o6_ifClassOfIs" };
		a_mOPCodes[0x6E] = { "o6_setClass" };
		a_mOPCodes[0x6F] = { "o6_getState" };
		a_mOPCodes[0x70] = { "o6_setState" };
		a_mOPCodes[0x71] = { "o6_setOwner" };
		a_mOPCodes[0x72] = { "o6_getOwner" };
		a_mOPCodes[0x73] = { "o6_jump", jump_cmd };
		a_mOPCodes[0x74] = { "o6_startSound" };
		a_mOPCodes[0x75] = { "o6_stopSound" };
		// TODO: a_mOPCodes[0x76] = { "o6_startMusic"};
		a_mOPCodes[0x77] = { "o6_stopObjectScript" };
		a_mOPCodes[0x78] = { "o6_panCameraTo" };
		a_mOPCodes[0x79] = { "o6_actorFollowCamera" };
		a_mOPCodes[0x7A] = { "o6_setCameraAt" };
		a_mOPCodes[0x7B] = { "o6_loadRoom" };
		a_mOPCodes[0x7C] = { "o6_stopScript" };
		a_mOPCodes[0x7D] = { "o6_walkActorToObj" };
		a_mOPCodes[0x7E] = { "o6_walkActorTo" };
		a_mOPCodes[0x7F] = { "o6_putActorAtXY" };
		a_mOPCodes[0x80] = { "o6_putActorAtObject" };
		a_mOPCodes[0x81] = { "o6_faceActor" };
		a_mOPCodes[0x82] = { "o6_animateActor" };
		a_mOPCodes[0x83] = { "o6_doSentence" };
		a_mOPCodes[0x84] = { "o6_pickupObject" };
		a_mOPCodes[0x85] = { "o6_loadRoomWithEgo" };
		a_mOPCodes[0x87] = { "o6_getRandomNumber" };
		a_mOPCodes[0x88] = { "o6_getRandomNumberRange" };
		a_mOPCodes[0x8A] = { "o6_getActorMoving" };
		a_mOPCodes[0x8B] = { "o6_isScriptRunning" };
		a_mOPCodes[0x8C] = { "o6_getActorRoom" };
		a_mOPCodes[0x8D] = { "o6_getObjectX" };
		a_mOPCodes[0x8E] = { "o6_getObjectY" };
		a_mOPCodes[0x8F] = { "o6_getObjectOldDir" };
		a_mOPCodes[0x90] = { "o6_getActorWalkBox" };
		a_mOPCodes[0x91] = { "o6_getActorCostume" };
		a_mOPCodes[0x92] = { "o6_findInventory" };
		a_mOPCodes[0x93] = { "o6_getInventoryCount" };
		a_mOPCodes[0x94] = { "o6_getVerbFromXY" };
		a_mOPCodes[0x95] = { "o6_beginOverride" };
		a_mOPCodes[0x96] = { "o6_endOverride" };
		a_mOPCodes[0x97] = { "o6_setObjectName", msg_op };
		a_mOPCodes[0x98] = { "o6_isSoundRunning" };
		a_mOPCodes[0x99] = { "o6_setBoxFlags" };
		a_mOPCodes[0x9A] = { "o6_createBoxMatrix" };
		a_mOPCodes[0x9B] = { "o6_resourceRoutines", extended_b_op };
		a_mOPCodes[0x9C] = { "o6_roomOps", extended_b_op };
		a_mOPCodes[0x9D] = { "o6_actorOps", actor_ops_v6 };
		a_mOPCodes[0x9E] = { "o6_verbOps", verb_ops_v6 };
		a_mOPCodes[0x9F] = { "o6_getActorFromXY" };
		a_mOPCodes[0xA0] = { "o6_findObject" };
		a_mOPCodes[0xA1] = { "o6_pseudoRoom" };
		a_mOPCodes[0xA2] = { "o6_getActorElevation" };
		a_mOPCodes[0xA3] = { "o6_getVerbEntrypoint" };
		a_mOPCodes[0xA4] = { "o6_arrayOps", array_ops_v6 };
		a_mOPCodes[0xA5] = { "o6_saveRestoreVerbs", extended_b_op };
		a_mOPCodes[0xA6] = { "o6_drawBox" };
		a_mOPCodes[0xA7] = { "o6_pop" };
		a_mOPCodes[0xA8] = { "o6_getActorWidth" };
		a_mOPCodes[0xA9] = { "o6_wait", wait_ops };
		a_mOPCodes[0xAA] = { "o6_getActorScaleX" };
		a_mOPCodes[0xAB] = { "o6_getActorAnimCounter" };
		a_mOPCodes[0xAC] = { "o6_soundKludge" };
		a_mOPCodes[0xAD] = { "o6_isAnyOf" };
		a_mOPCodes[0xAE] = { "o6_systemOps", extended_b_op };
		a_mOPCodes[0xAF] = { "o6_isActorInBox" };
		a_mOPCodes[0xB0] = { "o6_delay" };
		a_mOPCodes[0xB1] = { "o6_delaySeconds" };
		a_mOPCodes[0xB2] = { "o6_delayMinutes" };
		a_mOPCodes[0xB3] = { "o6_stopSentence" };
		a_mOPCodes[0xB4] = { "o6_printLine", msg_cmd };
		a_mOPCodes[0xB5] = { "o6_printText", msg_cmd };
		a_mOPCodes[0xB6] = { "o6_printDebug", msg_cmd };
		a_mOPCodes[0xB7] = { "o6_printSystem", msg_cmd };
		a_mOPCodes[0xB8] = { "o6_printActor", msg_cmd };
		a_mOPCodes[0xB9] = { "o6_printEgo", msg_cmd };
		a_mOPCodes[0xBA] = { "o6_talkActor", msg_op };
		a_mOPCodes[0xBB] = { "o6_talkEgo", msg_op };
		a_mOPCodes[0xBC] = { "o6_dimArray", extended_bw_op };
		a_mOPCodes[0xBD] = { "o6_dummy" };
		a_mOPCodes[0xBE] = { "o6_startObjectQuick" };
		a_mOPCodes[0xBF] = { "o6_startScriptQuick2" };
		a_mOPCodes[0xC0] = { "o6_dim2dimArray", extended_bw_op };
		a_mOPCodes[0xC4] = { "o6_abs" };
		a_mOPCodes[0xC5] = { "o6_distObjectObject" };
		// TODO: a_mOPCodes[0xc6] = { "o6_distObjectPt"};
		a_mOPCodes[0xC7] = { "o6_distPtPt" };
		a_mOPCodes[0xC8] = { "o6_kernelGetFunctions" };
		a_mOPCodes[0xC9] = { "o6_kernelSetFunctions" };
		a_mOPCodes[0xCA] = { "o6_delayFrames" };
		a_mOPCodes[0xCB] = { "o6_pickOneOf" };
		a_mOPCodes[0xCC] = { "o6_pickOneOfDefault" };
		a_mOPCodes[0xCD] = { "o6_stampObject" };
		a_mOPCodes[0xD0] = { "o6_getDateTime" };
		a_mOPCodes[0xD1] = { "o6_stopTalking" };
		a_mOPCodes[0xD2] = { "o6_getAnimateVariable" };
		a_mOPCodes[0xD4] = { "o6_shuffle", extended_w_op };
		a_mOPCodes[0xD5] = { "o6_jumpToScript" };
		a_mOPCodes[0xD6] = { "o6_band" };  // bitwise and
		a_mOPCodes[0xD7] = { "o6_bor" };  // bitwise or
		a_mOPCodes[0xD8] = { "o6_isRoomScriptRunning" };
		a_mOPCodes[0xDD] = { "o6_findAllObjects" };
		a_mOPCodes[0xE1] = { "o6_getPixel" };
		a_mOPCodes[0xE3] = { "o6_pickVarRandom", extended_w_op };
		a_mOPCodes[0xE4] = { "o6_setBoxSet", extended_b_op };
		a_mOPCodes[0xEC] = { "o6_getActorLayer" };
		a_mOPCodes[0xED] = { "o6_getObjectNewDir" };
	}

	//---------------------------------------------------------------------
	inline void GetHE60codes(OPCodeMap& a_mOPCodes)
	{
		GetV6codes(a_mOPCodes);

		a_mOPCodes[0x63] = { };
		a_mOPCodes[0x64] = { };
		a_mOPCodes[0x70] = { "o60_setState" };
		a_mOPCodes[0x9A] = { };
		a_mOPCodes[0x9C] = { "o60_roomOps", room_ops_he60 };
		a_mOPCodes[0x9D] = { "o60_actorOps", actor_ops_he60 };
		a_mOPCodes[0xAC] = { };
		a_mOPCodes[0xBD] = { "o6_stopObjectCodeReturn" };
		a_mOPCodes[0xC8] = { "o60_kernelGetFunctions" };
		a_mOPCodes[0xC9] = { "o60_kernelSetFunctions" };
		a_mOPCodes[0xD9] = { "o60_closeFile" };
		a_mOPCodes[0xDA] = { "o60_openFile", msg_op };
		a_mOPCodes[0xDB] = { "o60_readFile" };
		a_mOPCodes[0xDC] = { "o60_writeFile" };
		a_mOPCodes[0xDE] = { "o60_deleteFile", msg_op };
		a_mOPCodes[0xDF] = { "o60_rename", dmsg_op };
		a_mOPCodes[0xE0] = { "o60_soundOps", extended_b_op };
		a_mOPCodes[0xE2] = { "o60_localizeArrayToScript" };
		a_mOPCodes[0xE9] = { "o60_seekFilePos" };
		a_mOPCodes[0xEA] = { "o60_redimArray", extended_bw_op };
		a_mOPCodes[0xEB] = { "o60_readFilePos" };
		a_mOPCodes[0xEC] = { };
		a_mOPCodes[0xED] = { };
	}

	//---------------------------------------------------------------------
	inline void GetHE70codes(OPCodeMap& a_mOPCodes)
	{
		GetHE60codes(a_mOPCodes);

		a_mOPCodes[0x74] = { "o70_soundOps", extended_b_op };
		a_mOPCodes[0x84] = { "o70_pickupObject" };
		a_mOPCodes[0x8C] = { "o70_getActorRoom" };
		a_mOPCodes[0x9B] = { "o70_resourceRoutines", extended_b_op };
		a_mOPCodes[0xAE] = { "o70_systemOps", extended_b_op };
		a_mOPCodes[0xEE] = { "o70_getStringLen" };
		a_mOPCodes[0xF2] = { "o70_isResourceLoaded", extended_b_op };
		a_mOPCodes[0xF3] = { "o70_readINI", msg_op };
		a_mOPCodes[0xF4] = { "o70_writeINI", ini_op_v71 };
		a_mOPCodes[0xF9] = { "o70_createDirectory", msg_op };
		a_mOPCodes[0xFA] = { "o70_setSystemMessage", sys_msg };
	}

	//---------------------------------------------------------------------
	inline void GetHE71codes(OPCodeMap& a_mOPCodes)
	{
		GetHE70codes(a_mOPCodes);

		a_mOPCodes[0xC9] = { "o71_kernelSetFunctions" };
		a_mOPCodes[0xEC] = { "o71_copyString" };
		a_mOPCodes[0xED] = { "o71_getStringWidth" };
		a_mOPCodes[0xEF] = { "o71_appendString" };
		a_mOPCodes[0xF0] = { "o71_concatString" };
		a_mOPCodes[0xF1] = { "o71_compareString" };
		a_mOPCodes[0xF5] = { "o71_getStringLenForWidth" };
		a_mOPCodes[0xF6] = { "o71_getCharIndexInString" };
		a_mOPCodes[0xF7] = { "o71_findBox" };
		a_mOPCodes[0xFB] = { "o71_polygonOps", extended_b_op };
		a_mOPCodes[0xFC] = { "o71_polygonHit" };
	}

	//---------------------------------------------------------------------
	inline void GetHE72codes(OPCodeMap& a_mOPCodes)
	{
		GetHE71codes(a_mOPCodes);

		a_mOPCodes[0x02] = { "o72_pushDWord", extended_dw_op };
		a_mOPCodes[0x04] = { "o72_getScriptString", msg_op };
		a_mOPCodes[0x0A] = { };
		a_mOPCodes[0x1B] = { "o72_isAnyOf" };
		a_mOPCodes[0x42] = { };
		a_mOPCodes[0x46] = { };
		a_mOPCodes[0x4A] = { };
		a_mOPCodes[0x4E] = { };
		a_mOPCodes[0x50] = { "o72_resetCutscene" };
		a_mOPCodes[0x51] = { "o72_getHeap", extended_b_op };
		a_mOPCodes[0x52] = { "o72_findObjectWithClassOf" };
		a_mOPCodes[0x54] = { "o72_getObjectImageX" };
		a_mOPCodes[0x55] = { "o72_getObjectImageY" };
		a_mOPCodes[0x56] = { "o72_captureWizImage" };
		a_mOPCodes[0x58] = { "o72_getTimer", extended_b_op };
		a_mOPCodes[0x59] = { "o72_setTimer", extended_b_op };
		a_mOPCodes[0x5A] = { "o72_getSoundPosition" };
		a_mOPCodes[0x5E] = { "o72_startScript", extended_b_op };
		a_mOPCodes[0x60] = { "o72_startObject", extended_b_op };
		a_mOPCodes[0x61] = { "o72_drawObject", extended_b_op };
		a_mOPCodes[0x62] = { "o72_printWizImage" };
		a_mOPCodes[0x63] = { "o72_getArrayDimSize", extended_bw_op };
		a_mOPCodes[0x64] = { "o72_getNumFreeArrays" };
		a_mOPCodes[0x97] = { };
		a_mOPCodes[0x9C] = { "o72_roomOps", extended_b_op };
		a_mOPCodes[0x9D] = { "o72_actorOps", extended_b_op };
		a_mOPCodes[0x9E] = { "o72_verbOps", extended_b_op };
		// TODO: a_mOPCodes[0xa0] = { "o72_findObject" };
		a_mOPCodes[0xA4] = { "o72_arrayOps", array_ops };
		a_mOPCodes[0xAE] = { "o72_systemOps", extended_b_op };
		a_mOPCodes[0xBA] = { "o72_talkActor", msg_op };
		a_mOPCodes[0xBB] = { "o72_talkEgo", msg_op };
		a_mOPCodes[0xBC] = { "o72_dimArray", extended_bw_op };
		a_mOPCodes[0xC0] = { "o72_dim2dimArray", extended_bw_op };
		a_mOPCodes[0xC1] = { "o72_traceStatus" };
		a_mOPCodes[0xC8] = { "o72_kernelGetFunctions" };
		a_mOPCodes[0xCE] = { "o72_drawWizImage" };
		a_mOPCodes[0xCF] = { "o72_debugInput" };
		a_mOPCodes[0xD5] = { "o72_jumpToScript", extended_b_op };
		a_mOPCodes[0xDA] = { "o72_openFile" };
		a_mOPCodes[0xDB] = { "o72_readFile", file_op };
		a_mOPCodes[0xDC] = { "o72_writeFile", file_op };
		a_mOPCodes[0xDD] = { "o72_findAllObjects" };
		a_mOPCodes[0xDE] = { "o72_deleteFile" };
		a_mOPCodes[0xDF] = { "o72_rename" };
		a_mOPCodes[0xE1] = { "o72_getPixel", extended_b_op };
		// TODO: a_mOPCodes[0xe3] = { "o72_pickVarRandom" };
		a_mOPCodes[0xEA] = { "o72_redimArray", extended_bw_op };
		a_mOPCodes[0xF3] = { "o72_readINI", extended_b_op };
		a_mOPCodes[0xF4] = { "o72_writeINI", extended_b_op };
		a_mOPCodes[0xF8] = { "o72_getResourceSize" };
		a_mOPCodes[0xF9] = { "o72_createDirectory" };
		a_mOPCodes[0xFA] = { "o72_setSystemMessage", extended_b_op };
	}

	//---------------------------------------------------------------------
	inline void GetHE73codes(OPCodeMap& a_mOPCodes)
	{
		GetHE72codes(a_mOPCodes);

		a_mOPCodes[0xF8] = { "o73_getResourceSize", extended_b_op };
	}

	//---------------------------------------------------------------------
	inline void GetHE80codes(OPCodeMap& a_mOPCodes)
	{
		GetHE73codes(a_mOPCodes);

		a_mOPCodes[0x45] = { "o80_createSound", extended_b_op };
		a_mOPCodes[0x46] = { "o80_getFileSize" };
		a_mOPCodes[0x48] = { "o80_stringToInt" };
		a_mOPCodes[0x49] = { "o80_getSoundVar" };
		a_mOPCodes[0x4A] = { "o80_localizeArrayToRoom" };
		// TODO: a_mOPCodes[0x4C] = { "o80_sourceDebug" };
		a_mOPCodes[0x4D] = { "o80_readConfigFile", extended_b_op };
		a_mOPCodes[0x4E] = { "o80_writeConfigFile", extended_b_op };
		a_mOPCodes[0x69] = { };
		a_mOPCodes[0x6B] = { "o80_cursorCommand", extended_b_op };
		a_mOPCodes[0x70] = { "o80_setState" };
		a_mOPCodes[0x76] = { };
		a_mOPCodes[0x94] = { };
		a_mOPCodes[0x9E] = { };
		a_mOPCodes[0xA5] = { };
		a_mOPCodes[0xAC] = { "o80_drawWizPolygon" };
		a_mOPCodes[0xE0] = { "o80_drawLine", extended_b_op };
		a_mOPCodes[0xE3] = { "o80_pickVarRandom", extended_w_op };
	}

	//---------------------------------------------------------------------
	inline void GetHE90codes(OPCodeMap& a_mOPCodes)
	{
		GetHE80codes(a_mOPCodes);

		a_mOPCodes[0x0A] = { "o90_dup_n", extended_w_op };
		a_mOPCodes[0x1C] = { "o90_wizImageOps", extended_b_op };
		a_mOPCodes[0x1D] = { "o90_min" };
		a_mOPCodes[0x1E] = { "o90_max" };
		a_mOPCodes[0x1F] = { "o90_sin" };
		a_mOPCodes[0x20] = { "o90_cos" };
		a_mOPCodes[0x21] = { "o90_sqrt" };
		a_mOPCodes[0x22] = { "o90_atan2" };
		a_mOPCodes[0x23] = { "o90_getSegmentAngle" };
		a_mOPCodes[0x24] = { "o90_getDistanceBetweenPoints", extended_b_op };
		a_mOPCodes[0x25] = { "o90_getSpriteInfo", extended_b_op };
		a_mOPCodes[0x26] = { "o90_setSpriteInfo", extended_b_op };
		a_mOPCodes[0x27] = { "o90_getSpriteGroupInfo", extended_b_op };
		a_mOPCodes[0x28] = { "o90_setSpriteGroupInfo", extended_b_op };
		a_mOPCodes[0x29] = { "o90_getWizData", extended_b_op };
		a_mOPCodes[0x2A] = { "o90_getActorData" };
		a_mOPCodes[0x2B] = { "o90_startScriptUnk", extended_b_op };
		a_mOPCodes[0x2C] = { "o90_jumpToScriptUnk", extended_b_op };
		a_mOPCodes[0x2D] = { "o90_videoOps", extended_b_op };
		a_mOPCodes[0x2E] = { "o90_getVideoData", extended_b_op };
		a_mOPCodes[0x2F] = { "o90_floodFill", extended_b_op };
		a_mOPCodes[0x30] = { "o90_mod" };
		a_mOPCodes[0x31] = { "o90_shl" };
		a_mOPCodes[0x32] = { "o90_shr" };
		a_mOPCodes[0x33] = { "o90_xor" };
		a_mOPCodes[0x34] = { "o90_findAllObjectsWithClassOf" };
		a_mOPCodes[0x35] = { "o90_getPolygonOverlap" };
		a_mOPCodes[0x36] = { "o90_cond" };
		a_mOPCodes[0x37] = { "o90_dim2dim2Array", extended_bw_op };
		a_mOPCodes[0x38] = { "o90_redim2dimArray", extended_bw_op };
		a_mOPCodes[0x39] = { "o90_getLinesIntersectionPoint", extended_ww_op };
		a_mOPCodes[0x3A] = { "o90_sortArray", extended_bw_op };
		a_mOPCodes[0x44] = { "o90_getObjectData", extended_b_op };
		a_mOPCodes[0x69] = { "o90_disabled_windowOps", extended_b_op };
		a_mOPCodes[0x94] = { "o90_getPaletteData", extended_b_op };
		a_mOPCodes[0x9E] = { "o90_paletteOps", extended_b_op };
		a_mOPCodes[0xA5] = { "o90_fontEnum", extended_b_op };
		// TODO: a_mOPCodes[0xab] = { "o90_getActorAnimProgress" };
		// TODO: a_mOPCodes[0xc8] = { "o90_kernelGetFunctions" };
		// TODO: a_mOPCodes[0xc9] = { "o90_kernelSetFunctions" };
	}

	//---------------------------------------------------------------------
	inline void GetHE100codes(OPCodeMap& a_mOPCodes)
	{
		a_mOPCodes[0x00] = { "o100_actorOps", extended_b_op };
		a_mOPCodes[0x01] = { "o6_add" };
		a_mOPCodes[0x02] = { "o6_faceActor" };
		a_mOPCodes[0x03] = { "o90_sortArray", extended_bw_op };
		a_mOPCodes[0x04] = { "o100_arrayOps", array_ops_he100 };
		a_mOPCodes[0x05] = { "o6_band" };
		a_mOPCodes[0x06] = { "o6_bor" };
		a_mOPCodes[0x07] = { "o6_breakHere" };
		a_mOPCodes[0x08] = { "o6_delayFrames" };
		a_mOPCodes[0x09] = { "o90_shl" };
		a_mOPCodes[0x0A] = { "o90_shr" };
		a_mOPCodes[0x0B] = { "o90_xor" };
		a_mOPCodes[0x0C] = { "o6_setCameraAt" };
		a_mOPCodes[0x0D] = { "o6_actorFollowCamera" };
		a_mOPCodes[0x0E] = { "o6_loadRoom" };
		// TODO: a_mOPCodes[0x0f] = { "o6_panCameraTo" };
		// TODO: a_mOPCodes[0x10] = { "o72_captureWizImage" };
		a_mOPCodes[0x11] = { "o100_jumpToScript", extended_b_op };
		a_mOPCodes[0x12] = { "o6_setClass" };
		a_mOPCodes[0x13] = { "o60_closeFile" };
		// TODO: a_mOPCodes[0x14] = { "o6_loadRoomWithEgo" };
		a_mOPCodes[0x16] = { "o72_createDirectory" };
		a_mOPCodes[0x17] = { "o100_createSound", extended_b_op };
		// TODO: a_mOPCodes[0x18] = { "o6_cutscene" };
		a_mOPCodes[0x19] = { "o6_pop" };
		a_mOPCodes[0x1A] = { "o72_traceStatus" };
		a_mOPCodes[0x1B] = { "o6_wordVarDec", extended_w_op };
		a_mOPCodes[0x1C] = { "o6_wordArrayDec", extended_w_op };
		a_mOPCodes[0x1D] = { "o72_deleteFile" };
		a_mOPCodes[0x1E] = { "o100_dim2dimArray", extended_bw_op };
		a_mOPCodes[0x1F] = { "o100_dimArray", extended_bw_op };
		a_mOPCodes[0x20] = { "o6_div" };
		a_mOPCodes[0x21] = { "o6_animateActor" };
		// TODO: a_mOPCodes[0x22] = { "o6_doSentence" };
		a_mOPCodes[0x23] = { "o6_drawBox" };
		// TODO: a_mOPCodes[0x24] = { "o72_drawWizImage" };
		// TODO: a_mOPCodes[0x25] = { "o80_drawWizPolygon" };
		a_mOPCodes[0x26] = { "o100_drawLine", extended_b_op };
		a_mOPCodes[0x27] = { "o100_drawObject", extended_b_op };
		a_mOPCodes[0x28] = { "o6_dup" };
		a_mOPCodes[0x29] = { "o90_dup_n", extended_w_op };
		// TODO: a_mOPCodes[0x2a] = { "o6_endCutscene" };
		a_mOPCodes[0x2B] = { "o6_stopObjectCodeObject" };  // o6_stopObjectCode
		a_mOPCodes[0x2C] = { "o6_stopObjectCodeScript" };  // o6_stopObjectCode
		a_mOPCodes[0x2D] = { "o6_eq" };
		// TODO: a_mOPCodes[0x2e] = { "o100_floodFill" };
		// TODO: a_mOPCodes[0x2f] = { "o6_freezeUnfreeze" };
		a_mOPCodes[0x30] = { "o6_ge" };
		a_mOPCodes[0x31] = { "o6_getDateTime" };
		a_mOPCodes[0x32] = { "o100_setSpriteGroupInfo", extended_b_op };
		a_mOPCodes[0x33] = { "o6_gt" };
		a_mOPCodes[0x34] = { "o100_resourceRoutines", extended_b_op };
		a_mOPCodes[0x35] = { "o6_if", jump_cmd };
		a_mOPCodes[0x36] = { "o6_ifNot", jump_cmd };
		a_mOPCodes[0x37] = { "o100_wizImageOps", extended_b_op };
		a_mOPCodes[0x38] = { "o72_isAnyOf" };
		a_mOPCodes[0x39] = { "o6_wordVarInc", extended_w_op };
		a_mOPCodes[0x3A] = { "o6_wordArrayInc", extended_w_op };
		a_mOPCodes[0x3B] = { "o6_jump", jump_cmd };
		a_mOPCodes[0x3C] = { "o90_kernelSetFunctions" };
		a_mOPCodes[0x3D] = { "o6_land" };
		a_mOPCodes[0x3E] = { "o6_le" };
		a_mOPCodes[0x3F] = { "o60_localizeArrayToScript" };
		a_mOPCodes[0x40] = { "o6_wordArrayRead", extended_w_op };
		a_mOPCodes[0x41] = { "o6_wordArrayIndexedRead", extended_w_op };
		a_mOPCodes[0x42] = { "o6_lor" };
		a_mOPCodes[0x43] = { "o6_lt" };
		a_mOPCodes[0x44] = { "o90_mod" };
		a_mOPCodes[0x45] = { "o6_mul" };
		a_mOPCodes[0x46] = { "o6_neq" };
		a_mOPCodes[0x47] = { "o100_dim2dim2Array", extended_bw_op };
		a_mOPCodes[0x49] = { "o100_redim2dimArray", extended_bw_op };
		a_mOPCodes[0x4A] = { "o6_not" };
		a_mOPCodes[0x4C] = { "o6_beginOverride" };
		a_mOPCodes[0x4D] = { "o6_endOverride" };
		a_mOPCodes[0x4E] = { "o72_resetCutscene" };
		a_mOPCodes[0x4F] = { "o6_setOwner" };
		a_mOPCodes[0x50] = { "o100_paletteOps", extended_b_op };
		a_mOPCodes[0x51] = { "o70_pickupObject" };
		a_mOPCodes[0x52] = { "o100_polygonOps", extended_b_op };  // o71_polygonOps
		a_mOPCodes[0x53] = { "o6_pop" };
		a_mOPCodes[0x54] = { "o100_printDebug", msg_cmd_he100 };  // o6_printDebug
		a_mOPCodes[0x55] = { "o72_printWizImage" };
		a_mOPCodes[0x56] = { "o100_printLine", msg_cmd_he100 };  // o6_printLine
		a_mOPCodes[0x57] = { "o100_printSystem", msg_cmd_he100 };  // o6_printSystem
		a_mOPCodes[0x58] = { "o100_printText", msg_cmd_he100 };  // o6_printText
		// TODO: a_mOPCodes[0x59] = { "o100_jumpToScriptUnk" };
		a_mOPCodes[0x5A] = { "o100_startScriptUnk", extended_b_op };
		// TODO: a_mOPCodes[0x5b] = { "o6_pseudoRoom" };
		a_mOPCodes[0x5C] = { "o6_pushByte", extended_b_op };
		a_mOPCodes[0x5D] = { "o72_pushDWord", extended_dw_op };
		a_mOPCodes[0x5E] = { "o72_getScriptString", msg_op };
		a_mOPCodes[0x5F] = { "o6_pushWord", extended_w_op };
		a_mOPCodes[0x60] = { "o6_pushWordVar", extended_w_op };
		a_mOPCodes[0x61] = { "o6_putActorAtObject" };
		a_mOPCodes[0x62] = { "o6_putActorAtXY" };
		a_mOPCodes[0x64] = { "o100_redimArray", extended_bw_op };
		a_mOPCodes[0x65] = { "o72_rename" };
		a_mOPCodes[0x66] = { "o6_stopObjectCodeReturn" };  // o6_stopObjectCode
		// TODO: a_mOPCodes[0x67] = { "o80_localizeArrayToRoom" };
		a_mOPCodes[0x68] = { "o100_roomOps", extended_b_op };
		a_mOPCodes[0x69] = { "o100_printActor", msg_cmd_he100 };  // o6_printActor
		a_mOPCodes[0x6A] = { "o100_printEgo", msg_cmd_he100 };  // o6_printEgo
		a_mOPCodes[0x6B] = { "o72_talkActor", msg_op };
		a_mOPCodes[0x6C] = { "o72_talkEgo", msg_op };
		a_mOPCodes[0x6E] = { "o60_seekFilePos" };
		a_mOPCodes[0x6F] = { "o6_setBoxFlags" };
		// TODO: a_mOPCodes[0x71] = { "o6_setBoxSet" };
		a_mOPCodes[0x72] = { "o100_setSystemMessage", extended_b_op };
		a_mOPCodes[0x73] = { "o6_shuffle", extended_w_op };
		a_mOPCodes[0x74] = { "o6_delay" };
		// TODO: a_mOPCodes[0x75] = { "o6_delayMinutes" };
		a_mOPCodes[0x76] = { "o6_delaySeconds" };
		a_mOPCodes[0x77] = { "o100_soundOps", extended_b_op };
		a_mOPCodes[0x78] = { "o80_sourceDebug", extended_ddw_op };
		a_mOPCodes[0x79] = { "o100_setSpriteInfo", extended_b_op };
		a_mOPCodes[0x7A] = { "o6_stampObject" };
		a_mOPCodes[0x7B] = { "o72_startObject", extended_b_op };
		a_mOPCodes[0x7C] = { "o100_startScript", extended_b_op };
		// TODO: a_mOPCodes[0x7d] = { "o6_startScriptQuick" };
		a_mOPCodes[0x7E] = { "o80_setState" };
		a_mOPCodes[0x7F] = { "o6_stopObjectScript" };
		a_mOPCodes[0x80] = { "o6_stopScript" };
		a_mOPCodes[0x81] = { "o6_stopSentence" };
		a_mOPCodes[0x82] = { "o6_stopSound" };
		a_mOPCodes[0x83] = { "o6_stopTalking" };
		a_mOPCodes[0x84] = { "o6_writeWordVar", extended_w_op };
		a_mOPCodes[0x85] = { "o6_wordArrayWrite", extended_w_op };
		a_mOPCodes[0x86] = { "o6_wordArrayIndexedWrite", extended_w_op };
		a_mOPCodes[0x87] = { "o6_sub" };
		a_mOPCodes[0x88] = { "o100_systemOps", extended_b_op };
		a_mOPCodes[0x8A] = { "o72_setTimer", extended_b_op };
		a_mOPCodes[0x8B] = { "o100_cursorCommand", extended_b_op };
		a_mOPCodes[0x8C] = { "o100_videoOps", extended_b_op };
		a_mOPCodes[0x8D] = { "o100_wait", wait_ops_he100 };
		// TODO: a_mOPCodes[0x8e] = { "o6_walkActorToObj" };
		a_mOPCodes[0x8F] = { "o6_walkActorTo" };
		a_mOPCodes[0x89] = { "o100_disabled_windowOps", extended_b_op };
		a_mOPCodes[0x90] = { "o100_writeFile", file_op_he100 };
		a_mOPCodes[0x91] = { "o72_writeINI", extended_b_op };
		a_mOPCodes[0x92] = { "o80_writeConfigFile", extended_b_op };
		a_mOPCodes[0x93] = { "o6_abs" };
		// TODO: a_mOPCodes[0x94] = { "o6_getActorWalkBox" };
		a_mOPCodes[0x95] = { "o6_getActorCostume" };
		a_mOPCodes[0x96] = { "o6_getActorElevation" };
		a_mOPCodes[0x97] = { "o6_getObjectOldDir" };
		a_mOPCodes[0x98] = { "o6_getActorMoving" };
		a_mOPCodes[0x99] = { "o90_getActorData" };
		a_mOPCodes[0x9A] = { "o6_getActorRoom" };
		a_mOPCodes[0x9B] = { "o6_getActorScaleX" };
		a_mOPCodes[0x9C] = { "o6_getAnimateVariable" };
		// TODO: a_mOPCodes[0x9d] = { "o6_getActorWidth" };
		a_mOPCodes[0x9E] = { "o6_getObjectX" };
		a_mOPCodes[0x9F] = { "o6_getObjectY" };
		a_mOPCodes[0xA0] = { "o90_atan2" };
		a_mOPCodes[0xA1] = { "o90_getSegmentAngle" };
		// TODO: a_mOPCodes[0xa2] = { "o90_getActorAnimProgress" };
		a_mOPCodes[0xA3] = { "o90_getDistanceBetweenPoints", extended_b_op };
		a_mOPCodes[0xA4] = { "o6_ifClassOfIs" };
		a_mOPCodes[0xA6] = { "o90_cond" };
		a_mOPCodes[0xA7] = { "o90_cos" };
		a_mOPCodes[0xA8] = { "o100_debugInput", extended_b_op };
		a_mOPCodes[0xA9] = { "o80_getFileSize" };
		a_mOPCodes[0xAA] = { "o6_getActorFromXY" };
		a_mOPCodes[0xAB] = { "o72_findAllObjects" };
		a_mOPCodes[0xAC] = { "o90_findAllObjectsWithClassOf" };
		// TODO: a_mOPCodes[0xad] = { "o71_findBox" };
		// TODO: a_mOPCodes[0xae] = { "o6_findInventory" };
		a_mOPCodes[0xAF] = { "o72_findObject" };
		// TODO: a_mOPCodes[0xb0] = { "o72_findObjectWithClassOf" };
		a_mOPCodes[0xB1] = { "o71_polygonHit" };
		// TODO: a_mOPCodes[0xb2] = { "o90_getLinesIntersectionPoint" };
		a_mOPCodes[0xB3] = { "o90_fontEnum", extended_b_op };
		a_mOPCodes[0xB4] = { "o72_getNumFreeArrays" };
		a_mOPCodes[0xB5] = { "o72_getArrayDimSize", extended_bw_op };
		a_mOPCodes[0xB6] = { "o100_isResourceLoaded", extended_b_op };
		a_mOPCodes[0xB7] = { "o100_getResourceSize", extended_b_op };
		a_mOPCodes[0xB8] = { "o100_getSpriteGroupInfo", extended_b_op };
		a_mOPCodes[0xB9] = { "o100_getHeap", extended_b_op };
		a_mOPCodes[0xBA] = { "o100_getWizData", extended_b_op };
		// TODO: a_mOPCodes[0xbb] = { "o6_isActorInBox" };
		a_mOPCodes[0xBC] = { "o6_isAnyOf" };
		// TODO: a_mOPCodes[0xbd] = { "o6_getInventoryCount" };
		a_mOPCodes[0xBE] = { "o90_kernelGetFunctions" };
		a_mOPCodes[0xBF] = { "o90_max" };
		a_mOPCodes[0xC0] = { "o90_min" };
		a_mOPCodes[0xC1] = { "o72_getObjectImageX" };
		a_mOPCodes[0xC2] = { "o72_getObjectImageY" };
		a_mOPCodes[0xC3] = { "o6_isRoomScriptRunning" };
		// TODO: a_mOPCodes[0xc4] = { "o90_getObjectData" };
		a_mOPCodes[0xC5] = { "o72_openFile" };
		a_mOPCodes[0xC6] = { "o90_getPolygonOverlap" };
		a_mOPCodes[0xC7] = { "o6_getOwner" };
		a_mOPCodes[0xC8] = { "o100_getPaletteData", extended_b_op };
		a_mOPCodes[0xC9] = { "o6_pickOneOf" };
		a_mOPCodes[0xCA] = { "o6_pickOneOfDefault" };
		a_mOPCodes[0xCB] = { "o80_pickVarRandom", extended_w_op };
		// TODO: a_mOPCodes[0xcc] = { "o72_getPixel" };
		// TODO: a_mOPCodes[0xcd] = { "o6_distObjectObject" };
		// TODO: a_mOPCodes[0xce] = { "o6_distObjectPt" };
		// TODO: a_mOPCodes[0xcf] = { "o6_distPtPt" };
		a_mOPCodes[0xD0] = { "o6_getRandomNumber" };
		a_mOPCodes[0xD1] = { "o6_getRandomNumberRange" };
		a_mOPCodes[0xD3] = { "o100_readFile", file_op_he100 };
		a_mOPCodes[0xD4] = { "o72_readINI", extended_b_op };
		a_mOPCodes[0xD5] = { "o80_readConfigFile", extended_b_op };
		a_mOPCodes[0xD6] = { "o6_isScriptRunning" };
		a_mOPCodes[0xD7] = { "o90_sin" };
		a_mOPCodes[0xD8] = { "o72_getSoundPosition" };
		a_mOPCodes[0xD9] = { "o6_isSoundRunning" };
		// TODO: a_mOPCodes[0xda] = { "o80_getSoundVar" };
		a_mOPCodes[0xDB] = { "o100_getSpriteInfo", extended_b_op };
		a_mOPCodes[0xDC] = { "o90_sqrt" };
		a_mOPCodes[0xDD] = { "o6_startObjectQuick" };
		a_mOPCodes[0xDE] = { "o6_startScriptQuick2" };
		a_mOPCodes[0xDF] = { "o6_getState" };
		a_mOPCodes[0xE0] = { "o71_compareString" };
		a_mOPCodes[0xE1] = { "o71_copyString" };
		a_mOPCodes[0xE2] = { "o71_appendString" };
		// TODO: a_mOPCodes[0xe3] = { "o71_concatString" };
		a_mOPCodes[0xE4] = { "o70_getStringLen" };
		a_mOPCodes[0xE5] = { "o71_getStringLenForWidth" };
		a_mOPCodes[0xE6] = { "o80_stringToInt" };
		a_mOPCodes[0xE7] = { "o71_getCharIndexInString" };
		a_mOPCodes[0xE8] = { "o71_getStringWidth" };
		a_mOPCodes[0xE9] = { "o60_readFilePos" };
		a_mOPCodes[0xEA] = { "o72_getTimer", extended_b_op };
		a_mOPCodes[0xEB] = { "o6_getVerbEntrypoint" };
		a_mOPCodes[0xEC] = { "o100_getVideoData", extended_b_op };
	}

	//---------------------------------------------------------------------
	inline void GetHE101codes(OPCodeMap& a_mOPCodes)
	{
		GetHE100codes(a_mOPCodes);

		a_mOPCodes[0xA8] = { "o72_debugInput" };
	}

	//---------------------------------------------------------------------
	inline void GetV8codes(OPCodeMap& a_mOPCodes)
	{
		a_mOPCodes[0x01] = { "o6_pushWord", extended_dw_op };
		a_mOPCodes[0x02] = { "o6_pushWordVar", extended_dw_op };
		a_mOPCodes[0x03] = { "o6_wordArrayRead", extended_dw_op };
		a_mOPCodes[0x04] = { "o6_wordArrayIndexedRead", extended_dw_op };
		a_mOPCodes[0x05] = { "o6_dup" };
		a_mOPCodes[0x06] = { "o6_pop" };
		a_mOPCodes[0x07] = { "o6_not" };
		a_mOPCodes[0x08] = { "o6_eq" };
		a_mOPCodes[0x09] = { "o6_neq" };
		a_mOPCodes[0x0A] = { "o6_gt" };
		a_mOPCodes[0x0B] = { "o6_lt" };
		a_mOPCodes[0x0C] = { "o6_le" };
		a_mOPCodes[0x0D] = { "o6_ge" };
		a_mOPCodes[0x0E] = { "o6_add" };
		a_mOPCodes[0x0F] = { "o6_sub" };
		a_mOPCodes[0x10] = { "o6_mul" };
		a_mOPCodes[0x11] = { "o6_div" };
		a_mOPCodes[0x12] = { "o6_land" };
		a_mOPCodes[0x13] = { "o6_lor" };
		a_mOPCodes[0x14] = { "o6_band" };
		a_mOPCodes[0x15] = { "o6_bor" };
		a_mOPCodes[0x16] = { "o8_mod" };
		a_mOPCodes[0x64] = { "o6_if" };
		a_mOPCodes[0x65] = { "o6_ifNot", djump_cmd };
		a_mOPCodes[0x66] = { "o6_jump", djump_cmd };
		a_mOPCodes[0x67] = { "o6_breakHere" };
		a_mOPCodes[0x68] = { "o6_delayFrames" };
		a_mOPCodes[0x69] = { "o8_wait", wait_ops_v8 };
		a_mOPCodes[0x6A] = { "o6_delay" };
		a_mOPCodes[0x6B] = { "o6_delaySeconds" };
		a_mOPCodes[0x6C] = { "o6_delayMinutes" };
		a_mOPCodes[0x6D] = { "o6_writeWordVar", extended_dw_op };
		a_mOPCodes[0x6E] = { "o6_wordVarInc", extended_dw_op };
		a_mOPCodes[0x6F] = { "o6_wordVarDec", extended_dw_op };
		a_mOPCodes[0x70] = { "o8_dimArray", extended_bdw_op };
		a_mOPCodes[0x71] = { "o6_wordArrayWrite", extended_dw_op };
		a_mOPCodes[0x72] = { "o6_wordArrayInc", extended_dw_op };
		a_mOPCodes[0x73] = { "o6_wordArrayDec", extended_dw_op };
		a_mOPCodes[0x74] = { "o8_dim2dimArray", extended_bdw_op };
		a_mOPCodes[0x75] = { "o6_wordArrayIndexedWrite", extended_dw_op };
		a_mOPCodes[0x76] = { "o8_arrayOps", array_ops_v8 };
		a_mOPCodes[0x79] = { "o6_startScript" };
		a_mOPCodes[0x7A] = { "o6_startScriptQuick" };
		a_mOPCodes[0x7B] = { "o6_stopObjectCodeScript" };  // o6_stopObjectCode
		a_mOPCodes[0x7C] = { "o6_stopScript" };
		a_mOPCodes[0x7D] = { "o6_jumpToScript" };
		a_mOPCodes[0x7E] = { "o6_dummy" };
		a_mOPCodes[0x7F] = { "o6_startObject" };
		a_mOPCodes[0x80] = { "o6_stopObjectScript" };
		a_mOPCodes[0x81] = { "o6_cutscene" };
		a_mOPCodes[0x82] = { "o6_endCutscene" };
		a_mOPCodes[0x83] = { "o6_freezeUnfreeze" };
		a_mOPCodes[0x84] = { "o6_beginOverride" };
		a_mOPCodes[0x85] = { "o6_endOverride" };
		a_mOPCodes[0x86] = { "o6_stopSentence" };
		a_mOPCodes[0x87] = { "o8_debug" };
		a_mOPCodes[0x89] = { "o6_setClass" };
		a_mOPCodes[0x8A] = { "o6_setState" };
		a_mOPCodes[0x8B] = { "o6_setOwner" };
		a_mOPCodes[0x8C] = { "o6_panCameraTo" };
		a_mOPCodes[0x8D] = { "o6_actorFollowCamera" };
		a_mOPCodes[0x8E] = { "o6_setCameraAt" };
		a_mOPCodes[0x8F] = { "o8_printActor", msg_cmd_v8 };  // o6_printActor
		a_mOPCodes[0x90] = { "o8_printEgo", msg_cmd_v8 };  // o6_printEgo
		a_mOPCodes[0x91] = { "o8_talkActor", msg_op_v8 };  // o6_talkActor
		a_mOPCodes[0x92] = { "o8_talkEgo", msg_op_v8 };  // o6_talkEgo
		a_mOPCodes[0x93] = { "o8_printLine", msg_cmd_v8 };  // o6_printLine
		a_mOPCodes[0x94] = { "o8_printText", msg_cmd_v8 };  // o6_printText
		a_mOPCodes[0x95] = { "o8_printDebug", msg_cmd_v8 };  // o6_printDebug
		a_mOPCodes[0x96] = { "o8_printSystem", msg_cmd_v8 };  // o6_printSystem
		a_mOPCodes[0x97] = { "o8_blastText", msg_cmd_v8 };
		a_mOPCodes[0x98] = { "o8_drawObject" };
		a_mOPCodes[0x9C] = { "o8_cursorCommand", extended_b_op };
		a_mOPCodes[0x9D] = { "o6_loadRoom" };
		a_mOPCodes[0x9E] = { "o6_loadRoomWithEgo" };
		a_mOPCodes[0x9F] = { "o6_walkActorToObj" };
		a_mOPCodes[0xA0] = { "o6_walkActorTo" };
		a_mOPCodes[0xA1] = { "o6_putActorAtXY" };
		a_mOPCodes[0xA2] = { "o6_putActorAtObject" };
		a_mOPCodes[0xA3] = { "o6_faceActor" };
		a_mOPCodes[0xA4] = { "o6_animateActor" };
		a_mOPCodes[0xA5] = { "o8_doSentence" };  // o6_doSentence
		a_mOPCodes[0xA6] = { "o6_pickupObject" };
		a_mOPCodes[0xA7] = { "o6_setBoxFlags" };
		a_mOPCodes[0xA8] = { "o6_createBoxMatrix" };
		a_mOPCodes[0xAA] = { "o8_resourceRoutines", extended_b_op };
		a_mOPCodes[0xAB] = { "o8_roomOps", extended_b_op };
		a_mOPCodes[0xAC] = { "o8_actorOps", actor_ops_v8 };
		a_mOPCodes[0xAD] = { "o8_cameraOps", extended_b_op };
		a_mOPCodes[0xAE] = { "o8_verbOps", verb_ops_v8 };
		a_mOPCodes[0xAF] = { "o6_startSound" };
		a_mOPCodes[0xB0] = { "o6_startMusic" };
		a_mOPCodes[0xB1] = { "o6_stopSound" };
		a_mOPCodes[0xB2] = { "o6_soundKludge" };
		a_mOPCodes[0xB3] = { "o8_systemOps", extended_b_op };
		a_mOPCodes[0xB4] = { "o6_saveRestoreVerbs", extended_b_op };
		a_mOPCodes[0xB5] = { "o6_setObjectName", msg_op_v8 };
		a_mOPCodes[0xB6] = { "o6_getDateTime" };
		a_mOPCodes[0xB7] = { "o6_drawBox" };
		a_mOPCodes[0xB9] = { "o8_startVideo", msg_op_v8 };
		a_mOPCodes[0xBA] = { "o8_kernelSetFunctions" };
		a_mOPCodes[0xC8] = { "o6_startScriptQuick2" };
		a_mOPCodes[0xC9] = { "o6_startObjectQuick" };
		a_mOPCodes[0xCA] = { "o6_pickOneOf" };
		a_mOPCodes[0xCB] = { "o6_pickOneOfDefault" };
		a_mOPCodes[0xCD] = { "o6_isAnyOf" };
		a_mOPCodes[0xCE] = { "o6_getRandomNumber" };
		a_mOPCodes[0xCF] = { "o6_getRandomNumberRange" };
		a_mOPCodes[0xD0] = { "o6_ifClassOfIs" };
		a_mOPCodes[0xD1] = { "o6_getState" };
		a_mOPCodes[0xD2] = { "o6_getOwner" };
		a_mOPCodes[0xD3] = { "o6_isScriptRunning" };
		a_mOPCodes[0xD5] = { "o6_isSoundRunning" };
		a_mOPCodes[0xD6] = { "o6_abs" };
		a_mOPCodes[0xD8] = { "o8_kernelGetFunctions" };
		a_mOPCodes[0xD9] = { "o6_isActorInBox" };
		a_mOPCodes[0xDA] = { "o6_getVerbEntrypoint" };
		a_mOPCodes[0xDB] = { "o6_getActorFromXY" };
		a_mOPCodes[0xDC] = { "o6_findObject" };
		a_mOPCodes[0xDD] = { "o6_getVerbFromXY" };
		a_mOPCodes[0xDF] = { "o6_findInventory" };
		a_mOPCodes[0xE0] = { "o6_getInventoryCount" };
		a_mOPCodes[0xE1] = { "o6_getAnimateVariable" };
		a_mOPCodes[0xE2] = { "o6_getActorRoom" };
		a_mOPCodes[0xE3] = { "o6_getActorWalkBox" };
		a_mOPCodes[0xE4] = { "o6_getActorMoving" };
		a_mOPCodes[0xE5] = { "o6_getActorCostume" };
		a_mOPCodes[0xE6] = { "o6_getActorScaleX" };
		a_mOPCodes[0xE7] = { "o6_getActorLayer" };
		a_mOPCodes[0xE8] = { "o6_getActorElevation" };
		a_mOPCodes[0xE9] = { "o6_getActorWidth" };
		a_mOPCodes[0xEA] = { "o6_getObjectNewDir" };
		a_mOPCodes[0xEB] = { "o6_getObjectX" };
		a_mOPCodes[0xEC] = { "o6_getObjectY" };
		a_mOPCodes[0xED] = { "o8_getActorChore" };
		a_mOPCodes[0xEE] = { "o6_distObjectObject" };
		a_mOPCodes[0xEF] = { "o6_distPtPt" };
		a_mOPCodes[0xF0] = { "o8_getObjectImageX" };
		a_mOPCodes[0xF1] = { "o8_getObjectImageY" };
		a_mOPCodes[0xF2] = { "o8_getObjectImageWidth" };
		a_mOPCodes[0xF3] = { "o8_getObjectImageHeight" };
		a_mOPCodes[0xF6] = { "o8_getStringWidth", msg_op_v8 };
		a_mOPCodes[0xF7] = { "o8_getActorZPlane" };
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