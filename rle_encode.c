#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s <input> <output>\n", argv[0]);

        return 1;
    }

    char *pszInputFile = argv[1];
    char *pszOuputFile = argv[2];

    FILE *pInputFile = fopen(pszInputFile, "rb");
    FILE *pOutputFile = fopen(pszOuputFile, "wb");

    if(!pInputFile)
    {
        printf("Failed to open input file \"%s\"!\n", pszInputFile);

        return 1;
    }

    if(!pOutputFile)
    {
        printf("Failed to open output file \"%s\"!\n", pszOuputFile);

        return 1;
    }

    printf("Compressing \"%s\" > \"%s\"...\n", pszInputFile, pszOuputFile);

    fseek(pInputFile, 0, SEEK_END);
    long lInputSize = ftell(pInputFile);
    fseek(pInputFile, 0, SEEK_SET);

    printf("Input file size: %li bytes!\n", lInputSize);

    uint32_t ulReadIndex = 0;
    uint32_t ulWriteIndex = 0;
    uint16_t usPrevData = 65535;

    while(ulReadIndex < lInputSize)
    {
        uint8_t ubData = fgetc(pInputFile);
        ulReadIndex++;

        fputc(ubData, pOutputFile);
        ulWriteIndex++;

        if(usPrevData == ubData)
        {
            uint8_t ubCount = 0;

            while(ubCount < 255 && ulReadIndex < lInputSize)
            {
                ubData = fgetc(pInputFile);
                ulReadIndex++;

                if(usPrevData != ubData)
                {
                    ulReadIndex--;
                    fseek(pInputFile, -1, SEEK_CUR);

                    break;
                }

                ubCount++;
            }

            fputc(ubCount, pOutputFile);
            ulWriteIndex++;

            usPrevData = 65535;
        }
        else
        {
            usPrevData = ubData;
        }
    }

    printf("Output file size: %u bytes!\n", ulWriteIndex);
    printf("Compression ratio: %.2f %%!\n", 100.f - (float)ulWriteIndex * 100.f / lInputSize);

    fclose(pInputFile);
    fclose(pOutputFile);

    return 0;
}
