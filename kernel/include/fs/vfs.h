#ifndef __VFS_H__
#define __VFS_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <libc/stdio/printf.h>

typedef enum {
  TYPE_INITRD = 0
} vfs_drive_type;

typedef enum
{
  STATUS_OK = 0,
  STATUS_ERROR_FILE_NOT_FOUND,
  STATUS_ERROR_NO_SPACE_LEFT,
  STATUS_DRIVE_NOT_FOUND,
  STATUS_MALLOC_FAILURE,
  STATUS_INVALID_ARGUMENTS,
  STATUS_INVALID_DRIVE_TYPE,
  STATUS_UNKNOWN_ERROR
} vfs_op_status;
 
typedef struct {
  vfs_drive_type driveType;
  uint64_t driveAddr;
  void (*driveRead)(char*, void*);
  void (*driveWrite)(char*, void*);
} drive_t;

typedef struct {
  drive_t *drives;
  uint64_t address;
} VFS_t;

VFS_t *init_vfs();
vfs_op_status mount_drive(VFS_t* vfs, uint64_t driveAddr, void* read, void* write, vfs_drive_type type);

#endif // __VFS_H__
