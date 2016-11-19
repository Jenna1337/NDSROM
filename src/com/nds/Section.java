package com.nds;

public abstract class Section implements FormatType
{
	private final String magicid;
	private final Class<? extends FileFormatReader> clazz;
	private Section(String magicid, Class<? extends FileFormatReader> clazz)
	{
		this.magicid = magicid;
		this.clazz = clazz;
	}
	public String getMagicID(){
		return magicid;
	}
	public FileFormatReader getFormatReader(){
		try
		{
			return clazz.newInstance();
		}
		catch(InstantiationException | IllegalAccessException e)
		{
			throw new InternalError(e);
		}
	}
}
