#include "Application.h"
#include "common/SingleInstance.h"

int main(int argc,char* argv[])
{
    /* 单实例运行 */
    if (proc_is_exist(argv[0]) == 1) {
        printf("an \"%s\" already running in system. exit now...\n", argv[0]);
        return 0;
    } else {
        Application app(argc,argv);
        app.run();
    }

    return 0;
}
