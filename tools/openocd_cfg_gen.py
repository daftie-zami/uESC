#!/usr/bin/env python3
"""
This script generates an OpenOCD configuration file based on the selected interface and full target chip name.
Example usage:
    python3 tools/openocd_cfg_gen.py --interface STLINK --target stm32f103c8t6 --build-dir build
"""

from pathlib import Path
import argparse
import sys

INTERFACES = {
    "STLINK": "interface/stlink.cfg",
    "JLINK": "interface/jlink.cfg",
    "CMSIS-DAP": "interface/cmsis-dap.cfg",
}

FAMILY_MAP = {
    "STM32F0": "target/stm32f0x.cfg",
    "STM32F1": "target/stm32f1x.cfg",
    "STM32F3": "target/stm32f3x.cfg",
    "STM32F4": "target/stm32f4x.cfg",
    "STM32G4": "target/stm32g4x.cfg",
}

def identify_family(chipname: str) -> str:
    upper = chipname.upper()
    for prefix in FAMILY_MAP:
        if upper.startswith(prefix):
            return prefix
    return None

def main():
    parser = argparse.ArgumentParser(description="OpenOCD config file generator")
    parser.add_argument("--interface", required=True, choices=INTERFACES.keys(), help="Debug probe interface")
    parser.add_argument("--target", required=True, help="Full chip name (e.g. stm32f103c8t6)")
    parser.add_argument("--build-dir", required=True, help="Directory to output the config file")

    args = parser.parse_args()

    build_dir = Path(args.build_dir)
    cfg_path = build_dir / "openocd.cfg"
    build_dir.mkdir(parents=True, exist_ok=True)

    chipname = args.target.upper()
    family = identify_family(chipname)

    if family is None:
        print(f"[ERROR] Could not identify family from chip name '{chipname}'", file=sys.stderr)
        sys.exit(1)

    interface_cfg = INTERFACES[args.interface]
    target_cfg = FAMILY_MAP[family]

    content = f"""\
source [find {interface_cfg}]
set CHIPNAME {chipname}
transport select hla_swd
source [find {target_cfg}]
"""

    try:
        cfg_path.write_text(content)
        print(f"[INFO] OpenOCD config file generated at: {cfg_path}")
    except Exception as e:
        print(f"[ERROR] Failed to write OpenOCD config: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
