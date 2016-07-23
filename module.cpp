#include <obs-module.h>

#include "twitch.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-twitch", "en-US")

static Twitch *twitch;

MODULE_EXTERN bool obs_module_load(void)
{
    twitch = new Twitch();

    return true;
}

MODULE_EXTERN void obs_module_unload(void)
{
    delete twitch;
}
