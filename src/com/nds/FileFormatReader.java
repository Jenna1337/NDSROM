package com.nds;

import java.io.InputStream;

public abstract class FileFormatReader implements FormatType
{
	private byte[] MagicID;
	
	/*
	off siz 
	0x0 0x4 Magic ID  Identifies the file format.  
	0x4 0x4 Constant Always (0xFFFE0001) 
	0x8 0x4 Section Size  Size of this section, including the header.  
	0xC 0x2 Header Size  Size of this header. (Should always equal 0x10)  
	0xE 0x2 Number of Sections  The number of sub-sections in this section.  
	*/
	public FileFormatReader(FileFormat format)
	{
		this.MagicID = format.getMagicID().getBytes();
	}
	public String getMagicID(){
		return new String(MagicID);
	}
	public abstract void readData(final InputStream in);
}
