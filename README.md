# innocent-rusty-module
A (not so) innocent rusty module for Linux.

# Repository
this repo contains an already configured environment to test the rootkit safely in a vagrant vm ~~(provider libvirt)~~ (provider virtualbox).
~~Vagrant-libvirt is provided via a docker container since on ArchLinux there are *many* compatibility issues with ruby dependencies (which are required for libvirt).~~

# Getting started
First: you need to run `source .irmrc`. 

This adds a bash helper function to always run vagrant with vagrant-libvirt docker container if libvirt is the provider.

To change default provider (default is "virtualbox") edit `.irmc`.

Now, you only need to run `sh start.sh` to get environment up and running.

## Get script
a simple script to get files from the vm 
```
|vagrant-box| -> | host |
```

## Upload script
a simple script to upload all files in the host directory and .env directory to the vm.
```
|vagrant-box| <- | host |
```

## Directory structure

### dir: host/
all files to be uploaded to the vm goes in here.

### dir: .env/
this directory contains environment configurations, such as `.bashrc` or environment variables.

# Contents
## docs
Inside docs you will find some `.md` files I've written for my personal needs. Maybe you'll find answers there.

## host
Inside `host/` folder, you'll find two subfolders: `test-c` and `test-rs`. Inside the first one, there is the xcellerator tutorial code I've tested previously.

This code, especially the one on the `rootkit/` folder is not guaranteed to work. Maybe this will be fixed later on.

The rust code I've written for testing resides in `test-rs`.

A final `irm/` folder will be used for the actual rust rootkit.



