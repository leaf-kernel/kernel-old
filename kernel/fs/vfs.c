#include <fs/initrd.h>
#include <fs/vfs.h>
#include <libc/stdlib/memory/kheap.h>
#include <sys/logger.h>

VFS_t *init_vfs() {
    VFS_t *vfs = (VFS_t *)kmalloc(sizeof(VFS_t));
    if(vfs == NULL) {
        fail("Failed to allocate memory for VFS stuct!");
        return NULL;
    }

    vfs->address = (uint64_t)vfs;
    ok("vfs at 0x%0.16llx", vfs->address);

    vfs->drives = (drive_t *)kmalloc(sizeof(drive_t));
    if(vfs->drives == NULL) {
        fail("Failed to allocate memory for VFS drives!");
        return NULL;
    }

    ok("done.");
    return vfs;
}

vfs_op_status mount_drive(VFS_t *vfs, uint64_t driveAddr, vfs_drive_type type) {
    if(vfs == NULL) {
        return STATUS_INVALID_ARGUMENTS;
    }

    drive_t *newDrive = (drive_t *)kmalloc(sizeof(drive_t));
    if(newDrive == NULL) {
        return STATUS_MALLOC_FAILURE;
    }

    newDrive->driveAddr = driveAddr;
    newDrive->driveType = type;

    if(vfs->drives == NULL) {
        vfs->drives = (drive_t *)kmalloc(sizeof(drive_t));
        if(vfs->drives == NULL) {
            kfree(newDrive);
            return STATUS_MALLOC_FAILURE;
        }
    } else {
        drive_t *temp = (drive_t *)krealloc(vfs->drives, (vfs->numDrives + 1) *
                                                             sizeof(drive_t));
        if(temp == NULL) {
            kfree(newDrive);
            return STATUS_MALLOC_FAILURE;
        }
        vfs->drives = temp;
    }

    vfs->drives[vfs->numDrives++] = *newDrive;
    kfree(newDrive);
    ok("mounted drive from 0x%.16llx", driveAddr);
    return STATUS_OK;
}

vfs_op_status umount_drive(VFS_t *vfs, int driveId) {
    if(vfs == NULL || driveId < 0 || driveId >= vfs->numDrives) {
        return STATUS_INVALID_ARGUMENTS;
    }

    drive_t *driveToRemove = &(vfs->drives[driveId]);

    for(int i = driveId; i < vfs->numDrives - 1; i++) {
        vfs->drives[i] = vfs->drives[i + 1];
    }

    vfs->numDrives--;

    if(vfs->numDrives > 0) {
        drive_t *temp =
            (drive_t *)krealloc(vfs->drives, vfs->numDrives * sizeof(drive_t));
        if(temp != NULL) {
            vfs->drives = temp;
        }
    } else {
        kfree(vfs->drives);
        vfs->drives = NULL;
    }

    ok("-0x%016x", driveToRemove->driveAddr);

    return STATUS_OK;
}

vfs_op_status drive_read(VFS_t *vfs, int driveId, char *fileName, char **out) {
    if(vfs == NULL) {
        fail("A null pointer to the VFS was passed!");
        return STATUS_INVALID_ARGUMENTS;
    }

    if(driveId > vfs->numDrives) {
        fail("Invalid driveId passed!");
        return STATUS_INVALID_ARGUMENTS;
    }

    drive_t *temp = &vfs->drives[driveId];

    switch(temp->driveType) {
    case TYPE_INITRD:
        uint32_t hash = hash_string(fileName);
        int fileId = find_file_by_hash((Ramdisk *)temp->driveAddr, hash);

        if(fileId == -1) {
            fail("%s not found!", fileName);
            fail("%s not found!", fileName);
            return STATUS_ERROR_FILE_NOT_FOUND;
        }

        RamdiskEntry *tempEntry =
            (RamdiskEntry *)((Ramdisk *)temp->driveAddr)->content[fileId];

        if(tempEntry->file->path == NULL) {
            fail("%s not found!", fileName);
            fail("%s not found!", fileName);
            return STATUS_ERROR_FILE_NOT_FOUND;
        }

        *out = (char *)kmalloc(tempEntry->file->size);
        if(*out == NULL) {
            fail("Failed to allocate memory for file content!");
            return STATUS_MALLOC_FAILURE;
        }

        size_t numBytes = tempEntry->file->size * sizeof((*out)[0]);
        memcpy((*out), tempEntry->file->content, numBytes);

        ok("done.");
        break;
    default:
        fail("Invalid drive type ! %d", temp->driveType);
        return STATUS_INVALID_DRIVE_TYPE;
    }

    return STATUS_OK;
}