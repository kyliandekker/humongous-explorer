#include "OPCodesHE.h"

#include <helib/script/ScrArgumentType.h>
#include <helib/script/ScrArgument.h>

namespace humongousexplorer::script
{
	//---------------------------------------------------------------------
	// bytecode
	//---------------------------------------------------------------------
	bytecode::~bytecode()
	{}

	//---------------------------------------------------------------------
	bytecode::bytecode(const std::string& a_sName) :
		m_sName(a_sName)
	{}

	//---------------------------------------------------------------------
	bytecode::bytecode(const std::string& a_sName, std::function<std::vector<std::unique_ptr<ScrArgument>>(uint8_t, const unsigned char*)> a_fnSize) :
		m_sName(a_sName),
		m_fnSize(a_fnSize)
	{}

	//---------------------------------------------------------------------
	const std::string& bytecode::GetName() const
	{
		return m_sName;
	}

	//---------------------------------------------------------------------
	std::function<std::vector<std::unique_ptr<ScrArgument>>(uint8_t, const unsigned char*)> bytecode::GetSizeFn() const
	{
		return m_fnSize;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> default_func(uint8_t a_iByte, const unsigned char* a_pData)
	{
		return {};
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> extended_b_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(uint8_t)), ScrArgumentType::Byte));
		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> extended_w_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(uint16_t)), ScrArgumentType::Int16));
		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> extended_ww_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(uint16_t)), ScrArgumentType::Int16));
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData + sizeof(uint16_t), sizeof(uint16_t)), ScrArgumentType::Int16));
		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> extended_dw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(uint32_t)), ScrArgumentType::Int32));
		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> extended_ddw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(uint32_t)), ScrArgumentType::Int32));
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData + sizeof(uint32_t), sizeof(uint32_t)), ScrArgumentType::Int32));
		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> extended_bw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(uint8_t)), ScrArgumentType::Byte));
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData + sizeof(uint8_t), sizeof(uint16_t)), ScrArgumentType::Int16));
		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> extended_bdw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(uint8_t)), ScrArgumentType::Byte));
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData + sizeof(uint8_t), sizeof(uint32_t)), ScrArgumentType::Int32));
		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> jump_cmd(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(int16_t)), ScrArgumentType::Ref));
		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> djump_cmd(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;
		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(int32_t)), ScrArgumentType::Ref));
		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> msg_cmd(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		size_t pos = 1;

		if (cmd == 75 || cmd == 194)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> msg_cmd_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		size_t pos = 1;

		if (cmd == 209)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, sizeof(uint32_t)),
				ScrArgumentType::Int32
			));

			pos += 4;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, len),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> msg_cmd_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		size_t pos = 1;

		if (cmd == 35 || cmd == 79)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> msg_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		size_t pos = 0;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		args.emplace_back(std::make_unique<ScrArgument>(
			core::Data(a_pData, pos),
			ScrArgumentType::String
		));

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> msg_op_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint32_t len =
			static_cast<uint32_t>(a_pData[0]) |
			(static_cast<uint32_t>(a_pData[1]) << 8) |
			(static_cast<uint32_t>(a_pData[2]) << 16) |
			(static_cast<uint32_t>(a_pData[3]) << 24);

		args.emplace_back(std::make_unique<ScrArgument>(
			core::Data(a_pData, sizeof(uint32_t)),
			ScrArgumentType::Int32
		));

		args.emplace_back(std::make_unique<ScrArgument>(
			core::Data(a_pData + 4, len),
			ScrArgumentType::String
		));

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> actor_ops_v6(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		size_t pos = 1;

		if (cmd == 0x58)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> actor_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		size_t pos = 1;

		if (cmd == 0x71)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, sizeof(uint32_t)),
				ScrArgumentType::Int32
			));

			pos += 4;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, len),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> actor_ops_he60(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		size_t pos = 1;

		if (cmd == 225)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> verb_ops_v6(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		size_t pos = 1;

		if (cmd == 0x7D)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> verb_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		size_t pos = 1;

		if (cmd == 0x99 || cmd == 0xA4)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, sizeof(uint32_t)),
				ScrArgumentType::Int32
			));

			pos += 4;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, len),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> array_ops_v6(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		args.emplace_back(std::make_unique<ScrArgument>(
			core::Data(a_pData + 1, sizeof(uint16_t)),
			ScrArgumentType::Int16
		));

		size_t pos = 1 + sizeof(uint16_t);

		if (cmd == 205)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(
					a_pData + 1 + sizeof(uint16_t),
					pos - (1 + sizeof(uint16_t))
				),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> array_ops(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		args.emplace_back(std::make_unique<ScrArgument>(
			core::Data(a_pData + 1, sizeof(uint16_t)),
			ScrArgumentType::Int16
		));

		size_t pos = 1 + sizeof(uint16_t);

		if (cmd == 127)
		{
			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, sizeof(uint16_t)),
				ScrArgumentType::Int16
			));
		}
		else if (cmd == 138)
		{
			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, sizeof(uint16_t)),
				ScrArgumentType::Int16
			));

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos + sizeof(uint16_t), sizeof(uint16_t)),
				ScrArgumentType::Int16
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> array_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		args.emplace_back(std::make_unique<ScrArgument>(
			core::Data(a_pData + 1, sizeof(uint32_t)),
			ScrArgumentType::Int32
		));

		size_t pos = 1 + sizeof(uint32_t);

		if (cmd == 0x14)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, sizeof(uint32_t)),
				ScrArgumentType::Int32
			));

			pos += 4;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, len),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> array_ops_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		args.emplace_back(std::make_unique<ScrArgument>(
			core::Data(a_pData + 1, sizeof(uint16_t)),
			ScrArgumentType::Int16
		));

		size_t pos = 1 + sizeof(uint16_t);

		if (cmd == 131)
		{
			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, sizeof(uint16_t)),
				ScrArgumentType::Int16
			));
		}
		else if (cmd == 132)
		{
			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos, sizeof(uint16_t)),
				ScrArgumentType::Int16
			));

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + pos + sizeof(uint16_t), sizeof(uint16_t)),
				ScrArgumentType::Int16
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> wait_ops(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		if (cmd == 168 || cmd == 226 || cmd == 232)
		{
			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, sizeof(int16_t)),
				ScrArgumentType::Ref
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> wait_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		if (cmd == 30 || cmd == 34 || cmd == 35)
		{
			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, sizeof(int32_t)),
				ScrArgumentType::Ref
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> wait_ops_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		if (cmd == 128)
		{
			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, sizeof(int16_t)),
				ScrArgumentType::Ref
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> room_ops_he60(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		size_t pos = 1;

		if (cmd == 221)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, pos - 1),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> dmsg_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		size_t pos = 0;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		args.emplace_back(std::make_unique<ScrArgument>(
			core::Data(a_pData, pos),
			ScrArgumentType::String
		));

		size_t start = pos;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		args.emplace_back(std::make_unique<ScrArgument>(
			core::Data(a_pData + start, pos - start),
			ScrArgumentType::String
		));

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> sys_msg(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		size_t pos = 1;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		args.emplace_back(std::make_unique<ScrArgument>(
			core::Data(a_pData + 1, pos - 1),
			ScrArgumentType::String
		));

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> ini_op_v71(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t type = a_pData[-3];
		size_t pos = 0;

		if (type == 1)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData, pos),
				ScrArgumentType::String
			));
		}
		else if (type == 2)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData, pos),
				ScrArgumentType::String
			));

			size_t start = pos;

			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + start, pos - start),
				ScrArgumentType::String
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> file_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		if (cmd == 8)
		{
			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, sizeof(uint8_t)),
				ScrArgumentType::Byte
			));
		}

		return args;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>> file_op_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<std::unique_ptr<ScrArgument>> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(std::make_unique<ScrArgument>(core::Data(a_pData, sizeof(cmd)), ScrArgumentType::Byte));

		if (cmd == 5)
		{
			args.emplace_back(std::make_unique<ScrArgument>(
				core::Data(a_pData + 1, sizeof(uint8_t)),
				ScrArgumentType::Byte
			));
		}

		return args;
	}
}