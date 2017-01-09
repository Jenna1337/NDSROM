package com.nds.graphics;

import java.io.IOException;
import com.data.ROMDataInputStream;
import com.nds.GenericFormatReader;

public class NCER extends GenericFormatReader
{
	public NCER()
	{
		// TODO Auto-generated constructor stub
	}
	public void readData(final ROMDataInputStream in) throws IOException
	{
		readGenericData(in);
		// TODO Auto-generated method stub
		
	}
}
