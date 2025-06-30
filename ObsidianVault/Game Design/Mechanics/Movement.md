# 🧠 Concept Overview: Movement

The movement system is built around **high mobility and directional freedom**, leveraging a unique [Input Scheme](Game%20Design/Input%20Scheme.md) with **modifier buttons** that enable complex actions without sacrificing clarity. Movement is not just traversal—it’s a strategic layer that ties into offense, defense, and character expression.

Jumping, air options, and grounded movement offer **fluidity**, while still respecting **risk/reward** dynamics, especially through interaction with [Balance](Game%20Design/Mechanics/Balance.md) and [Stagger](Game%20Design/Mechanics/Stagger.md).

---

# ⚙️ Mechanic Details
## 🕹️ Directional Jumps
- Characters can **jump in 8 directions** using the input system.
- Each jump direction is treated as a **unique stance** (implicitly):
    - Unlocks **direction-specific aerial move sets**.
    - Adds depth and character flexibility **without explicit stance input**.
    - Enables mixups, evasive movement, and offensive setups.
    - Character specific, most will **not** have 8 jump stances and/or will have the same options from multiple directions

## 🔁 Double Jump / Aerial Variant
- Most characters have **a second air action**, including:
    - **Standard double-jump**.
    - **Character-specific variants**:
        - Air-dash
        - Flip-kick
        - Hover/floats
- Designed to **extend mixups or reposition**, not to reset pressure.
    

## 🛡️ Air Blocking
- Air blocking is allowed **only after a short delay** post-jump:
    - Prevents **abusive pre-buffered defense**.
    - Forces **commitment to aerial choices**.
- Air blocking has **trade-offs**:
    - Most characters have low [Balance](Game%20Design/Mechanics/Balance.md) while airborne.
    - Vulnerable to **launchers** and **aerial knockdowns**.
    - Meant to soften attacks, not nullify pressure.
- **To Try**: Air blocking with **soft penalties**:
	- Slower fall / Downward gravity reset

---

# 🎯 Design Intent
- **Mobility-First Combat**: Players can control space and tempo through movement variety, not just buttons.
- **Depth Without Clutter**: Directional jumps act as implicit stances, enabling diverse options while keeping inputs clean.
- **Risky Air Game**: Air options are strong but vulnerable—forcing players to balance aggression and caution.
- **Interconnected Systems**: Movement ties directly into [Balance](Game%20Design/Mechanics/Balance.md), [Stagger](Game%20Design/Mechanics/Stagger.md), and [Counter](Game%20Design/Mechanics/Counter%20System.md) to ensure every choice has consequence.
- **Expressive Play**: Characters express personality through unique aerial behavior and grounded movement styles.