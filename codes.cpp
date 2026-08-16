#pragma once

#include <cstdio>

#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <string>
#include <vector>

size_t default_func(unsigned char* data);
struct bytecode
{
	std::string m_sName;
	std::function<size_t(unsigned char*)> m_fnSize = default_func;
};

size_t extended_b_op(unsigned char* data)
{
	return sizeof(uint8_t);
}

size_t extended_w_op(unsigned char* data)
{
	return sizeof(uint16_t);
}

size_t extended_ww_op(unsigned char* data)
{
	return sizeof(uint16_t) * 2;
}

size_t extended_dw_op(unsigned char* data)
{
	return sizeof(uint32_t);
}

size_t extended_ddw_op(unsigned char* data)
{
	return sizeof(uint32_t) * 2;
}

size_t extended_bw_op(unsigned char* data)
{
	return sizeof(uint8_t) + sizeof(uint16_t);
}

size_t extended_bdw_op(unsigned char* data)
{
	return sizeof(uint8_t) + sizeof(uint32_t);
}

size_t jump_cmd(unsigned char* data)
{
	return sizeof(int16_t);
}

size_t djump_cmd(unsigned char* data)
{
	return sizeof(int32_t);
}

size_t msg_cmd(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 75 || cmd == 194) {
		while (data[pos] != 0) pos++;
		pos++;
	}
	return pos;
}

size_t msg_cmd_v8(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 209) {
		uint32_t len = data[pos] | (data[pos + 1] << 8) | (data[pos + 2] << 16) | (data[pos + 3] << 24);
		pos += 4 + len;
	}
	return pos;
}

size_t msg_cmd_he100(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 35 || cmd == 79) {
		while (data[pos] != 0) pos++;
		pos++;
	}
	return pos;
}

size_t msg_op(unsigned char* data)
{
	size_t pos = 0;
	while (data[pos] != 0) pos++;
	return pos + 1;
}

size_t msg_op_v8(unsigned char* data)
{
	uint32_t len = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
	return 4 + len;
}

size_t actor_ops_v6(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 0x58) {
		while (data[pos] != 0) pos++;
		pos++;
	}
	return pos;
}

size_t actor_ops_v8(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 0x71) {
		uint32_t len = data[pos] | (data[pos + 1] << 8) | (data[pos + 2] << 16) | (data[pos + 3] << 24);
		pos += 4 + len;
	}
	return pos;
}

size_t actor_ops_he60(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 225) {
		while (data[pos] != 0) pos++;
		pos++;
	}
	return pos;
}

size_t verb_ops_v6(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 0x7D) {
		while (data[pos] != 0) pos++;
		pos++;
	}
	return pos;
}

size_t verb_ops_v8(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 0x99 || cmd == 0xA4) {
		uint32_t len = data[pos] | (data[pos + 1] << 8) | (data[pos + 2] << 16) | (data[pos + 3] << 24);
		pos += 4 + len;
	}
	return pos;
}

size_t array_ops_v6(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1 + sizeof(uint16_t);
	if (cmd == 205) {
		while (data[pos] != 0) pos++;
		pos++;
	}
	return pos;
}

size_t array_ops(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1 + sizeof(uint16_t);
	if (cmd == 127) {
		pos += sizeof(uint16_t);
	}
	else if (cmd == 138) {
		pos += sizeof(uint16_t) * 2;
	}
	return pos;
}

size_t array_ops_v8(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1 + sizeof(uint32_t);
	if (cmd == 0x14) {
		uint32_t len = data[pos] | (data[pos + 1] << 8) | (data[pos + 2] << 16) | (data[pos + 3] << 24);
		pos += 4 + len;
	}
	return pos;
}

size_t array_ops_he100(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1 + sizeof(uint16_t);
	if (cmd == 131) {
		pos += sizeof(uint16_t);
	}
	else if (cmd == 132) {
		pos += sizeof(uint16_t) * 2;
	}
	return pos;
}

size_t wait_ops(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 168 || cmd == 226 || cmd == 232) {
		pos += sizeof(int16_t);
	}
	return pos;
}

size_t wait_ops_v8(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 30 || cmd == 34 || cmd == 35) {
		pos += sizeof(int32_t);
	}
	return pos;
}

size_t wait_ops_he100(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 128) {
		pos += sizeof(int16_t);
	}
	return pos;
}

size_t room_ops_he60(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 221) {
		while (data[pos] != 0) pos++;
		pos++;
	}
	return pos;
}

size_t dmsg_op(unsigned char* data)
{
	size_t pos = 0;
	while (data[pos] != 0) pos++;
	pos++;
	while (data[pos] != 0) pos++;
	return pos + 1;
}

size_t sys_msg(unsigned char* data)
{
	size_t pos = 1;
	while (data[pos] != 0) pos++;
	return pos + 1;
}

size_t ini_op_v71(unsigned char* data)
{
	uint8_t type = data[-3];
	size_t pos = 0;
	if (type == 1) {
		while (data[pos] != 0) pos++;
		pos++;
	}
	else if (type == 2) {
		while (data[pos] != 0) pos++;
		pos++;
		while (data[pos] != 0) pos++;
		pos++;
	}
	return pos;
}

size_t file_op(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 8) {
		pos += sizeof(uint8_t);
	}
	return pos;
}

size_t file_op_he100(unsigned char* data)
{
	uint8_t cmd = data[0];
	size_t pos = 1;
	if (cmd == 5) {
		pos += sizeof(uint8_t);
	}
	return pos;
}

size_t default_func(unsigned char* data)
{
	return 0;
}

