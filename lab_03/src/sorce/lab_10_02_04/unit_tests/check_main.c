#include "check_main.h"

int main(void)
{
    int no_failed = 0;
    SRunner *runner = srunner_create(list_actions());

    srunner_run_all(runner, CK_VERBOSE);

    no_failed = srunner_ntests_failed(runner);

    srunner_free(runner);

    return (no_failed == 0) ? 0 : 1;
}
