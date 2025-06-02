##!/opt/tbotspython/bin/python3

import re
import os
from glob import glob

OUTPUT_PATH = "proto/__init__.py"
INPUT_PATH = "proto/"

def extract_proto_defs(proto_file):
    """Extract only top-level message and enum names from a .proto file"""
    top_level_defs = []
    brace_level = 0  # Track nesting depth using braces

    with open(proto_file, 'r') as f:
        for line in f:
            # Skip lines with comments
            line = re.sub(r'//.*', '', line).strip()
            if not line:
                continue

            # Update brace level count
            brace_level += line.count('{')
            brace_level -= line.count('}')

            # Only process top-level definitions (brace_level == 0)
            if brace_level == 0:
                # Match top-level messages/enums
                match = re.match(r'\s*(message|enum)\s+(\w+)\b', line)
                if match:
                    top_level_defs.append(match.group(2))

    return top_level_defs

def generate_init_py():
    """Generate __init__.py with imports for top-level protobuf definitions"""
    proto_files = glob(os.path.join(INPUT_PATH, '*.proto'))
    imports = []
    all_names = []

    for proto_file in proto_files:
        if "ssl_simulation" in proto_file:
            continue
        base_name = os.path.basename(proto_file).replace('.proto', '')
        def_names = extract_proto_defs(proto_file)

        if def_names:
            imports.append(f"from .{base_name}_pb2 import {', '.join(def_names)}")
            all_names.extend(def_names)

    # Generate file content
    content = "\n".join(imports)
    if all_names:
        content += "\n\n__all__ = [\n    "
        content += ",\n    ".join(f'"{name}"' for name in all_names)
        content += "\n]"
    else:
        content += "\n\n__all__ = []"

    # Write to output file
    with open(OUTPUT_PATH, 'w') as f:
        f.write(content + "\n")
    print(f"Generated {OUTPUT_PATH} with {len(all_names)} top-level definitions")

if __name__ == "__main__":
    generate_init_py()

