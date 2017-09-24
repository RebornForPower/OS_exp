#include <iostream>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;
int id_int=1;
static _int64 ns(){
	//vc6.0环境里不能支持比较位数大的类型，如long long之类，最大能支持的类
	//型就是_int64了，这个函数ns()返回当前以纳秒(nanoseconds)计的时间
	  static _int64 is_init = 0;
//	#if defined(_WIN32)
		static LARGE_INTEGER win_frequency;
		if (0 == is_init) {
			QueryPerformanceFrequency(&win_frequency);
			is_init = 1;
		}
	    LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
    return (_int64) ((1e9 * now.QuadPart)  / win_frequency.QuadPart);
//	#endif
		//printf("%I64d",ns); //调用时_int64 类型的应当用%I64d占位符来格式化输出
}
template<class T>
void to_string(string & result,const T& t){
	//to_string 用了类模板，将数值类型如int之类转化成string类型
	//to_string函数要用到sstream.h这个头文件（原stringstream.h较老，已弃用）
	//原理是创建一个流，把值传递如流中，获取转换后的字符转并将其写入目的字符串
		ostringstream oss;//创建一个流
		oss<<t;//把值传递如流中
		result=oss.str();//获取转换后的字符转并将其写入result
}
string getRandom1_50_str(){
	//getRandom1_50_str函数功能是得到1到50的随机数的对应字符串
	//注意，在此次实验中，我们调用此函数的相隔时间极短，远不到1s，srand要放到循环外面。
	const int SIZE = 50;
	int Random1_50_int=rand()%SIZE; //产生0~SIZE-1的随机数
    //srand((int)time(0));
	//srand用秒为单位，要放在循环外面，不然因为时间太短而产生的随机数相同
	string Random1_50_str;
	stringstream ss;
	ss.clear();//不clear的话，第二次用同一stringstream对象来转换就会有问题
	ss<<Random1_50_int;
	ss>>Random1_50_str;
	return Random1_50_str;
}
string getid_str(){
	//getid_str得到id字段的字符串，设一个int型全局变量id，初始时为1，以后每调用一次，
	//id自增1，本函数利用to_string将id转为字符串输出。
	string id_str;
	to_string(id_str,id_int);
	id_int++;
	return id_str;
}
string getSystemTime_str(){
	//getSystemTime_str输出当前系统时间的字符串（格式例如22:50:55），这是对系统API函数
	//GetLocalTime的进一步封装，通过GetLocalTime得到wHour,wMinute,wSecond分量，
	//再分别转成字符串，连接得到显示系统时间的整体字符串。
	SYSTEMTIME currentSysTime;
	GetLocalTime(&currentSysTime);
	string SysTimeStr;
	stringstream ss;
	//cout<<currentSysTime.wHour<<":"<<currentSysTime.wMinute<<":"<<currentSysTime.wSecond<<endl;
	ss<<currentSysTime.wHour<<":"<<currentSysTime.wMinute<<":"<<currentSysTime.wSecond;
	ss>>SysTimeStr;
	return SysTimeStr;
}
string getFileFullPath(string myFileName){
	//getFileFullPath返回文件名的全路径（string字符串类型）
	char Current_Dir[100];
	GetCurrentDirectory(100,Current_Dir);
	string dir = Current_Dir;
	string FileFullPath=dir;
	FileFullPath+="\\";
	FileFullPath+=myFileName;
	return 	FileFullPath;
}
void DeleteOldFile_str(string OldFileName_str){
	// DeleteOldFile_str,向此函数传入一个string类型的文件名，本函数把相应文件删除。
	//这个函数是对win api函数DeleteFile的进一步封装，
	//DeleteFile要求参数是char *,这个函数利用string类型的data方法传入。
	//string 转*char :方法mystr.data()
	//DeleteFile()
	DeleteFile(OldFileName_str.data());
}
//char *_i64toa(_int64 value,char *string ,int radix);
string my_int64_to_str(_int64 aa){
	//_int64是vc6.0这个环境所能支持的最大数值类型，
	//要把此类型转化成string类型不能直接转换，要通过c风格的字符串char *类型来过渡一下，
	//_int64转成char,有相关内置函数_i64toa,其函数原型如下：
	//char *_i64toa(_int64 value,char *string ,int radix);
	char buffer[32];
	string _int64_str(_i64toa(aa,buffer,10));
	//cout<<_i64toa(aa,buffer,10);
	//cout<<str;
	return _int64_str;
}
void NewRecordFile(string NewFileName){
	//NewRecordFile函数是顶层模块了，此函数调用以上函数，
	//完成本次实验的所有功能，删除旧文件，写新txt文件，存入十条记录。
	string Record;
	const int RecordNum=10;
	DeleteOldFile_str(NewFileName);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout<<"Delete Old "+NewFileName+"."<<endl;
	cout<<"Write to new "+NewFileName<<"."<<endl;
	HANDLE hf = CreateFile(getFileFullPath(NewFileName).data(), GENERIC_WRITE, 0,
        0, CREATE_ALWAYS, 0, 0);
    DWORD written;
	string headline;
	char headline_char[100];
	sprintf(headline_char,"id\tname\tinputTime\tns\t\t runtimes\r\n");
	headline= headline_char;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout<<headline;
    WriteFile(hf, "id\tname\tinputTime\t\tns\truntimes\r\n", 32, &written, 0);
	string ns_str;
	srand((int)time(0));
	_int64 initns=ns();
	for(int i=0;i<RecordNum;i++){
	_int64 currentns=ns();
	_int64 recordns=currentns-initns;
	
	string temp=my_int64_to_str(recordns);
	int space_num=14-temp.size();
	for(int j=0;j<space_num;j++)
	temp=" "+temp;
	//cout<<"temp"<<temp;
	Record=getid_str()+"\ta1\t"+getSystemTime_str()+"\t"+temp+"\t\t"+getRandom1_50_str()+"\r\n";
		//cout<<"size"<<my_int64_to_str(recordns).size();
	WriteFile(hf,Record.data(),Record.size() , &written, 0);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	cout<<Record;
//	Sleep(1);
	}
    CloseHandle(hf);
}
int main(){
	const string myFileName="19215116_win32.txt";
	NewRecordFile(myFileName);
	return 0;
}