#!/bin/sh
# run each example and compare against tests/NAME.out
fail=0
for asm in examples/*.asm; do
    name=$(basename "$asm" .asm)
    exp="tests/$name.out"
    [ -f "$exp" ] || continue
    if ! ./tinyvm "$asm" | cmp -s - "$exp"; then
        echo "FAIL $name"
        fail=1
    fi
done
[ $fail = 0 ] && echo "all ok"
exit $fail
