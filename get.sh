#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi

FILENAME="$1"
vagrant ssh -c "sudo cat /home/vagrant/$FILENAME" > "$FILENAME"
