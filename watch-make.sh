#!/bin/bash
source env
FILE=linux-$LINUX_VERSION/samples/rust/irm/irm.rs

LAST_MODIFIED=$(stat -c %Y "$FILE")

clear
echo "kernel watch compile watch"
while true; do
	CURRENT_MODIFIED=$(stat -c %Y "$FILE")
	if [ "$CURRENT_MODIFIED" -ne "$LAST_MODIFIED" ]; then
		clear 
		echo "kernel watch compile watch"
		cd linux-$LINUX_VERSION/
			make LLVM=../llvm -j$(nproc)
		cd ..
		LAST_MODIFIED=$CURRENT_MODIFIED
	fi

	sleep 0.2
done

