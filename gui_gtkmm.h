#ifndef GUI_GTKMM_H
#define GUI_GTKMM_H

#include <gtkmm.h>
#include "romfs.h"
#include "ivfc.h"
#include "ByteBuffer.h"
#include <fstream>
#include <string.h>
#include <iostream>

class gui : public Gtk::Window {
	public:
		gui();
		virtual ~gui();
		
		char *rfilename;
		char *rfolder;
	
	protected:
		void on_folder_clicked();
		void on_file_clicked();
		void on_begin_clicked();
		
		Gtk::Fixed m_Fixed;
		Gtk::Entry m_Entry_Folder;
		Gtk::Entry m_Entry_File;
		Gtk::Button m_Button_Folder;
		Gtk::Button m_Button_File;
		Gtk::Button m_Button_Begin;
};

#endif