// ============================================================================
// V6
// ============================================================================
std::map<uint8_t, bytecode> GetV6codes()
{
	static std::map<uint8_t, bytecode> OPCODES_V6 =
	{
		{ 0x00, { "o6_pushByte", extended_b_op } },
		{ 0x01, { "o6_pushWord", extended_w_op } },
		{ 0x02, { "o6_pushByteVar", extended_b_op } },
		{ 0x03, { "o6_pushWordVar", extended_w_op } },
		// { 0x06, { "o6_byteArrayRead" } },
		{ 0x07, { "o6_wordArrayRead", extended_w_op } },
		{ 0x0A, { "o6_byteArrayIndexedRead", extended_b_op } },
		{ 0x0B, { "o6_wordArrayIndexedRead", extended_w_op } },
		{ 0x0C, { "o6_dup" } },
		{ 0x0D, { "o6_not" } },
		{ 0x0E, { "o6_eq" } },
		{ 0x0F, { "o6_neq" } },
		{ 0x10, { "o6_gt" } },
		{ 0x11, { "o6_lt" } },
		{ 0x12, { "o6_le" } },
		{ 0x13, { "o6_ge" } },
		{ 0x14, { "o6_add" } },
		{ 0x15, { "o6_sub" } },
		{ 0x16, { "o6_mul" } },
		{ 0x17, { "o6_div" } },
		{ 0x18, { "o6_land" } }, // logical and
		{ 0x19, { "o6_lor" } }, // logical or
		{ 0x1A, { "o6_pop" } },
		// { 0x42, { "o6_writeByteVar" } },
		{ 0x43, { "o6_writeWordVar", extended_w_op } },
		// { 0x46, { "o6_byteArrayWrite" } },
		{ 0x47, { "o6_wordArrayWrite", extended_w_op } },
		// { 0x4A, { "o6_byteArrayIndexedWrite" } },
		{ 0x4B, { "o6_wordArrayIndexedWrite", extended_w_op } },
		// { 0x4E, { "o6_byteVarInc" } },
		{ 0x4F, { "o6_wordVarInc", extended_w_op } },
		// { 0x52, { "o6_byteArrayInc" } },
		{ 0x53, { "o6_wordArrayInc", extended_w_op } },
		// { 0x56, { "o6_byteVarDec" } },
		{ 0x57, { "o6_wordVarDec", extended_w_op } },
		// { 0x5A, { "o6_byteArrayDec" } },
		{ 0x5B, { "o6_wordArrayDec", extended_w_op } },
		{ 0x5C, { "o6_if", jump_cmd } }, // jump if
		{ 0x5D, { "o6_ifNot", jump_cmd } }, // jump if not
		{ 0x5E, { "o6_startScript" } },
		{ 0x5F, { "o6_startScriptQuick" } },
		{ 0x60, { "o6_startObject" } },
		{ 0x61, { "o6_drawObject" } },
		{ 0x62, { "o6_drawObjectAt" } },
		{ 0x63, { "o6_drawBlastObject" } },
		{ 0x64, { "o6_setBlastObjectWindow" } },
		{ 0x65, { "o6_stopObjectCodeObject" } }, // o6_stopObjectCode
		{ 0x66, { "o6_stopObjectCodeScript" } }, // o6_stopObjectCode
		{ 0x67, { "o6_endCutscene" } },
		{ 0x68, { "o6_cutscene" } },
		// { 0x69, { "o6_stopMusic" } },
		{ 0x6A, { "o6_freezeUnfreeze" } },
		{ 0x6B, { "o6_cursorCommand", extended_b_op } },
		{ 0x6C, { "o6_breakHere" } },
		{ 0x6D, { "o6_ifClassOfIs" } },
		{ 0x6E, { "o6_setClass" } },
		{ 0x6F, { "o6_getState" } },
		{ 0x70, { "o6_setState" } },
		{ 0x71, { "o6_setOwner" } },
		{ 0x72, { "o6_getOwner" } },
		{ 0x73, { "o6_jump", jump_cmd } },
		{ 0x74, { "o6_startSound" } },
		{ 0x75, { "o6_stopSound" } },
		// { 0x76, { "o6_startMusic" } },
		{ 0x77, { "o6_stopObjectScript" } },
		{ 0x78, { "o6_panCameraTo" } },
		{ 0x79, { "o6_actorFollowCamera" } },
		{ 0x7A, { "o6_setCameraAt" } },
		{ 0x7B, { "o6_loadRoom" } },
		{ 0x7C, { "o6_stopScript" } },
		{ 0x7D, { "o6_walkActorToObj" } },
		{ 0x7E, { "o6_walkActorTo" } },
		{ 0x7F, { "o6_putActorAtXY" } },
		{ 0x80, { "o6_putActorAtObject" } },
		{ 0x81, { "o6_faceActor" } },
		{ 0x82, { "o6_animateActor" } },
		{ 0x83, { "o6_doSentence" } },
		{ 0x84, { "o6_pickupObject" } },
		{ 0x85, { "o6_loadRoomWithEgo" } },
		{ 0x87, { "o6_getRandomNumber" } },
		{ 0x88, { "o6_getRandomNumberRange" } },
		{ 0x8A, { "o6_getActorMoving" } },
		{ 0x8B, { "o6_isScriptRunning" } },
		{ 0x8C, { "o6_getActorRoom" } },
		{ 0x8D, { "o6_getObjectX" } },
		{ 0x8E, { "o6_getObjectY" } },
		{ 0x8F, { "o6_getObjectOldDir" } },
		{ 0x90, { "o6_getActorWalkBox" } },
		{ 0x91, { "o6_getActorCostume" } },
		{ 0x92, { "o6_findInventory" } },
		{ 0x93, { "o6_getInventoryCount" } },
		{ 0x94, { "o6_getVerbFromXY" } },
		{ 0x95, { "o6_beginOverride" } },
		{ 0x96, { "o6_endOverride" } },
		{ 0x97, { "o6_setObjectName", msg_op } },
		{ 0x98, { "o6_isSoundRunning" } },
		{ 0x99, { "o6_setBoxFlags" } },
		{ 0x9A, { "o6_createBoxMatrix" } },
		{ 0x9B, { "o6_resourceRoutines", extended_b_op } },
		{ 0x9C, { "o6_roomOps", extended_b_op } },
		{ 0x9D, { "o6_actorOps", actor_ops_v6 } },
		{ 0x9E, { "o6_verbOps", verb_ops_v6 } },
		{ 0x9F, { "o6_getActorFromXY" } },
		{ 0xA0, { "o6_findObject" } },
		{ 0xA1, { "o6_pseudoRoom" } },
		{ 0xA2, { "o6_getActorElevation" } },
		{ 0xA3, { "o6_getVerbEntrypoint" } },
		{ 0xA4, { "o6_arrayOps", array_ops_v6 } },
		{ 0xA5, { "o6_saveRestoreVerbs", extended_b_op } },
		{ 0xA6, { "o6_drawBox" } },
		{ 0xA7, { "o6_pop" } },
		{ 0xA8, { "o6_getActorWidth" } },
		{ 0xA9, { "o6_wait", wait_ops } },
		{ 0xAA, { "o6_getActorScaleX" } },
		{ 0xAB, { "o6_getActorAnimCounter" } },
		{ 0xAC, { "o6_soundKludge" } },
		{ 0xAD, { "o6_isAnyOf" } },
		{ 0xAE, { "o6_systemOps", extended_b_op } },
		{ 0xAF, { "o6_isActorInBox" } },
		{ 0xB0, { "o6_delay" } },
		{ 0xB1, { "o6_delaySeconds" } },
		{ 0xB2, { "o6_delayMinutes" } },
		{ 0xB3, { "o6_stopSentence" } },
		{ 0xB4, { "o6_printLine", msg_cmd } },
		{ 0xB5, { "o6_printText", msg_cmd } },
		{ 0xB6, { "o6_printDebug", msg_cmd } },
		{ 0xB7, { "o6_printSystem", msg_cmd } },
		{ 0xB8, { "o6_printActor", msg_cmd } },
		{ 0xB9, { "o6_printEgo", msg_cmd } },
		{ 0xBA, { "o6_talkActor", msg_op } },
		{ 0xBB, { "o6_talkEgo", msg_op } },
		{ 0xBC, { "o6_dimArray", extended_bw_op } },
		{ 0xBD, { "o6_dummy" } },
		{ 0xBE, { "o6_startObjectQuick" } },
		{ 0xBF, { "o6_startScriptQuick2" } },
		{ 0xC0, { "o6_dim2dimArray", extended_bw_op } },
		{ 0xC4, { "o6_abs" } },
		{ 0xC5, { "o6_distObjectObject" } },
		// { 0xC6, { "o6_distObjectPt" } },
		{ 0xC7, { "o6_distPtPt" } },
		{ 0xC8, { "o6_kernelGetFunctions" } },
		{ 0xC9, { "o6_kernelSetFunctions" } },
		{ 0xCA, { "o6_delayFrames" } },
		{ 0xCB, { "o6_pickOneOf" } },
		{ 0xCC, { "o6_pickOneOfDefault" } },
		{ 0xCD, { "o6_stampObject" } },
		{ 0xD0, { "o6_getDateTime" } },
		{ 0xD1, { "o6_stopTalking" } },
		{ 0xD2, { "o6_getAnimateVariable" } },
		{ 0xD4, { "o6_shuffle", extended_w_op } },
		{ 0xD5, { "o6_jumpToScript" } },
		{ 0xD6, { "o6_band" } }, // bitwise and
		{ 0xD7, { "o6_bor" } }, // bitwise or
		{ 0xD8, { "o6_isRoomScriptRunning" } },
		{ 0xDD, { "o6_findAllObjects" } },
		{ 0xE1, { "o6_getPixel" } },
		{ 0xE3, { "o6_pickVarRandom", extended_w_op } },
		{ 0xE4, { "o6_setBoxSet", extended_b_op } },
		{ 0xEC, { "o6_getActorLayer" } },
		{ 0xED, { "o6_getObjectNewDir" } },
	};
	return OPCODES_V6;
}

