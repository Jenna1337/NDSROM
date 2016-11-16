package com.nds;

public abstract class FileFormat
{
	public final MagicID MAGIC_ID;
	
	private FileFormat()
	{
		MAGIC_ID = null;
	}
	public FileFormat(final String id)
	{
		this.MAGIC_ID = MagicID.valueOf(id);
		// TODO Auto-generated constructor stub
	}
	
}
