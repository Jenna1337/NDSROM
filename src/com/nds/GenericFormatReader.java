package com.nds;

import java.io.IOException;
import com.data.ROMDataInputStream;

public abstract class GenericFormatReader implements FormatType
{
	protected int MagicID;
	protected int Constant;
	protected int SectionSize;
	protected short HeaderSize;
	protected short NumSections;
	
	/*
	off siz 
	0x0 0x4 Magic ID  Identifies the file format.  
	0x4 0x4 Constant Always (0xFFFE0001) 
	0x8 0x4 Section Size  Size of this section, including the header.  
	0xC 0x2 Header Size  Size of this header. (Should always equal 0x10)  
	0xE 0x2 Number of Sections  The number of sub-sections in this section.  
	*/
	public void readGenericData(final ROMDataInputStream in) throws IOException
	{
		MagicID = in.readInt();
		Constant = in.readInt();
		SectionSize = in.readInt();
		HeaderSize = in.readShort();
		NumSections = in.readShort();
		assert Constant==0xFFFE0001;
		assert HeaderSize==0x10;
	}
}
