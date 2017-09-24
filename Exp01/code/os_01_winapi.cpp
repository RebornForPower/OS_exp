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
	//vc6.0�����ﲻ��֧�ֱȽ�λ��������ͣ���long long֮�࣬�����֧�ֵ���
	//�;���_int64�ˣ��������ns()���ص�ǰ������(nanoseconds)�Ƶ�ʱ��
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
		//printf("%I64d",ns); //����ʱ_int64 ���͵�Ӧ����%I64dռλ������ʽ�����
}
template<class T>
void to_string(string & result,const T& t){
	//to_string ������ģ�壬����ֵ������int֮��ת����string����
	//to_string����Ҫ�õ�sstream.h���ͷ�ļ���ԭstringstream.h���ϣ������ã�
	//ԭ���Ǵ���һ��������ֵ���������У���ȡת������ַ�ת������д��Ŀ���ַ���
		ostringstream oss;//����һ����
		oss<<t;//��ֵ����������
		result=oss.str();//��ȡת������ַ�ת������д��result
}
string getRandom1_50_str(){
	//getRandom1_50_str���������ǵõ�1��50��������Ķ�Ӧ�ַ���
	//ע�⣬�ڴ˴�ʵ���У����ǵ��ô˺��������ʱ�伫�̣�Զ����1s��srandҪ�ŵ�ѭ�����档
	const int SIZE = 50;
	int Random1_50_int=rand()%SIZE; //����0~SIZE-1�������
    //srand((int)time(0));
	//srand����Ϊ��λ��Ҫ����ѭ�����棬��Ȼ��Ϊʱ��̫�̶��������������ͬ
	string Random1_50_str;
	stringstream ss;
	ss.clear();//��clear�Ļ����ڶ�����ͬһstringstream������ת���ͻ�������
	ss<<Random1_50_int;
	ss>>Random1_50_str;
	return Random1_50_str;
}
string getid_str(){
	//getid_str�õ�id�ֶε��ַ�������һ��int��ȫ�ֱ���id����ʼʱΪ1���Ժ�ÿ����һ�Σ�
	//id����1������������to_string��idתΪ�ַ��������
	string id_str;
	to_string(id_str,id_int);
	id_int++;
	return id_str;
}
string getSystemTime_str(){
	//getSystemTime_str�����ǰϵͳʱ����ַ�������ʽ����22:50:55�������Ƕ�ϵͳAPI����
	//GetLocalTime�Ľ�һ����װ��ͨ��GetLocalTime�õ�wHour,wMinute,wSecond������
	//�ٷֱ�ת���ַ��������ӵõ���ʾϵͳʱ��������ַ�����
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
	//getFileFullPath�����ļ�����ȫ·����string�ַ������ͣ�
	char Current_Dir[100];
	GetCurrentDirectory(100,Current_Dir);
	string dir = Current_Dir;
	string FileFullPath=dir;
	FileFullPath+="\\";
	FileFullPath+=myFileName;
	return 	FileFullPath;
}
void DeleteOldFile_str(string OldFileName_str){
	// DeleteOldFile_str,��˺�������һ��string���͵��ļ���������������Ӧ�ļ�ɾ����
	//��������Ƕ�win api����DeleteFile�Ľ�һ����װ��
	//DeleteFileҪ�������char *,�����������string���͵�data�������롣
	//string ת*char :����mystr.data()
	//DeleteFile()
	DeleteFile(OldFileName_str.data());
}
//char *_i64toa(_int64 value,char *string ,int radix);
string my_int64_to_str(_int64 aa){
	//_int64��vc6.0�����������֧�ֵ������ֵ���ͣ�
	//Ҫ�Ѵ�����ת����string���Ͳ���ֱ��ת����Ҫͨ��c�����ַ���char *����������һ�£�
	//_int64ת��char,��������ú���_i64toa,�亯��ԭ�����£�
	//char *_i64toa(_int64 value,char *string ,int radix);
	char buffer[32];
	string _int64_str(_i64toa(aa,buffer,10));
	//cout<<_i64toa(aa,buffer,10);
	//cout<<str;
	return _int64_str;
}
void NewRecordFile(string NewFileName){
	//NewRecordFile�����Ƕ���ģ���ˣ��˺����������Ϻ�����
	//��ɱ���ʵ������й��ܣ�ɾ�����ļ���д��txt�ļ�������ʮ����¼��
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