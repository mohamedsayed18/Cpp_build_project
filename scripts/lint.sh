#!/bin/bash
# Linter runner script for the challenge project

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

ERROR=0

# Run clang-format check
echo "=== Running clang-format (check mode) ==="
for file in "${SOURCES[@]}"; do
    if ! clang-format --dry-run --Werror "$PROJECT_ROOT/$file" > /dev/null 2>&1; then
        echo "❌ FAIL: $file"
        clang-format --output-replacements-xml "$PROJECT_ROOT/$file" | head -5
        ERROR=1
    else
        echo "✅ $file"
    fi
done

if [ $ERROR -eq 0 ]; then
    echo ""
    echo "✅ clang-format check passed!"
else
    echo ""
    echo "❌ clang-format check failed!"
    exit 1
fi

