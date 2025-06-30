
## 🧠 Concept Overview

The **Balance Mechanic** represents a character’s current stability and resistance to knockdown. It acts as a dynamic threshold that is constantly challenged by the opponent’s attacks. Rather than relying solely on health or damage to determine advantage, the Balance system rewards precision, timing, and positioning.

Balance directly interacts with **Knockdown Power**, **Stagger**, and the game’s **aerial states**, creating a layered system that supports expressive offense and reactive defense.

Balance also influences **frame advantage** on-hit and on-block, dynamically adjusting pressure outcomes based on the defender’s current stability.

---

## ⚙️ Mechanic Details

### **Move Properties**

Every attack has two relevant stats:

- **Knockdown Power** – A numerical value representing the destabilizing force of the move.
- **Balance Value** – A numerical resistance threshold of the defender, which changes based on state and previous interactions.

### **Knockdown & Stagger Logic**

- **Knockdown** occurs if:
    - `Attack Knockdown Power > Defender Balance`
- If Knockdown Power **approaches but does not exceed** Balance:
    - Defender receives a **proportional amount of Stagger**.
    - The ratio between Knockdown Power and Balance can scale:
        - Stagger duration  
        - Stagger type (e.g., flinch, stumble, forced crouch)  
        - Follow-up vulnerability  

### **Balance Scaling Examples**

- **Full Knockdown**: Knockdown Power is significantly higher than Balance.
- **Heavy Stagger**: Knockdown Power is just below Balance.
- **Minor Flinch**: Knockdown Power is close but not threatening.

---

## 📏 Rules & Modifiers

### **Orientation Bonus**

- Attacks from the **side or rear** apply an **Orientation Bonus**, increasing their effective Knockdown Power.
    - Encourages flanking and movement-based play.

### **Airborne Rules**

- While **airborne**, defenders:
    - Cannot be staggered (no flinch/stumble reactions).
    - Suffer from **reduced Balance**, making them more susceptible to knockdown.
    - This enables juggle starters or **aerial takedowns** when Balance is broken mid-air.

### **Frame Advantage Scaling**

- On **hit or block**, the players frame advantage are modified based on their **current Balance**.
- A defender with **low Balance** will suffer **increased frame disadvantage**, allowing deeper pressure or new combo routes.
- A defender with **high Balance** may reduce the attacker’s advantage, potentially preventing follow-ups or making risky moves safer to challenge.

This scaling affects:
- Most grounded normals and specials
- Select pressure tools and stagger states  
- Option: **Not applied** to launchers or key combo starters to preserve route consistency

---

## ♻️ Dynamic Balance

- Balance is **not static**:
    - May **regenerate** slowly when idle or blocking cleanly.
    - May **decay** from continuous blocking, hitstun, or specific anti-mash penalties.
    - Can be influenced by **character traits**, stances, or abilities.

---

## 🎯 Design Intent

- **Encourages Deliberate Offense**:
    - Players must manage spacing, timing, and character orientation to break through Balance effectively.
    - Power-spamming is discouraged; tactical setup and precision are rewarded.

- **Defensive Skill Expression**:
    - Blocking well and recovering from stagger builds **momentum resistance**.
    - Characters can clutch out of bad situations by preserving Balance intelligently.
    - Frame advantage scaling rewards players who maintain composure and clean defense.

- **Synergizes With Other Systems**:
    - **Stagger System**: Adds nuance between full knockdowns and simple hits.
    - **Counter System**: Allows defenders to reverse pressure with well-timed input, especially while their Balance is under pressure.
    - **Aerial Recovery**: Encourages attackers to break air Balance for reward, while defenders manage aerial survivability.
    - **Frame Scaling**: Expands the Balance system into every hit/block interaction, creating dynamic pressure windows and varied offense timing.

- **Foundation for Depth**:
    - Balance provides **read-based tension**: When is the opponent’s Balance low enough to risk a power move?
    - Unlocks character-specific options and mixups based on the opponent’s current Balance state.

---

## 🔮 Future Considerations

- **Visual Feedback**: Subtle, [diegetic indicators of Balance state](Procedural%20Posture%20Feedback%20System.md)

- **Character Differentiation**:
    - Tanky characters: Higher base Balance, slower regen.
    - Agile characters: Lower Balance, but recover faster or avoid orientation-based penalties.

- **Advanced Interactions**:
    - Chain knockdown/stagger setups  
    - Conditional moves that deal bonus Knockdown Power if opponent’s Balance is under X%  
    - Balance-sensitive pressure strings and traps that only work at low Balance thresholds
