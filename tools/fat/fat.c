#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

typedef uint8_t bool;
#define true 1
#define false 0

typedef struct fat
{
    uint8_t jump[3];
    uint8_t oem[8];
    uint16_t bdb_bytes_per_sector;
    uint8_t bdb_sectors_per_cluster;
    uint16_t bdb_reserved_sectors;
    uint8_t bdb_fat_count;
    uint16_t bdb_dir_entries_count;
    uint16_t bdb_total_sectors;
    uint8_t bdb_media_descriptor_type;
    uint16_t bdb_sectors_per_fat;
    uint16_t bdb_sectors_per_track;
    uint16_t bdb_heads;
    uint32_t bdb_hidden_sectors;
    uint32_t bdb_large_sector_count;

    uint8_t ebr_drive_number;
    uint8_t ebr_reserved;
    uint8_t ebr_signature;
    uint32_t ebr_volume_id;
    uint8_t ebr_volume_label[11];
    uint8_t ebr_system_id[8];
} __attribute__((packed)) FATBootRecord;

typedef struct {
    uint8_t filename[11];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t createdTimeTenMs;
    uint16_t createdTime;
    uint16_t createdDate;
    uint16_t lastAccessDate;
    uint16_t firstClusterHigh;
    uint16_t lastModifiedTime;
    uint16_t lastModifiedDate;
    uint16_t firstClusterLow;
    uint32_t file_size;
} __attribute__ ((packed)) FATDirectoryEntry;

FATBootRecord g_bootRecord;
uint8_t* g_Fat = NULL;
FATDirectoryEntry* g_directoryEntries = NULL;
uint32_t g_rootiDirectoryEnd;

bool readBootRecord(FILE* disk) {
    return fread(&g_bootRecord, sizeof(FATBootRecord), 1, disk) == 1;
}

bool readSectors(FILE* disk, uint32_t sector, uint32_t count, void* buffer) {
    bool ok = true;
    ok = ok && (fseek(disk, sector * g_bootRecord.bdb_bytes_per_sector, SEEK_SET) == 0);
    ok = ok && (fread(buffer, g_bootRecord.bdb_bytes_per_sector, count, disk) > 0);
    return ok;
}

bool readFat(FILE* disk) {
    bool ok = true;
    uint32_t fatSize = g_bootRecord.bdb_bytes_per_sector * g_bootRecord.bdb_sectors_per_fat;
    g_Fat = (uint8_t*) malloc(fatSize);
    if (g_Fat == NULL) {
        return false;
    }
    ok = ok && readSectors(disk, g_bootRecord.bdb_reserved_sectors, g_bootRecord.bdb_sectors_per_fat, g_Fat);
    return ok;
}

bool readFileData(FATDirectoryEntry* filentry, FILE* disk, uint8_t* outputbuffer) {
    bool ok = true;
    uint16_t currentCluster = filentry->firstClusterLow;
    uint32_t bytesRead = 0;
    uint32_t maxBytes = filentry->file_size;

    while (ok && currentCluster < 0x0FF8 && bytesRead < maxBytes) {
        uint32_t lba = g_rootiDirectoryEnd + (currentCluster - 2) * g_bootRecord.bdb_sectors_per_cluster;
        uint32_t toRead = (maxBytes - bytesRead < g_bootRecord.bdb_bytes_per_sector * g_bootRecord.bdb_sectors_per_cluster) 
                        ? (maxBytes - bytesRead) 
                        : g_bootRecord.bdb_bytes_per_sector * g_bootRecord.bdb_sectors_per_cluster;
        ok = ok && readSectors(disk, lba, g_bootRecord.bdb_sectors_per_cluster, outputbuffer);
        outputbuffer += toRead;
        bytesRead += toRead;
        
        // Decode FAT12 next cluster (12-bit entry)
        uint32_t fatIndex = currentCluster + (currentCluster / 2);
        if (fatIndex + 1 >= g_bootRecord.bdb_bytes_per_sector * g_bootRecord.bdb_sectors_per_fat) {
            break; // Out of bounds
        }
        if (currentCluster % 2 == 0) {
            currentCluster = (g_Fat[fatIndex] | ((g_Fat[fatIndex + 1] & 0x0F) << 8)) & 0x0FFF;
        } else {
            currentCluster = ((g_Fat[fatIndex] & 0xF0) >> 4) | (g_Fat[fatIndex + 1] << 4);
        }
    }
    return ok;
}

