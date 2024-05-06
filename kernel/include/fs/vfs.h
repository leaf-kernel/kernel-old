#ifndef __VFS_H__
#define __VFS_H__

#include <libc/stdio/printf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <utils/hash.h>

typedef enum { TYPE_INITRD = 0 } vfs_drive_type;

typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR_FILE_NOT_FOUND,
    STATUS_ERROR_NO_SPACE_LEFT,
    STATUS_DRIVE_NOT_FOUND,
    STATUS_MALLOC_FAILURE,
    STATUS_INVALID_ARGUMENTS,
    STATUS_INVALID_DRIVE_TYPE,
    STATUS_DRIVE_ID_ALREADY_EXISTS,
    STATUS_UNKNOWN_ERROR
} vfs_op_status;

typedef struct {
    vfs_drive_type driveType;
    uint64_t driveAddr;
} drive_t;

typedef struct {
    drive_t *drives;
    uint64_t address;
    int numDrives;
} VFS_t;

VFS_t *init_vfs();
vfs_op_status mount_drive(VFS_t *vfs, uint64_t driveAddr, vfs_drive_type type,
                          uint8_t id);
vfs_op_status umount_drive(VFS_t *vfs, int driveId);
vfs_op_status drive_read(VFS_t *vfs, int driveId, char *fileName, char **out);

#endif  // __VFS_H__
