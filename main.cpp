#ifdef __linux__
#include "gui_gtkmm.h"
#elif _WIN32
#include "gui_winapi.h"
#elif __APPLE__
#include "gui_cocoa.h"
#endif

#ifdef __linux__
int main(int argc, char *argv[]) {
	if (argc == 2 ||  argc > 3) {
			printf("MakeROMFS\n");
			printf("使い方：%s <romfs dir> <romfs bin>\n", argv[0]);
			return 1;
	}
	else if (argc > 2 && argc == 3) {
		// create romfs
		Romfs romfs;
		romfs.CreateRomfs(argv[1]);
		
		Ivfc ivfc;
		ivfc.CreateIvfcHashTree(romfs.data_blob(), romfs.data_size());
		
		// write to file
		std::ofstream outfile(argv[2], std::ofstream::binary);
		
		// header
		outfile.write((const char*)ivfc.header_blob(), ivfc.header_size());
		
		// level2 aka actual ROMFS
		for (u32 i = 0; i < romfs.data_size() / Ivfc::kBlockSize; i++) {
			outfile.write((const char*)romfs.data_blob() + i*Ivfc::kBlockSize, Ivfc::kBlockSize);
		}
		if (romfs.data_size() % Ivfc::kBlockSize) {
			u8 block[Ivfc::kBlockSize] = { 0 };
			memcpy(block, romfs.data_blob() + (romfs.data_size() / Ivfc::kBlockSize)*Ivfc::kBlockSize, romfs.data_size() % Ivfc::kBlockSize);
			outfile.write((const char*)block, Ivfc::kBlockSize);
		}
		
		// level0 & level1
		outfile.write((const char*)ivfc.level0_blob(), ivfc.level0_size());
		outfile.write((const char*)ivfc.level1_blob(), ivfc.level1_size());
		
		std::cout << "完了した。" << std::endl;
		
		return 0;
	}
	else {
		auto app = Gtk::Application::create(argc, argv, "org.dshack.makeromfsgui");
		gui rui;
		
		return app->run(rui);
	}
}
#elif _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	gui rui(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	rui.show();
}
#elif __APPLE__
int main (int argc, char *argv[]) {
    if (argc == 2 ||  argc > 3) {
			NSLog(@"MakeROMFS\n");
			NSLog(@"使い方：%s <romfs dir> <romfs bin>\n", argv[0]);
			return 1;
	}
	else if (argc > 2 && argc == 3) {
		// create romfs
		Romfs romfs;
		romfs.CreateRomfs(argv[1]);
		
		Ivfc ivfc;
		ivfc.CreateIvfcHashTree(romfs.data_blob(), romfs.data_size());
		
		// write to file
		std::ofstream outfile(argv[2], std::ofstream::binary);
		
		// header
		outfile.write((const char*)ivfc.header_blob(), ivfc.header_size());
		
		// level2 aka actual ROMFS
		for (u32 i = 0; i < romfs.data_size() / Ivfc::kBlockSize; i++) {
			outfile.write((const char*)romfs.data_blob() + i*Ivfc::kBlockSize, Ivfc::kBlockSize);
		}
		if (romfs.data_size() % Ivfc::kBlockSize) {
			u8 block[Ivfc::kBlockSize] = { 0 };
			memcpy(block, romfs.data_blob() + (romfs.data_size() / Ivfc::kBlockSize)*Ivfc::kBlockSize, romfs.data_size() % Ivfc::kBlockSize);
			outfile.write((const char*)block, Ivfc::kBlockSize);
		}
		
		// level0 & level1
		outfile.write((const char*)ivfc.level0_blob(), ivfc.level0_size());
		outfile.write((const char*)ivfc.level1_blob(), ivfc.level1_size());
		
		NSLog(@"完了した。\n");
		
		return 0;
	}
	else {
		gui *rui = [[gui alloc]init];
        [rui grui];
        return 0;
	}
}
#endif
