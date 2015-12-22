#!/bin/sh
num=0
tmp=`mktemp -d`
arc=$3
pv $2 | ./zlr $1 | while read line; do
  case "$line" in
    *::__:SAVEBUFF:__::*)
      if [ $num -ne 0 ]; then
        tar -C $tmp/ -rf $arc ./buffer$num.log  #add to archive
        rm $tmp/buffer$num.log
        echo "Written out buffer #$num."
      fi
      num=`expr $num + 1`
      ;;
  esac
  echo $num > $tmp/number.tmp        #write number to tempfile
  echo $line >> $tmp/buffer$num.log  #write to file
done
num=`cat $tmp/number.tmp`
tar -C $tmp/ -rf $arc ./buffer$num.log
rm $tmp/buffer$num.log
rm $tmp/number.tmp
rmdir $tmp
echo "Written out buffer #$num."
