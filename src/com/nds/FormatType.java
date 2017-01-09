package com.nds;

import java.io.IOException;
import com.data.ROMDataInputStream;

public interface FormatType
{
	public void readData(final ROMDataInputStream in) throws IOException;
}
