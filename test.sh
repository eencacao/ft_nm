#!/bin/bash

# Test script for ft_nm - tests all option combinations
# Options: -a -g -u -r -p (5 options = 32 combinations)

BINARY="./ft_nm"
PASS=0
FAIL=0

echo "Compiling ft_nm..."
make re
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi
echo ""

echo "Testing ft_nm against system nm..."
echo "=================================="

# Test with no options
test_options() {
    local opts="$1"
    local desc="$2"
    
    if [ -z "$opts" ]; then
        nm $BINARY >nm.txt 2>&1
        $BINARY $BINARY >ft.txt 2>&1
    else
        nm $opts $BINARY >nm.txt 2>&1
        $BINARY $opts $BINARY >ft.txt 2>&1
    fi
    
    if diff -q nm.txt ft.txt >/dev/null 2>&1; then
        echo "✓ PASS: $desc"
        ((PASS++))
    else
        echo "✗ FAIL: $desc"
        echo "  Differences:"
        diff nm.txt ft.txt | head -5
        ((FAIL++))
    fi
}

# No options
test_options "" "no options"

# Single options
test_options "-a" "-a (all symbols)"
test_options "-g" "-g (global only)"
test_options "-u" "-u (undefined only)"
test_options "-r" "-r (reverse sort)"
test_options "-p" "-p (no sort)"

# Two options
test_options "-ag" "-ag"
test_options "-au" "-au"
test_options "-ar" "-ar"
test_options "-ap" "-ap"
test_options "-gu" "-gu"
test_options "-gr" "-gr"
test_options "-gp" "-gp"
test_options "-ur" "-ur"
test_options "-up" "-up"
test_options "-rp" "-rp"

# Three options
test_options "-agu" "-agu"
test_options "-agr" "-agr"
test_options "-agp" "-agp"
test_options "-aur" "-aur"
test_options "-aup" "-aup"
test_options "-arp" "-arp"
test_options "-gur" "-gur"
test_options "-gup" "-gup"
test_options "-grp" "-grp"
test_options "-urp" "-urp"

# Four options
test_options "-agur" "-agur"
test_options "-agup" "-agup"
test_options "-agrp" "-agrp"
test_options "-aurp" "-aurp"
test_options "-gurp" "-gurp"

# All five options
test_options "-agurp" "-agurp"

echo ""
echo "Testing error cases..."
echo "=================================="

# Test error cases
test_error() {
    local args="$1"
    local desc="$2"
    
    nm $args >nm.txt 2>&1
    local nm_ret=$?
    $BINARY $args >ft.txt 2>&1
    local ft_ret=$?
    
    # Check if both return non-zero (error)
    if [ $nm_ret -ne 0 ] && [ $ft_ret -ne 0 ]; then
        echo "✓ PASS: $desc (both return error)"
        ((PASS++))
    elif [ $nm_ret -eq 0 ] && [ $ft_ret -eq 0 ]; then
        # Both succeed - check output matches
        if diff -q nm.txt ft.txt >/dev/null 2>&1; then
            echo "✓ PASS: $desc (both succeed with same output)"
            ((PASS++))
        else
            echo "✗ FAIL: $desc (output differs)"
            ((FAIL++))
        fi
    else
        echo "✗ FAIL: $desc (nm=$nm_ret, ft_nm=$ft_ret)"
        ((FAIL++))
    fi
}

# Create test files
echo "not an elf or macho file" > /tmp/invalid_file.txt
touch /tmp/empty_file
mkdir -p /tmp/test_dir

# Error cases
test_error "/tmp/nonexistent_file" "nonexistent file"
test_error "/tmp/test_dir" "directory"
test_error "/tmp/empty_file" "empty file"
test_error "/tmp/invalid_file.txt" "invalid file format"
test_error "-z $BINARY" "invalid option -z"

# Multiple files with one error
test_error "$BINARY /tmp/nonexistent_file" "valid + nonexistent"
test_error "/tmp/nonexistent_file $BINARY" "nonexistent + valid"

# Cleanup test files
rm -f /tmp/invalid_file.txt /tmp/empty_file
rmdir /tmp/test_dir 2>/dev/null

# Cleanup
rm -f nm.txt ft.txt

echo ""
echo "=================================="
echo "Results: $PASS passed, $FAIL failed"

if [ $FAIL -eq 0 ]; then
    echo "All tests passed!"
    exit 0
else
    echo "Some tests failed."
    exit 1
fi
