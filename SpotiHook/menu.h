#pragma once

namespace vars {
	bool playing = false;
	const char* lastSong;
	bool download = false;
}

namespace GUI {

	void showSong(const char* songName) {
		zgui::text("Now playing: ");
		zgui::same_line();
		zgui::text(songName);
	}

	void showButton() {
		zgui::toggle_button("Download", { 70, 20 }, vars::download);
	}

	void renderMenu() {
		zgui::poll_input("CEF-OSC-WIDGET");
		if (zgui::begin_window("advancedHook", { 300, 350 }, 5, zgui::zgui_window_flags_none))
		{
			zgui::begin_groupbox("Spotify", { 200, 200 }, zgui::zgui_groupbox_flags::zgui_groupbox_flags_legacy_design);
			if (vars::playing)
			{
				showSong(vars::lastSong);
				showButton();
			}
			zgui::end_groupbox();
			zgui::end_window();
		}
	}
}