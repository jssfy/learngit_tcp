#include "tools.h"
#include "assert.h"
#include <stdio.h>
#include <time.h>

// return true if the machine is little endian
bool IsLittleIndian()
{
	union temp {
		char buf[4];
		int i;
	};

	temp l;
	l.i = 0x01020304;
	return (l.buf[0] == 4);
}

#include <sys/time.h>
// return a string of local time to the ms level, only for linux now, e.g., 2012-08-30 07:09:28.589
// the caller should prepare the buffer no shorter than 24
void GetLocalTime(char *iBuf)
{
	assert(iBuf != NULL);
	timeval lTime;
	::gettimeofday(&lTime, NULL);
	struct tm now;
	::localtime_r(&lTime.tv_sec, &now);
	sprintf(iBuf, "%d-%02d-%02d %02d:%02d:%02d.%03ld\0",
		now.tm_year + 1900,
		now.tm_mon + 1,
		now.tm_mday,
		now.tm_hour, now.tm_min, now.tm_sec, lTime.tv_usec / 1000);

}

void WriteToFile(char *iFile, char *iBuf, unsigned int iLen)
{
	if (NULL == iBuf)
		return;
	FILE *lOutput;
	unsigned int lToWrite = iLen, lWritten = 0;
	if (lOutput = fopen(iFile, "wb")) {
		while (lToWrite > 0) {
			lWritten = fwrite(iBuf, sizeof(char), iLen, lOutput);
			if (lWritten > 0)
				lToWrite -= lWritten;
			else {
				perror("stop writing");
				break;
			}
		}
		fclose(lOutput);

	}

}

// the size should not be bigger than an unsigned int can hold
void ReadFileToSingleBuffer(char *iFile, char **oBuf, unsigned int &oLen)
{
	FILE *lInput;
	if (lInput = fopen(iFile, "rb")) {
		fseek(lInput, 0, SEEK_END);
		//unsigned int lLen = ftell(lInput);
		// get the number of bytes from beginning to end
		oLen = ftell(lInput);
		//printf("size is %ud\n", lLen);
		if (oLen == 0) {
			perror("file is empty");
			return;
		}
		char *lBuf = new char[oLen];
		if (NULL == lBuf) {
			oLen = 0;
			perror("not enough memory");
			return;
		} else {
			unsigned int lToRead = oLen;
			unsigned int lRead = 0, lIndex = 0;
			fseek(lInput, 0, SEEK_SET);
			while (lToRead > 0) {
				if (lRead > 0)
					printf("to read one more time\n");
				lRead =
				    fread(lBuf, sizeof(char), lToRead, lInput);
				if (lRead > 0) {
					lToRead -= lRead;
					lIndex += lRead;
					lBuf += lRead;
				} else {
					perror("stop reading");
				}
			}
			lBuf -= lIndex;
			*oBuf = lBuf;
		}
		fclose(lInput);
	}
}
