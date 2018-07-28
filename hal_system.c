#include <stdint.h>
#include "mk66f18.h"
#include "core_cm4.h"
#include "hal_system.h"

void hal_system_reset()
{
    NVIC_SystemReset();
}