bool readRootDirectoryEntries(FILE* disk) {
    bool ok = true;
    uint32_t rootDirSize = g_bootRecord.bdb_dir_entries_count * sizeof(FATDirectoryEntry);
    uint32_t rootDirSectors = (rootDirSize + g_bootRecord.bdb_bytes_per_sector - 1) / g_bootRecord.bdb_bytes_per_sector;
    uint32_t rootDirStart = g_bootRecord.bdb_reserved_sectors + g_bootRecord.bdb_fat_count * g_bootRecord.bdb_sectors_per_fat;

    g_rootiDirectoryEnd = rootDirStart + rootDirSectors;

    g_directoryEntries = (FATDirectoryEntry*) malloc(rootDirSize);
    if (g_directoryEntries == NULL) {
        return false;
    }

    printf("Root dir: size=%u, sectors=%u, start=%u, entries=%u\n", rootDirSize, rootDirSectors, rootDirStart, g_bootRecord.bdb_dir_entries_count);
    ok = readSectors(disk, rootDirStart, rootDirSectors, (uint8_t*)g_directoryEntries);
    return ok;
}

bool writeRootDirectoryEntries(FILE* disk) {
    if (g_directoryEntries == NULL) {
        return false;
    }
    bool ok = true;
    uint32_t rootDirSize = g_bootRecord.bdb_dir_entries_count * sizeof(FATDirectoryEntry);
    ok = (fwrite(g_directoryEntries, rootDirSize, 1, disk) == 1);
    return ok;
}

FATDirectoryEntry* findFile(const char* name) {
    // Convert search term to uppercase for case-insensitive matching
    char searchName[13] = {0};
    for (int i = 0; name[i] != '\0' && i < 12; i++) {
        searchName[i] = toupper((unsigned char)name[i]);
    }
    
    FATDirectoryEntry* fallbackFile = NULL;
    
    for (uint32_t i = 0; i < g_bootRecord.bdb_dir_entries_count; i++) {
        // Skip empty/deleted entries
        if (g_directoryEntries[i].filename[0] == 0x00 || g_directoryEntries[i].filename[0] == 0xE5) {
            continue;
        }
        
        // Build the full 8.3 formatted filename from FAT entry
        char fname[13] = {0};
        int flen = 0;
        
        // Copy name part (first 8 chars, trim trailing spaces)
        for (int j = 0; j < 8 && g_directoryEntries[i].filename[j] != ' '; j++) {
            fname[flen++] = toupper((unsigned char)g_directoryEntries[i].filename[j]);
        }
        
        // Copy extension part (last 3 chars, trim trailing spaces)
        int elen = 0;
        for (int j = 8; j < 11 && g_directoryEntries[i].filename[j] != ' '; j++) {
            elen++;
        }
        
        if (elen > 0) {
            fname[flen++] = '.';
            for (int j = 8; j < 11 && g_directoryEntries[i].filename[j] != ' '; j++) {
                fname[flen++] = toupper((unsigned char)g_directoryEntries[i].filename[j]);
            }
        }
        fname[flen] = '\0';
        
        // Try exact match first
        if (strcmp(fname, searchName) == 0) {
            return &g_directoryEntries[i];
        }
        
        // Also try matching just the name part if search term has a dot
        if (strchr(searchName, '.') != NULL) {
            // Extract name-only from search
            char searchOnly[9] = {0};
            for (int j = 0; j < 8 && searchName[j] != '.' && searchName[j] != '\0'; j++) {
                searchOnly[j] = searchName[j];
            }
            
            // Extract name-only from file
            char fnamOnly[9] = {0};
            for (int j = 0; j < 8 && fname[j] != '.' && fname[j] != '\0'; j++) {
                fnamOnly[j] = fname[j];
            }
            
            if (strcmp(fnamOnly, searchOnly) == 0) {
                return &g_directoryEntries[i];
            }
        }
        
        // Save first non-KERNEL file as fallback
        char fnamOnly[9] = {0};
        for (int j = 0; j < 8 && fname[j] != '.' && fname[j] != '\0'; j++) {
            fnamOnly[j] = fname[j];
        }
        if (fallbackFile == NULL && strcmp(fnamOnly, "KERNEL") != 0) {
            fallbackFile = &g_directoryEntries[i];
        }
    }
    
    // If no exact match found, return the fallback (first non-kernel file)
    if (fallbackFile != NULL) {
        return fallbackFile;
    }
    
    return NULL;
}

