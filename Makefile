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
	mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o cdrom.iso cdrom/

	cp cdrom.iso docs/.
	
	cd nextfs-loader; echo "no-boot" | sh build.sh; cd ..

run: iso
	qemu-system-i386 -cdrom cdrom.iso -hda nextfs-loader/fs.nfs -boot d -serial stdio

test: iso
	node os-test/index.js

clean: iso
	make -C nxkrnl clean
	make -C user clean

pack: clean
	mkdir pack
	zip -r pack/nxkrnl.zip nxkrnl/nxkrnl
	zip -r pack/nxkrnl-src.zip nxkrnl/*
	cp cdrom.iso pack/nextos.iso

update: pack
	git clone https://github.com/Glowman554/Glowman554.github.io
	cp -v pack/* Glowman554.github.io/downloads/.
	cd Glowman554.github.io/; git add *; git commit -m "autoupdate"; git push	
	rm -rf Glowman554.github.io/
	rm -rf pack

push: clean
	git add .
	git commit -m "autopush"
	git push -u origin master
