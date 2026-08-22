#include "OPCodesHE.h"

#include "script/ScrArgumentType.h"

namespace humongousexplorer::script
{
	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> default_func(uint8_t a_iByte, const unsigned char* a_pData)
	{
		return std::make_unique<ScrInstruction>(a_iByte);
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> extended_b_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);
		instruction->AddArgument(core::Data(a_pData, sizeof(uint8_t)), ScrArgumentType::Byte);
		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> extended_w_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);
		instruction->AddArgument(core::Data(a_pData, sizeof(uint16_t)), ScrArgumentType::Int16);
		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> extended_ww_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);
		instruction->AddArgument(core::Data(a_pData, sizeof(uint16_t)), ScrArgumentType::Int16);
		instruction->AddArgument(core::Data(a_pData + sizeof(uint16_t), sizeof(uint16_t)), ScrArgumentType::Int16);
		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> extended_dw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);
		instruction->AddArgument(core::Data(a_pData, sizeof(uint32_t)), ScrArgumentType::Int32);
		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> extended_ddw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);
		instruction->AddArgument(core::Data(a_pData, sizeof(uint32_t)), ScrArgumentType::Int32);
		instruction->AddArgument(core::Data(a_pData + sizeof(uint32_t), sizeof(uint32_t)), ScrArgumentType::Int32);
		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> extended_bw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);
		instruction->AddArgument(core::Data(a_pData, sizeof(uint8_t)), ScrArgumentType::Byte);
		instruction->AddArgument(core::Data(a_pData + sizeof(uint8_t), sizeof(uint16_t)), ScrArgumentType::Int16);
		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> extended_bdw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);
		instruction->AddArgument(core::Data(a_pData, sizeof(uint8_t)), ScrArgumentType::Byte);
		instruction->AddArgument(core::Data(a_pData + sizeof(uint8_t), sizeof(uint32_t)), ScrArgumentType::Int32);
		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> jump_cmd(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		instruction->AddArgument(core::Data(a_pData, sizeof(int16_t)), ScrArgumentType::Ref);

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> djump_cmd(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		instruction->AddArgument(core::Data(a_pData, sizeof(int32_t)), ScrArgumentType::Ref);

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> msg_cmd(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		size_t pos = 1;

		if (cmd == 75 || cmd == 194)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			instruction->AddArgument(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> msg_cmd_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		size_t pos = 1;

		if (cmd == 209)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			instruction->AddArgument(
				core::Data(a_pData + pos, sizeof(uint32_t)),
				ScrArgumentType::Int32
			);

			pos += 4;

			instruction->AddArgument(
				core::Data(a_pData + pos, len),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> msg_cmd_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		size_t pos = 1;

		if (cmd == 35 || cmd == 79)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			instruction->AddArgument(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> msg_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		size_t pos = 0;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		instruction->AddArgument(
			core::Data(a_pData, pos),
			ScrArgumentType::String
		);

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> msg_op_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint32_t len =
			static_cast<uint32_t>(a_pData[0]) |
			(static_cast<uint32_t>(a_pData[1]) << 8) |
			(static_cast<uint32_t>(a_pData[2]) << 16) |
			(static_cast<uint32_t>(a_pData[3]) << 24);

		instruction->AddArgument(
			core::Data(a_pData, sizeof(uint32_t)),
			ScrArgumentType::Int32
		);

		instruction->AddArgument(
			core::Data(a_pData + 4, len),
			ScrArgumentType::String
		);

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> actor_ops_v6(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		size_t pos = 1;

		if (cmd == 0x58)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			instruction->AddArgument(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> actor_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		size_t pos = 1;

		if (cmd == 0x71)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			instruction->AddArgument(
				core::Data(a_pData + pos, sizeof(uint32_t)),
				ScrArgumentType::Int32
			);

			pos += 4;

			instruction->AddArgument(
				core::Data(a_pData + pos, len),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> actor_ops_he60(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		size_t pos = 1;

		if (cmd == 225)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			instruction->AddArgument(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> verb_ops_v6(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		size_t pos = 1;

		if (cmd == 0x7D)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			instruction->AddArgument(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> verb_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		size_t pos = 1;

		if (cmd == 0x99 || cmd == 0xA4)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			instruction->AddArgument(
				core::Data(a_pData + pos, sizeof(uint32_t)),
				ScrArgumentType::Int32
			);

			pos += 4;

			instruction->AddArgument(
				core::Data(a_pData + pos, len),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> array_ops_v6(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		instruction->AddArgument(
			core::Data(a_pData + 1, sizeof(uint16_t)),
			ScrArgumentType::Int16
		);

		size_t pos = 1 + sizeof(uint16_t);

		if (cmd == 205)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			instruction->AddArgument(
				core::Data(
					a_pData + 1 + sizeof(uint16_t),
					pos - (1 + sizeof(uint16_t))
				),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> array_ops(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		instruction->AddArgument(
			core::Data(a_pData + 1, sizeof(uint16_t)),
			ScrArgumentType::Int16
		);

		size_t pos = 1 + sizeof(uint16_t);

		if (cmd == 127)
		{
			instruction->AddArgument(
				core::Data(a_pData + pos, sizeof(uint16_t)),
				ScrArgumentType::Int16
			);
		}
		else if (cmd == 138)
		{
			instruction->AddArgument(
				core::Data(a_pData + pos, sizeof(uint16_t)),
				ScrArgumentType::Int16
			);

			instruction->AddArgument(
				core::Data(a_pData + pos + sizeof(uint16_t), sizeof(uint16_t)),
				ScrArgumentType::Int16
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> array_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		instruction->AddArgument(
			core::Data(a_pData + 1, sizeof(uint32_t)),
			ScrArgumentType::Int32
		);

		size_t pos = 1 + sizeof(uint32_t);

		if (cmd == 0x14)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			instruction->AddArgument(
				core::Data(a_pData + pos, sizeof(uint32_t)),
				ScrArgumentType::Int32
			);

			pos += 4;

			instruction->AddArgument(
				core::Data(a_pData + pos, len),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> array_ops_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		instruction->AddArgument(
			core::Data(a_pData + 1, sizeof(uint16_t)),
			ScrArgumentType::Int16
		);

		size_t pos = 1 + sizeof(uint16_t);

		if (cmd == 131)
		{
			instruction->AddArgument(
				core::Data(a_pData + pos, sizeof(uint16_t)),
				ScrArgumentType::Int16
			);
		}
		else if (cmd == 132)
		{
			instruction->AddArgument(
				core::Data(a_pData + pos, sizeof(uint16_t)),
				ScrArgumentType::Int16
			);

			instruction->AddArgument(
				core::Data(a_pData + pos + sizeof(uint16_t), sizeof(uint16_t)),
				ScrArgumentType::Int16
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> wait_ops(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		if (cmd == 168 || cmd == 226 || cmd == 232)
		{
			instruction->AddArgument(
				core::Data(a_pData + 1, sizeof(int16_t)),
				ScrArgumentType::Ref
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> wait_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		if (cmd == 30 || cmd == 34 || cmd == 35)
		{
			instruction->AddArgument(
				core::Data(a_pData + 1, sizeof(int32_t)),
				ScrArgumentType::Ref
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> wait_ops_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		if (cmd == 128)
		{
			instruction->AddArgument(
				core::Data(a_pData + 1, sizeof(int16_t)),
				ScrArgumentType::Ref
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> room_ops_he60(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		size_t pos = 1;

		if (cmd == 221)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			instruction->AddArgument(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> dmsg_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		size_t pos = 0;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		instruction->AddArgument(
			core::Data(a_pData, pos),
			ScrArgumentType::String
		);

		size_t start = pos;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		instruction->AddArgument(
			core::Data(a_pData + start, pos - start),
			ScrArgumentType::String
		);

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> sys_msg(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		size_t pos = 1;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		instruction->AddArgument(
			core::Data(a_pData + 1, pos - 1),
			ScrArgumentType::String
		);

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> ini_op_v71(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t type = a_pData[-3];
		size_t pos = 0;

		if (type == 1)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			instruction->AddArgument(
				core::Data(a_pData, pos),
				ScrArgumentType::String
			);
		}
		else if (type == 2)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			instruction->AddArgument(
				core::Data(a_pData, pos),
				ScrArgumentType::String
			);

			size_t start = pos;

			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			instruction->AddArgument(
				core::Data(a_pData + start, pos - start),
				ScrArgumentType::String
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> file_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		if (cmd == 8)
		{
			instruction->AddArgument(
				core::Data(a_pData + 1, sizeof(uint8_t)),
				ScrArgumentType::Byte
			);
		}

		return instruction;
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ScrInstruction> file_op_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::unique_ptr<ScrInstruction> instruction = std::make_unique<ScrInstruction>(a_iByte);

		uint8_t cmd = a_pData[0];

		instruction->AddArgument(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte);

		if (cmd == 5)
		{
			instruction->AddArgument(
				core::Data(a_pData + 1, sizeof(uint8_t)),
				ScrArgumentType::Byte
			);
		}

		return instruction;
	}
}