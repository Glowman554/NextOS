gcc make-initrd.c -o make-initrd
./make-initrd vaspel.txt vaspel.txt
rm make-initrd
mv initrd.img ../cdrom/.
