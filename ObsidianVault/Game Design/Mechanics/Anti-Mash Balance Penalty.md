## 🧠 Concept Overview

This mechanic discourages button mashing by tying invalid input behavior to the existing **[Balance](Game%20Design/Mechanics/Balance.md)** system. When players press attack or step buttons while not in control (e.g., during recovery, hitstun, or blockstun), their **[Balance](Game%20Design/Mechanics/Balance.md) is temporarily reduced**. This only becomes relevant if they get hit, making the penalty skill-based and non-intrusive.

---

## ⚙️ Mechanic Details

- **Trigger Conditions**:
    - Player inputs **P**, **K**, or **S** while **not in control**, such as during hitstun, blockstun, attack/recovery frames, etc...
    - Inputs that are **buffered correctly** just before regaining control **do not trigger** the penalty.
    - **B (Block/Counter)** is excluded to preserve counter system integrity.
        
- **Penalty Effect**:
    - Each invalid input reduces **[Balance](Game%20Design/Mechanics/Balance.md)** by a small amount.

---

## 🎯 Design Intent

- **Discourage Panic Mashing**:
    - Sloppy input during pressure or hitstun becomes a meaningful liability.
        
- **Reward Clean, Skillful Input**:
    - Players who time their buffers and stay composed maintain their Balance advantage.
        
- **Avoid Penalizing Good Execution**:
    - Legitimate buffering during valid frames is unaffected, preserving high-level play integrity.
        
- **Maintain System Clarity**:
    - [Balance](Game%20Design/Mechanics/Balance.md) is already a core mechanic; this integration adds anti-mash behavior without introducing confusing new systems.

---

## Implementation Notes
- Training mode or replay tools can highlight “invalid inputs” and their [Balance](Game%20Design/Mechanics/Balance.md) impact to support learning and refinement.
- Consider exposing mash frequency in analytics