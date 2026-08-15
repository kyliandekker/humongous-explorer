#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "core/Data.h"

#include <d3d11.h>

struct Color
{
	uint8_t r, g, b;
};

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

		virtual std::string GetSize() const = 0;
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

		virtual void Open() {};
		virtual void Replace(const core::Data& a_Data) {};
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

		std::string GetSize() const override;

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

		void Replace(const core::Data& a_Data) override;
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

		std::string GetSize() const override;
		std::string GetDurationStr() override;
	};

	//---------------------------------------------------------------------
	class LocalScriptResource : public Resource
	{
	public:
		LocalScriptResource();

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);

		std::string GetSize() const override;

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

		std::string GetSize() const override;

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

		std::string GetSize() const override;

		std::string GetDurationStr() override;
	protected:
		parsing::Chunk* m_pDataChunk = nullptr;
	};

	//---------------------------------------------------------------------
	class RoomBackgroundResource : public Resource
	{
	public:
		RoomBackgroundResource();
		~RoomBackgroundResource();

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);
		const core::Data& GetImageData() const;

		ID3D11ShaderResourceView* GetSRV();

		std::string GetSize() const override;

		std::string GetDurationStr() override;

		void Open() override;

		uint16_t GetWidth() const;
		uint16_t GetHeight() const;
		const std::vector<Color>& GetColors() const;
	protected:
		parsing::Chunk* m_pDataChunk = nullptr;
		core::Data m_ImageData;

		uint16_t m_iWidth = 0;
		uint16_t m_iHeight = 0;
		std::vector<Color> m_aColors;
		ID3D11ShaderResourceView* m_pSRV = nullptr;
	};

	//---------------------------------------------------------------------
	class RoomImageResource : public Resource
	{
	public:
		RoomImageResource();

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);

		std::string GetSize() const override;

		std::string GetDurationStr() override;

		void Open() override;
	protected:
		parsing::Chunk* m_pDataChunk = nullptr;
	};
}