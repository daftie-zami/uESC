#!/usr/bin/env python3

import sys
import json

if len(sys.argv) != 3:
    print("Usage: get_json_field.py <json_file> <field>")
    sys.exit(1)

json_path = sys.argv[1]
field = sys.argv[2]

with open(json_path) as f:
    data = json.load(f)

print(data.get(field, ""))
