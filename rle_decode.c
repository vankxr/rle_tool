#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Invalid argument count!\n");

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

    printf("Expanding \"%s\" > \"%s\"...\n", pszInputFile, pszOuputFile);

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
            uint8_t ubCount = fgetc(pInputFile);
            ulReadIndex++;

            while(ubCount)
            {
                fputc(ubData, pOutputFile);
                ulWriteIndex++;

                ubCount--;
            }

            if(ubCount)
            {
                printf("Invalid format!\n");

                fclose(pInputFile);
                fclose(pOutputFile);

                return 1;
            }

            usPrevData = 65535;
        }
        else
        {
            usPrevData = ubData;
        }
    }

    printf("Output file size: %u bytes!\n", ulWriteIndex);
    printf("Compression ratio: %.2f %%!\n", 100.f - (float)lInputSize * 100.f / ulWriteIndex);

    fclose(pInputFile);
    fclose(pOutputFile);

    return 0;
}