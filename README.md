# innocent-rusty-module
An innocent rusty module for Linux

# Repository
this repo contains an already configured environment to test the rootkit safely in a vagrant vm (provider libvirt).
Vagrant-libvirt is provided via a docker container since on arch (where i'm testing and writing all this) there are compatibility issues with ruby dependencies.

# Getting started
First: you need to run `source .irmrc`. This adds a bash helper function to always run vagrant with vagrant-libvirt docker container.

If you don't need to use the container, simply comment `source .irmrc` inside every .sh script and install libvirt plugin for vagrant manually or by making it a dependency in `start.sh` script.

Now, you only need to run `sh start.sh` to get environment up and running.

## Get script
a simple script to get files from the vm 

## Upload script
a simple script to upload all files in the host directory and .env directory to the vm.

### dir: host/
all files to be uploaded to the vm goes in here.

### dir: .env/
this directory simply contains environment configurations, such as `.bashrc`.
