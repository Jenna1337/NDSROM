package com.data;

import java.io.EOFException;
import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;

public class ROMDataInputStream extends FilterInputStream
{
	protected ROMDataInputStream(InputStream in)
	{
		super(in);
		// TODO Auto-generated constructor stub
	}
	public byte readByte() throws IOException
	{
		int v=read();
		if(v==-1)
			throw new EOFException();
		return (byte)read();
	}
	public short readShort() throws IOException
	{
		short v=readByte();
		v<<=8;
		v|=readByte();
		return v;
	}
	public int readInt() throws IOException
	{
		int v=readShort();
		v<<=16;
		v|=readShort();
		return v;
	}
	public long readLong() throws IOException
	{
		long v=readInt();
		v<<=32;
		v|=readInt();
		return v;
	}
}
