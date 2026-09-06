#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <d3d11.h>

// helib
#include <helib/core/Data.h>

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
		virtual std::string GetDurationStr() const
		{
			return "";
		}
		virtual std::string GetDimensions() const
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

		void SetChunk(parsing::Chunk* a_pChunk);

		virtual void Open() {};
		virtual void Replace(const core::Data& a_Data) {};
	protected:
		parsing::Chunk* m_pChunk = nullptr;
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

		std::string GetDurationStr() const override;
	protected:
		uint16_t m_iSampleRate = 0;
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
	};

	//---------------------------------------------------------------------
	class LocalScriptResource : public Resource
	{
	public:
		LocalScriptResource();

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);

		std::string GetSize() const override;
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
	protected:
		parsing::Chunk* m_pDataChunk = nullptr;
	};

	//---------------------------------------------------------------------
	class ImageResource : public Resource
	{
	public:
		ImageResource();
		~ImageResource();

		core::Data GetData() const;
		void SetDataChunk(parsing::Chunk* a_pChunk);
		const core::Data& GetImageData() const;

		ID3D11ShaderResourceView* GetSRV();

		std::string GetSize() const override;

		std::string GetDimensions() const override;
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
	class RoomBackgroundResource : public ImageResource
	{
	public:
		RoomBackgroundResource();

		void Open() override;
	};

	//---------------------------------------------------------------------
	class RoomImageResource : public ImageResource
	{
	public:
		RoomImageResource();

		void Open() override;

		bool ShowAsLayer() const;
		void SetShowAsLayer(bool a_bShowAsLayer);
	private:
		bool m_bShowAsLayer = false;
	};
}