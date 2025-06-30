# 🧠 Concept Overview

**Stagger** is a brief, controlled slowdown state designed to **interrupt a player's momentum** without fully disabling their actions. It is **not a stun**, **does not accumulate**, and **cannot directly cause a knockdown**. Its purpose is to inject a moment of tension or hesitation without sacrificing agency.

It works as a **tempo control mechanic**, influencing [Balance](Game%20Design/Mechanics/Balance.md) and rewarding players who stay mobile and aware under pressure.

---

# ⚙️ Mechanic Details
## ⏱️ Duration and Effect
- **Stagger is short**—measured in frames, not seconds.
- Designed for **brief impact** on decision timing, not long-term disruption.
- **Slows specific actions**, such as attacks or blocking, but **movement remains mostly unaffected**.

## 🔢 Stagger Value
- A **numeric value** represents the strength of stagger.
    - **Decays rapidly** on its own.
    - **Higher stagger values decay faster**.
- Influences [Balance](Game%20Design/Mechanics/Balance.md) calculations but cannot directly trigger a knockdown.

## 🛡️ Counterplay: Movement
- The best way to escape stagger is **to move**:
    - Moving actively **reduces stagger value faster**.
    - **Dash, step, or jump** actions are viable escape tools.
- Passive defense (e.g., blocking or crouching) is less effective during stagger.
    

## 🧍‍♂️ Animation Handling
- **Stagger overlays on existing animations**, rather than replacing them.
    - Blend dynamic stagger effects with current actions (e.g., limb reactions or full-body weight shifts).
- Core rule: **Stagger must resolve before the next input finishes**, or else transitions will feel unresponsive.
- **Minor hitbox distortions are acceptable** if they preserve flow and feedback.

## 🛠️ Implementation Guidelines
- Use **animation layering** with logic for:
    - **Partial-body overrides** (e.g., upper body reacts while legs move normally).
    - **Timed recovery blending** between stagger and next move.
    - **Blend shape or socket offset** for responsive hitbox updates.
- **Edge cases to visualize and test**
    - Mid-air stagger (especially during juggle).
    - Staggered recovery into or out of a **stance**.
    - Effects on **stance transitions** or **charge moves**.

---

# 🎯 Design Intent

- **Control Tempo, Not Agency**: Forces hesitation or hesitation _moments_ without removing the ability to act.
- **Micro-Interaction Depth**: Adds texture to exchanges—especially counter-hits and defensive timing games.
- **Reward Movement, Punish Panic**: Encourages active, thoughtful movement rather than button mashing during stagger.
- **Clean Integration with Systems**: Built to complement [Balance](Game%20Design/Mechanics/Balance.md), [Counter](Game%20Design/Mechanics/Counter%20System.md), and [movement](Game%20Design/Mechanics/Movement.md) without interfering with core responsiveness.