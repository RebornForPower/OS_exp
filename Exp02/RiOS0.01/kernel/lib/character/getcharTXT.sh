##
##  RiOS/kernel/getcharTXT.sh
##
##  Copyright (C) 2017  Curie （邱日）
##
## a shell script to generate RiOSASCII.txt & RiAlphaNum.txt
###reference:http://blog.csdn.net/weiwang876253631/article/details/8824858
##利用此网站上的取点阵程序，只能得到单个点阵，不能得到我想要的全部ASCII字符数组
##为此写了此脚本批量生成点阵数组（在前面加上符合C语法的char []..申明）重定向到
##txt,再在main函数中把生成的txt include进来就可以用了。
ZiKu=RiAlphaNum.txt
: > $ZiKu
echo "$ZiKu cleaned up."

MyASCII=RiASCII.txt
: > $MyASCII
echo "$MyASCII cleaned up."

#echo "char RiASCII[$alphabet_charsize]={">>$MyASCII;
#for((i=32;i<=127;i++)) do printf \\$(printf '%03o\t' "$i");  done;printf
#ASCII32是空格
strASCII=" !\"#$%&'() +,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_\`abcdefghijklmnopqrstuvwxyz{|}~"
#里面有很多转义字符，搞得很麻烦，还要恢复它们原来的意思
#	$man ascii
echo -e $strASCII
#32～126(共95个)是字符(32是空格
echo -e "\n\ngenerating allASCII...  \n"
#-e 支持转义，默认不转义
echo -e ".\n\ngenerating allASCII(printable) from ASCII_32(space) to ASCII_126(~)...  \n"
Ri_ASCII_charsize=$[(126-32+1)*32];
echo "char RiASCII[$Ri_ASCII_charsize]={">>$MyASCII;
for i in `seq ${#strASCII}`
do
		echo -e ${strASCII:$i-1:1};
		echo -e $[($i+31)];
		if [ "${strASCII:$i-1:1}" = " " ];then
			echo "0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, ">>$MyASCII;
		else
			echo "/*" >> $MyASCII;
			./zimo a ${strASCII:$i-1:1} |head -n 1| tail -n 1 >>$MyASCII;
			echo "*/" >> $MyASCII;
			./zimo a ${strASCII:$i-1:1} |head -n 2| tail -n 1 >>$MyASCII;
		fi
done ;
echo "};">>$MyASCII;
echo -e "$MyASCII done.\n"

echo -e "\n\ngenerating alphabetASCII...  \n"


#Rialphabet[..]+Rinumber[..]	(32*)
#char Rialphabet[832]  26*32
#char Rinumber[288]	9*32
alphabet_a="a"
alphabet_temp="a"
alphabet_ASCIItotal=26;
alphabet_charsize=$[32*alphabet_ASCIItotal];

echo "char Rialphabet[$alphabet_charsize]={">>$ZiKu;
for alphabet_temp in {a..z}
		do
		echo -n $alphabet_temp
		echo "/*" >> $ZiKu;
		./zimo a $alphabet_temp |head -n 1| tail -n 1 >>$ZiKu;
		echo "*/" >> $ZiKu;
		./zimo a $alphabet_temp |head -n 2| tail -n 1 >>$ZiKu;
		#echo $i;
		done
echo "};">>$ZiKu;

echo -e ".\n\ngenerating numberASCII...  \n"


number_ASCIItotal=9;
number_charsize=$[32*number_ASCIItotal];
#echo "$ZiKu cleaned up."
echo "char Rinumber[$number_charsize]={">>$ZiKu;
for((i=0;i<=ASCIItotal+number_ASCIItotal;i++));
		do
		echo "/*" >> $ZiKu;
		./zimo a $i |head -n 1| tail -n 1 >>$ZiKu;
		echo "*/" >> $ZiKu;
		./zimo a $i |head -n 2| tail -n 1 >>$ZiKu;
		echo $i;
		done
echo "};">>$ZiKu;

#for((i=1;i<=10;i++));
#	do
#	echo $(expr $i \* 3 + 1);
#	done
