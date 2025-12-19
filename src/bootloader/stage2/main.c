#include "stdint.h"
// #include "stdio.h"
// #include "disk.h"
// #include "fat.h"
// #include "bioshelpers.h"

// void  g_data = (void )0x00500200;

void __attribute((_cdecl)) cstart_(uint16_t bootDrive)
{
    // DISK disk;
    // if (!DISK_Initialize(&disk, bootDrive))
    // {
    //     printf("Disk init error\r\n");
    //     goto end;
    // }

    // DISK_ReadSectors(&disk, 19, 1, g_data);

    // if (!FAT_Initialize(&disk))
    // {
    //     printf("FAT init error\r\n");
    //     goto end;
    // }

    // // browse files in root
    // FAT_File  fd = FAT_Open(&disk, "/");
    // FAT_DirectoryEntry entry;
    // int i = 0;
    // while (FAT_ReadEntry(&disk, fd, &entry) && i++ < 5)
    // {
    //     printf("  ");
    //     for (int i = 0; i < 11; i++)
    //         putc(entry.Name[i]);
    //     printf("\r\n");
    // }
    // FAT_Close(fd);

    // // read test.txt
    // char buffer[256];
    // uint32_t read;
    // fd = FAT_Open(&disk, "test.txt");
    // while ((read = FAT_Read(&disk, fd, sizeof(buffer), buffer)))
    // {
    //     for (uint32_t i = 0; i < read; i++)
    //     {
    //         if (buffer[i] == '\n') {
    //         putc('\r');
    //     }
    //         putc(buffer[i]);
    //     }
    // }
    // FAT_Close(fd);

    puts("\r");
    puts("   ,,                                                                          ");
    puts("  `7MM                                                                           ");
    puts("   MM                                                                           ");
    puts("   MM `7MM  `7MM  `7MMpMMMb.  .gP\"Ya       ,pW\"Wq.  ,pP\"Ybd                        ");
    puts("MM   MM    MM    MM    MM ,M'   Yb     6W'   `Wb 8I   `\"                        ");
    puts("MM   MM    MM    MM    MM 8M\"\"\"\"\"\"     8M     M8 `YMMMa.                       ");
    puts(" MM   MM    MM    MM    MM YM.    ,     YA.   ,A9 L.   I8                      ");
    puts(".JMML. `Mbod\"YML..JMML  JMML.`Mbmmd'      `Ybmd9'  M9mmmP'                      ");
    puts("                                                                                 ");
    puts("ALPHA 1.0.0                                               ");
    puts("\r");

    // checkKeyStroke();

end:
    for (;;);
}