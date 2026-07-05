# Copilot / AI Agent Instructions for LPTM 🔧

## Project snapshot
- PlatformIO firmware for an **ESP32 (env: `esp32doit-devkit-v1`)** using the **Arduino** framework.
- Purpose: Low-power timer module with RTC + FRAM + peripheral drivers (relays, LEDs, Modbus, etc.).
- Layout (important paths):
  - `platformio.ini` — envs & board (esp32doit-devkit-v1)
  - `src/` — `main.cpp` (Arduino `setup()`/`loop()`), app glue code
  - `include/` — public headers (project-wide)
  - `lib/` — hardware driver modules (each library has `.h`/`.cpp` pair)
  - `test/` — PlatformIO unit tests (Unity)

## Big picture & intent 💡
- Design goal: keep changes minimal and hardware-friendly. Hardware interactions live in `lib/` modules (e.g., `BRD_TimerModule`, `Rtc1307`, `FM24C32`).
- Modules follow a simple lifecycle pattern: construct → `begin()` (init) → `update()` / `FSM_Handler()` (called from `loop()`). Try to preserve that pattern when adding features.
- Typical communication buses and devices:
  - I2C (Wire): RTC (wrapper `Rtc1307`), FRAM (`FM24C32` at 0x50 by default)
  - Serial: debug prints use `Serial.begin(9600)` in `BRD_TimerModule::begin()` — watch for IO blocking in early init

## Key files & examples (practical) 🔧
- `lib/BRD_TimerModule.{cpp,h}` — board-level initialization and example FRAM/RTC tests. Note: `begin()` currently performs FRAM read/write tests and prints to Serial; guard or remove verbose tests in production flows.
  - Example: `fram.writeString(0x0100, "Hello FRAM!")`, `rtc.readDateTimeString()`
- `lib/Rtc1307.{cpp,h}` — small RTC wrapper around RTClib/DateTime. Use `rtc.begin()`, `rtc.readDateTimeString()`, `rtc.setDateTime(...)`.
- `lib/FM24C32.{cpp,h}` — FRAM helper that chunk-writes to avoid Wire buffer limits. Expect 16-bit memory addressing and device addr 0x50.
- `lib/` contains other device drivers (ModbusMaster/Slave, RELAY, LED, RMC5230Control, etc.) — follow existing API patterns.

## Developer workflows (commands) ✅
- Build: `platformio run`
- Build this env explicitly: `platformio run -e esp32doit-devkit-v1`
- Upload: `platformio run -e esp32doit-devkit-v1 -t upload`
- Serial monitor (match baud used in code, commonly 9600): `platformio device monitor -e esp32doit-devkit-v1 -b 9600`
- Unit tests: `platformio test` (use Unity; prefer hardware-agnostic tests)
- Clean: `platformio run --target clean`
- Debug (if supported): `platformio debug -e esp32doit-devkit-v1`

## Project-specific conventions & patterns 📐
- Public headers live in `include/` and are included with `#include "MyHeader.h"`.
- Drivers live under `lib/<Name>/` and expose a class with `begin()` and runtime `update()`/`FSM_Handler()` hooks.
- Many modules currently use small global instances inside `.cpp` files (e.g., `Rtc1307 rtc;`, `FM24C32 fram(0x50);`) — avoid duplicating these globals across files.
- Prefer non-blocking logic in `loop()`; existing code has some blocking tests/delays in `BRD_TimerModule` — be cautious when integrating.
- Use PlatformIO LDF rather than adding explicit `lib_deps` unless a dependency is external and not auto-detected.

## Tests & hardware isolation 🧪
- Keep unit tests hardware-agnostic by isolating logic from I/O. Example: put time math, parsing, and scheduling logic in testable functions in `lib/Time.*` or utility modules.
- When hardware interaction must be tested, add small integration tests and document required hardware or mocks.

## Integration & cross-component notes 🔁
- I2C device addresses and Wire behavior matter: `FM24C32` uses chunked writes to avoid the Wire 32-byte buffer; review `MAX_I2C_WRITE_DATA` if changing.
- Serial prints are used for debugging and FRAM tests — expect verbose output during `BRD_TimerModule::begin()`.
- When adding or changing shared state, prefer passing references or adding explicit getters/setters to avoid implicit cross-file globals.

## PR checklist (quick) ✅
1. Build locally: `platformio run` (resolve compile errors)
2. Add/modify unit tests in `test/` for pure logic where possible
3. Avoid adding new blocking `delay()` calls in production path; if necessary, document rationale
4. Run `platformio device monitor -e esp32doit-devkit-v1 -b 9600` to confirm runtime output
5. Update `platformio.ini` only if you add a new environment or change board settings (leave a comment explaining the change)

---
If any piece of hardware integration or a module API is unclear, tell me which file or feature you want expanded and I will add a short example or a test template. 🙋‍♂️
