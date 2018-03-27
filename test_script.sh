#!/bin/bash
echo -en '\n'
echo -en '\n'
echo "The current date and time is"
date
echo -en '\n'
echo "The linux kernel version is"
uname -r
echo -en '\n'
echo "Disk Partition Information"
echo -en '\n'
sudo fdisk -l
echo -en '\n'
echo "Current disk usage"
df -h
echo -en '\n'
echo "Current running processes of root user"
ps -e -o pid,uname=USERNAME,pcpu=CPU_USAGE,pmem,comm | grep root
echo -en '\n'
echo "Current running processes of debian user"
ps -e -o pid,uname=USERNAME,pcpu=CPU_USAGE,pmem,comm | grep debian
echo -en '\n'
echo "List Hardware of the Beaglebone Black"
echo -en '\n'

sudo lshw -short
echo -en '\n'
echo "Report of the Beaglebone Black CPU and Processing Units"
echo -en '\n'

lscpu
echo -en '\n'
echo "RAM Usage"
echo -en '\n'
free -m
echo -en '\n'
echo "Done with current messages"
