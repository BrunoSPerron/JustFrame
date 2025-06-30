## 🧠 Concept Overview: Focus

Focus is a defensive mechanic that triggers a brief time dilation effect when the player is about to be hit by a fast, non-reactable move in neutral. It provides a short reaction window—just enough to allow a well-timed block or counter input. The goal is to enhance defense against fast attacks without removing skill or commitment.

## ⚙️ Mechanic Details
- Trigger Condition:
     - Automatically activates if:
          - Defender is in neutral
           - About to be hit by a non-reactable move (e.g., 14f startup or faster)
           - Has enough Focus available
- Effect:
	- Brief slowdown effect (approx. 3–4 frames worth of simulation stretch)
    - No automatic defense—player must still press G (Block) or execute a Counter if desired
    -  Includes visual and audio FX: wind burst, radial ripple, subtle screen ripple, and sound cue
	- Optional: slight camera shake or time dilation localized to attacker
- Focus Resource:
    -  Small regenerating pool (e.g., 100 max)
    - Each activation costs ~30–40
    - Regenerates over time (e.g., 10 per second)
    - Only visible as a subtle visual state, not a full HUD meter
- Limitations:
	- Doesn’t trigger in blockstun, recovery, jump, or wake-up states
    - Doesn’t trigger on slow moves
    - Can be baited if attacker pressures intelligently
    - Cannot be spammed—Focus must regenerate between uses

## 🎯 Design Intent
- Elevate defensive decision-making without removing skill expression
- Allow trained players to defend against mixups that would otherwise feel like coin-flips
- Maintain risk/reward for fast, high-commitment attacks by making their timing more telegraphed, not invalidated
- Introduce a new defensive layer without cluttering UI or adding passive meters
- Reinforce the game’s core principle: "Every Frame is a Choice", by making high-speed decisions more visible and intentional

## 🔧 Development Notes
 - Tune for perceptibility over precision—FX must sell the effect in 2–3 frames
- Consider tagging specific attacks as bCanTriggerFocus = true to restrict scope and reduce false positives
- Focus is a supportive mechanic, not a comeback tool—don’t let it reset or reverse momentum on its own