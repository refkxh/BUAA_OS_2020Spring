/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}

int is_big_endian(u_char *binary) {
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
	if (ehdr->e_ident[5] == 2) return 1;
	return 0;
}

uint16_t get_16(uint16_t *ptr, int is_bigendian) {
	u_char char1, char2;
	if (is_bigendian) {
		char1 = *(u_char *)ptr;
		char2 = *((u_char *)ptr + 1);
		return ((uint16_t)char1 << 8) + char2;
	}
	else return *ptr;
}

uint32_t get_32(uint32_t *ptr, int is_bigendian) {
	u_char char1, char2, char3, char4;
	if (is_bigendian) {
		char1 = *(u_char *)ptr;
		char2 = *((u_char *)ptr + 1);
		char3 = *((u_char *)ptr + 2);
		char4 = *((u_char *)ptr + 3);
		return ((uint32_t)char1 << 24) + ((uint32_t)char2 << 16) +
		   	((uint32_t)char3 << 8) + char4;
	}
	else return *ptr;
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf". 
*/
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr, endian;

        Elf32_Phdr *phdr = NULL;

        u_char *ptr_ph_table = NULL;
        Elf32_Half ph_entry_count;
        Elf32_Half ph_entry_size;


        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return -1;
        }

		endian = is_big_endian(binary);

        // get program table addr, program header number and program header size.
		phdr = binary + get_32(&ehdr->e_phoff, endian);
		ph_entry_count = get_16(&ehdr->e_phnum, endian);
		ph_entry_size = get_16(&ehdr->e_phentsize, endian);

        // for each program header, output segment number, filesize and memsize. 
        // hint: section number starts at 0.
		ptr_ph_table = phdr;
		for (Nr = 0; Nr < ph_entry_count; Nr++) {
			Elf32_Word filesize = get_32(&((Elf32_Phdr *)ptr_ph_table)->p_filesz, endian);
			Elf32_Word memsize = get_32(&((Elf32_Phdr *)ptr_ph_table)->p_memsz, endian);
			printf("%d:0x%x,0x%x\n", Nr, filesize, memsize);
			ptr_ph_table += ph_entry_size;
		}

        return 0;
}

