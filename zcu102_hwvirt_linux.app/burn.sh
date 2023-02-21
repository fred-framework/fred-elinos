cp ../pool/images/hwvirt-linux-zynq-zcu102-hwvirt-uboot_dtb /media/ubuntu/ROOT
sudo tar -zxvf ./rootfs.tgz -C /media/ubuntu/ROOT1/
sync
umount /media/ubuntu/ROOT
umount /media/ubuntu/ROOT1
