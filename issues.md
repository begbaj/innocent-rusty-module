# Unable to activate vagrant_cloud-3.1.1, because rexml-3.3.2 conflicts with rexml (~> 3.2.5) (Gem::ConflictError):
## Resources
https://bbs.archlinux.org/viewtopic.php?id=299104

## Tried
Setting VAGRANT_DISABLE_STRICT_DEPENDENCY_ENFORCEMENT=1 -> not worked (The environment variable mbork suggested no longer works in this case.)
Changing version requirement of rexml in /opt/vagrant/embedded/gems/specifications/vagrant_cloud-3.1.1.gemspec
## Fix
Changing version requirement of rexml in /opt/vagrant/embedded/gems/specifications/vagrant_cloud-3.1.1.gemspec

```sh

sudo vim /opt/vagrant/embedded/gems/specifications/vagrant_cloud-3.1.1.gemspec

```

then change rexml version requirement to 3.3.2


# VirtualBox is complaining that the kernel module is not loaded. Please run `VBoxManage --version` or open the VirtualBox GUI to see the error message which should contain instructions on how to fix this error.
## fix
sudo modprobe vboxdrv

# vagrant doesn't support current version of virtual box (too high)

## resources 
https://github.com/hashicorp/vagrant/issues/13501

## process
downgrading virtualbox and virtualbox-host-dkms to 7.0.8 (latest version supported by vagrant), but when trying to modprobe vboxdrv, it shows:
`modprobe: FATAL: Module vboxdrv not found in directory /lib/modules/6.10.10-zen1-1-zen`.
Maybe virtualbox-host-dkms should be to latest version? Or maybe using insmod instead of modprobe.
I'm tring to upgrade dkms. Obviously modules now loaded but virtualbox shows:
```txt
NS_ERROR_FACTORY_NOT_REGISTERED (0X80040154)
```
Cause: This error often occurs when VirtualBox components are not properly registered, which can happen due to incomplete installation, version mismatches, or missing kernel modules.
Maybe trying to solve this is not convenient since this is probably a version mismatche issue.

## fix (no fix found)
I'm proceding to change provider into libvirt.

# Libvirt switch
```sh
sudo pacman -Syu libvirt qemu virt-manager ebtables dnsmasq
sudo systemctl enable --now libvirtd
sudo usermod -aG libvirt $(whoami)
vagrant plugin install vagrant-libvirt
```
and remember to start vagrant with:
```sh
vagrant up --provider=libvirt
```
## conflicting packages
while installing vagrant-libvirt another expection occured
```txt
conflicting dependencies bigdecimal (= 3.1.3) and bigdecimal (= 3.1.8)
  Activated bigdecimal-3.1.8
  which does not match conflicting dependency (= 3.1.3)

  Conflicting dependency chains:
    bigdecimal (= 3.1.8), 3.1.8 activated

  versus:
    bigdecimal (= 3.1.3)

  Gems matching bigdecimal (= 3.1.3):
    bigdecimal-3.1.3
```
upgrading gems (`gem update`) did not work, unexpectedly since my version of bigdecimal was actually outdated.
changing req from gemspec only highlighetd that all my gem packages are still outdated since another conflict rised.
trying `sudo gem update --system` didn't solve the problem.
trying `sudo gem update` outlined that packages were still not updated, wondering what gem update did than, but no succes with installing the plugin.
now I think that vagrant is in fact using embedded ruby but that one is outdated.
what is the version vagrant is requiring? the more recent one or the other one? the newer, since when I changed gemspec I required an older version of bigdecimal.
trying `export VAGRANT_DISABLE_STRICT_DEPENDENCY_ENFORCEMENT=1` worked.
I feel dumb, it was a thing I tried for another issue, didn't think to try it for a similar one.
Finally vagrant works!

# Error while activating network: Call to virNetworkCreate failed: internal error: firewalld is set to use the nftables backend, but the required firewalld 'libvirt' zone is missing. Either set the firewalld backend to 'iptables', or ensure that firewalld has a 'libvirt' zone by upgrading firewalld to a version supporting rule priorities (0.7.0+) and/or rebuilding libvirt with --with-firewalld-zone.
## temporary fix
disable firewalld








