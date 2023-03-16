cd fred-lib.app || exit; ./configure -n; . ELINOS.sh
make install
cd ..
cd fred-server.app || exit; ./configure -n; . ELINOS.sh
make install
cd ..
cd zcu102_hwvirt_linux.app || exit; /opt/elinos-7.1/bin/elinos-share-project import; . ELINOS.sh
make boot
cd ..
cd zcu102_hwvirt_pikeos.int || exit; /opt/pikeos-5.1/bin/pikeos-share-project import; . ELINOS.sh;
make all
cd ..
