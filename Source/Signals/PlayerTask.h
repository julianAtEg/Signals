#pragma once

#include "TaskSchedule.h"

struct Combatant;
typedef Task<Combatant> PlayerTask;
typedef TaskSchedule<PlayerTask> PlayerSchedule;