#!/bin/bash
# Clang-tidy check script using compile_commands.json

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"

if [ ! -f "$BUILD_DIR/compile_commands.json" ]; then
    echo "❌ compile_commands.json not found in $BUILD_DIR"
    echo "Please run 'cmake -B build' first"
    exit 1
fi

SOURCE_FILES=(
    "app/src/main.cpp"
    "plugin/src/plugin.cpp"
    "plugin_segfault/src/plugin_segfault.cpp"
    "tests/test_plugin.cpp"
    "tests/segfault_probe.cpp"
    "tests/test_plugin_segfault.cpp"
)

ERROR=0

echo "=== Running clang-tidy ==="
for file in "${SOURCE_FILES[@]}"; do
    echo "Checking $file..."
    if ! clang-tidy -p "$BUILD_DIR" "$PROJECT_ROOT/$file" > /tmp/clang-tidy.log 2>&1; then
        echo "❌ FAIL: $file"
        tail -20 /tmp/clang-tidy.log
        ERROR=1
    else
        # Check if there were any warnings/errors even if exit code was 0
        if grep -q "warning:" /tmp/clang-tidy.log || grep -q "error:" /tmp/clang-tidy.log; then
            # Count issues
            ISSUES=$(grep -c "warning:\|error:" /tmp/clang-tidy.log || true)
            echo "⚠️  $file: $ISSUES issues found"
        else
            echo "✅ $file"
        fi
    fi
done

if [ $ERROR -eq 0 ]; then
    echo ""
    echo "✅ clang-tidy check passed!"
    exit 0
else
    echo ""
    echo "❌ clang-tidy check failed!"
    exit 1
fi
