#!/usr/bin/env bash
#
# compile-check.sh — build/syntax-check every code sample in this repo.
#
# Runs three kinds of checks and reports a per-sample PASS/FAIL summary:
#   * PlatformIO projects (a directory with platformio.ini)  -> `pio run`
#   * Arduino .ino sketches                                   -> `arduino-cli compile`
#   * CircuitPython .py sample files                          -> `python3 -m py_compile`
#
# Missing tools (pio / arduino-cli / python3 / west) are reported as SKIP,
# not FAIL, so the script is usable in a partial toolchain. Exits non-zero
# if any check actually fails.
#
# Prerequisites for a full run:
#   arduino-cli core install STMicroelectronics:stm32 adafruit:nrf52 esp32:esp32
#   arduino-cli lib install "Blues Wireless Notecard" "Adafruit BME680 Library" \
#       "Adafruit SSD1306" "Adafruit GFX Library"
#   (PlatformIO downloads its own platforms/libs on first build.)

set -u
cd "$(dirname "$0")/.."   # repo root

PASS=0 FAIL=0 SKIP=0
FAILED_ITEMS=()

have() { command -v "$1" >/dev/null 2>&1; }

report() { # status label
  case "$1" in
    PASS) PASS=$((PASS+1)); printf '  \033[32mPASS\033[0m  %s\n' "$2" ;;
    FAIL) FAIL=$((FAIL+1)); FAILED_ITEMS+=("$2"); printf '  \033[31mFAIL\033[0m  %s\n' "$2" ;;
    SKIP) SKIP=$((SKIP+1)); printf '  \033[33mSKIP\033[0m  %s\n' "$2" ;;
  esac
}

# Map an Arduino .ino sketch directory to an arduino-cli FQBN.
# NOTE: The SparkFun MicroMod STM32 uses SparkFun's own core; we compile it
# against the STM32F405 Feather FQBN as a proxy (same STM32F405 MCU).
fqbn_for() {
  case "$1" in
    */stm32/stm32f405-feather/*)  echo "STMicroelectronics:stm32:GenF4:pnum=FEATHER_F405" ;;
    */stm32/micromod-stm32/*)     echo "STMicroelectronics:stm32:GenF4:pnum=FEATHER_F405" ;;
    */test/stresstest/*)          echo "STMicroelectronics:stm32:GenF4:pnum=FEATHER_F405" ;;
    *)                            echo "" ;;
  esac
}

echo "== PlatformIO projects =="
if have pio; then
  while IFS= read -r ini; do
    dir="$(dirname "$ini")"
    if (cd "$dir" && pio run >/tmp/pio_out.$$ 2>&1); then
      report PASS "pio: $dir"
    else
      report FAIL "pio: $dir"; sed 's/^/      /' /tmp/pio_out.$$ | tail -15
    fi
  done < <(find . -name platformio.ini -not -path '*/.pio/*' | sort)
  rm -f /tmp/pio_out.$$
else
  report SKIP "pio not installed (PlatformIO projects not built)"
fi

echo "== Arduino .ino sketches =="
if have arduino-cli; then
  while IFS= read -r ino; do
    dir="$(dirname "$ino")"
    fqbn="$(fqbn_for "$dir")"
    if [ -z "$fqbn" ]; then
      report SKIP "arduino-cli: $dir (no FQBN mapping)"; continue
    fi
    if arduino-cli compile --fqbn "$fqbn" "$dir" >/tmp/acli_out.$$ 2>&1; then
      report PASS "arduino-cli ($fqbn): $dir"
    else
      report FAIL "arduino-cli ($fqbn): $dir"; sed 's/^/      /' /tmp/acli_out.$$ | tail -15
    fi
  done < <(find . -name '*.ino' -not -path '*/.pio/*' -not -path '*/.git/*' | sort)
  rm -f /tmp/acli_out.$$
else
  report SKIP "arduino-cli not installed (.ino sketches not built)"
fi

echo "== CircuitPython .py samples =="
if have python3; then
  # Sample entry points only; vendored libraries under lib/ are skipped.
  while IFS= read -r py; do
    # Syntax-check without writing any .pyc bytecode/__pycache__ artifacts.
    if python3 -c 'import sys; compile(open(sys.argv[1]).read(), sys.argv[1], "exec")' "$py" 2>/tmp/py_out.$$; then
      report PASS "py syntax: $py"
    else
      report FAIL "py syntax: $py"; sed 's/^/      /' /tmp/py_out.$$
    fi
  done < <(find . -name '*.py' -not -path '*/lib/*' -not -path '*/.pio/*' -not -path '*/.git/*' | sort)
  rm -f /tmp/py_out.$$
else
  report SKIP "python3 not installed (.py samples not checked)"
fi

# Zephyr and MCUboot examples now live in the note-zephyr repo
# (https://github.com/blues/note-zephyr) and are built/verified there.

echo
echo "Summary: $PASS passed, $FAIL failed, $SKIP skipped"
if [ "$FAIL" -gt 0 ]; then
  printf 'Failed:\n'; printf '  - %s\n' "${FAILED_ITEMS[@]}"
  exit 1
fi
exit 0
