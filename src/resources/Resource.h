#pragma once

#include <cstdint>
#include <string>

namespace humongousexplorer::core
{
	class Data;
}
namespace humongousexplorer::parsing
{
	class Chunk;
}
namespace humongousexplorer::resources
{
	enum class ResourceType;

	//---------------------------------------------------------------------
	class Resource
	{
	public:
		ResourceType GetResourceType() const
		{
			return m_eResourceType;
		}

		virtual size_t GetSize() const = 0;
		virtual std::string GetDurationStr()
		{
			return "";
		}

		const std::string& GetName() const
		{
			return m_sName;
		}
		void SetName(const std::string& a_sName)
		{
			m_sName = a_sName;
		}
	protected:
		ResourceType m_eResourceType;
		std::string m_sName;
	};

	//---------------------------------------------------------------------
	class SoundResource : public Resource
	{
	public:
		uint16_t GetSampleRate() const;
		void SetSampleRate(uint16_t a_iSampleRate);

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);

		size_t GetSize() const override;

		std::string GetDurationStr() override;
	protected:
		uint16_t m_iSampleRate;
		parsing::Chunk* m_pDataChunk = nullptr;
	};

	//---------------------------------------------------------------------
	class SongResource : public SoundResource
	{
	public:
		SongResource();
	};

	//---------------------------------------------------------------------
	class TalkResource : public SoundResource
	{
	public:
		TalkResource();

		core::Data GetLipSyncData() const;
		void SetLipSyncChunk(parsing::Chunk* a_pChunk);
	private:
		parsing::Chunk* m_pLipSyncChunk = nullptr;
	};

	//---------------------------------------------------------------------
	class SFXResource : public SoundResource
	{
	public:
		SFXResource();
	};

	//---------------------------------------------------------------------
	class RoomResource : public Resource
	{
	public:
		RoomResource();

		size_t GetSize() const override;
		std::string GetDurationStr() override;
	};

	//---------------------------------------------------------------------
	class LocalScriptResource : public Resource
	{
	public:
		LocalScriptResource();

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);

		size_t GetSize() const override;

		std::string GetDurationStr() override;
	protected:
		parsing::Chunk* m_pDataChunk = nullptr;
	};

	//---------------------------------------------------------------------
	class GlobalScriptResource : public Resource
	{
	public:
		GlobalScriptResource();

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);

		size_t GetSize() const override;

		std::string GetDurationStr() override;
	protected:
		parsing::Chunk* m_pDataChunk = nullptr;
	};

	//---------------------------------------------------------------------
	class VerbScriptResource : public Resource
	{
	public:
		VerbScriptResource();

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);

		size_t GetSize() const override;

		std::string GetDurationStr() override;
	protected:
		parsing::Chunk* m_pDataChunk = nullptr;
	};

	//---------------------------------------------------------------------
	class RoomBackgroundResource : public Resource
	{
	public:
		RoomBackgroundResource();

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);

		size_t GetSize() const override;

		std::string GetDurationStr() override;
	protected:
		parsing::Chunk* m_pDataChunk = nullptr;
	};

	//---------------------------------------------------------------------
	class RoomImageResource : public Resource
	{
	public:
		RoomImageResource();

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);

		size_t GetSize() const override;

		std::string GetDurationStr() override;
	protected:
		parsing::Chunk* m_pDataChunk = nullptr;
	};
}