std::map<uint8_t, bytecode> GetHE60codes()
{
	static std::map<uint8_t, bytecode> OPCODES_HE60 = GetV6codes();

	OPCODES_HE60[0x63] = {};
	OPCODES_HE60[0x64] = {};
	OPCODES_HE60[0x70] = { "o60_setState" };
	OPCODES_HE60[0x9A] = {};
	OPCODES_HE60[0x9C] = { "o60_roomOps", room_ops_he60 };
	OPCODES_HE60[0x9D] = { "o60_actorOps", actor_ops_he60 };
	OPCODES_HE60[0xAC] = {};
	OPCODES_HE60[0xBD] = { "o6_stopObjectCodeReturn" };
	OPCODES_HE60[0xC8] = { "o60_kernelGetFunctions" };
	OPCODES_HE60[0xC9] = { "o60_kernelSetFunctions" };
	OPCODES_HE60[0xD9] = { "o60_closeFile" };
	OPCODES_HE60[0xDA] = { "o60_openFile", msg_op };
	OPCODES_HE60[0xDB] = { "o60_readFile" };
	OPCODES_HE60[0xDC] = { "o60_writeFile" };
	OPCODES_HE60[0xDE] = { "o60_deleteFile", msg_op };
	OPCODES_HE60[0xDF] = { "o60_rename", dmsg_op };
	OPCODES_HE60[0xE0] = { "o60_soundOps", extended_b_op };
	OPCODES_HE60[0xE2] = { "o60_localizeArrayToScript" };
	OPCODES_HE60[0xE9] = { "o60_seekFilePos" };
	OPCODES_HE60[0xEA] = { "o60_redimArray", extended_bw_op };
	OPCODES_HE60[0xEB] = { "o60_readFilePos" };
	OPCODES_HE60[0xEC] = {};
	OPCODES_HE60[0xED] = {};

	return OPCODES_HE60;
}

std::map<uint8_t, bytecode> GetHE70codes()
{
	static std::map<uint8_t, bytecode> OPCODES_HE70 = GetHE60codes();

	OPCODES_HE70[0x74] = { "o70_soundOps", extended_b_op };
	OPCODES_HE70[0x84] = { "o70_pickupObject" };
	OPCODES_HE70[0x8C] = { "o70_getActorRoom" };
	OPCODES_HE70[0x9B] = { "o70_resourceRoutines", extended_b_op };
	OPCODES_HE70[0xAE] = { "o70_systemOps", extended_b_op };
	OPCODES_HE70[0xEE] = { "o70_getStringLen" };
	OPCODES_HE70[0xF2] = { "o70_isResourceLoaded", extended_b_op };
	OPCODES_HE70[0xF3] = { "o70_readINI", msg_op };
	OPCODES_HE70[0xF4] = { "o70_writeINI", ini_op_v71 };
	OPCODES_HE70[0xF9] = { "o70_createDirectory", msg_op };
	OPCODES_HE70[0xFA] = { "o70_setSystemMessage", sys_msg };

	return OPCODES_HE70;
}

std::map<uint8_t, bytecode> GetHE71codes()
{
	static std::map<uint8_t, bytecode> OPCODES_HE71 = GetHE70codes();

	OPCODES_HE71[0xC9] = { "o71_kernelSetFunctions" };
	OPCODES_HE71[0xEC] = { "o71_copyString" };
	OPCODES_HE71[0xED] = { "o71_getStringWidth" };
	OPCODES_HE71[0xEF] = { "o71_appendString" };
	OPCODES_HE71[0xF0] = { "o71_concatString" };
	OPCODES_HE71[0xF1] = { "o71_compareString" };
	OPCODES_HE71[0xF5] = { "o71_getStringLenForWidth" };
	OPCODES_HE71[0xF6] = { "o71_getCharIndexInString" };
	OPCODES_HE71[0xF7] = { "o71_findBox" };
	OPCODES_HE71[0xFB] = { "o71_polygonOps", extended_b_op };
	OPCODES_HE71[0xFC] = { "o71_polygonHit" };

	return OPCODES_HE71;
}

std::map<uint8_t, bytecode> GetHE72codes()
{
	static std::map<uint8_t, bytecode> OPCODES_HE72 = GetHE71codes();

	OPCODES_HE72[0x02] = { "o72_pushDWord", extended_dw_op };
	OPCODES_HE72[0x04] = { "o72_getScriptString", msg_op };
	OPCODES_HE72[0x0A] = {};
	OPCODES_HE72[0x1B] = { "o72_isAnyOf" };
	OPCODES_HE72[0x42] = {};
	OPCODES_HE72[0x46] = {};
	OPCODES_HE72[0x4A] = {};
	OPCODES_HE72[0x4E] = {};
	OPCODES_HE72[0x50] = { "o72_resetCutscene" };
	OPCODES_HE72[0x51] = { "o72_getHeap", extended_b_op };
	OPCODES_HE72[0x52] = { "o72_findObjectWithClassOf" };
	OPCODES_HE72[0x54] = { "o72_getObjectImageX" };
	OPCODES_HE72[0x55] = { "o72_getObjectImageY" };
	OPCODES_HE72[0x56] = { "o72_captureWizImage" };
	OPCODES_HE72[0x58] = { "o72_getTimer", extended_b_op };
	OPCODES_HE72[0x59] = { "o72_setTimer", extended_b_op };
	OPCODES_HE72[0x5A] = { "o72_getSoundPosition" };
	OPCODES_HE72[0x5E] = { "o72_startScript", extended_b_op };
	OPCODES_HE72[0x60] = { "o72_startObject", extended_b_op };
	OPCODES_HE72[0x61] = { "o72_drawObject", extended_b_op };
	OPCODES_HE72[0x62] = { "o72_printWizImage" };
	OPCODES_HE72[0x63] = { "o72_getArrayDimSize", extended_bw_op };
	OPCODES_HE72[0x64] = { "o72_getNumFreeArrays" };
	OPCODES_HE72[0x97] = {};
	OPCODES_HE72[0x9C] = { "o72_roomOps", extended_b_op };
	OPCODES_HE72[0x9D] = { "o72_actorOps", extended_b_op };
	OPCODES_HE72[0x9E] = { "o72_verbOps", extended_b_op };
	// OPCODES_HE72[0xA0] = { "o72_findObject" };
	OPCODES_HE72[0xA4] = { "o72_arrayOps", array_ops };
	OPCODES_HE72[0xAE] = { "o72_systemOps", extended_b_op };
	OPCODES_HE72[0xBA] = { "o72_talkActor", msg_op };
	OPCODES_HE72[0xBB] = { "o72_talkEgo", msg_op };
	OPCODES_HE72[0xBC] = { "o72_dimArray", extended_bw_op };
	OPCODES_HE72[0xC0] = { "o72_dim2dimArray", extended_bw_op };
	OPCODES_HE72[0xC1] = { "o72_traceStatus" };
	OPCODES_HE72[0xC8] = { "o72_kernelGetFunctions" };
	OPCODES_HE72[0xCE] = { "o72_drawWizImage" };
	OPCODES_HE72[0xCF] = { "o72_debugInput" };
	OPCODES_HE72[0xD5] = { "o72_jumpToScript", extended_b_op };
	OPCODES_HE72[0xDA] = { "o72_openFile" };
	OPCODES_HE72[0xDB] = { "o72_readFile", file_op };
	OPCODES_HE72[0xDC] = { "o72_writeFile", file_op };
	OPCODES_HE72[0xDD] = { "o72_findAllObjects" };
	OPCODES_HE72[0xDE] = { "o72_deleteFile" };
	OPCODES_HE72[0xDF] = { "o72_rename" };
	OPCODES_HE72[0xE1] = { "o72_getPixel", extended_b_op };
	// OPCODES_HE72[0xE3] = { "o72_pickVarRandom" };
	OPCODES_HE72[0xEA] = { "o72_redimArray", extended_bw_op };
	OPCODES_HE72[0xF3] = { "o72_readINI", extended_b_op };
	OPCODES_HE72[0xF4] = { "o72_writeINI", extended_b_op };
	OPCODES_HE72[0xF8] = { "o72_getResourceSize" };
	OPCODES_HE72[0xF9] = { "o72_createDirectory" };
	OPCODES_HE72[0xFA] = { "o72_setSystemMessage", extended_b_op };

	return OPCODES_HE72;
}

