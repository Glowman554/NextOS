all:
	make -C nxkrnl
	make -C user
	make -C initrd

iso: all
	cp nxkrnl/nxkrnl cdrom/.
	cp user/*.bin cdrom/progs/.
	cp user/*.drv cdrom/driver/.
	cp initrd/initrd.img cdrom/.
	cp README.md cdrom/.
	cp LICENSE cdrom/.
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o cdrom.iso cdrom/

	cp cdrom.iso docs/.

img:
	qemu-img create test.img 1m

run: iso img
	qemu-system-i386 -cdrom cdrom.iso -hda test.img -boot d

test: iso
	node os-test/index.js

clean:
	make -C nxkrnl clean
	make -C user clean
	rm test.img

pack: clean
	mkdir pack
	zip -r pack/nxkrnl.zip nxkrnl/nxkrnl
	zip -r pack/nxkrnl-src.zip nxkrnl/*
	cp cdrom.iso pack/nextos.iso

push: clean
	git add .
	git commit -m "autopush"
	git push -u origin master
