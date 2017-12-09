IFS=","
echo "请输入访问序列：默认输入1,2,3,1,4,5,1,2,1,4"
make
touch input.txt
for i in `seq 10`
do
	read a 
	echo ","
        echo $a>>input.txt 
done; 
./laji<input.txt
#`pwd/input.txt>./laji

