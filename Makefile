kernel:
	make -C nxkrnl

iso: kernel
	cp nxkrnl/nxkrnl cdrom/.
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o cdrom.iso cdrom/
	
run: iso
	qemu-system-i386.exe -cdrom cdrom.iso

clean:
	make -C nxkrnl clean

push: clean
	git add .
	git commit -m "autopush"
	git push -u origin master