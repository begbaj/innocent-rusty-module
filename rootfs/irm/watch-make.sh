#!/bin/bash

# Files to watch
FILES=("irm.rs" "kprobes.c" "cr0_monitor.rs" "rootkit.c")

# Initialize last modified times
declare -A LAST_MODIFIED

for FILE in "${FILES[@]}"; do
    if [ -e "$FILE" ]; then
        LAST_MODIFIED["$FILE"]=$(stat -c %Y "$FILE")
    else
        echo "Warning: $FILE does not exist. It will be ignored."
        LAST_MODIFIED["$FILE"]=0
    fi
done

clear
echo "irm compile watch"
while true; do
    CHANGED=false

    for FILE in "${FILES[@]}"; do
        if [ -e "$FILE" ]; then
            CURRENT_MODIFIED=$(stat -c %Y "$FILE")
            if [ "$CURRENT_MODIFIED" -ne "${LAST_MODIFIED[$FILE]}" ]; then
                CHANGED=true
                LAST_MODIFIED["$FILE"]=$CURRENT_MODIFIED
            fi
        else
            echo "Warning: $FILE was deleted or does not exist anymore."
        fi
    done

    if $CHANGED; then
        clear
        echo "compiling"
        bash make.sh
    fi

    sleep 0.2
done
