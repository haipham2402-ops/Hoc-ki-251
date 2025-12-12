/* src/libmem.c - Cleaned to avoid multiple definition */
#include "mm.h"
#include "libmem.h"
#include "syscall.h"
#include <stdlib.h>
#include <stdio.h>

/* File này được giữ lại để đảm bảo cấu trúc Makefile không bị lỗi.
 * - libsyscall: Đã có trong src/libstd.c
 * - liballoc/free...: Đã có trong src/mm-vm.c
 */
