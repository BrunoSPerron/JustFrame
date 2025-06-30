# 🥋 Core Frame Ruleset – Character Moveset Design

## ✅ Fastest Attack: `i12`
- Fastest possible normal is **12 frames**.
- No punishes are possible faster than this.
- Punishes within this range are **read-based** or require **prior knowledge**.

## ✅ Punishable on Block: `-12 to -17`
- Any move **-12 or worse on block** is punishable by the i12 attack.
- Breakdown:
  - `-12 to -13`: Only punishable by i12 attacks.
  - `-14 to -17`: Punishable by broader fast move pool (i13–i17).
- Moves **worse than -17** are **heavily unsafe** and open to big punishes or combos.

---

## 📊 Move Categories Reference

|Move Type|Startup|On Block|Purpose|
|---|---|---|---|
|**Jab / Fast Poke**|`i12`|`-2 to -6`|Fastest punish, pressure tool|
|**Mid Poke**|`i14–16`|`-5 to -9`|Range/mixup starter|
|**Power Move**|`i18+`|`-12 to -17`|Big reward, high risk|
|**Launcher**|`i16–20+`|`-14 to -20`|Risky, high reward|
|**Sweep / Low Crush**|`i15–18`|`-13 to -17`|Risky mixup low|
|**Grab / Throw**|`i16–18`|`—`|Beat block, reset, or oki tool|

---

## 🔍 Design Implications

### 1. **Block Punish Thresholds**
- **Safe on block**: `-11 or better`
- **Punishable on block**: `-12 to -17`
- **Highly unsafe**: `-18 or worse` (e.g., launcher punishable)

### 2. **Risk/Reward Tuning**
- **Hard-to-land but powerful** moves: `-14 to -17`
- **Very slow / heavy commitment** moves: `-18 or worse`
- Use frame data to balance high-reward options with risk.

### 3. **Mixups and Reactability**
- **Reactable** by top-level players: `15–17f` startup
- **Unreactable** / read-based: `<13f` startup


---

## ⚙️ Summary
- `i12` defines the **punish window floor**.
- `-12` defines the **safe/unsafe threshold**.
- Use these rules to control pressure, risk, and defensive viability across your character’s moveset.
