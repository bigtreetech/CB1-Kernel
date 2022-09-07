#include "common.h"

namespace dragonboard {

int get_card(const char *card_name)
{
    int ret;
    int fd;
    int i;
    char path[128];
    char name[64];

    for (i = 0; i < 10; i++) {
        sprintf(path, "/sys/class/sound/card%d/id", i);
        ret = access(path, F_OK);
        if (ret) {
            db_warn("can't find node %s\n", path);
            return -1;
        }

        fd = open(path, O_RDONLY);
        if (fd <= 0) {
            db_warn("can't open %s\n", path);
            return -1;
        }

        ret = read(fd, name, sizeof(name));
        close(fd);
        if (ret > 0) {
            name[ret-1] = '\0';
            if (NULL != strstr(name, card_name))
                return i;
        }
    }

    db_warn("can't find card:%s", card_name);
    return -1;
}

} //namespace
