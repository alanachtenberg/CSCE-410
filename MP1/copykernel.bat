REM THIS CODE MOUNTS THE DISK FILE, COPIES THE CODE, AND UNMOUNTS THE DISK FILE AFTERWARDS.
REM REPLACE THE FILE PATH WITH THE ABSOLUTE PATH FOR YOUR IMAGE FILE.
filedisk /mount 0 "C:\Documents and Settings\bettati.MALVASIA-XP1\My Documents\Projects\MP1\dev_kernel_grub.img" g:
copy kernel.bin g:
filedisk /umount g: