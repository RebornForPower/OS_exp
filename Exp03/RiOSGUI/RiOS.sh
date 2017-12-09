echo "即将编译RiOS内核...\n"
cd kernel/Page/LRU_counter
make -s
cd ../../../
echo "RiOS正在编译...\n"
echo "RiOS内核正在从虚拟机中启动..."
make
