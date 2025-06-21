// Every Frame is a Choice //
#pragma once

UENUM()
enum class EStanceState : uint8 {
  Standing,
  Crouching,
  Airborne,
  Downed,
};

enum class EMoveTagBit : uint8_t {
  None = 0,
  Throw = 1 << 0,
  Reversal = 1 << 1,
  Movement = 1 << 2,
};