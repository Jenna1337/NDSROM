package com.data;

import java.io.IOException;
import java.io.InputStream;

/**
 * A class that acts like a byte array
 * @author Jonah Sloan
 *
 */
public class MBytes extends Number
{
	private final int length;
	private byte[] data;
	private class LengthMismatchError extends IllegalAccessError
	{
		public LengthMismatchError(int targlen){
			super("The length"+length+" does not match the length of required bytes("+targlen+").");
		}
	}
	public MBytes(int numbytes, final InputStream in)
	{
		this.length = numbytes;
		data = new byte[length];
		try{
			for(int i=0;i<length;++i)
				data[i]=(byte)in.read();
		}catch (Exception e) {
			throw new Error(e);
		}
	}
	public MBytes(int numbytes)
	{
		this.length = numbytes;
		data = new byte[length];
	}
	public MBytes readFromStream(final InputStream in) throws IOException
	{
		for(int i=0;i<length;++i)
			data[i]=(byte)in.read();
		return this;
	}
	private void check(int reqlen){
		if (length != reqlen) throw new LengthMismatchError(reqlen);
	}
	
	
	/*Taken from 
	 * https://www.daniweb.com/programming/software-development/code/216874/primitive-types-as-byte-arrays
	 */
	/**
	 * Returns the value of the specified number as a {@code short}.
	 * @return the numeric value represented by this object after conversion to type {@code short}.
	 */
	public short shortValue() {
		check(2);
		return ByteUtils.toShort(data);
	}
	/**
	 * Returns the value of the specified number as a {@code char}.
	 * @return the numeric value represented by this object after conversion to type {@code char}.
	 */
	public char charValue() {
		check(2);
		return ByteUtils.toChar(data);
	}
	/**
	 * Returns the value of the specified number as a {@code int}.
	 * @return the numeric value represented by this object after conversion to type {@code int}.
	 */
	public int intValue() {
		check(4);
		return ByteUtils.toInt(data);
	}
	/**
	 * Returns the value of the specified number as a {@code long}.
	 * @return the numeric value represented by this object after conversion to type {@code long}.
	 */
	public long longValue() {
		check(8);
		return ByteUtils.toLong(data);
	}
	/**
	 * Returns the value of the specified number as a {@code float}.
	 * @return the numeric value represented by this object after conversion to type {@code float}.
	 */
	public float floatValue() {
		//check(4);
		return Float.intBitsToFloat(this.intValue());
	}
	/**
	 * Returns the value of the specified number as a {@code double}.
	 * @return the numeric value represented by this object after conversion to type {@code double}.
	 */
	public double doubleValue() {
		//check(8);
		return Double.longBitsToDouble(this.longValue());
	}
	/**
	 * Returns the value of the specified number as a {@code boolean}.
	 * @return the numeric value represented by this object after conversion to type {@code boolean}.
	 */
	public boolean booleanValue() {
		return ByteUtils.toBoolean(data);
	}
}
