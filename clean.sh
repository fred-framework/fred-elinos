cd fred-lib.app || exit; . ELINOS.sh
make distclean
cd ..
cd fred-server.app || exit; . ELINOS.sh
make distclean
cd ..
cd zcu102_hwvirt_linux.app || exit; . ELINOS.sh
# TODO: this rule is erasing important binary files in zcu102_hwvirt_linux.app/app.rootfs/. Please fix me !
make distclean
cd ..
cd zcu102_hwvirt_pikeos.int || exit; . ELINOS.sh;
make distclean
cd ..
