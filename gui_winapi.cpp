#include "gui_winapi.h"

HWND m_Entry_Folder;
HWND m_Button_Folder;
HWND m_Entry_File;
HWND m_Button_File;
HWND m_Button_Begin;
		
char *rfilename;
char *rfolder;

//BOOL on_folder_clicked (HWND hwnd, LPCTSTR szCurrent, LPTSTR szPath);
void on_folder_clicked ();
void on_file_clicked ();
void on_begin_clicked ();

OPENFILENAME ofn;
char szFile[260];
OPENFILENAMEA ofnf;
char szFold[260];
HWND hwnd;
HANDLE hf;

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
	
	::MessageBox(hwnd, "Now you can safely make your game great again.", "Done!", MB_OK); 
}

bool FindExt (const std::string &str, const std::string &suffix) {
	return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

LRESULT CALLBACK gui::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CREATE:
			m_Entry_Folder = CreateWindow("EDIT", rfolder,
				WS_BORDER | WS_CHILD | WS_VISIBLE | WS_DISABLED,
				10, 10, 441, 24, hwnd, NULL, NULL, NULL);
			Edit_Enable(m_Entry_Folder, false);
			
			m_Button_Folder = CreateWindow("BUTTON", "Input Folder",
				WS_VISIBLE | WS_CHILD | WS_BORDER,
				460, 10, 90, 24, hwnd, (HMENU) 1, NULL, NULL);
			
			m_Entry_File = CreateWindow("EDIT", rfilename,
				WS_BORDER | WS_CHILD | WS_VISIBLE | WS_DISABLED,
				10, 50, 441, 24, hwnd, NULL, NULL, NULL);
			
			m_Button_File = CreateWindow("BUTTON", "Output File",
				WS_VISIBLE | WS_CHILD | WS_BORDER | WS_DISABLED,
				460, 50, 90, 24, hwnd, (HMENU) 2, NULL, NULL);
			
			m_Button_Begin = CreateWindow("BUTTON", "Begin",
				WS_VISIBLE | WS_CHILD | WS_BORDER | WS_DISABLED,
				235, 130, 90, 24, hwnd, (HMENU) 3, NULL, NULL);
			
			break;
		
		case WM_KEYDOWN:
			break;
		
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case 1:
					on_folder_clicked ();
					EnableWindow(m_Button_File, true);
					SetWindowText (m_Entry_Folder, (LPSTR) rfolder);
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
					break;
				
				case 2:
					on_file_clicked ();
					EnableWindow(m_Button_Begin, true);
					SetWindowText (m_Entry_File, (LPSTR) rfilename);
					RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
					break;
				
				case 3:
					exec (rfolder, rfilename);
					break;
			}
			break;
		
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	
	return 0;
}

gui::gui (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wind;
	wind.cbSize = sizeof(wind);
	wind.style = CS_HREDRAW | CS_VREDRAW;
	wind.lpfnWndProc = &WndProc;
	wind.cbClsExtra = 0;
	wind.cbWndExtra = 0;
	wind.hInstance = hInstance;
    wind.hIcon = NULL;
    wind.hIconSm = NULL;
    wind.hCursor = NULL;
	wind.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wind.lpszMenuName = NULL;
	wind.lpszClassName = "gui_class";
	
    if (RegisterClassEx(&wind)) {
		hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "gui_class", "MakeROMFS-GUI by MKGirlism", 
			WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, 570, 194, NULL, NULL, NULL, NULL);
		
		Edit_Enable(m_Entry_Folder, false);
		Button_Enable(m_Button_Folder, true);
		Edit_Enable(m_Entry_File, false);
		Button_Enable(m_Button_File, false);
		Button_Enable(m_Button_Begin, false);
		
		ShowWindow(this->hwnd, SW_NORMAL);
		UpdateWindow(this->hwnd);
		
		MSG msg;
		
		while (GetMessage(&msg, NULL, 0, 0) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

gui::~gui() {}

void on_folder_clicked () {
	BROWSEINFO bi = { 0 };
    bi.lpszTitle = "Choose ROMFS Directory";
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
	
    if ( pidl != 0 ) {
        // get the name of the folder
        TCHAR path[MAX_PATH];
        if ( SHGetPathFromIDList ( pidl, path ) ) {
			std::string fname = path;
			
			char *fn;
			std::string finame = fname;
			fn = new char [finame.size()+1];
			strcpy(fn, finame.c_str());
			
			rfolder = fn;
        }

        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) ) {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    }
}

void on_file_clicked () {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "ROMFS Files\0*.romfs\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	if (GetSaveFileName(&ofn) == TRUE) 
		hf = CreateFile(ofn.lpstrFile, 
                    GENERIC_READ,
                    0,
                    (LPSECURITY_ATTRIBUTES) NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    (HANDLE) NULL);
	
	std::string fname = ofn.lpstrFile;
	
	if (!FindExt(fname, ".romfs")) {
		fname = fname + ".romfs";
	}
	
	char *fn;
	std::string finame = fname;
	fn = new char [finame.size()+1];
	strcpy(fn, finame.c_str());
	
	rfilename = fn;
}

void on_begin_clicked () {
	exec(rfolder, rfilename);
}

void gui::show() {
	ShowWindow(this->hwnd, SW_NORMAL);
	UpdateWindow(this->hwnd);
}
