#include <cassert>

#include <helib/archive/ArchiveSet.h>
#include <helib/core/DataStream.h>
#include <helib/core/Log.h>
#include <helib/parsing/Chunk.h>
#include <helib/parsing/ChunkIDs.h>
#include <helib/resources/resources/TalkResource.h>
#include <helib/building/HE4Builder.h>
#include <helib/building/HE0Builder.h>
#include <helib/building/ScriptBuilder.h>
#include <helib/audio/WaveLoader.h>

#include <chrono>
#include <cstdio>

class ScopeTimer
{
public:
	ScopeTimer(const char* a_pName)
		: m_pName(a_pName)
		, m_Start(std::chrono::steady_clock::now())
	{
	}

	~ScopeTimer()
	{
		const auto end = std::chrono::steady_clock::now();
		const std::chrono::duration<double> elapsed = end - m_Start;

		std::printf("[TIME] %-30s %.3f seconds\n", m_pName, elapsed.count());
	}

private:
	const char* m_pName;
	std::chrono::steady_clock::time_point m_Start;
};

using namespace humongousexplorer;

//---------------------------------------------------------------------
int main()
{
	core::InitializeLog();

	fs::path archivesPath = "C:/Program Files (x86)/Steam/steamapps/common/Spy Fox 3/SPYOZON.HE2";

	archive::ArchiveSet set;

	{
		ScopeTimer timer("Load Archives");
		bool loaded = set.LoadArchives(archivesPath);

		if (!loaded)
		{
			core::DestroyLog();
			return 1;
		}
	}

	archive::Archive* he4 = nullptr;
	archive::Archive* he2 = nullptr;
	archive::Archive* he0 = nullptr;
	archive::Archive* a = nullptr;
	for (std::unique_ptr<archive::Archive>& archive : set.GetArchives())
	{
		if (archive->GetType() == archive::ArchiveType::HE4)
		{
			he4 = archive.get();
		}
		else if (archive->GetType() == archive::ArchiveType::HE2)
		{
			he2 = archive.get();
		}
		else if (archive->GetType() == archive::ArchiveType::HE0)
		{
			he0 = archive.get();
		}
		else if (archive->GetType() == archive::ArchiveType::A)
		{
			a = archive.get();
		}
	}

	building::HE4Builder he4Builder;
	{
		ScopeTimer timer("Binded HE4");
		if (!he4Builder.Bind(set))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE4.");
			core::DestroyLog();
			return 0;
		}
	}

	building::ScriptBuilder scriptBuilder;
	{
		ScopeTimer timer("Binded scripts");
		if (!scriptBuilder.Bind(set))
		{
			core::Log(core::LogLevel::Error, "Could not bind scripts.");
			core::DestroyLog();
			return 0;
		}
	}

	building::HE0Builder he0Builder;
	{
		ScopeTimer timer("Binded HE0");
		if (!he0Builder.Bind(set))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0.");
			core::DestroyLog();
			return 0;
		}
	}

	std::vector<parsing::Chunk*> talkChunks;
	he2->GetRoot().TryFindChildren(parsing::TALK_CHUNK_ID, talkChunks);

	fs::path replacePath = "C:/Users/Kylian/Downloads/HumongousExplorer/replace/";
	std::error_code ec;
	fs::directory_iterator it(replacePath, ec);
	if (ec)
	{
		core::Log(core::LogLevel::Error, "Could not open replace directory: " + ec.message());
		core::DestroyLog();
		return 0;
	}

	for (const auto& entry : it)
	{
		if (!entry.is_regular_file())
		{
			continue;
		}

		const fs::path& filePath = entry.path();
		size_t number = std::stoi(filePath.filename());

		core::Data waveData;
		uint16_t waveSampleRate = 0;
		if (!audio::WaveLoader::Load(filePath, waveData, waveSampleRate))
		{
			core::Log(core::LogLevel::Error, "Could not load: \"" + filePath.string() + "\".");
			core::DestroyLog();
			return 0;
		}

		talkChunks[number]->TryFindChild(parsing::SDAT_CHUNK_ID)->SetData(waveData);
	}

	// First HE4. It does not change anything in HE0 or (A).
	{
		ScopeTimer timer("Built HE4");
		if (!he4Builder.Build())
		{
			core::Log(core::LogLevel::Error, "Could not build HE4.");
			core::DestroyLog();
			return 0;
		}
	}

	// Then scripts. This changes (A) and HE0.
	{
		ScopeTimer timer("Built scripts");
		if (!scriptBuilder.Build())
		{
			core::Log(core::LogLevel::Error, "Could not build scripts.");
			core::DestroyLog();
			return 0;
		}
	}

	// Last, build the HE0 again because all the changes in scripts, (A), HE2 are done.
	{
		ScopeTimer timer("Built HE0");
		if (!he0Builder.Build())
		{
			core::Log(core::LogLevel::Error, "Could not build HE0.");
			core::DestroyLog();
			return 0;
		}
	}

	fs::path newArchiveFolderPath = archivesPath.parent_path().string() + "/build";
	file::CreateFolder(newArchiveFolderPath);
	for (std::unique_ptr<archive::Archive>& archive : set.GetArchives())
	{
		core::DataStream data;
		archive->Build(data);
		fs::path newArchivePath = newArchiveFolderPath.string() + "/" + archive->GetName();
		file::SaveFile(newArchivePath, data);
	}

	core::DestroyLog();

	return 0;
}
