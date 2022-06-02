#pragma once

#include "callbacks21.h"

void GLUTBackendInit(int argc, char** argv);

bool GLUTBackendCreateWindow(unsigned int Width, unsigned int Height, unsigned int bpp, bool isFullScreen, const char* pTitle);

void GLUTBackendRun(ICallbacks* pCallbacks);//запуска главного цикла GLUT
//интерфейс ICallbacks, который поможет в регистрации функций обратного вызова. 
//Вместо того, что бы приложение само регистрировало функции, теперь этим будет заниматься интерфейс