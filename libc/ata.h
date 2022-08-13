#pragma once

#include "ports.h"

void read_sectors_ATA_PIO(uint32_t *target_var_address, uint32_t LBA, uint8_t sector_count);
void write_sectors_ATA_PIO(uint32_t LBA, uint8_t sector_count, char *bytes);
