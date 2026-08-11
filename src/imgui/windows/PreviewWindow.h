#pragma once

#include "imgui/windows/HEBaseWindow.h"

struct ID3D11ShaderResourceView;

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// PreviewWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for previewing image resources with zoom controls and metadata.
	/// </summary>
	class PreviewWindow : public HEBaseWindow
	{
	public:
		/// <summary>
		/// Constructs a preview window.
		/// </summary>
		PreviewWindow();

		/// <summary>
		/// Renders the preview window content.
		/// </summary>
		void Update() override;
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		float m_fZoom = 1.0f;
		ImVec2 m_vPan = ImVec2(0.0f, 0.0f);

		static const int s_iPresetCount = 6;
		static const float s_aPresets[s_iPresetCount];
		static const char* s_aPresetLabels[s_iPresetCount];

		bool m_bShowCheckerboard = true;

		float m_fAudioPosition = 32.482f;
		float m_fAudioDuration = 83.456f;
		float m_fAudioVolume = 0.75f;
		bool m_bAudioPlaying = false;
		bool m_bAudioLoop = false;

		void RenderCheckerboard(const ImVec2& a_vMin, const ImVec2& a_vMax);
		void RenderImageControlsBar();
		void RenderImage();

		void RenderAudio();
		void RenderSoundControlsBar();
	};
}
