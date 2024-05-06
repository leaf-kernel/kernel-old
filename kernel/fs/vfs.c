#include <fs/initrd.h>
#include <fs/vfs.h>
#include <libc/stdlib/memory/kheap.h>
#include <sys/logger.h>

VFS_t *init_vfs() {
    VFS_t *vfs = (VFS_t *)kmalloc(sizeof(VFS_t));
    if(vfs == NULL) {
        plog("Failed to allocate memory for VFS stuct!");
        return NULL;
    }

    vfs->address = (uint64_t)vfs;
    vcplog("vfs at 0x%0.16llx", vfs->address);

    vfs->drives = (drive_t *)kmalloc(sizeof(drive_t));
    if(vfs->drives == NULL) {
        plog("Failed to allocate memory for VFS drives!");
        return NULL;
    }

    vvcplog("done.");
    return vfs;
}

vfs_op_status mount_drive(VFS_t *vfs, uint64_t driveAddr, vfs_drive_type type,
                          uint8_t id) {
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

    if(vfs->drives[id].driveAddr == 0) {
        vfs->drives[id] = *newDrive;
    } else {
        plog("Failed to mount drive from 0x%.16llx with id %d. ERROR: Drive "
             "with id %d already exists",
             driveAddr, id, id);
        return STATUS_DRIVE_ID_ALREADY_EXISTS;
    }
    kfree(newDrive);
    vcplog("mounted drive from 0x%.16llx with id %d", driveAddr, id);
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

    vcplog("-0x%016x", driveToRemove->driveAddr);

    return STATUS_OK;
}

vfs_op_status drive_read(VFS_t *vfs, int driveId, char *fileName, char **out) {
    if(vfs == NULL) {
        plog("A null pointer to the VFS was passed!");
        return STATUS_INVALID_ARGUMENTS;
    }

    if(driveId > vfs->numDrives) {
        plog("Invalid driveId passed!");
        return STATUS_INVALID_ARGUMENTS;
    }

    drive_t *temp = &vfs->drives[driveId];
    if(temp == NULL) {
        plog_fail("Drive with id %u not found", driveId);
        plog("Drive with id %u not found", driveId);
        return STATUS_DRIVE_NOT_FOUND;
    }

    switch(temp->driveType) {
    case TYPE_INITRD:
        uint32_t hash = hash_string(fileName);
        int fileId = find_file_by_hash((Ramdisk *)temp->driveAddr, hash);
        RamdiskEntry *tempEntry =
            (RamdiskEntry *)((Ramdisk *)temp->driveAddr)->content[fileId];

        if(tempEntry == NULL) {
            return STATUS_ERROR_FILE_NOT_FOUND;
        }

        *out = (char *)kmalloc(tempEntry->file->size);

        for(int i = 0; i < tempEntry->file->size; ++i) {
            (*out)[i] = tempEntry->file->content[i];
        }

        vvcplog("done.");
        break;
    default:
        plog("Invalid drive type! Got %d", temp->driveType);
        return STATUS_INVALID_DRIVE_TYPE;
    }

    return STATUS_OK;
}
