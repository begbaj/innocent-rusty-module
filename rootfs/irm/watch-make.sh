#!/bin/bash

FILE="irm.rs"
LAST_MODIFIED=$(stat -c %Y "$FILE")
clear
echo "irm compile watch"
while true; do
	CURRENT_MODIFIED=$(stat -c %Y "$FILE")
	if [ "$CURRENT_MODIFIED" -ne "$LAST_MODIFIED" ]; then
		clear 
		echo "compiling"
		bash make.sh
		LAST_MODIFIED=$CURRENT_MODIFIED
	fi

	sleep 0.2
done