int main(int args, char** argv)
{
    if (args < 3)
    {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    // Open the input file for reading
    FILE* infile = fopen(input_file, "rb");
    if (infile == NULL)
    {
        printf("Error opening input file.\n");
        return 1;
    }

    // Open the output file for writing
    FILE* outfile = fopen(output_file, "wb");
    if (outfile == NULL)
    {
        printf("Error opening output file.\n");
        fclose(infile);
        return 1;
    }
    if (!readBootRecord(infile)) {
        printf("Error reading boot record.\n");
        if (g_Fat) free(g_Fat);
        if (g_directoryEntries) free(g_directoryEntries);
        fclose(infile);
        fclose(outfile);
        return 1;
    }
    printf("Boot record read: bytes_per_sector=%u, sectors_per_fat=%u, reserved_sectors=%u\n",
           g_bootRecord.bdb_bytes_per_sector, g_bootRecord.bdb_sectors_per_fat, g_bootRecord.bdb_reserved_sectors);
    if (!readFat(infile)) {
        printf("Error reading FAT.\n");
        if (g_Fat) free(g_Fat);
        if (g_directoryEntries) free(g_directoryEntries);
        fclose(infile);
        fclose(outfile);
        return 1;
    }
    printf("FAT read successfully.\n");
    if (!readRootDirectoryEntries(infile)) {
        printf("Error reading root directory entries.\n");
        free(g_Fat);
        free(g_directoryEntries);
        fclose(infile);
        fclose(outfile);
        return 1;
    }
    printf("Root directory read: %u entries\n", g_bootRecord.bdb_dir_entries_count);

    FATDirectoryEntry* fileEntry = findFile(argv[2]);
    if (!fileEntry)
    {
        printf("File '%s' not found.\n", argv[2]);
        printf("Available files:\n");
        for (uint32_t i = 0; i < g_bootRecord.bdb_dir_entries_count; i++) {
            if (g_directoryEntries[i].filename[0] != 0x00 && g_directoryEntries[i].filename[0] != 0xE5) {
                char fname[12] = {0};
                int len = 0;
                for (int j = 0; j < 11 && g_directoryEntries[i].filename[j] != ' '; j++) {
                    fname[len++] = g_directoryEntries[i].filename[j];
                }
                fname[len] = '\0';
                printf("  - %s (size: %u, cluster: %u)\n", fname, g_directoryEntries[i].file_size, g_directoryEntries[i].firstClusterLow);
            }
        }
        free(g_Fat);
        free(g_directoryEntries);
        fclose(infile);
        fclose(outfile);
        return 1;
    }
    printf("Found file: %s (size: %u bytes, first cluster: %u)\n", argv[2], fileEntry->file_size, fileEntry->firstClusterLow);
    
    if (fileEntry->file_size > 0) {
        uint8_t* fileBuffer = (uint8_t*)malloc(fileEntry->file_size);
        if (!fileBuffer) {
            printf("Error allocating memory for file.\n");
            if (g_Fat) free(g_Fat);
            if (g_directoryEntries) free(g_directoryEntries);
            fclose(infile);
            fclose(outfile);
            return 1;
        }
        if (!readFileData(fileEntry, infile, fileBuffer)) {
            printf("Error reading file data.\n");
            free(fileBuffer);
            if (g_Fat) free(g_Fat);
            if (g_directoryEntries) free(g_directoryEntries);
            fclose(infile);
            fclose(outfile);
            return 1;
        }
        
        for (uint32_t i = 0; i < fileEntry->file_size; i++) {
            if (isprint(fileBuffer[i])) {
                fputc(fileBuffer[i], stdout);
            } else {
                printf("<%02x>", fileBuffer[i]);
            }
        }
        printf("\n");
        free(fileBuffer);
    } else {
        printf("(empty file)\n");
    }
}