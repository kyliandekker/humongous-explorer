#pragma once

namespace humongousexplorer::imgui
{
	/// <summary>
	/// Generic UI view class used by all elements in the editor.
	/// </summary>
	class ImGuiUIView
	{
	public:
		/// <summary>
		/// Renders the element.
		/// </summary>
		virtual void Render() = 0;
	};
}