#!/bin/bash

# Exit on error
set -e

# Recursively find all Python files
find . -type f -name "*.py" | while read -r file; do
  # Use sed to do the replacement in-place (GNU sed syntax)
  sed -i 's/from[[:space:]]\+proto\.import_all_protos[[:space:]]\+import[[:space:]]\+\*/from proto import */g' "$file"
done

echo "Replacement complete."
