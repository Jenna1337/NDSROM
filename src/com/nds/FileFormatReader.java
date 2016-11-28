package com.nds;

import java.io.InputStream;

public abstract class FileFormatReader
{
	public final FileFormat MAGIC_ID;
	
	protected byte[] MagicID;
	protected int section_size;// includes header
	protected short header_size;
	protected short num_sections;
	
	/*
	off siz 
	0x0 0x4 Magic ID  Identifies the file format.  
	0x4 0x4 Constant Always (0xFFFE0001) 
	0x8 0x4 Section Size  Size of this section, including the header.  
	0xC 0x2 Header Size  Size of this header. (Should always equal 0x10)  
	0xE 0x2 Number of Sections  The number of sub-sections in this section.  
	*/
	public FileFormatReader(FileFormat format, final InputStream in)
	{
		this.MAGIC_ID = FileFormat.valueOf(format.getMagicID());
		// TODO Auto-generated constructor stub
	}
	
}
