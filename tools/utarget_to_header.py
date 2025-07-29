#!/usr/bin/env python3
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
    return config

def write_target_header(config, output_path):
    with open(output_path, 'w') as f:
        f.write("// Auto-generated from .utarget file. DO NOT EDIT.\n\n")
        f.write("#pragma once\n\n")
        for key, value in config.items():
            if key.startswith("USE_"):
                f.write(f"#define {key}\n")
            else:
                f.write(f"#define {key:<12} {value}\n")

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
