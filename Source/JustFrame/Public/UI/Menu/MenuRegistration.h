#pragma once
#include "UI/Menu/MenuFactory.h"

#define REGISTER_MENU_WIDGET(Type, Name)                                                           \
  static struct FAutoRegister_##Type {                                                             \
    FAutoRegister_##Type() { UMenuFactory::Register(Name, Type::StaticClass()); }                  \
  } AutoRegister_##Type