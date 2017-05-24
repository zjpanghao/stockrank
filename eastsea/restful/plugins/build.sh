#!/bin/bash
dir=`ls -l|grep '^d'|awk '{print $9}'`
for line in $dir
do
echo $line
make -C  $line
if [ $? != 0 ]
then
exit 1
fi
done
