tmux new-session -d -s "irm-development"
tmux rename-window "linux nvim" 
tmux send-keys "cd linux" C-m
tmux send-keys "nvim" C-m


tmux new-window -n "shell"
tmux split-window -v
tmux attach-session -t "irm-development"
