## 🧠 Concept Overview

The **Counter System** is a core defensive mechanic that rewards precision timing. By pressing **[Guard (G)](Game%20Design/Input%20Scheme.md)** with accurate timing during specific vulnerable moments (e.g., hit in neutral or [Staggered](Game%20Design/Mechanics/Stagger.md)), the defender gains access to **reversal options** or **[Balance](Game%20Design/Mechanics/Balance.md)-based advantages**, creating dynamic and skillful defense scenarios.

This system integrates tightly with the game's broader mechanics, especially [Stagger](Game%20Design/Mechanics/Stagger.md) and [Balance](Game%20Design/Mechanics/Balance.md), making defense an active, tactical layer of gameplay rather than passive waiting.

---

## ⚙️ Mechanic Details

### 🛡️ Counter Activation (Core Trigger)

- Input: **Well-timed [Guard (G)](Game%20Design/Input%20Scheme.md)** when being hit.
- Context:
    - From **Neutral or Air Neutral**, a successful input triggers a **character-specific reversal** (e.g., parry, dodge-into-counter, or positional flip).
    - During **[Stagger](Game%20Design/Mechanics/Stagger.md) or Attack animations**, the same input offers Balance-related benefits rather than a reversal.
        

---

### 📈 Balance-Related Effects

#### ✅ **Balance Boost**

- Input Timing: During **any animation** (including attacking or hitstun).
- Effect:
    - Temporarily **increases the defender's [Balance](Game%20Design/Mechanics/Balance.md)**.
    - Influences knockdown resilience and [Stagger](Game%20Design/Mechanics/Stagger.md) thresholds.
    - Encourages players to time blocks even when not in a neutral state.

#### ♻️ **Balance Reversal**
- Input Timing: During **[Stagger](Game%20Design/Mechanics/Stagger.md)**.
- Effect:
    - **Transfers defender's [Stagger Value](Game%20Design/Mechanics/Stagger.md#Stagger%20Value)** to the **attacker**.
    - Can destabilize overly aggressive opponents and regain neutral footing.
    - Precision timing required—window is tight and unforgiving.

---

## 🎯 Design Intent

- **Encourage Skill Expression**: The mechanic rewards precise timing and matchup knowledge, raising the defensive skill ceiling.
- **Avoid Passive Blocking**: Defense becomes interactive and high-stakes rather than a fallback or stalling tactic.
- **Enhance System Synergy**: Ties together the [Stagger](Game%20Design/Mechanics/Stagger.md) and [Balance](Game%20Design/Mechanics/Balance.md) systems to make defense as layered and thoughtful as offense.
- **Support Diegetic Communication**: Defensive success and timing should be visually and audibly intuitive without relying on UI elements.