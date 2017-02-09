#import "gui_cocoa.h"

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
	
	//Gtk::MessageDialog dialog("Done!");
	//dialog.set_secondary_text("Now you can safely make your game great again.");
	
	//dialog.run(); 
}

bool FindExt (const std::string &str, const std::string &suffix) {
	return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

@implementation gui
- (void)grui {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
    [NSApplication sharedApplication];
    
    //NSUInteger windowStyle = NSTitledWindowMask | NSClosableWindowMask | ~NSResizeableWindowMask;
    
    NSRect windowRect = NSMakeRect(0, 0, 550, 164);
    NSWindow * window = [[NSWindow alloc] initWithContentRect:windowRect
            styleMask:windowStyle
            backing:NSBackingStoreBuffered
            defer:NO];
    
    [window setTitle:@"MakeROMFS-GUI by MKGirlism"];
    [window setStyleMask:[window styleMask] & ~NSResizableWindowMask];
    
    [window autorelease];
    
    NSWindowController * windowController = [[NSWindowController alloc] initWithWindow:window];
    [windowController autorelease];
    
    m_Entry_Folder = [[NSTextField alloc] initWithFrame:NSMakeRect(10, 10, 441, 24)];
    [m_Entry_Folder setStringValue:@""];
    [m_Entry_Folder setEditable:NO];
	[m_Entry_Folder autorelease];
	
    m_Button_Folder = [[NSButton alloc] initWithFrame:NSMakeRect(460, 10, 80, 24)];
    [m_Button_Folder setTitle:@"Input Folder"];
    [m_Button_Folder setEnabled:YES];
    [m_Button_Folder setButtonType:NSMomentaryLightButton];
    [m_Button_Folder setBezelStyle:NSRoundedBezelStyle];
    [m_Button_Folder setTarget:self];
    [m_Button_Folder setAction:@selector(on_folder_clicked)];
    
    m_Entry_File = [[NSTextField alloc] initWithFrame:NSMakeRect(10, 50, 441, 24)];
    [m_Entry_File setStringValue:@""];
    [m_Entry_File setEditable:NO];
	[m_Entry_File autorelease];
	
    m_Button_File = [[NSButton alloc] initWithFrame:NSMakeRect(460, 50, 80, 24)];
    [m_Button_File setTitle:@"Output File"];
    [m_Button_File setEnabled:NO];
    [m_Button_File setButtonType:NSMomentaryLightButton];
    [m_Button_File setBezelStyle:NSRoundedBezelStyle];
    [m_Button_File setTarget:self];
    [m_Button_File setAction:@selector(on_file_clicked)];
	
    m_Button_Begin = [[NSButton alloc] initWithFrame:NSMakeRect(235, 130, 80, 24)];
    [m_Button_Begin setTitle:@"Output File"];
    [m_Button_Begin setEnabled:NO];
    [m_Button_Begin setButtonType:NSMomentaryLightButton];
    [m_Button_Begin setBezelStyle:NSRoundedBezelStyle];
    [m_Button_Begin setTarget:self];
    [m_Button_Begin setAction:@selector(on_begin_clicked)];
	
	[window orderFrontRegardless];
	[NSApp run];
	
	[pool drain];
}

- (void)on_folder_clicked {
    NSLog(@"Folder pressed!");
    [m_Entry_Folder setStringValue:@"KURWA"];
    [m_Button_File setEnabled:YES];
	/*Gtk::FileChooserDialog dialog("Choose ROMFS Directory",
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
	}*/
}

- (void)on_file_clicked {
    NSLog(@"File pressed!");
    [m_Entry_File setStringValue:@"KURWA"];
    [m_Button_Begin setEnabled:YES];
	/*Gtk::FileChooserDialog dialog("Save ROMFS File",
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
	}*/
}

- (void)on_begin_clicked {
	exec(rfolder, rfilename);
}

@end
