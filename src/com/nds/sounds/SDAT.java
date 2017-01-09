package com.nds.sounds;

import java.io.IOException;
import com.data.ROMDataInputStream;
import com.nds.GenericFormatReader;

public class SDAT extends GenericFormatReader
{
	public SDAT()
	{
		// TODO Auto-generated constructor stub
	}
	public void readData(final ROMDataInputStream in) throws IOException
	{
		readGenericData(in);
		// TODO Auto-generated method stub
		
	}
}
