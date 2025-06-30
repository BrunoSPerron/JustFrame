
- 🥋 Skill & Depth > Accessibility
	- Emphasis on **player expression**, **precision**, and **adaptation**.
	- Designed for **high skill ceiling**, not simplified onboarding.
	- No artificial equalizers or rubber-banding mechanics.
- 🎮 Gameplay-First
	- Everything revolves around **engaging, expressive combat**.
	- Avoid bloated meta-systems or distractions from core mechanics.
	- Priority is always **feel**, **flow**, and **interaction**.
- 🧼 Clean UI Philosophy
	- Minimalist UI: No visual clutter, no overexplanation.
	- Player should **read the situation**, not the screen.
	- Trust in **animation, spacing, and timing** to tell the story.

- 🚫 Avoid Resource Management
	- No meters, gauges, or consumable

---

# Distillated Wisdom

#### 1. Clarity First, Always

**Why it matters**: If players can’t read what’s happening, they can’t make decisions—and depth collapses.

**Design**
- Favor readable animations, consistent hit effects, and minimal UI.
- Telegraph intent and commitment (windups, recovery, spacing).
- Prioritize animation/feedback rhythm over system noise.

**Code**
- Build your engine to sync animations tightly with hitboxes, hurtboxes, and state changes.
- Prioritize deterministic, testable state transitions. Avoid unpredictable or opaque behavior.

---

#### 2. Meaningful Commitment = Real Tension

**Why it matters**: Without commitment, there’s no risk; without risk, there’s no mindgame.

**Design**
- Inputs and movement should commit—no loosey-goosey cancels or overly safe options.
- Avoid input bloating—depth comes from choice context, not command lists.
- Make actions punishable and decisions visible (via startup/recovery, lock-ins).

**Code**
- State transitions must be strict: once committed, a move plays out unless design says otherwise.
- Avoid adding “just-in-case” interrupts or rollback-friendly fudge—build for clarity, not undo.

---

#### 3. Depth Through Universality

**Why it matters**: Shared systems let players explore the full game, not just their character.

**Design**
- Keep core systems consistent across the cast (movement, defense, knockdown).
- Character variety should emerge from playstyle and timing differences, not bespoke subsystems.
- Think "one engine, many strategies"—like VF or Tekken.

**Code**
- Architect a clean, modular base system that characters inherit from.
- Avoid per-character hacks or exceptions unless they add meaningful difference.
- Prioritize data-driven behavior for tuning (startup, frame data, priority) instead of hardcoding logic.

---

#### 4. Movement is Strategy

**Why it matters**: Movement defines neutral, pressure, escape, and mindgames.

**Design**
- Movement should feel intentional and expressive: no floaty jumps, no autopilot dashes.
- Incorporate commitment into movement: sidesteps, dash recovery, jump arcs.
- Make positioning affect move success, escape routes, and initiative.

**Code**
- Build a responsive, momentum-aware movement system.
- Let movement states influence input parsing and action availability.
- Hardcode friction, momentum, and commitment timings—this isn’t just "feel," it’s core balance.

---

#### 5. Every Phase Must Be Playable

**Why it matters**: Fights are only fun when both players are always engaged.

**Design**
- Knockdown, air reset, pressure—always give the defender meaningful timing, option select, or counterplay.
- Avoid long uninteractive phases (like cinematic combos, infinite pressure).
- Consider fast wake-ups, layered okizeme, or counter-timing mechanics.

**Code**
- Ensure game states expose player agency wherever possible—recovery techs, reversals, buffer windows.
- Track and expose frame advantage data in debug tools to validate fairness and risk design.

---

#### 6. Risk/Reward is the Real Meta

**Why it matters**: If choices don’t have real stakes, the game’s mindgame layer dies.

**Design**
- Every move should have an obvious tradeoff: startup, recovery, whiff punish risk.
- Don’t shield players with safe pressure or input leniency—risk is the fun part.
- Use counter-hit states, guard breaks, and throw escapes as expressive risk layers.

**Code**
- Build your hit detection and frame tracking system to natively support advantage/disadvantage and counter-state triggers.
- Make sure punishment is reliable and consistent at all frame timings.

---

#### 7. Psychology > Mechanics

**Why it matters**: Players win with their brains, not their thumbs.

**Design**
- Favor mindgames, feints, baiting, and tempo shifts over meter management or memory tests.
- Minimal UI, maximum read-based gameplay (think Kengo, VF, Power Stone).
- Let hesitation and tempo be valid tactics.

**Code**
- Input buffering and read windows should respect rhythm—not just raw input speed.
- Support feint actions, cancel points, fake-outs cleanly in your animation/state graph.



-----

# Core Tenet

- **Expressive Precision:**  
    Inputs are simple, but every frame counts. Mastery means _clarity under pressure_, not memorizing scripts.
    
- **Movement is Power:**  
    Jumps, steps, rolls—movement isn’t just evasion, it’s a canvas for offense, defense, and baiting. The fight _moves_, not just trades.
    
- **Risk Means Reward:**  
    You don’t win by being safe—you win by _reading_, _committing_, and _executing_. The system supports this at every level.
    
- **Diegetic Combat:**  
    The screen tells you _everything_. Mechanics aren’t hidden—they’re _felt_, _heard_, _seen_ in the way characters react and move. No bars. No noise. Just fighters.
    
- **Tactical Engagement at All Times:**  
    On the ground? In the air? Knocked down? You’re never out. You’re _thinking_, _countering_, _trying_. Always.