#include <fs/vfs.h>
#include <fs/initrd.h>
#include <sys/logger.h>
#include <memory/kheap.h>

VFS_t *init_vfs() {
  VFS_t* vfs = (VFS_t*)kmalloc(sizeof(VFS_t));
  if(vfs == NULL) {
    debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for VFS stuct!");
    return NULL;
  }

  vfs->address = (uint64_t)vfs;
  cdebug_log(__func__, "VFS located at 0x%016X", vfs->address);

  vfs->drives = (drive_t*)kmalloc(sizeof(drive_t));
  if(vfs->drives == NULL) {
    debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for VFS drives!");
    return NULL;
  }

  cdebug_log(__func__, "done.");
  return vfs;
}

vfs_op_status mount_drive(VFS_t* vfs, uint64_t driveAddr, void* read, void* write, vfs_drive_type type) {
  if(vfs == NULL) {
    debug_log(__FILE__, __LINE__, __func__, "mount_drive got a NULL vfs pointer passed?");
    return STATUS_INVALID_ARGUMENTS;
  }

  (void)driveAddr;
  (void)read;
  (void)write;
  (void)type;
  return STATUS_OK;
}
