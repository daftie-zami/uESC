#!/usr/bin/env python3
"""
This script converts a `.upin` pin configuration file into a CubeMX-style C header file.

Example:
    LED = PA6  →  
        #define LED_GPIO_Port   GPIOA
        #define LED_Pin         GPIO_PIN_6

Usage:
    python3 tools/upin_to_header.py board.upin board_pins.h
"""

import argparse
import re
from pathlib import Path

def parse_upin_file(file_path):
    defines = []
    with open(file_path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith(('#', ';')):
                continue
            if "=" in line:
                key, value = line.split("=", 1)
                key = key.strip().upper()
                value = value.strip().upper()
                defines.append((key, value))
    return defines

def split_pin(value):
    match = re.fullmatch(r"P([A-Z])(\d+)", value)
    if not match:
        return None
    port = f"GPIO{match.group(1)}"
    pin = f"GPIO{match.group(2)}"
    return port, pin

def generate_header(defines, header_path):
    header_path.parent.mkdir(parents=True, exist_ok=True)

    with open(header_path, 'w') as f:
        f.write("// Auto-generated from .upin file. DO NOT EDIT.\n\n")
        f.write("#pragma once\n\n")
        for key, value in defines:
            pin_info = split_pin(value)
            if pin_info:
                port, pin = pin_info
                f.write(f"#define {key}_GPIO_Port        {port}\n")
                f.write(f"#define {key}_Pin              {pin}\n")
                
            else:
                f.write(f"#define {key} {value}\n")

def main():
    parser = argparse.ArgumentParser(description="Convert .upin to CubeIDE-style C header")
    parser.add_argument("input", type=Path, help=".upin file path")
    parser.add_argument("output", type=Path, help="output .h file path")

    args = parser.parse_args()

    if not args.input.exists():
        print(f"[ERROR] File not found: {args.input}")
        return

    defines = parse_upin_file(args.input)
    generate_header(defines, args.output)
    print(f"[INFO] pins.h generated: {args.output}")

if __name__ == "__main__":
    main()
