#include "gui_gtkmm.h"

void exec (char *folder, char *filename) {
	// create romfs
	Romfs romfs;
	romfs.CreateRomfs(folder);

	Ivfc ivfc;
	ivfc.CreateIvfcHashTree(romfs.data_blob(), romfs.data_size());

	// write to file
	std::ofstream outfile(filename, std::ofstream::binary);

	// header
	outfile.write((const char*)ivfc.header_blob(), ivfc.header_size());

	// level2 aka actual ROMFS
	for (u32 i = 0; i < romfs.data_size() / Ivfc::kBlockSize; i++)
	{
		outfile.write((const char*)romfs.data_blob() + i*Ivfc::kBlockSize, Ivfc::kBlockSize);
	}
	if (romfs.data_size() % Ivfc::kBlockSize)
	{
		u8 block[Ivfc::kBlockSize] = { 0 };
		memcpy(block, romfs.data_blob() + (romfs.data_size() / Ivfc::kBlockSize)*Ivfc::kBlockSize, romfs.data_size() % Ivfc::kBlockSize);
		outfile.write((const char*)block, Ivfc::kBlockSize);
	}

	// level0 & level1
	outfile.write((const char*)ivfc.level0_blob(), ivfc.level0_size());
	outfile.write((const char*)ivfc.level1_blob(), ivfc.level1_size());
	
	Gtk::MessageDialog dialog("Done!");
	dialog.set_secondary_text("Now you can safely make your game great again.");
	
	dialog.run(); 
}

bool FindExt (const std::string &str, const std::string &suffix) {
	return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

gui::gui () {
	m_Entry_Folder.set_size_request(441, 24);
	m_Entry_Folder.set_text("");
	m_Entry_Folder.set_sensitive(false);
	m_Fixed.put(m_Entry_Folder, 0, 0);
	
	m_Button_Folder.add_label ("Input Folder");
	m_Button_Folder.set_size_request(80, 24);
	m_Button_Folder.set_sensitive (true);
	m_Fixed.put(m_Button_Folder, 450, 0);
	
	m_Entry_File.set_size_request(441, 24);
	m_Entry_File.set_text("");
	m_Entry_File.set_sensitive(false);
	m_Fixed.put(m_Entry_File, 0, 40);
	
	m_Button_File.add_label ("Output File");
	m_Button_File.set_size_request(80, 24);
	m_Button_File.set_sensitive (false);
	m_Fixed.put(m_Button_File, 450, 40);
	
	m_Button_Begin.add_label ("Begin");
	m_Button_Begin.set_size_request(80, 24);
	m_Button_Begin.set_sensitive (false);
	m_Fixed.put(m_Button_Begin, 225, 120);
	
	set_title("Make-ROMFS-GUI by MKGirlism");
	set_border_width(10);
	set_resizable (false);
	
	add(m_Fixed);
	
	m_Button_Folder.signal_clicked().connect(sigc::mem_fun(*this,
				&gui::on_folder_clicked));
	
	m_Button_File.signal_clicked().connect(sigc::mem_fun(*this,
				&gui::on_file_clicked) );
	
	m_Button_Begin.signal_clicked().connect(sigc::mem_fun(*this,
				&gui::on_begin_clicked) );
	
	show_all_children();
	
	set_icon_from_file("/usr/share/icons/makeromfs-gui.png");
}

gui::~gui() {}

void gui::on_folder_clicked () {
	Gtk::FileChooserDialog dialog("Choose ROMFS Directory",
			Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	dialog.set_transient_for(*this);
	
	//Add response buttons the the dialog:
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);
	
	int result = dialog.run();
	
	//Handle the response:
	switch(result) {
		case(Gtk::RESPONSE_OK): {
			std::cout << "選択を押した。" << std::endl;
			std::cout << "フォールダは：" << dialog.get_filename() << std::endl;
			std::string fname = dialog.get_filename();
			
			char *fn;
			std::string finame = fname;
			fn = new char [finame.size()+1];
			strcpy(fn, finame.c_str());
			
			rfolder = fn;
			m_Button_File.set_sensitive (true);
			m_Entry_Folder.set_text("" + fname);
			
			break;
		}
		case(Gtk::RESPONSE_CANCEL): {
			std::cout << "キャンセルを押した。" << std::endl;
			break;
		}
		default: {
			std::cout << "不明なボタンを押した。" << std::endl;
			break;
		}
	}
}

void gui::on_file_clicked () {
	Gtk::FileChooserDialog dialog("Save ROMFS File",
			Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);
	
	// Add response buttons the the dialog:
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);
	
	// Add filters, so that only certain file types can be selected:
	auto filter_romfs = Gtk::FileFilter::create();
	filter_romfs->set_name("ROMFS files");
	filter_romfs->add_pattern("*.romfs");
	dialog.add_filter(filter_romfs);
	
	//Show the dialog and wait for a user response:
	int result = dialog.run();
	
	//Handle the response:
	switch(result) {
		case(Gtk::RESPONSE_OK): {
			std::cout << "オープンを押した。" << std::endl;
			std::cout << "ファイルは：" <<  dialog.get_filename() << std::endl;
			std::string fname = dialog.get_filename();
			
			if (!FindExt(fname, ".romfs")) {
				fname = fname + ".romfs";
			}
			
			char *fn;
			std::string finame = fname;
			fn = new char [finame.size()+1];
			strcpy(fn, finame.c_str());
			
			rfilename = fn;
			m_Button_Begin.set_sensitive (true);
			m_Entry_File.set_text("" + fname);
			
			break;
		}
		case(Gtk::RESPONSE_CANCEL): {
			std::cout << "キャンセルを押した。" << std::endl;
			break;
		}
		default: {
			std::cout << "不明なボタンを押した。" << std::endl;
			break;
		}
	}
}

void gui::on_begin_clicked () {
	exec(rfolder, rfilename);
}
