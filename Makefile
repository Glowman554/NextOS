all:
	make -C nxkrnl
	make -C user

iso: all
	cp nxkrnl/nxkrnl cdrom/.
	cp user/*.bin cdrom/progs/.
	cp user/*.drv cdrom/driver/.
	cp README.md cdrom/.
	cp LICENSE cdrom/.
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o cdrom.iso cdrom/
	
run: iso
	qemu-system-i386 -cdrom cdrom.iso

clean:
	make -C nxkrnl clean
	make -C user clean

push: clean
	git add .
	git commit -m "autopush"
	git push -u origin master
