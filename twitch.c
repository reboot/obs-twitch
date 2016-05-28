#include <obs-module.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-twitch", "en-US")

extern void viewers_init();

bool obs_module_load(void)
{
    viewers_init();

    return true;
}

void obs_module_unload(void)
{
}
