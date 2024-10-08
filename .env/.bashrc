# ~/.bashrc: executed by bash(1) for non-login shells.

# If not running interactively, don't do anything
case $- in
    *i*) ;;
      *) return;;
esac

# Enable color support for `ls` and add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# Set a colorful prompt
PS1='\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '

# Make `less` more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# Alias definitions
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Some useful functions
mkcd () {
    mkdir -p "$1" && cd "$1"
}

# Enable history appending instead of overwriting
shopt -s histappend

# Set history length
HISTSIZE=1000
HISTFILESIZE=2000

# Color definitions for directories and files in `ls`
export LS_COLORS="di=34:fi=0:ln=36:pi=33:so=35:bd=33;1:cd=33;1:or=31;1:ex=32;1"

# Add custom paths (if needed)
# export PATH=$PATH:/your/custom/path

# Source global definitions
if [ -f /etc/bashrc ]; then
    . /etc/bashrc
fi
