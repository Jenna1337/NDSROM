package com.nds.graphics;

import java.io.InputStream;
import com.nds.FileFormat;
import com.nds.FileFormatReader;

public class NCLR extends FileFormatReader
{
	int MagicID;
	int Constant;
	int SectionSize;
	short HeaderSize;
	short NumSections;
	
	public NCLR()
	{
		super(FileFormat.NCLR);
		// TODO Auto-generated constructor stub
	}
	public void readData(InputStream in)
	{
		// TODO Auto-generated method stub
		
	}
}
