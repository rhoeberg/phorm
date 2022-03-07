#pragma once

bool SingleMousePress(int MOUSE_BUTTON);
void GetWindowSize(int *width, int *height);
void GetFramebufferSize(int *width, int *height);
void ResetTime();
double GetTime();

global double startTime;
