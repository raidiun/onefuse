#define FUSE_USE_VERSION 26

#include <fuse.h>

#include "dlfcn.h"
#define symLoad dlsym

#include <cstring>
#include <cerrno>

static int getattr_callback(const char *path, struct stat *stbuf) {
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
        }

    return -ENOENT;
    }

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    // Call the graph API to get results

    return 0;
    }

static int open_callback(const char *path, struct fuse_file_info *fi) {
    return 0;
    }

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi) {

    return -ENOENT;
    }

static struct fuse_operations onefuse_operations = {
    getattr_callback,
    NULL, // readlink
    NULL, // getdir (deprecated)
    NULL, // mknod
    NULL, // mkdir
    NULL, // unlink
    NULL, // rmdir
    NULL, // symlink
    NULL, // rename
    NULL, // link // Unsupported
    NULL, // chmod // Support limited to local files
    NULL, // chown // Unsupported, all files will be with user permissions
    NULL, // truncate
    NULL, // utime
    NULL, // open_callback,
    read_callback,
    NULL, // write
    NULL, // statfs
    NULL, // flush
    NULL, // release
    NULL, // fsync
    NULL, // setxattr
    NULL, // getxattr
    NULL, // listxattr
    NULL, // removexattr
    NULL, // opendir
    readdir_callback,
    NULL, // releasedir
    NULL, // fsyncdir
    NULL, // init
    NULL, // destroy // TODO: Perform proper logout from graph API
    NULL, // access
    NULL, // create // TODO
    NULL, // ftruncate
    NULL, // fgetattr
    NULL, // lock // Equivalent to checkout?
    NULL, // utimens
    NULL, // bmap
    0, // flag_nullpath_ok
    0, // flag_nopath
    0, // flag_utime_omit_ok
    0, // flag_reserved
    NULL, // ioctl
    NULL, // poll
    NULL, // write_buf
    NULL, // read_buf
    NULL, // flock // Equivalent to checkout?
    NULL, // fallocate // Check for space on cloud?
    };

int main(int const argc, char ** argv) {
    return fuse_main(argc, argv, &onefuse_operations, NULL);
    }
