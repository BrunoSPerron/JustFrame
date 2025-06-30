# 🧠 Concept Overview

The combo system emphasizes **precision, stance dynamics, and risk-managed reward**. It rejects chain or cancel-based systems in favor of **manual links**, leveraging a combination of **dynamic hitstun**, **stance-specific routing**, and **scaling mechanics** to preserve skill expression and prevent degenerate loops.

Combos are meant to feel earned, **not automated**, with real decisions at every phase—from opener, to juggle, to knockdown.

---

# ⚙️ Mechanic Details

### Core Rules
- **Manual Links Only**: No cancels, or chains. Players must manually time each hit.
- **Dynamic Hitstun**: Varies based on opponent’s state (standing, crouching, airborne, etc.), affecting routes.
- **Stance-Dependent Routing**: Your stance (neutral, crouch, rising, jump directions) directly alters move access and combo potential.

---

### Juggle Logic
- **Launcher-Driven Combos**: Juggles begin with launchers; launch height determines combo duration.
- **Launch Scaling**: Each juggle hit reduces vertical launch. If launch power drops below a threshold, the combo ends naturally.
- **No Bounce Mechanics**: No ground bounce or wall bounce to artificially extend juggles.
- **Wall Splat Only**: Wall splats provide positional advantage or short wall combos but don’t extend juggles.
- **Air Launchers**: Launching mid-juggle slightly increases vertical height, encouraging creative air routes.

---

### Stance-Based Routing
- Stance determines:
    - **Move availability**
    - **Frame data / hitstun values**
    - **Combo routes and launch behavior**
- Common states:
    - **Standing**
    - **Crouching**
    - **Rising**
    - **Airborne (8 jump directions)**
    - **Character-specific states**

---

### Combo Limits

- **Flat Damage Scaling**: Each hit applies a cumulative penalty (e.g., -15% per hit).
    
- **Launch Height Scaling**: Primary limiter for juggle length. Prevents infinite air routes naturally.
    
- **Aerial Recovery**: From the 3rd hit onward, defenders can attempt recovery, potentially ending the combo early.
    

---

### Block Pressure Rules

- No global system for pushback, frame decay, or auto-reset.
- Pressure naturally ends after 2~3 blocked hits.  
- Blockstrings are finite—**your toolkit, not the engine, defines pressure.**

---

### Counter-Hit Rules
- **No Extra Hitstun**: Counter-hits don’t grant extended combos.
- **Balance Penalty**: Light counter = -5 Balance, heavy = -15 (tunable).
- **Stagger**: Counter-hit can give up to a high [stagger](Game%20Design/Mechanics/Stagger.md) value
- Counter-hits open opportunity, not damage routes. Keeps offense **honest** and **measured**.

---

### Knockdowns & Enders

- **Soft vs. Hard Knockdowns**:
    - Knockdown strength scales with defender’s Balance.
    - High Balance = soft knockdown (techable // fast rise options).
    - Low Balance = hard knockdown (guaranteed oki).

- **Aerial Takedown**
    - Basic descending attack with high knockdown power.
    - Causes grounded knockdown with no special cinematic or gimmick.
    - highly punishable, usually -17 on block

---

# 🎯 Design Intent

- **Emphasize skill**: Timing, awareness, and adaptation over memorized chains.
- **Encourage variety**: Stance diversity and move context push players to explore.
- **Prevent loop abuse**: Launch scaling and lack of bounce mechanics kill infinite juggles at the root.
- **Respect defense**: Let defenders recover, counter-hit wisely, and re-engage.
- **Maintain flow**: Every system supports the game's tactical pace without system gimmicks or padding.