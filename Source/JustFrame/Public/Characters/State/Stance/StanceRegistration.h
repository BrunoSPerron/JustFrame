// Every Frame is a Choice //
#pragma once

#include "Characters/State/Stance/StanceInstanceFactory.h"

#define REGISTER_STANCE_INSTANCE(Type, Id)                                                         \
  static struct FAutoRegister_##Type {                                                             \
    FAutoRegister_##Type() {                                                                       \
      UStanceInstanceFactory::Register(Id,                                                         \
                                       [](ARollbackCharacter *Owner, const FStanceData &Data) {    \
                                         return MakeShared<Type>(Owner, Data);                     \
                                       });                                                         \
    }                                                                                              \
  } AutoRegister_##Type
