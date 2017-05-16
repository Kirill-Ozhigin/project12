#ifndef __WAVE_H
#define __WAVE_H 1

#include "../include/PlatformDefines.h"

#ifndef GUID_DEFINED
#define GUID_DEFINED 1
#if defined(__midl)
typedef struct {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	byte           Data4[8];
} GUID;
#else
typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;
#endif
#endif

#ifndef WAVE_FORMAT_PCM
/* OLD general waveform format structure (information common to all formats) */
typedef struct waveformat_tag {
	unsigned short wFormatTag; /* format type */
	unsigned short nChannels; /* number of channels (i.e. mono, stereo, etc.) */
	unsigned long nSamplesPerSec; /* sample rate */
	unsigned long nAvgBytesPerSec; /* for buffer estimation */
	unsigned short nBlockAlign; /* block size of data */
} WAVEFORMAT, *PWAVEFORMAT;

/* flags for wFormatTag field of WAVEFORMAT */
#define WAVE_FORMAT_PCM 1

/* specific waveform format structure for PCM data */
typedef struct pcmwaveformat_tag {
	WAVEFORMAT  wf;
	unsigned short wBitsPerSample;
} PCMWAVEFORMAT, *PPCMWAVEFORMAT;
#endif /* WAVE_FORMAT_PCM */

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_ 1
typedef struct tWAVEFORMATEX
{
	unsigned short wFormatTag; /* format type */
	unsigned short nChannels; /* number of channels (i.e. mono, stereo...) */
	unsigned long nSamplesPerSec; /* sample rate */
	unsigned long nAvgBytesPerSec; /* for buffer estimation */
	unsigned short nBlockAlign; /* block size of data */
	unsigned short wBitsPerSample; /* number of bits per sample of mono data */
	unsigned short cbSize; /* the count in bytes of the size of */
						   /* extra information (after cbSize) */
} WAVEFORMATEX, *PWAVEFORMATEX;
#endif /* _WAVEFORMATEX_ */

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_ 1
typedef struct {
	WAVEFORMATEX Format;
	union {
		unsigned short wValidBitsPerSample; /* bits of precision  */
		unsigned short wSamplesPerBlock; /* valid if wBitsPerSample==0 */
		unsigned short wReserved; /* If neither applies, set to zero. */
	} Samples;
	unsigned long dwChannelMask; /* which channels are */
								 /* present in stream  */
	GUID SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_

#ifndef _SPEAKER_POSITIONS_
#define _SPEAKER_POSITIONS_ 1
// Speaker Positions for dwChannelMask in WAVEFORMATEXTENSIBLE:
#define SPEAKER_FRONT_LEFT              0x1
#define SPEAKER_FRONT_RIGHT             0x2
#define SPEAKER_FRONT_CENTER            0x4
#define SPEAKER_LOW_FREQUENCY           0x8
#define SPEAKER_BACK_LEFT               0x10
#define SPEAKER_BACK_RIGHT              0x20
#define SPEAKER_FRONT_LEFT_OF_CENTER    0x40
#define SPEAKER_FRONT_RIGHT_OF_CENTER   0x80
#define SPEAKER_BACK_CENTER             0x100
#define SPEAKER_SIDE_LEFT               0x200
#define SPEAKER_SIDE_RIGHT              0x400
#define SPEAKER_TOP_CENTER              0x800
#define SPEAKER_TOP_FRONT_LEFT          0x1000
#define SPEAKER_TOP_FRONT_CENTER        0x2000
#define SPEAKER_TOP_FRONT_RIGHT         0x4000
#define SPEAKER_TOP_BACK_LEFT           0x8000
#define SPEAKER_TOP_BACK_CENTER         0x10000
#define SPEAKER_TOP_BACK_RIGHT          0x20000

// Bit mask locations reserved for future use
#define SPEAKER_RESERVED                0x7FFC0000

// Used to specify that any possible permutation of speaker configurations
#define SPEAKER_ALL                     0x80000000
#endif // _SPEAKER_POSITIONS_

typedef struct
{
	char szRIFF[4];
	unsigned long ulRIFFSize;
	char szWAVE[4];
} WAVEFILEHEADER;

typedef struct
{
	char szChunkName[4];
	unsigned long ulChunkSize;
} RIFFCHUNK;

typedef struct
{
	unsigned short usFormatTag;
	unsigned short usChannels;
	unsigned long ulSamplesPerSec;
	unsigned long ulAvgBytesPerSec;
	unsigned short usBlockAlign;
	unsigned short usBitsPerSample;
	unsigned short usSize;
	unsigned short usReserved;
	unsigned long ulChannelMask;
	GUID guidSubFormat;
} WAVEFMT;



#endif // !__WAVE_H
