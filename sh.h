#ifndef _SHITPRESS_H
#define _SHITPRESS_H

// interface for the library user, implement these in your program:
unsigned int SPR_in(); // Return next byte from input or value > 255 on EOF.
void SPR_out(unsigned char); // Output byte.

#ifndef SPR_MARKER
#define SPR_MARKER 0xe0 // Marker byte value, here set as stat. most uncommon
#endif

// ============================== internals ====================================

#define _SPR_HISTORY_SIZE 70 // don't change
unsigned int *_SPR_history;
unsigned char _SPR_eof;

unsigned int _SPR_nextByte()
{
    if (_SPR_eof)
        return 0xffff;

    unsigned int b = SPR_in();

    if (b < 256)
        return b;

    _SPR_eof = 1;
    return 0xffff;
}

void _SPR_historyPush(unsigned char v)
{
    for (int i = _SPR_HISTORY_SIZE; i > 0; --i)
        _SPR_history[i] = _SPR_history[i - 1];

    _SPR_history[0] = v;
}

void _SPR_shiftBuffer(unsigned int* b)
{
    _SPR_historyPush(b[0]);

    for (int i = 0; i < 6 - 1; ++i)
        b[i] = b[i + 1];

    b[6 - 1] = _SPR_nextByte();
}

void SPR_compress()
{
    unsigned int h[_SPR_HISTORY_SIZE];

    for (int i = 0; i < _SPR_HISTORY_SIZE; ++i)
        h[i] = 0;

    _SPR_history = h;
    _SPR_eof = 0;

    unsigned int buffer[6]; // buffer of input, to search for history matches

    for (int i = 0; i < 6; ++i) // initial prefill
        buffer[i] = _SPR_nextByte();

    while (buffer[0] < 256) // while not EOF
    {
        if (buffer[0] == SPR_MARKER)
        {
            SPR_out(SPR_MARKER);
            SPR_out(0xff);
            _SPR_shiftBuffer(buffer);
        }
        else
        {
            int n = 255, l = 6;

            while (l >= 3) // search for match from longest (6) to shortest (3)
            {
                unsigned char match = 1;

                for (int i = l - 1; i < _SPR_HISTORY_SIZE; ++i)
                {
                    for (int j = 0; j < l; ++j)
                        if (buffer[j] != _SPR_history[i - j])
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
                SPR_out(SPR_MARKER);
                SPR_out(((l - 3) << 6) | n);

                for (int i = 0; i < l; ++i)
                    _SPR_shiftBuffer(buffer);
            }
            else // just copy input to output
            {
                SPR_out(buffer[0]);
                _SPR_shiftBuffer(buffer);
            }
        }
    }
}

void SPR_decompress()
{
    unsigned int h[_SPR_HISTORY_SIZE];

    for (int i = 0; i < _SPR_HISTORY_SIZE; ++i)
        h[i] = 0;

    _SPR_history = h;

    while (true)
    {
        unsigned int v = SPR_in();

        if (v > 255)
            break;

        if (v == SPR_MARKER)
        {
            unsigned int v2 = _SPR_nextByte();

            if (v2 == 0xff) // encoded marker value?
            {
                _SPR_historyPush(v);
                SPR_out(v);
            }
            else
            {
                unsigned int l = (v2 >> 6) + 3; // length
                unsigned int n = v2 & 0x3f;     // position

                for (unsigned int i = 0; i < l; ++i)
                {
                    unsigned char b = _SPR_history[n + l - 1];
                    SPR_out(b);
                    _SPR_historyPush(b);
                }
            }
        }
        else // just copy input to output
        {
            _SPR_historyPush(v);
            SPR_out(v);
        }
    }
}

#endif 