#!/bin/bash
# Format fixer script for the challenge project

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SOURCES=(
    "app/src/main.cpp"
    "plugin/src/plugin.cpp"
    "plugin/include/plugin/plugin.h"
    "plugin_segfault/src/plugin_segfault.cpp"
    "plugin_segfault/include/plugin_segfault/plugin_segfault.h"
    "tests/test_plugin.cpp"
    "tests/segfault_probe.cpp"
    "tests/test_plugin_segfault.cpp"
)

echo "=== Formatting all C++ files ==="
for file in "${SOURCES[@]}"; do
    echo "Formatting $file..."
    clang-format -i "$PROJECT_ROOT/$file"
done

echo ""
echo "✅ All files formatted!"
