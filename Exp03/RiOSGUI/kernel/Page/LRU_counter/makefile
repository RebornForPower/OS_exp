ok:
	g++ -w -o laji  LRU.cpp
	echo "请输入访问序列：默认1,2,3,1,4,5,1,2,1,4\n"
	./laji > LRU_counter.txt
	cp LRU_counter.txt ../Clock/
	cp LRU_counter.txt ../LRU_stack/
	cd ../Clock/;make
	cd ../LRU_stack/;make
	cd ../LRU_counter



#一共有三个方法，LRU的计数器方法这里输入一次访问序列，
#其他两个也按照这个来，就不用再输入了	
#./input.txt>./laji 
##-w 不显示warning
