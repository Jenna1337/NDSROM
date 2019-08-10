#!/usr/bin/env python2
#Make sure 'convert' from ImageMagick is in the same folder as this script.

import os, sys, zlib, struct
from subprocess import call

def write_png(buf, width, height): #http://code.activestate.com/recipes/577443-write-a-png-image-in-native-python/
    import zlib, struct
    width_byte_4 = width * 4
    raw_data = b"".join(b'\x00' + buf[span:span + width_byte_4] for span in range(0, (height - 1) * width * 4+1, width_byte_4))
    def png_pack(png_tag, data):
        chunk_head = png_tag + data
        return struct.pack("!I", len(data)) + chunk_head + struct.pack("!I", 0xFFFFFFFF & zlib.crc32(chunk_head))
    return b"".join([
        b'\x89PNG\r\n\x1a\n',
        png_pack(b'IHDR', struct.pack("!2I5B", width, height, 8, 6, 0, 0, 0)),
        png_pack(b'IDAT', zlib.compress(raw_data, 9)),
        png_pack(b'IEND', b'')])

def parsePalette(pData):
    '''
    0bbbbbgggggrrrrr
    '''
    palette = []
    for x in xrange(16):
        col = struct.unpack('<H', pData[x*2:x*2+2])[0]
        colr = int(round(((col & 0x1F) * 255) / 31.0)) #Extract the color, then convert from 5 to 8 bits.
        colg = int(round((((col >> 5) & 0x1F) * 255) / 31.0))
        colb = int(round((((col >> 10) & 0x1F) * 255) / 31.0))
        palette.append([colr, colg, colb])
    
    return palette

def parseTiming(animData):
    '''
    one byte frame delay(in 60ths of a second)
    one byte vhpppfff
          v Vertical flip
          h Horizontal flip
        ppp Palette
        fff Frame
    '''
    
    timing = []
    for x in xrange(64): #Pretty sure there can be a max of 64 frames of animation
        flip = 0
        flop = 0
        delay = struct.unpack('<B', animData[x*2])[0]
        frame = struct.unpack('<B', animData[x*2+1])[0]
        if delay == 0 and frame == 0: #End of animation
            break
        if delay == 0: #Jelly cars 2, for example, does this. That game only has one frame, but it's timing goes 01 00 00 01
            print '0 delay frame, skipping!'
            continue
        if frame & 0x80: #Vertical flip
            flip = 1
        if frame & 0x40: #Horizontal flip
            flop = 1
        palette = (frame >> 3) & 7
        timing.append([delay, frame & 7, palette, flip, flop])

    return timing

def parseIcon(iconData, palette):
    buf = [0] * (32 * 32 * 4) #32x32 pixels at 32bpp
    iconData = map(ord, iconData)
    iconData = [i for sub in [(x & 0xF, (x >> 4) & 0xF) for x in iconData] for i in sub] #kek, based on http://stackoverflow.com/a/15269211
    
    #based on https://github.com/pleonex/tinke/blob/23ba755a86c9e1155c4f105252b0a55c979691ac/Tinke/Nitro/NDS.cs#L315
    i = 0
    for ty in xrange(4):
        for tx in xrange(4):
            for y in xrange(8):
                for x in xrange(8):
                    tmp1 = (y + (ty * 8)) * 32
                    tmp2 = x + (tx * 8)
                    offs = (tmp1 + tmp2) * 4
                    buf[offs] = palette[iconData[i]][0]
                    buf[offs+1] = palette[iconData[i]][1]
                    buf[offs+2] = palette[iconData[i]][2]
                    buf[offs+3] = 0x00 if iconData[i] == 0 else 0xFF
                    i += 1
    
    return buf

if __name__ == '__main__':
    if len(sys.argv) < 2 or not os.path.isfile(sys.argv[1]):
        sys.exit('usage: %s file.nds' % (os.path.basename(__file__)))
    
    iconFname = os.path.splitext(os.path.basename(sys.argv[1]))[0]
    
    nds = open(sys.argv[1], 'rb')
    nds.seek(0)
    
    nds.seek(0x68)
    iconOffs = struct.unpack('<I', nds.read(4))[0]
    
    if iconOffs == 0:
        print 'No icon!'
        sys.exit()
    
    nds.seek(iconOffs)
    iconVer = struct.unpack('<H', nds.read(2))[0]
    if not iconVer in [0x0001, 0x0002, 0x0103]:
        sys.exit('Unknown icon version: 0x%X' % (iconVer))
    
    if iconVer != 0x103: #If not a DSi icon, then just parse the NDS icon
        nds.seek(iconOffs + 0x220)
        pData = nds.read(0x20)
        palette = parsePalette(pData) #NDS icon palette starts 0x220 into the icon data
        nds.seek(iconOffs + 0x20)
        iconData = nds.read(0x200)
        iconImage = parseIcon(iconData, palette) #NDS icon starts 0x20 into the icon data
        with open('%s.png' % (iconFname), 'wb') as fh:
            fh.write(write_png(''.join(chr(x) for x in iconImage), 32, 32))
        print 'Icon created for "%s"!' % (iconFname)
        sys.exit()
    
    tmpDir = 'tmpFrames'
    if not os.path.exists('./%s' % (tmpDir)):
        os.makedirs('./%s' % (tmpDir))
    
    nds.seek(iconOffs + 0x2340)
    animData = nds.read(0x80)
    iconAnim = parseTiming(animData) #Animation timing data starts at 0x2340 into the icon data
    
    palette = []
    for x in xrange(8):
        nds.seek(iconOffs + 0x2240 + (0x20 * x))
        pData = nds.read(0x20)
        palette.append(parsePalette(pData)) #DSi icon palette starts 0x2240 into the icon data

    for icon in iconAnim: #Lazy, if the same frame/palette combo appears more than once in the animation, this will write it multiple times
        nds.seek(iconOffs + 0x1240 + (0x200 * icon[1]))
        iconData = nds.read(0x200)
        iconDataDSi = parseIcon(iconData, palette[icon[2]]) #DSi icon starts 0x1240 into the icon data
        iconName = '%s_icon_%u_%u' % (iconFname, icon[1], icon[2])
        with open('./%s/%s.png' % (tmpDir, iconName), 'wb') as fh:
            fh.write(write_png(''.join(chr(x) for x in iconDataDSi), 32, 32))
    
    resizeCmd = ''#'-filter point -resize 64x64'
    command = 'convert %s -dispose Background ' % (resizeCmd)
    for timing in iconAnim:
        flip = ''
        if timing[3]: flip += '-flip '
        if timing[4]: flip += '-flop '
        command += '%s -delay %u %s "%s/%s_icon_%u_%u.png" %s ' % (resizeCmd, round((timing[0] * 100) / 60.0), flip, tmpDir, iconFname, timing[1], timing[2], flip)
    command += '%s -loop 0 "%s.gif"' % (resizeCmd, iconFname)
    
    call(command)
    print 'Icon created for "%s"!' % (iconFname)
    
    nds.close()