std::map<uint8_t, bytecode> GetHE73codes()
{
	static std::map<uint8_t, bytecode> OPCODES_HE73 = GetHE72codes();

	OPCODES_HE73[0xF8] = { "o73_getResourceSize", extended_b_op };

	return OPCODES_HE73;
}

std::map<uint8_t, bytecode> GetHE80codes()
{
	static std::map<uint8_t, bytecode> OPCODES_HE80 = GetHE73codes();

	OPCODES_HE80[0x45] = { "o80_createSound", extended_b_op };
	OPCODES_HE80[0x46] = { "o80_getFileSize" };
	OPCODES_HE80[0x48] = { "o80_stringToInt" };
	OPCODES_HE80[0x49] = { "o80_getSoundVar" };
	OPCODES_HE80[0x4A] = { "o80_localizeArrayToRoom" };
	// OPCODES_HE80[0x4C] = { "o80_sourceDebug" };
	OPCODES_HE80[0x4D] = { "o80_readConfigFile", extended_b_op };
	OPCODES_HE80[0x4E] = { "o80_writeConfigFile", extended_b_op };
	OPCODES_HE80[0x69] = {};
	OPCODES_HE80[0x6B] = { "o80_cursorCommand", extended_b_op };
	OPCODES_HE80[0x70] = { "o80_setState" };
	OPCODES_HE80[0x76] = {};
	OPCODES_HE80[0x94] = {};
	OPCODES_HE80[0x9E] = {};
	OPCODES_HE80[0xA5] = {};
	OPCODES_HE80[0xAC] = { "o80_drawWizPolygon" };
	OPCODES_HE80[0xE0] = { "o80_drawLine", extended_b_op };
	OPCODES_HE80[0xE3] = { "o80_drawLine", /*o80_pickVarRandom*/extended_b_op };

	return OPCODES_HE80;
}

std::map<uint8_t, bytecode> GetHE90codes()
{
	static std::map<uint8_t, bytecode> OPCODES_HE90 = GetHE80codes();

	OPCODES_HE90[0x0A] = { "o90_dup_n", extended_w_op };
	OPCODES_HE90[0x1C] = { "o90_wizImageOps", extended_b_op };
	OPCODES_HE90[0x1D] = { "o90_min" };
	OPCODES_HE90[0x1E] = { "o90_max" };
	OPCODES_HE90[0x1F] = { "o90_sin" };
	OPCODES_HE90[0x20] = { "o90_cos" };
	OPCODES_HE90[0x21] = { "o90_sqrt" };
	OPCODES_HE90[0x22] = { "o90_atan2" };
	OPCODES_HE90[0x23] = { "o90_getSegmentAngle" };
	OPCODES_HE90[0x24] = { "o90_getDistanceBetweenPoints", extended_b_op };
	OPCODES_HE90[0x25] = { "o90_getSpriteInfo", extended_b_op };
	OPCODES_HE90[0x26] = { "o90_setSpriteInfo", extended_b_op };
	OPCODES_HE90[0x27] = { "o90_getSpriteGroupInfo", extended_b_op };
	OPCODES_HE90[0x28] = { "o90_setSpriteGroupInfo", extended_b_op };
	OPCODES_HE90[0x29] = { "o90_getWizData", extended_b_op };
	OPCODES_HE90[0x2A] = { "o90_getActorData" };
	OPCODES_HE90[0x2B] = { "o90_startScriptUnk", extended_b_op };
	OPCODES_HE90[0x2C] = { "o90_jumpToScriptUnk", extended_b_op };
	OPCODES_HE90[0x2D] = { "o90_videoOps", extended_b_op };
	OPCODES_HE90[0x2E] = { "o90_getVideoData", extended_b_op };
	OPCODES_HE90[0x2F] = { "o90_floodFill", extended_b_op };
	OPCODES_HE90[0x30] = { "o90_mod" };
	OPCODES_HE90[0x31] = { "o90_shl" };
	OPCODES_HE90[0x32] = { "o90_shr" };
	OPCODES_HE90[0x33] = { "o90_xor" };
	OPCODES_HE90[0x34] = { "o90_findAllObjectsWithClassOf" };
	OPCODES_HE90[0x35] = { "o90_getPolygonOverlap" };
	OPCODES_HE90[0x36] = { "o90_cond" };
	OPCODES_HE90[0x37] = { "o90_dim2dim2Array", extended_bw_op };
	OPCODES_HE90[0x38] = { "o90_redim2dimArray", extended_bw_op };
	OPCODES_HE90[0x39] = { "o90_getLinesIntersectionPoint", extended_ww_op };
	OPCODES_HE90[0x3A] = { "o90_sortArray", extended_bw_op };
	OPCODES_HE90[0x44] = { "o90_getObjectData", extended_b_op };
	OPCODES_HE90[0x69] = { "o90_disabled_windowOps", extended_b_op };
	OPCODES_HE90[0x94] = { "o90_getPaletteData", extended_b_op };
	OPCODES_HE90[0x9E] = { "o90_paletteOps", extended_b_op };
	OPCODES_HE90[0xA5] = { "o90_fontEnum", extended_b_op };
	// OPCODES_HE90[0xAB] = { "o90_getActorAnimProgress" };
	// OPCODES_HE90[0xC8] = { "o90_kernelGetFunctions" };
	// OPCODES_HE90[0xC9] = { "o90_kernelSetFunctions" };

	return OPCODES_HE90;
}

