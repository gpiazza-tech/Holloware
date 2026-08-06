#pragma once

#include "perplex_defs.h"
#include "perplex_bool.h"

PX_EXTERN void SaveBool(const char* key, bool value);
PX_EXTERN void LoadBool(const char* key, bool* value);
PX_EXTERN void SaveInt(const char* key, int value);
PX_EXTERN void LoadInt(const char* key, int* value);
PX_EXTERN void SaveFloat(const char* key, float value);
PX_EXTERN void LoadFloat(const char* key, float* value);
PX_EXTERN void SaveDouble(const char* key, double value);
PX_EXTERN void LoadDouble(const char* key, double* value);