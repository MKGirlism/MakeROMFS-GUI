#ifndef GUI_COCOA_H
#define GUI_COCOA_H

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#include "romfs.h"
#include "ivfc.h"
#include "ByteBuffer.h"
#include <fstream>
#include <string.h>
#include <iostream>

@interface gui : NSView
    - (void)grui;
    
    char *rfilename;
    char *rfolder;
	
    - (void)on_folder_clicked;
    - (void)on_file_clicked;
    - (void)on_begin_clicked;
    
    NSTextField * m_Entry_Folder;
    NSTextField * m_Entry_File;
    NSButton * m_Button_Folder;
    NSButton * m_Button_File;
    NSButton * m_Button_Begin;
@end

#endif
