# 🧠 Concept Overview

Aerial Recovery is a **skill-based defensive mechanic** that allows players to escape [juggle](Game%20Design/Mechanics/Combo%20system.md#Juggle%20Logic) hitstun by pressing **[Guard (G)](Game%20Design/Input%20Scheme.md)** with precise timing. It is **not a reversal**—there's no invincibility or guaranteed advantage—but it restores **neutral airborne state**.
The goal is to keep the defender **engaged during air combos** without giving them a free escape.

---

# ⚙️ Mechanic Details

### Activation

- Input: Precise input **[Guard (G)](Game%20Design/Input%20Scheme.md)** when hit during [juggle](Game%20Design/Mechanics/Combo%20system.md#Juggle%20Logic)
- Becomes available from the **3rd combo hit** onward (can be tuned per character or universally)
- Input window: Starts **tight (2–3 frames)**, slightly widens with combo length or air hitstun
    

### Effect

- Immediately ends [juggle](Game%20Design/Mechanics/Combo%20system.md#Juggle%20Logic) hitstun
- Defender enters a **neutral airborne state**
- **No invincibility or reversal properties**

### Post-Recovery Options

- Air block
- Double jump (if launched from ground)
- Character-specific air actions (as per stance)

### Tradeoffs

- Defender gains **half the usual on-block advantage**
- May still be punishable if attacker reads recovery

### Anti-Spam

- Block spamming is ineffective unless input is timed precisely
- **Spam protection**: input ignored if pressed in the **last ~10 frames**

---

# 🎯 Design Intent

- **Encourage defensive timing skill**, not panic mashing
- Allow defender **limited agency** in air combos without granting escape tools
- Reward attackers that use **varied combo** with different timing
- Enable **expressive defensive play** while preserving the attacker’s control of flow
- Build toward **diegetic, skill-based defensive mechanics**

---

# ⚖️ Balance Considerations

- **Perfectly timed recovery**: Defender resets to neutral, but attacker may still pressure
- **Loose combo timing by attacker**: Encouraged, as mind game against good defenders
- **Mashers**: Ineffective due to tight window and spam gating—timing is mandatory

---

# 🧩 Character-Specific Tuning

- **Fast jugglers**: Get stricter recovery windows to maintain pressure
- **Floaty characters**: May access earlier or more forgiving input windows
- **Grapplers/Tanks**: May lack this mechanic or receive weaker versions; rely on grounded options instead

---

# 🔧 Implementation Notes

- Add **audio/visual cue** for successful recovery input to reinforce skill feedback
- Scale input window dynamically based on combo length or cumulative air hitstun
- Penalize repeated invalid inputs with **temporary Balance loss**

---

# 💡 Additional Ideas

- Tie **Balance value** to input leniency: higher balance = wider window
- Slightly missed input could result in **partial balance restoration**
- Create mind-games around **timing bait** and air trap setups

---

# ✅ Summary

Aerial Recovery introduces **depth, skill expression, and interactive defense** during juggles. It **keeps both players engaged**, offers clear counterplay, and integrates cleanly into the game’s timing- and risk-reward-driven philosophy.

By maintaining modest benefits and clear drawbacks, this mechanic **enhances the aerial meta** without undermining offensive flow or combo structure.