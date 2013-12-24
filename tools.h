#ifndef TOOLS_H
#define TOOLS_H

bool IsLittleIndian();

void GetLocalTime(char *iBuf);

void WriteToFile(char *iFile, char *iBuf, unsigned int iLen);
void ReadFileToSingleBuffer(char *iFile, char **oBuf, unsigned int &oLen);

#endif
