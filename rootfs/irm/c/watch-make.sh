#!/bin/bash

# Set the directory to watch (default to current directory)
WATCH_DIR="."

# Watch for changes in .c files only
inotifywait -m -e close_write,moved_to,create --format '%w%f' -r "$WATCH_DIR" --exclude '.*[^.c]$' |
  while read FILE; do
    if [[ "$FILE" == *.c ]]; then
      echo "Detected change in $FILE. Running make.sh..."
      bash make.sh
      if [[ $? -eq 0 ]]; then
        echo "make.sh executed successfully."
      else
        echo "make.sh encountered an error."
      fi
    fi
  done
