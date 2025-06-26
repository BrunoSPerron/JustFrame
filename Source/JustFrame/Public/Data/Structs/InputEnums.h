// Every Frame is a Choice //
#pragma once

enum class EInputBit : uint16 {
  Up = 1 << 0,
  Down = 1 << 1,
  Left = 1 << 2,
  Right = 1 << 3,
  Low = 1 << 4,
  High = 1 << 5,
  Punch = 1 << 6,
  Kick = 1 << 7,
  Guard = 1 << 8,
  Step = 1 << 9,
};