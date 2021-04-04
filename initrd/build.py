import os

os.system("gcc make-initrd.c -o make-initrd")

files = os.listdir("./files")

build = "./make-initrd "

for i in range(0, len(files)):
	build = build + " ./files/" + files[i] + " " + files[i]
	
os.system(build)
os.system("rm make-initrd")