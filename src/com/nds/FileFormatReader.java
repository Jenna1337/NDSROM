package com.nds;

public abstract class FileFormatReader
{
	public final FileFormat MAGIC_ID;
	
	
	public FileFormatReader(FileFormat format)
	{
		this.MAGIC_ID = FileFormat.valueOf(format.getMagicID());
		// TODO Auto-generated constructor stub
	}
	
}
