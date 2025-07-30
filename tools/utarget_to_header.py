#!/usr/bin/env python3
"""
This script generates a C header file (`target.h`) from a `.utarget` configuration file.

It supports both key-value pairs and standalone flags:
    - DEVICE = stm32g474ceu6     →  #define DEVICE stm32g474ceu6
    - USE_HSE                    →  #define USE_HSE

It also auto-generates additional defines like RCC macros for timers:
    - MOTOR_TIM = TIM1           →  #define MOTOR_TIM_RCC RCC_TIM1

Example:
    .utarget file:
        DEVICE = stm32g474ceu6
        USE_HSE
        MOTOR_TIM = TIM1

    Output (`target.h`):
        #define DEVICE         stm32g474ceu6
        #define USE_HSE
        #define MOTOR_TIM      TIM1
        #define MOTOR_TIM_RCC  RCC_TIM1

Usage:
    python3 tools/gen_target_header.py path/to/target.utarget --out path/to/target.h
"""

import argparse
from pathlib import Path

def parse_utarget_file(path):
    config = {}
    with open(path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue

            if '=' in line:
                key, value = line.split('=', 1)
                config[key.strip()] = value.strip()
            else:
                config[line] = None
    return config

def write_target_header(config, output_path):
    with open(output_path, 'w') as f:
        f.write("// Auto-generated from .utarget file. DO NOT EDIT.\n\n")
        f.write("#pragma once\n\n")
        for key, value in config.items():
            if key.startswith("USE_") and value is None:
                f.write(f"#define {key}\n")
            elif key.startswith("USE_"):
                f.write(f"#define {key}\n")
            else:
                f.write(f"#define {key:<20} {value}\n")
        
        motor_tim = config.get("MOTOR_TIM", "").strip() if config.get("MOTOR_TIM") else ""
        if motor_tim:
            f.write(f"#define MOTOR_TIM_RCC        RCC_{motor_tim}\n")
        if motor_tim:
            f.write(f"#define MOTOR_TIM_RST        RST_{motor_tim}\n")


def main():
    parser = argparse.ArgumentParser(description="Generate target.h from .utarget file")
    parser.add_argument("utarget_file", help=".utarget file path")
    parser.add_argument("--out", default="src/common/target.h", help="Output target.h path (default: src/common/target.h)")
    args = parser.parse_args()

    config = parse_utarget_file(args.utarget_file)

    output_path = Path(args.out)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    write_target_header(config, output_path)
    print(f"[INFO] target.h generated at {output_path}")

if __name__ == "__main__":
    main()
