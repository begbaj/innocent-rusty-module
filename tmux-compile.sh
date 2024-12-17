source env
tmux new-session -d -s "compile-irm"
tmux send-keys "bash start.sh" C-m
tmux split-window -h
tmux send-keys "cd rootfs/irm" C-m
tmux send-keys "bash watch-make.sh" C-m
tmux split-window -v
tmux send-keys "gdb linux-$LINUX_VERSION/vmlinux" C-m
tmux attach-session -t "compile-irm"
