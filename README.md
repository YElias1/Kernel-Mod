# Kernel-Mod
Birthday Kernel Mod for CSC 4320

Project details in proj1.pdf

C code to store the birthday information of 6 people.
When mod is loaded simple-solution.c will add 6 people, sort the linked list, and remove the head (Youngest)
When mod is unloaded simple-solution.c will delete and free kernel memory for remaining 5 people.

compile with make file using: make
load mod with: sudo insmod [name].ko
list all mods with: lsmod
remove mod with: sudo rmmod [name]
check kernel log buffer: dmesg
clear kernel log buffer: sudo dmesg -C