std::map<uint8_t, bytecode> GetHE100codes()
{
	static std::map<uint8_t, bytecode> OPCODES_HE100;

	OPCODES_HE100[0x00] = { "o100_actorOps", extended_b_op };
	OPCODES_HE100[0x01] = { "o6_add" };
	OPCODES_HE100[0x02] = { "o6_faceActor" };
	OPCODES_HE100[0x03] = { "o90_sortArray", extended_bw_op };
	OPCODES_HE100[0x04] = { "o100_arrayOps", array_ops_he100 };
	OPCODES_HE100[0x05] = { "o6_band" };
	OPCODES_HE100[0x06] = { "o6_bor" };
	OPCODES_HE100[0x07] = { "o6_breakHere" };
	OPCODES_HE100[0x08] = { "o6_delayFrames" };
	OPCODES_HE100[0x09] = { "o90_shl" };
	OPCODES_HE100[0x0A] = { "o90_shr" };
	OPCODES_HE100[0x0B] = { "o90_xor" };
	OPCODES_HE100[0x0C] = { "o6_setCameraAt" };
	OPCODES_HE100[0x0D] = { "o6_actorFollowCamera" };
	OPCODES_HE100[0x0E] = { "o6_loadRoom" };
	// OPCODES_HE100[0x0F] = { "o6_panCameraTo" };
	// OPCODES_HE100[0x10] = { "o72_captureWizImage" };
	OPCODES_HE100[0x11] = { "o100_jumpToScript", extended_b_op };
	OPCODES_HE100[0x12] = { "o6_setClass" };
	OPCODES_HE100[0x13] = { "o60_closeFile" };
	// OPCODES_HE100[0x14] = { "o6_loadRoomWithEgo" };
	OPCODES_HE100[0x16] = { "o72_createDirectory" };
	OPCODES_HE100[0x17] = { "o100_createSound", extended_b_op };
	// OPCODES_HE100[0x18] = { "o6_cutscene" };
	OPCODES_HE100[0x19] = { "o6_pop" };
	OPCODES_HE100[0x1A] = { "o72_traceStatus" };
	OPCODES_HE100[0x1B] = { "o6_wordVarDec", extended_w_op };
	OPCODES_HE100[0x1C] = { "o6_wordArrayDec", extended_w_op };
	OPCODES_HE100[0x1D] = { "o72_deleteFile" };
	OPCODES_HE100[0x1E] = { "o100_dim2dimArray", extended_bw_op };
	OPCODES_HE100[0x1F] = { "o100_dimArray", extended_bw_op };
	OPCODES_HE100[0x20] = { "o6_div" };
	OPCODES_HE100[0x21] = { "o6_animateActor" };
	// OPCODES_HE100[0x22] = { "o6_doSentence" };
	OPCODES_HE100[0x23] = { "o6_drawBox" };
	// OPCODES_HE100[0x24] = { "o72_drawWizImage" };
	// OPCODES_HE100[0x25] = { "o80_drawWizPolygon" };
	OPCODES_HE100[0x26] = { "o100_drawLine", extended_b_op };
	OPCODES_HE100[0x27] = { "o100_drawObject", extended_b_op };
	OPCODES_HE100[0x28] = { "o6_dup" };
	OPCODES_HE100[0x29] = { "o90_dup_n", extended_w_op };
	// OPCODES_HE100[0x2A] = { "o6_endCutscene" };
	OPCODES_HE100[0x2B] = { "o6_stopObjectCodeObject" };
	OPCODES_HE100[0x2C] = { "o6_stopObjectCodeScript" };
	OPCODES_HE100[0x2D] = { "o6_eq" };
	// OPCODES_HE100[0x2E] = { "o100_floodFill" };
	// OPCODES_HE100[0x2F] = { "o6_freezeUnfreeze" };
	OPCODES_HE100[0x30] = { "o6_ge" };
	OPCODES_HE100[0x31] = { "o6_getDateTime" };
	OPCODES_HE100[0x32] = { "o100_setSpriteGroupInfo", extended_b_op };
	OPCODES_HE100[0x33] = { "o6_gt" };
	OPCODES_HE100[0x34] = { "o100_resourceRoutines", extended_b_op };
	OPCODES_HE100[0x35] = { "o6_if", jump_cmd };
	OPCODES_HE100[0x36] = { "o6_ifNot", jump_cmd };
	OPCODES_HE100[0x37] = { "o100_wizImageOps", extended_b_op };
	OPCODES_HE100[0x38] = { "o72_isAnyOf" };
	OPCODES_HE100[0x39] = { "o6_wordVarInc", extended_w_op };
	OPCODES_HE100[0x3A] = { "o6_wordArrayInc", extended_w_op };
	OPCODES_HE100[0x3B] = { "o6_jump", jump_cmd };
	OPCODES_HE100[0x3C] = { "o90_kernelSetFunctions" };
	OPCODES_HE100[0x3D] = { "o6_land" };
	OPCODES_HE100[0x3E] = { "o6_le" };
	OPCODES_HE100[0x3F] = { "o60_localizeArrayToScript" };
	OPCODES_HE100[0x40] = { "o6_wordArrayRead", extended_w_op };
	OPCODES_HE100[0x41] = { "o6_wordArrayIndexedRead", extended_w_op };
	OPCODES_HE100[0x42] = { "o6_lor" };
	OPCODES_HE100[0x43] = { "o6_lt" };
	OPCODES_HE100[0x44] = { "o90_mod" };
	OPCODES_HE100[0x45] = { "o6_mul" };
	OPCODES_HE100[0x46] = { "o6_neq" };
	OPCODES_HE100[0x47] = { "o100_dim2dim2Array", extended_bw_op };
	OPCODES_HE100[0x49] = { "o100_redim2dimArray", extended_bw_op };
	OPCODES_HE100[0x4A] = { "o6_not" };
	OPCODES_HE100[0x4C] = { "o6_beginOverride" };
	OPCODES_HE100[0x4D] = { "o6_endOverride" };
	OPCODES_HE100[0x4E] = { "o72_resetCutscene" };
	OPCODES_HE100[0x4F] = { "o6_setOwner" };
	OPCODES_HE100[0x50] = { "o100_paletteOps", extended_b_op };
	OPCODES_HE100[0x51] = { "o70_pickupObject" };
	OPCODES_HE100[0x52] = { "o100_polygonOps", extended_b_op }; // o71_polygonOps
	OPCODES_HE100[0x53] = { "o6_pop" };
	OPCODES_HE100[0x54] = { "o100_printDebug", msg_cmd_he100 }; // o6_printDebug
	OPCODES_HE100[0x55] = { "o72_printWizImage" };
	OPCODES_HE100[0x56] = { "o100_printLine", msg_cmd_he100 }; // o6_printLine
	OPCODES_HE100[0x57] = { "o100_printSystem", msg_cmd_he100 }; // o6_printSystem
	OPCODES_HE100[0x58] = { "o100_printText", msg_cmd_he100 }; // o6_printText
	// OPCODES_HE100[0x59] = { "o100_jumpToScriptUnk" };
	OPCODES_HE100[0x5A] = { "o100_startScriptUnk", extended_b_op };
	// OPCODES_HE100[0x5b] = { "o6_pseudoRoom" };
	OPCODES_HE100[0x5C] = { "o6_pushByte", extended_b_op };
	OPCODES_HE100[0x5D] = { "o72_pushDWord", extended_dw_op };
	OPCODES_HE100[0x5E] = { "o72_getScriptString", msg_op };
	OPCODES_HE100[0x5F] = { "o6_pushWord", extended_w_op };
	OPCODES_HE100[0x60] = { "o72_getScriptString", extended_w_op };
	OPCODES_HE100[0x61] = { "o6_putActorAtObject" };
	OPCODES_HE100[0x62] = { "o6_putActorAtXY" };
	OPCODES_HE100[0x64] = { "o100_redimArray", extended_bw_op };
	OPCODES_HE100[0x65] = { "o72_rename" };
	OPCODES_HE100[0x66] = { "o6_stopObjectCodeReturn" }; // o6_stopObjectCode
	// OPCODES_HE100[0x67] = { "o80_localizeArrayToRoom" }; 
	OPCODES_HE100[0x68] = { "o100_roomOps", extended_b_op };
	OPCODES_HE100[0x69] = { "o100_printActor", msg_cmd_he100 }; // o6_printActor
	OPCODES_HE100[0x6A] = { "o100_printEgo", msg_cmd_he100 }; // o6_printEgo
	OPCODES_HE100[0x6B] = { "o72_talkActor", msg_op };
	OPCODES_HE100[0x6C] = { "o72_talkEgo", msg_op };
	OPCODES_HE100[0x6E] = { "o60_seekFilePos" };
	OPCODES_HE100[0x6F] = { "o6_setBoxFlags" };
	// OPCODES_HE100[0x71] = { "o6_setBoxSet" };
	OPCODES_HE100[0x72] = { "o100_setSystemMessage", extended_b_op };
	OPCODES_HE100[0x73] = { "o6_shuffle", extended_w_op };
	OPCODES_HE100[0x74] = { "o6_delay" };
	// OPCODES_HE100[0x75] = { "o6_delayMinutes" };
	OPCODES_HE100[0x76] = { "o6_delaySeconds" };
	OPCODES_HE100[0x77] = { "o100_soundOps", extended_b_op };
	OPCODES_HE100[0x78] = { "o80_sourceDebug", extended_ddw_op };
	OPCODES_HE100[0x79] = { "o100_setSpriteInfo", extended_b_op };
	OPCODES_HE100[0x7A] = { "o6_stampObject" };
	OPCODES_HE100[0x7B] = { "o72_startObject", extended_b_op };
	OPCODES_HE100[0x7C] = { "o100_startScript", extended_b_op };
	// OPCODES_HE100[0x7D] = { "o6_startScriptQuick" };
	OPCODES_HE100[0x7E] = { "o80_setState" };
	OPCODES_HE100[0x7F] = { "o6_stopObjectScript" };
	OPCODES_HE100[0x80] = { "o6_stopScript" };
	OPCODES_HE100[0x81] = { "o6_stopSentence" };
	OPCODES_HE100[0x82] = { "o6_stopSound" };
	OPCODES_HE100[0x83] = { "o6_stopTalking" };
	OPCODES_HE100[0x84] = { "o6_writeWordVar", extended_w_op };
	OPCODES_HE100[0x85] = { "o6_wordArrayWrite", extended_w_op };
	OPCODES_HE100[0x86] = { "o6_wordArrayIndexedWrite", extended_w_op };
	OPCODES_HE100[0x87] = { "o6_sub" };
	OPCODES_HE100[0x88] = { "o100_systemOps", extended_b_op };
	OPCODES_HE100[0x8A] = { "o72_setTimer", extended_b_op };
	OPCODES_HE100[0x8B] = { "o100_cursorCommand", extended_b_op };
	OPCODES_HE100[0x8C] = { "o100_videoOps", extended_b_op };
	OPCODES_HE100[0x8D] = { "o100_wait", wait_ops_he100 };
	// OPCODES_HE100[0x8E] = { "o6_walkActorToObj" };
	OPCODES_HE100[0x8F] = { "o6_walkActorTo" };
	OPCODES_HE100[0x89] = { "o100_disabled_windowOps", extended_b_op };
	OPCODES_HE100[0x90] = { "o100_writeFile", file_op_he100 };
	OPCODES_HE100[0x91] = { "o72_writeINI", extended_b_op };
	OPCODES_HE100[0x92] = { "o80_writeConfigFile", extended_b_op };
	OPCODES_HE100[0x93] = { "o6_abs" };
	// OPCODES_HE100[0x94] = { "o6_getActorWalkBox" };
	OPCODES_HE100[0x95] = { "o6_getActorCostume" };
	OPCODES_HE100[0x96] = { "o6_getActorElevation" };
	OPCODES_HE100[0x97] = { "o6_getObjectOldDir" };
	OPCODES_HE100[0x98] = { "o6_getActorMoving" };
	OPCODES_HE100[0x99] = { "o90_getActorData" };
	OPCODES_HE100[0x9A] = { "o6_getActorRoom" };
	OPCODES_HE100[0x9B] = { "o6_getActorScaleX" };
	OPCODES_HE100[0x9C] = { "o6_getAnimateVariable" };
	// OPCODES_HE100[0x9D] = { "o6_getActorWidth" };
	OPCODES_HE100[0x9E] = { "o6_getObjectX" };
	OPCODES_HE100[0x9F] = { "o6_getObjectY" };
	OPCODES_HE100[0xA0] = { "o90_atan2" };
	OPCODES_HE100[0xA1] = { "o90_getSegmentAngle" };
	// OPCODES_HE100[0xA2] = { "o90_getActorAnimProgress" };
	OPCODES_HE100[0xA3] = { "o90_getDistanceBetweenPoints", extended_b_op };
	OPCODES_HE100[0xA4] = { "o6_ifClassOfIs" };
	OPCODES_HE100[0xA6] = { "o90_cond" };
	OPCODES_HE100[0xA7] = { "o90_cos" };
	OPCODES_HE100[0xA8] = { "o100_debugInput", extended_b_op };
	OPCODES_HE100[0xA9] = { "o80_getFileSize" };
	OPCODES_HE100[0xAA] = { "o6_getActorFromXY" };
	OPCODES_HE100[0xAB] = { "o72_findAllObjects" };
	OPCODES_HE100[0xAC] = { "o90_findAllObjectsWithClassOf" };
	// OPCODES_HE100[0xAD] = { "o71_findBox" };
	// OPCODES_HE100[0xAE] = { "o6_findInventory" };
	OPCODES_HE100[0xAF] = { "o72_findObject" };
	// OPCODES_HE100[0xB0] = { "o72_findObjectWithClassOf" };
	OPCODES_HE100[0xB1] = { "o71_polygonHit" };
	// OPCODES_HE100[0xB2] = { "o90_getLinesIntersectionPoint" };
	OPCODES_HE100[0xB3] = { "o90_fontEnum", extended_b_op };
	OPCODES_HE100[0xB4] = { "o72_getNumFreeArrays" };
	OPCODES_HE100[0xB5] = { "o72_getArrayDimSize", extended_bw_op };
	OPCODES_HE100[0xB6] = { "o100_isResourceLoaded", extended_b_op };
	OPCODES_HE100[0xB7] = { "o100_getResourceSize", extended_b_op };
	OPCODES_HE100[0xB8] = { "o100_getSpriteGroupInfo", extended_b_op };
	OPCODES_HE100[0xB9] = { "o100_getHeap", extended_b_op };
	OPCODES_HE100[0xBA] = { "o100_getWizData", extended_b_op };
	// OPCODES_HE100[0xBB] = { "o6_isActorInBox" };
	OPCODES_HE100[0xBC] = { "o6_isAnyOf" };
	// OPCODES_HE100[0xBD] = { "o6_getInventoryCount" };
	OPCODES_HE100[0xBE] = { "o90_kernelGetFunctions" };
	OPCODES_HE100[0xBF] = { "o90_max" };
	OPCODES_HE100[0xC0] = { "o90_min" };
	OPCODES_HE100[0xC1] = { "o72_getObjectImageX" };
	OPCODES_HE100[0xC2] = { "o72_getObjectImageY" };
	OPCODES_HE100[0xC3] = { "o6_isRoomScriptRunning" };
	// OPCODES_HE100[0xC4] = { "o90_getObjectData" };
	OPCODES_HE100[0xC5] = { "o72_openFile" };
	OPCODES_HE100[0xC6] = { "o90_getPolygonOverlap" };
	OPCODES_HE100[0xC7] = { "o6_getOwner" };
	OPCODES_HE100[0xC8] = { "o100_getPaletteData", extended_b_op };
	OPCODES_HE100[0xC9] = { "o6_pickOneOf" };
	OPCODES_HE100[0xCA] = { "o6_pickOneOfDefault" };
	OPCODES_HE100[0xCB] = { "o80_pickVarRandom", extended_w_op };
	// OPCODES_HE100[0xCC] = { "o72_getPixel" };
	// OPCODES_HE100[0xCD] = { "o6_distObjectObject" };
	// OPCODES_HE100[0xCE] = { "o6_distObjectPt" };
	// OPCODES_HE100[0xCF] = { "o6_distPtPt" };
	OPCODES_HE100[0xD0] = { "o6_getRandomNumber" };
	OPCODES_HE100[0xD1] = { "o6_getRandomNumberRange" };
	OPCODES_HE100[0xD3] = { "o100_readFile", file_op_he100 };
	OPCODES_HE100[0xD4] = { "o72_readINI", extended_b_op };
	OPCODES_HE100[0xD5] = { "o80_readConfigFile", extended_b_op };
	OPCODES_HE100[0xD6] = { "o6_isScriptRunning" };
	OPCODES_HE100[0xD7] = { "o90_sin" };
	OPCODES_HE100[0xD8] = { "o72_getSoundPosition" };
	OPCODES_HE100[0xD9] = { "o6_isSoundRunning" };
	// OPCODES_HE100[0xDA] = { "o80_getSoundVar" };
	OPCODES_HE100[0xDB] = { "o100_getSpriteInfo", extended_b_op };
	OPCODES_HE100[0xDC] = { "o90_sqrt" };
	OPCODES_HE100[0xDD] = { "o6_startObjectQuick" };
	OPCODES_HE100[0xDE] = { "o6_startScriptQuick2" };
	OPCODES_HE100[0xDF] = { "o6_getState" };
	OPCODES_HE100[0xE0] = { "o71_compareString" };
	OPCODES_HE100[0xE1] = { "o71_copyString" };
	OPCODES_HE100[0xE2] = { "o71_appendString" };
	// OPCODES_HE100[0xE3] = { "o71_concatString" };
	OPCODES_HE100[0xE4] = { "o70_getStringLen" };
	OPCODES_HE100[0xE5] = { "o71_getStringLenForWidth" };
	OPCODES_HE100[0xE6] = { "o80_stringToInt" };
	OPCODES_HE100[0xE7] = { "o71_getCharIndexInString" };
	OPCODES_HE100[0xE8] = { "o71_getStringWidth" };
	OPCODES_HE100[0xE9] = { "o60_readFilePos" };
	OPCODES_HE100[0xEA] = { "o72_getTimer", extended_b_op };
	OPCODES_HE100[0xEB] = { "o6_getVerbEntrypoint" };
	OPCODES_HE100[0xEC] = { "o100_getVideoData", extended_b_op };

	return OPCODES_HE100;
}

