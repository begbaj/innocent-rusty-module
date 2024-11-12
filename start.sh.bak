# Check if the file exists
VFILE=Vagrantfile
VAGRANT_PLUGINS=("vagrant-libvirt")

if [ -e "$VFILE" ]; then
	echo "Vagrantfile already exists"
else
    read -p "No '$VFILE'. Create it with vagrant init <image> or type 'y' to create generic/ubuntu2004 (y/n): " choice
    case "$choice" in 
      y|Y ) 
		vagrant init generic/ubuntu2004
        ;;
      * ) 
		  exit
        ;;
    esac
fi

for PLUGIN in "${VAGRANT_PLUGINS[@]}"; do
    if vagrant plugin list | grep -q "^$PLUGIN\b"; then
        echo "Vagrant plugin '$PLUGIN' is already installed."
    else
        read -p "Vagrant plugin '$PLUGIN' is not installed. Do you want to install it? (y/n): " choice
        case "$choice" in 
          y|Y ) 
            echo "Installing Vagrant plugin '$PLUGIN'..."
			if VAGRANT_DISABLE_STRICT_DEPENDENCY_ENFORCEMENT=1 vagrant plugin install "$PLUGIN"; then
                echo "Plugin '$PLUGIN' installed successfully."
            else
                echo "Failed to install plugin '$PLUGIN'. Exiting."
                exit 1
            fi
            ;;
          * ) 
            echo "Plugin '$PLUGIN' not installed. Exiting."
            exit 1
            ;;
        esac
    fi
done
vagrant up
sh upload.sh
echo "Target os is up and running"

