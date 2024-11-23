# New start script using docker image of vagrant to 
# avoid dealing with ruby dependencies
#
# Author: Began Bajrami

VFILE=Vagrantfile
VAGRANT_PLUGINS=("")
DEFAULT_BOX=ubuntu/bionic64

source .irmrc

# Check if a Vagrantfile already exists
if [ -e "$VFILE" ]; then
	echo "Vagrantfile already exists"
else
    read -p "No '$VFILE'. Create it with vagrant init <image> or type 'y' for $DEFAULT_BOX (y/n): " choice
    case "$choice" in 
      y|Y ) 
		vagrant init $DEFAULT_BOX
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
read -p "do you want to upload files to the vm?(y/n)" choice
case "$choice" in 
  y|Y ) 
	sh upload.sh
	;;
  * ) 
	  echo "to upload files, run upload.sh"
	;;
esac
echo "Target os is up and running"

