package com.nds.graphics;

import java.awt.Color;
import java.io.IOException;
import com.data.ROMDataInputStream;
import com.nds.GenericFormatReader;

public class NCLR extends GenericFormatReader
{
	public NCLR()
	{
		// TODO Auto-generated constructor stub
	}
	public void readData(final ROMDataInputStream in) throws IOException
	{
		readGenericData(in);
		// TODO Auto-generated method stub
		
	}
	private Color parseColorBGR555(final short data) throws IOException
	{
		//Format is BGR555 (XBBBBBGGGGGRRRRR).
		int R = data & 0b11111;
		int G = (data>>5) & 0b11111;
		int B = (data>>10) & 0b11111;
		@SuppressWarnings("unused")
		boolean X = (data>>15)==1;
		return new Color(R*1f/0b11111, G*1f/0b11111, B*1f/0b11111);
	}
}
