#ifndef __GLVersion_H
#define __GLVersion_H 1

struct GLVersion
{
	unsigned int version;

	bool ver_1_0(void) const { return version & 0x00001; }
	bool ver_1_1(void) const { return version & 0x00002; }
	bool ver_1_2(void) const { return version & 0x00004; }
	bool ver_1_3(void) const { return version & 0x00008; }
	bool ver_1_4(void) const { return version & 0x00010; }
	bool ver_1_5(void) const { return version & 0x00020; }
	bool ver_2_0(void) const { return version & 0x00040; }
	bool ver_2_1(void) const { return version & 0x00080; }
	bool ver_3_0(void) const { return version & 0x00100; }
	bool ver_3_1(void) const { return version & 0x00200; }
	bool ver_3_2(void) const { return version & 0x00400; }
	bool ver_3_3(void) const { return version & 0x00800; }
	bool ver_4_0(void) const { return version & 0x01000; }
	bool ver_4_1(void) const { return version & 0x02000; }
	bool ver_4_2(void) const { return version & 0x04000; }
	bool ver_4_3(void) const { return version & 0x08000; }
	bool ver_4_4(void) const { return version & 0x10000; }
	bool ver_4_5(void) const { return version & 0x20000; }

	GLVersion(int major_version, int minor_version)
		: version(0ULL)
	{
		switch (major_version)
		{
		case 1:
		{
			version = 0x00001;
			switch (minor_version)
			{
			case 0:
				return;
			case 1:
				version |= 0x00002;
				return;
			case 2:
				version |= 0x00002 | 0x00004;
				return;
			case 3:
				version |= 0x00002 | 0x00004 | 0x00008;
				return;
			case 4:
				version |= 0x00002 | 0x00004 | 0x00008 | 0x00010;
				return;
			case 5:
				version |= 0x00002 | 0x00004 | 0x00008 | 0x00010 | 0x00020;
				return;
			}
		}
		return;
		case 2:
			version = 0x00001 | 0x00002 | 0x00004 | 0x00008 | 0x00010 | 0x00020 | 0x00040;
			switch (minor_version)
			{
			case 0:
				return;
			case 1:
				version |= 0x00080;
				return;
			}
			return;
		case 3:
			version = 0x00001 | 0x00002 | 0x00004 | 0x00008 | 0x00010 | 0x00020 | 0x00040 | 0x00080 | 0x00100;
			switch (minor_version)
			{
			case 0:
				return;
			case 1:
				version |= 0x00200;
				return;
			case 2:
				version |= 0x00200 | 0x00400;
				return;
			case 3:
				version |= 0x00200 | 0x00400 | 0x00800;
				return;
			}
			return;
		case 4:
			version = 0x00001 | 0x00002 | 0x00004 | 0x00008 | 0x00010 | 0x00020 | 0x00040 | 0x00080 | 0x00100 | 0x00200 | 0x00400 | 0x00800 | 0x01000;
			switch (minor_version)
			{
			case 0:
				return;
			case 1:
				version |= 0x02000;
				return;
			case 2:
				version |= 0x02000 | 0x04000;
				return;
			case 3:
				version |= 0x02000 | 0x04000 | 0x08000;
				return;
			case 4:
				version |= 0x02000 | 0x04000 | 0x08000 | 0x10000;
				return;
			case 5:
				version |= 0x02000 | 0x04000 | 0x08000 | 0x10000 | 0x20000;
				return;
			}
			return;
		default:
			return;
		}
	}
};



#endif // !__GLVersion_H
