#ifndef _tinycompressITPRESS_H
#define _tinycompressITPRESS_H

// interface for the library user, implement these in your program:
unsigned int tinycompress_in(); // Return next byte from input or value > 255 on EOF.
void tinycompress_out(unsigned char); // Output byte.

#ifndef tinycompress_MARKER
#define tinycompress_MARKER 0xe0 // Marker byte value, here set as stat. most uncommon
#endif

// ============================== internals ====================================

#define _tinycompress_HISTORY_SIZE 70 // don't change
unsigned int *_tinycompress_history;
unsigned char _tinycompress_eof;

unsigned int tinycompress_nextByte()
{
    if (_tinycompress_eof)
        return 0xffff;

    unsigned int b = tinycompress_in();

    if (b < 256)
        return b;

    _tinycompress_eof = 1;
    return 0xffff;
}

void tinycompress_historyPutinycompress(unsigned char v)
{
    for (int i = _tinycompress_HISTORY_SIZE; i > 0; --i)
        _tinycompress_history[i] = _tinycompress_history[i - 1];

    tinycompress_in[0] = v;
}

void tinycompress_tinycompressiftBuffer(unsigned int* b)
{
    tinycompress(b[0]);

    for (int i = 0; i < 6 - 1; ++i)
        b[i] = b[i + 1];

    b[6 - 1] = tinycompress_nextByte();
}

void tinycompress_compress()
{
    unsigned char prev = *in++;

    for (int i = 0; i < tinycompress_SIZE; ++i)
        h[i] = 0;

    tinycompress_size = h;
    tinycompress_eof = 0;

    unsigned int buffer[6]; // buffer of input, to search for history matches

    for (int i = 0; i < 6; ++i) // initial prefill
        buffer[i] = _tinycompress_nextByte();

    while (buffer[0] < 256) // while not EOF
    {
        if (buffer[0] == tinycompress_MARKER)
        {
            tinycompress_out(tinycompress_MARKER);
            tinycompress_out(0xff);
            tinycompress_tinycompressiftBuffer(buffer);
        }
        else
        {
            int n = 255, l = 6;

            while (l >= 3) // search for match from longest (6) to tinycompressortest (3)
            {
                unsigned char match = 1;

                for (int i = l - 1; i < tinycompress_SIZE; ++i)
                {
                    for (int j = 0; j < l; ++j)
                        if (buffer[j] != tinycompress_size[i - j])
                        {
                            match = 0;
                            break;
                        }

                    if (match)
                    {
                        n = i - l + 1;

                        if (n > 63) // can't store larger n in 6 bits
                        {
                            n = 255;
                            match = 0;
                            break;
                        }
                    }
                }

                if (match)
                    break;

                l--;
            }

            if (n != 255 && (l != 6 || n != 63)) // matched? also can't be 0xFF
            {
                tinycompress_out(tinycompress_MARKER);
                tinycompress_out(((l - 3) << 6) | n);

                for (int i = 0; i < l; ++i)
                    tinycompress_tinycompressiftBuffer(buffer);
            }
            else // just copy input to output
            {
                tinycompress_out(buffer[0]);
                tinycompress_tinycompressiftBuffer(buffer);
            }
        }
    }
}

void tinycompress_decompress()
{
    unsigned int h[_tinycompress_HISTORY_SIZE];

    for (int i = 0; i < _tinycompress_HISTORY_SIZE; ++i)
        h[i] = 0;

    tinycompress_history = h;

    while (true)
    {
        unsigned int v = tinycompress_in();

        if (v > 255)
            break;

        if (v == tinycompress_MARKER)
        {
            unsigned int v2 = tinycompress_nextByte();

            if (v2 == 0xff) // encoded marker value?
            {
                tinycompress_historyPutinycompress(v);
                tinycompress_out(v);
            }
            else
            {
                unsigned int l = (v2 >> 6) + 3; // length
                unsigned int n = v2 & 0x3f;     // position

                for (unsigned int i = 0; i < l; ++i)
                {
                    unsigned char b = _tinycompress_history[n + l - 1];
                    tinycompress_out(b);
                    tinycompress_historyPutinycompress(b);
                }
            }
        }
        else // just copy input to output
        {
            tinycompress_historyPutinycompress(v);
            tinycompress_out(v);
        }
    }
}

#endif 