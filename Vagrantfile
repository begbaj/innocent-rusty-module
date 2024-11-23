# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "alvistack/ubuntu-24.04"
  config.vm.provider "virtualbox"
  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update
    sudo apt-get install -y build-essential libncurses-dev bison flex libssl-dev libelf-dev curl src bindgen rustfmt rust-clippy
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
    source $HOME/.cargo/env
    rustup install nightly
    rustup default nightly
    rustup component add rust-src
    rustup component add rustfmt
    rustup component add clippy
    rustup component add rustdoc
    rustup component add rust-analyzer
  SHELL
end
