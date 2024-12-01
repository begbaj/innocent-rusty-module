tmux new-session -d -s "irm-development"
tmux rename-window "irm nvim" 
tmux send-keys "cd rootfs/irm" C-m
tmux send-keys "nvim" C-m

tmux new-window -n "shell"
tmux attach-session -t "irm-development"
