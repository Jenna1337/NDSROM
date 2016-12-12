package com.nds;

import java.io.InputStream;

public interface FormatType
{
	public String getMagicID();
	public void readData(final InputStream in);
}
