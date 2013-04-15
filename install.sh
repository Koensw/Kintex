# SCRIPT TO INSTALL OR UNINSTALL KINTEX (AND LATER TO CONFIGURE IT ALSO)

if [ `id -u` != 0 ]; then
    echo "This install script should be run as root!";
    exit;
fi

if [ -z $1 ]; then #install
    #install binaries
    echo "Installing binary kintex to /usr/bin/..."
    cp bin/kintex /usr/bin
    # install lib
    # - (nothing at the moment)
    # install headers 
    # - (nothing at the moment)
    echo "Install succesful!"
elif [ $1 = "uninstall" ]; then
    echo "Uninstall binary kintex from /usr/bin/..."
    rm -f /usr/bin/kintex
    # uninstall lib
    # - (nothing at the moment)
    # uninstall headers 
    # - (nothing at the moment)
    echo "Uninstall succesful!"
fi