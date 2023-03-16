cd fred-lib.app || exit; . ELINOS.sh
make distclean
cd ..
cd fred-server.app || exit; . ELINOS.sh
make distclean
cd ..
cd zcu102_hwvirt_linux.app || exit; . ELINOS.sh
make distclean
cd ..
cd zcu102_hwvirt_pikeos.int || exit; . ELINOS.sh;
make distclean
cd ..
