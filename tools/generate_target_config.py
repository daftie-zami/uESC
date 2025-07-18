#!/usr/bin/env python3
import json
import sys
from pathlib import Path

def generate_header(target_file, output_file):
    with open(target_file, 'r') as f:
        config = json.load(f)

    lines = [
        "#ifndef TARGET_CONFIG_H",
        "#define TARGET_CONFIG_H\n",
        f"// Auto-generated from {target_file}\n",
        f"#define MCU_MODEL           {config['mcu']}",
        f"#define MCU_FAMILY          {config['family']}",
        f"#define FLASH_SIZE_KB       {config['flash_kb']}",
        f"#define RAM_SIZE_KB         {config['ram_kb']}",
        f"#define SYS_CLOCK_HZ        {config['clock_hz']}",
        f"#define USE_HSE             {int(config['clock_sources']['use_hse'])}",
        f"#define HSE_FREQ_HZ         {config['clock_sources']['hse_freq']}",
        "\n#endif // TARGET_CONFIG_H"
    ]


    with open(output_file, 'w') as out:
        out.write('\n'.join(lines))

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python generate_target_config.py targets/bluepill.json include/target_config.h")
        sys.exit(1)

    generate_header(sys.argv[1], sys.argv[2])
