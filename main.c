#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>




void print_file_info(const char *filename, const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        printf("Permissions: %o\n", st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
        printf("User: %s\n", getpwuid(st.st_uid)->pw_name);
        printf("Group: %s\n", getgrgid(st.st_gid)->gr_name);
        printf("Size: %lld bytes\n", (long long)st.st_size);
        printf("Filename: %s\n", filename);
        printf("\n");
    }
}

void traverse_directory(const char *dir_name, int indent) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(dir_name)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

        print_file_info(entry->d_name, path);

        if (entry->d_type == DT_DIR) {
            for (int i = 0; i < indent; i++) {
                printf("  ");
            }
            printf("|- %s\n", entry->d_name);

            char new_dir[1024];
            snprintf(new_dir, sizeof(new_dir), "%s/%s", dir_name, entry->d_name);
            traverse_directory(new_dir, indent + 1);
        }
    }

    closedir(dir);
}

int main() {
    traverse_directory(".", 0);
    return 0;
}
