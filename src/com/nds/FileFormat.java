package com.nds;

import com.nds.archives.*;
import com.nds.graphics.*;
import com.nds.sounds.*;
import com.nds.video.*;

public enum FileFormat
{
	NANR("RNAN", NANR.class),
	NCER("RECN", NCER.class),
	NCGR("RGCN", NCGR.class),
	NCLR("RLCN", NCLR.class),
	NSCR("RCSN", NSCR.class),
	NARC("NARC", NARC.class),
	SDAT("SDAT", SDAT.class),
	VX  ("VXDS", VX.class  ),
	;
	private final String magicid;
	private final Class<? extends FileFormatReader> clazz;
	private FileFormat(String magicid, Class<? extends FileFormatReader> clazz)
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
