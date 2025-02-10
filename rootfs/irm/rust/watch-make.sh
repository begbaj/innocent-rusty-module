#!/bin/bash

WATCH_DIR="."

dothings() {
  clear
  echo "Detected change in $FILE. Running make.sh..."
  bash make.sh
  if [[ $? -eq 0 ]]; then
    echo "make.sh executed successfully."
  else
    echo "make.sh encountered an error."
  fi
}

while true; do
  inotifywait -m -e modify,create,delete --include '.*\.rs$' -r "$WATCH_DIR" | dothings
done