std::map<uint8_t, bytecode> GetHE101codes()
{
	static std::map<uint8_t, bytecode> OPCODES_HE101 = GetHE100codes();

	OPCODES_HE101[0xA8] = { "o72_debugInput" };

	return OPCODES_HE101;
}

std::map<uint8_t, bytecode> GetV8codes()
{
	static std::map<uint8_t, bytecode> OPCODES_V8;

	OPCODES_V8[0x01] = { "o6_pushWord", extended_dw_op };
	OPCODES_V8[0x02] = { "o6_pushWordVar", extended_dw_op };
	OPCODES_V8[0x03] = { "o6_wordArrayRead", extended_dw_op };
	OPCODES_V8[0x04] = { "o6_wordArrayIndexedRead", extended_dw_op };
	OPCODES_V8[0x05] = { "o6_dup" };
	OPCODES_V8[0x06] = { "o6_pop" };
	OPCODES_V8[0x07] = { "o6_not" };
	OPCODES_V8[0x08] = { "o6_eq" };
	OPCODES_V8[0x09] = { "o6_neq" };
	OPCODES_V8[0x0A] = { "o6_gt" };
	OPCODES_V8[0x0B] = { "o6_lt" };
	OPCODES_V8[0x0C] = { "o6_le" };
	OPCODES_V8[0x0D] = { "o6_ge" };
	OPCODES_V8[0x0E] = { "o6_add" };
	OPCODES_V8[0x0F] = { "o6_sub" };
	OPCODES_V8[0x10] = { "o6_mul" };
	OPCODES_V8[0x11] = { "o6_div" };
	OPCODES_V8[0x12] = { "o6_land" };
	OPCODES_V8[0x13] = { "o6_lor" };
	OPCODES_V8[0x14] = { "o6_band" };
	OPCODES_V8[0x15] = { "o6_bor" };
	OPCODES_V8[0x16] = { "o8_mod" };
	OPCODES_V8[0x64] = { "o6_if", djump_cmd };
	OPCODES_V8[0x65] = { "o6_ifNot", djump_cmd };
	OPCODES_V8[0x66] = { "o6_jump", djump_cmd };
	OPCODES_V8[0x67] = { "o6_breakHere" };
	OPCODES_V8[0x68] = { "o6_delayFrames" };
	OPCODES_V8[0x69] = { "o8_wait", wait_ops_v8 };
	OPCODES_V8[0x6A] = { "o6_delay" };
	OPCODES_V8[0x6B] = { "o6_delaySeconds" };
	OPCODES_V8[0x6C] = { "o6_delayMinutes" };
	OPCODES_V8[0x6D] = { "o6_writeWordVar", extended_dw_op };
	OPCODES_V8[0x6E] = { "o6_wordVarInc", extended_dw_op };
	OPCODES_V8[0x6F] = { "o6_wordVarDec", extended_dw_op };
	OPCODES_V8[0x70] = { "o8_dimArray", extended_bdw_op };
	OPCODES_V8[0x71] = { "o6_wordArrayWrite", extended_dw_op };
	OPCODES_V8[0x72] = { "o6_wordArrayInc", extended_dw_op };
	OPCODES_V8[0x73] = { "o6_wordArrayDec", extended_dw_op };
	OPCODES_V8[0x74] = { "o8_dim2dimArray", extended_bdw_op };
	OPCODES_V8[0x75] = { "o6_wordArrayIndexedWrite", extended_dw_op };
	OPCODES_V8[0x76] = { "o8_arrayOps", array_ops_v8 };
	OPCODES_V8[0x79] = { "o6_startScript" };
	OPCODES_V8[0x7A] = { "o6_startScriptQuick" };
	OPCODES_V8[0x7B] = { "o6_stopObjectCodeScript" };  // o6_stopObjectCode
	OPCODES_V8[0x7C] = { "o6_stopScript" };
	OPCODES_V8[0x7D] = { "o6_jumpToScript" };
	OPCODES_V8[0x7E] = { "o6_dummy" };
	OPCODES_V8[0x7F] = { "o6_startObject" };
	OPCODES_V8[0x80] = { "o6_stopObjectScript" };
	OPCODES_V8[0x81] = { "o6_cutscene" };
	OPCODES_V8[0x82] = { "o6_endCutscene" };
	OPCODES_V8[0x83] = { "o6_freezeUnfreeze" };
	OPCODES_V8[0x84] = { "o6_beginOverride" };
	OPCODES_V8[0x85] = { "o6_endOverride" };
	OPCODES_V8[0x86] = { "o6_stopSentence" };
	OPCODES_V8[0x87] = { "o8_debug" };
	OPCODES_V8[0x89] = { "o6_setClass" };
	OPCODES_V8[0x8A] = { "o6_setState" };
	OPCODES_V8[0x8B] = { "o6_setOwner" };
	OPCODES_V8[0x8C] = { "o6_panCameraTo" };
	OPCODES_V8[0x8D] = { "o6_actorFollowCamera" };
	OPCODES_V8[0x8E] = { "o6_setCameraAt" };
	OPCODES_V8[0x8F] = { "o8_printActor", msg_cmd_v8 };  // o6_printActor
	OPCODES_V8[0x90] = { "o8_printEgo", msg_cmd_v8 };  // o6_printEgo
	OPCODES_V8[0x91] = { "o8_talkActor", msg_op_v8 };  // o6_talkActor
	OPCODES_V8[0x92] = { "o8_talkEgo", msg_op_v8 };  // o6_talkEgo
	OPCODES_V8[0x93] = { "o8_printLine", msg_cmd_v8 };  // o6_printLine
	OPCODES_V8[0x94] = { "o8_printText", msg_cmd_v8 };  // o6_printText
	OPCODES_V8[0x95] = { "o8_printDebug", msg_cmd_v8 };  // o6_printDebug
	OPCODES_V8[0x96] = { "o8_printSystem", msg_cmd_v8 };  // o6_printSystem
	OPCODES_V8[0x97] = { "o8_blastText", msg_cmd_v8 };
	OPCODES_V8[0x98] = { "o8_drawObject" };
	OPCODES_V8[0x9C] = { "o8_cursorCommand", extended_b_op };
	OPCODES_V8[0x9D] = { "o6_loadRoom" };
	OPCODES_V8[0x9E] = { "o6_loadRoomWithEgo" };
	OPCODES_V8[0x9F] = { "o6_walkActorToObj" };
	OPCODES_V8[0xA0] = { "o6_walkActorTo" };
	OPCODES_V8[0xA1] = { "o6_putActorAtXY" };
	OPCODES_V8[0xA2] = { "o6_putActorAtObject" };
	OPCODES_V8[0xA3] = { "o6_faceActor" };
	OPCODES_V8[0xA4] = { "o6_animateActor" };
	OPCODES_V8[0xA5] = { "o8_doSentence" };  // o6_doSentence
	OPCODES_V8[0xA6] = { "o6_pickupObject" };
	OPCODES_V8[0xA7] = { "o6_setBoxFlags" };
	OPCODES_V8[0xA8] = { "o6_createBoxMatrix" };
	OPCODES_V8[0xAA] = { "o8_resourceRoutines", extended_b_op };
	OPCODES_V8[0xAB] = { "o8_roomOps", extended_b_op };
	OPCODES_V8[0xAC] = { "o8_actorOps", actor_ops_v8 };
	OPCODES_V8[0xAD] = { "o8_cameraOps", extended_b_op };
	OPCODES_V8[0xAE] = { "o8_verbOps", verb_ops_v8 };
	OPCODES_V8[0xAF] = { "o6_startSound" };
	OPCODES_V8[0xB0] = { "o6_startMusic" };
	OPCODES_V8[0xB1] = { "o6_stopSound" };
	OPCODES_V8[0xB2] = { "o6_soundKludge" };
	OPCODES_V8[0xB3] = { "o8_systemOps", extended_b_op };
	OPCODES_V8[0xB4] = { "o6_saveRestoreVerbs", extended_b_op };
	OPCODES_V8[0xB5] = { "o6_setObjectName", msg_op_v8 };
	OPCODES_V8[0xB6] = { "o6_getDateTime" };
	OPCODES_V8[0xB7] = { "o6_drawBox" };
	OPCODES_V8[0xB9] = { "o8_startVideo", msg_op_v8 };
	OPCODES_V8[0xBA] = { "o8_kernelSetFunctions" };
	OPCODES_V8[0xC8] = { "o6_startScriptQuick2" };
	OPCODES_V8[0xC9] = { "o6_startObjectQuick" };
	OPCODES_V8[0xCA] = { "o6_pickOneOf" };
	OPCODES_V8[0xCB] = { "o6_pickOneOfDefault" };
	OPCODES_V8[0xCD] = { "o6_isAnyOf" };
	OPCODES_V8[0xCE] = { "o6_getRandomNumber" };
	OPCODES_V8[0xCF] = { "o6_getRandomNumberRange" };
	OPCODES_V8[0xD0] = { "o6_ifClassOfIs" };
	OPCODES_V8[0xD1] = { "o6_getState" };
	OPCODES_V8[0xD2] = { "o6_getOwner" };
	OPCODES_V8[0xD3] = { "o6_isScriptRunning" };
	OPCODES_V8[0xD5] = { "o6_isSoundRunning" };
	OPCODES_V8[0xD6] = { "o6_abs" };
	OPCODES_V8[0xD8] = { "o8_kernelGetFunctions" };
	OPCODES_V8[0xD9] = { "o6_isActorInBox" };
	OPCODES_V8[0xDA] = { "o6_getVerbEntrypoint" };
	OPCODES_V8[0xDB] = { "o6_getActorFromXY" };
	OPCODES_V8[0xDC] = { "o6_findObject" };
	OPCODES_V8[0xDD] = { "o6_getVerbFromXY" };
	OPCODES_V8[0xDF] = { "o6_findInventory" };
	OPCODES_V8[0xE0] = { "o6_getInventoryCount" };
	OPCODES_V8[0xE1] = { "o6_getAnimateVariable" };
	OPCODES_V8[0xE2] = { "o6_getActorRoom" };
	OPCODES_V8[0xE3] = { "o6_getActorWalkBox" };
	OPCODES_V8[0xE4] = { "o6_getActorMoving" };
	OPCODES_V8[0xE5] = { "o6_getActorCostume" };
	OPCODES_V8[0xE6] = { "o6_getActorScaleX" };
	OPCODES_V8[0xE7] = { "o6_getActorLayer" };
	OPCODES_V8[0xE8] = { "o6_getActorElevation" };
	OPCODES_V8[0xE9] = { "o6_getActorWidth" };
	OPCODES_V8[0xEA] = { "o6_getObjectNewDir" };
	OPCODES_V8[0xEB] = { "o6_getObjectX" };
	OPCODES_V8[0xEC] = { "o6_getObjectY" };
	OPCODES_V8[0xED] = { "o8_getActorChore" };
	OPCODES_V8[0xEE] = { "o6_distObjectObject" };
	OPCODES_V8[0xEF] = { "o6_distPtPt" };
	OPCODES_V8[0xF0] = { "o8_getObjectImageX" };
	OPCODES_V8[0xF1] = { "o8_getObjectImageY" };
	OPCODES_V8[0xF2] = { "o8_getObjectImageWidth" };
	OPCODES_V8[0xF3] = { "o8_getObjectImageHeight" };
	OPCODES_V8[0xF6] = { "o8_getStringWidth", msg_op_v8 };
	OPCODES_V8[0xF7] = { "o8_getActorZPlane" };

	return OPCODES_V8;
}

const std::map<uint8_t, bytecode> GetOpcodeTable(int version, int heVersion)
{
	if (heVersion >= 101) return GetHE101codes();
	else if (heVersion >= 100) return GetHE100codes();
	else if (heVersion >= 90)  return GetHE90codes();
	else if (heVersion >= 80)  return GetHE80codes();
	else if (heVersion >= 73)  return GetHE73codes();
	else if (heVersion >= 72)  return GetHE72codes();
	else if (heVersion >= 71)  return GetHE71codes();
	else if (heVersion >= 70)  return GetHE70codes();
	else if (heVersion >= 60)  return GetHE60codes();
	else if (version >= 8)     return GetV8codes();
	else if (version >= 6)     return GetV6codes();
	return GetV6codes();
}

#include "Data.h"
#include "file_abstractions.h"

int main()
{
	Data data;
	if (!LoadFile("./test.test", data))
	{
		return 0;
	}

	auto map = GetOpcodeTable(6, 98);

	size_t tell = 0;
	while (tell < data.size())
	{
		unsigned char* pureDat = data.dataAs<unsigned char>() + tell;
		uint8_t code = *pureDat;

		size_t skip = map[code].m_fnSize(pureDat + 1);
		tell += skip + 1;
	}

	return 0;
}