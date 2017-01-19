/*	C語言常用副程式測試專案
	Develop by Jimmy
	開發版本資訊：
	2016.8.31(三)－
		建立測試專案
		內含Uint_binary_display(顯示正整數值二進位表示法)副程式、File_write(檔案寫入)副程式
		修正File_write(檔案寫入)副程式為第二版
	2016.9.1(四)－
		建立File_read_display_data(讀取並顯示檔案內容)副程式
	2016.9.5(五)－
		擴充File_read_display_data(讀取並顯示檔案內容)副程式延伸功能(二進位、八進位、十六進位顯示)
	2016.9.18(日)－
		新增Uint_binary_displayln(顯示正整數值二進位表示法並換行)副程式
		新增File_exist_check(檔案存在檢查)副程式
	2016.11.19(六)－
		修正File_write(檔案寫入)副程式為第三版
	2016.11.22(二)－
		新增Uint_octal_display(數值八進位化顯示)副程式
	2016.11.26(六)－
		新增Show_char_point_str(顯示字元指標)副程式
		新增Inverse_char_pint_str(反序字元指標)副程式
	2016.11.27(日)－
		新增StrIntNumberAdd(大數加法運算)副程式
		新增Show_data_structure(顯示資料結構)副程式
	2016.12.4(日)－
		加入影像處理相關副程式
		新增Count_char_point_str副程式之Detail參數，控制是否顯示計數細節
	

	參考資料：
	1、	In C, how should I read a text file and print all strings
		http://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings
	2、	What's the best way to check if a file exists in C? (cross platform)
		http://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
	3、	[C/C++] 使用access函數判斷檔案是否存在
		http://kaivy2001.pixnet.net/blog/post/32781082-%5Bc-c%2B%2B%5D-%E4%BD%BF%E7%94%A8access%E5%87%BD%E6%95%B8%E5%88%A4%E6%96%B7%E6%AA%94%E6%A1%88%E6%98%AF%E5%90%A6%E5%AD%98%E5%9C%A8
	4、	C?言access函?(确定文件的???限)
		http://www.lezhu99.com/482.html
	5、	C 語言標準函數庫分類導覽 - stdio.h putchar()
		http://pydoing.blogspot.tw/2010/07/c-putchar.html
	6、	C語言 如何將常數100以十進位 八進位和十六進位輸出?
		https://tw.answers.yahoo.com/question/index?qid=20120416000010KK04293
	7、	(筆記) struct對function可以call by value嗎?可以return一個struct嗎? (C/C++)
		http://www.cnblogs.com/oomusou/archive/2011/02/18/struct_pass_by_value.html
	8、	說明 typedef enum
		http://bodscar.pixnet.net/blog/post/61204511-%E8%AA%AA%E6%98%8E-typedef-enum
	9、	前置處理器－#define 和#include
		http://web2.fg.tp.edu.tw/~cc/blog/wp-content/uploads/2010/06/20100715advanceC.pdf
	10、C語言動態字串(字串的誤用)－高等C語言(陳鍾誠的網站)
		http://ccckmit.wikidot.com/cp:strmisuse
	11、C語言動態字串(動態字串物件)－高等C語言(陳鍾誠的網站)
		http://ccckmit.wikidot.com/cp:strobject
	12、基本資料型態
		http://yes.nctu.edu.tw/vc/ref/basicdatatype.htm
	
	

*/
//-----引用標頭檔(按照字母排序)-----
#include <math.h>														//	引用<math.h>函式庫
/*	Uint_binary_display副程式中使用math.h之log2函數計算二進位數值位數
*/
#include <stdio.h>														//	引用<stdio.h>函式庫
/*	File_delete副程式中使用檔案刪除函數(remove)
 */
#include <stdlib.h>														//	引用<stdlib.h>函式庫
/*	main程式中使用system函數定義於<stdlib.h>函式庫中
*/
#include <string.h>														//	引用<string.h>函式庫
/*	File_write副程式中使用strcmp比較字串函數，該函數定義於<string.h>函式庫中
*/
#include <unistd.h>														//	引用<unistd.h>函式庫
/*	File_exist副程式中用以檢查檔案是否存在
*/
//-----全域定義區-----
//-----全域結構宣告區-----
typedef enum 															//	宣告Boolean(布林變數)列舉
{																		//	進入Boolean列舉
	False,True															//	列出Boolean列舉中的元素
}Boolean;																//	結束Boolean列舉
/*typedef struct String													//	宣告String(字串)型態
{																		//	進入String(字串)型態
  int len, size;														//	宣告len(紀錄字串長度)、
  char *s;
};																		//	結束String(字串)型態*/
struct file_read														//	宣告(file_read)檔案讀取結構
{																		//	進入(file_read)檔案讀取結構
	long long int file_len;
	int *file_data;
};
//-----副程式宣告區-----
//String *StrNew();														//	宣告StrNew副程式
//void StrAppend(Str *str, char *s);										//	宣告StrAppend副程式
/**	Show_data_structure副程式
	該副程式顯示常用之資料結構型態、空間及可使用數值範圍，列表顯示，部分結果示意如下：
	---型態---	---空間---	------範圍------
	char		1			-128~127
	
**/
void Show_data_structure(void);											//	宣告Show_data_structure(顯示資料結構)副程式
/**	Count_char_point_str副程式
	該副程式用於計算字串指標長度；
	第一項參數為欲計算長度之字串；
	第二項參數為是否顯示計算細節之Boolean變數，若輸入Ture則顯示計算細節，反之則無
	如：Count_char_point_str("ABC", False)，可得長度為3
	如：Count_char_point_str("123456789a", True)，可顯示
	第1個字元為：1
	第2個字元為：2
	第3個字元為：3
	第4個字元為：4
	第5個字元為：5
	第6個字元為：6
	第7個字元為：7
	第8個字元為：8
	第9個字元為：9
	第10個字元為：a	
	並得長度為10
**/
unsigned long long int Count_char_point_str(const char *, const Boolean);
//	宣告Count_char_point_str(計算字元指標長度)副程式
/**	Show_char_point_str副程式
	該副程式用於顯示字串內容，如Show_char_point_str("123456abcdefghijklmn")，則顯示123456abcdefghijklmn
**/
void Show_char_point_str(const char *);									//	宣告Show_char_point_str(顯示字元指標)副程式
char *Inverse_char_pint_str(const char *);								//	宣告Inverse_char_pint_str(反序字元指標)副程式
char *StrIntNumberAdd(char *, char *);									//	宣告StrIntNumberAdd(大數加法運算)副程式

void Uint_binary_display(unsigned int);									//	宣告Uint_binary_display(數值二進位化顯示)副程式
void Uint_binary_displayln(unsigned int);								//	宣告Uint_binary_displayln(數值二進位化顯示並換行)副程式
void Ulong_binary_display(unsigned long);								//	宣告Ulong_binary_display(數值二進位化顯示)副程式
void Uint_octal_display(unsigned int);									//	宣告Uint_octal_display(數值八進位化顯示)副程式
Boolean File_exist_check(char *);										//	宣告File_exist_check(檔案存在檢查)副程式
Boolean File_read_permission_check(char *);								//	宣告File_read_permission_check(檔案讀取權限檢查)副程式
Boolean File_write_permission_check(char *);							//	宣告File_write_permission_check(檔案寫入權限檢查)副程式
struct file_read File_read_data(char *);								//	宣告File_read_data(讀取檔案資料)副程式
void File_read_display_data(char *, char *);							//	宣告File_read_display_data(檔案讀取並顯示檔案內容)副程式

Boolean File_write(char *, char *, char *);								//	宣告File_write(檔案寫入)副程式
void File_delete(const char *);											//	宣告File_delete(檔案刪除)副程式

void Neural_cell(void);													//	宣告Neural_cell(類神經細胞)副程式
//***影像處理相關副程式***
int Filling_byte_calc(int);												//	計算填充位元組大小副程式(考慮BMP檔案格式)
/**	bmp_read_filesize副程式
	該副程式用於將輸入路徑之圖檔大小讀出並傳回；
	第一項參數為圖檔路徑，如欲讀取test.bmp圖檔之大小，即可輸入"test"；
	第二項參數為
**/
unsigned long BMP_read_filesize(const char *);							//	宣告BMP圖檔大小(Byte)讀取副程式bmp_read_filesize


//-----主程式-----
int main(void)															//	main(主程式)
{																		//	進入主程式
	//***Show_data_structure副程式測試***
	//Show_data_structure();												//	呼叫Show_data_structure副程式
	
	//***Count_char_point_str副程式測試***
	//printf("123456789a之長度為：%u\n",Count_char_point_str("123456789a", True));
	//	呼叫Count_char_point_str副程式計算字串長度，其結果為10

	//***Show_char_point_str副程式測試***
	//Show_char_point_str("123456abcdefghijklmn");						//	顯示字串內容
	
	//***Inverse_char_pint_str副程式測試***
	//char test_str[] = "123456abcdefghijklmn";							//	宣告test_str測試字串
	//Show_char_point_str(Inverse_char_pint_str(test_str));
	
	//***StrIntNumberAdd副程式測試***
	//Show_char_point_str(StrIntNumberAdd("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999","99999999999999999999999999999999999999999999999999999999999999999999999999999999999999"));
	
	//***Uint_binary_display副程式測試***
	//Uint_binary_display(10);											//	呼叫Uint_binary_display副程式

	//***Uint_binary_displayln副程式測試***
	//Uint_binary_displayln(10);											//	呼叫Uint_binary_displayln副程式

	//***Ulong_binary_display副程式測試***
	//Ulong_binary_display(10000000);										//	呼叫Ulong_binary_display副程式

	//***Uint_octal_display副程式測試***
	//Uint_octal_display(10);
	//printf("\n%fo",10.5);
	//***File_exist副程式測試***
	//printf("File_write test:%d\n", (int)File_write("test.txt", "12345", "w"));
	//	為測試檔案檢查副程式，呼叫檔案寫入副程式，以覆寫模式將12345寫入至test.txt檔案中。
	//printf("File_exist test:%d\n",(int)File_exist_check("test.txt"));	//	檢查"test.txt"是否存在，此範例將布林型態轉型為整數(Integer)顯示，若檔案存在顯示「1」；若檔案不存在顯示「0」
	//printf("File_exist test:%d\n",(int)File_exist_check("zxc.txt"));

	//***File_read_permission_check副程式測試***
	//printf("File_read_permission_check test:%d\n",(int)File_read_permission_check("test.txt"));
	//	檢查"test.txt"是否可讀取，此範例將布林型態轉型為整數(Integer)顯示，若檔案可讀取顯示「1」；若檔案不可讀取顯示「0」

	//***File_write_permission_check副程式測試***
	//printf("File_write_permission_check test:%d\n",(int)File_write_permission_check("test.txt"));
	//	檢查"test.txt"是否可寫入，此範例將布林型態轉型為整數(Integer)顯示，若檔案可寫入顯示「1」；若檔案不可寫入顯示「0」

	//***File_read_display_data副程式測試***
	//File_read_display_data("test.txt", "r"); 								//	呼叫File_read_display_data(檔案讀取並顯示檔案內容)副程式
	//File_read_display_data("nuuno.bin", "rb_x"); 							//	呼叫File_read_display_data(檔案讀取並顯示檔案內容)副程式

	//***File_delete(檔案刪除)副程式***
	//File_delete("test123.txt");												//	呼叫檔案刪除副程式，刪除"test123.txt"

	//Neural_cell();
	//system("\"C:\\WINDOWS\\system32\\rundll32.exe\" C:\\WINDOWS\\system32\\shimgvw.dll,ImageView_Fullscreen D:\\學習\\電腦資訊\\軟體\\常用副程式\\C語言\\統整測試專案\\Dev C\\Vision5\\test.BMP");
	
	//***影像處理相關副程式測試***
	//***BMP_read_filesize副程式測試***
	printf("BMP_read_filesize測試：%u",BMP_read_filesize("test"));
	
	system("pause");													//	暫停程式
	system("cls");														//	清除螢幕
	system("pause");													//	暫停程式
	return 0;															//	回傳0，程式結束
}																		//	結束主程式
//-----副程式-----
/*String *StrNew()
{
  String *str = (String*)malloc(sizeof(String));
  str->s = (char*)malloc(1 * sizeof(char));
  str->s[0] = '\0';
  str->len = 0;
  str->size = 1;
}

void StrAppend(String *str, char *s) {
  int newLen = str->len + strlen(s);
  if (newLen+1 > str->size) {
    int newSize = fmax(str->size*2, newLen+1);
    char *t = (char*)malloc(newSize);
    sprintf(t, "%s%s", str->s, s);
    free(str->s);
    str->s = t;
    str->len = newLen;
    str->size = newSize;
  } else {
    strcat(&str->s[str->len], s);
    str->len += strlen(s);
  }
}*/
void Show_data_structure(void)											//	Show_data_structure副程式
{																		//	進入Show_data_structure副程式
	printf("---型態---\t\t---空間(bytes)---\t------範圍------\n");		//	顯示"---型態---		---空間(bytes)---	------範圍------"並換行
	printf("%s\t\t\t%d\t\t\t-%f~%f\n","char",sizeof(char),pow(2,(sizeof(char)*8.0)-1.0),pow(2,(sizeof(char)*8.0)-1.0)-1);		
	//	顯示char資料型態
	printf("%s\t\t\t%d\t\t\t-%f~%f\n","int",sizeof(int),pow(2,(sizeof(int)*8.0)-1.0),pow(2,(sizeof(int)*8.0)-1.0)-1);		
	//	顯示int資料型態
	printf("%s\t\t\t%d\t\t\t-%f~%f\n","short",sizeof(short),pow(2,(sizeof(short)*8.0)-1.0),pow(2,(sizeof(short)*8.0)-1.0)-1);		
	//	顯示short資料型態
	printf("%s\t\t\t%d\t\t\t-%f~%f\n","long",sizeof(long),pow(2,(sizeof(long)*8.0)-1.0),pow(2,(sizeof(long)*8.0)-1.0)-1);		
	//	顯示long資料型態
	printf("%s\t\t%d\t\t\t-%f~%f\n","short int",sizeof(short int),pow(2,(sizeof(short int)*8.0)-1.0),pow(2,(sizeof(short int)*8.0)-1.0)-1);		
	//	顯示short int資料型態
	printf("%s\t\t%d\t\t\t-%f~%f\n","long int",sizeof(long int),pow(2,(sizeof(long int)*8.0)-1.0),pow(2,(sizeof(long int)*8.0)-1.0)-1);		
	//	顯示long int資料型態
	printf("%s\t\t%d\t\t\t-%f~%f\n","long long int",sizeof(long long int),pow(2,(sizeof(long long int)*8.0)-1.0),pow(2,(sizeof(long long int)*8.0)-1.0)-1);		
	//	顯示long long int資料型態
	printf("%s\t\t%d\t\t\t%f~%f\n","unsigned char",sizeof(unsigned char),0,pow(2,(sizeof(unsigned char)*8.0))-1);		
	//	顯示unsigned char資料型態
	printf("%s\t\t%d\t\t\t%f~%f\n","unsigned int",sizeof(unsigned int),0,pow(2,(sizeof(unsigned int)*8.0))-1);		
	//	顯示unsigned int資料型態
	printf("%s\t\t%d\t\t\t%f~%f\n","unsigned short",sizeof(unsigned short),0,pow(2,(sizeof(unsigned short)*8.0))-1);		
	//	顯示unsigned short資料型態
	printf("%s\t\t%d\t\t\t%f~%f\n","unsigned long",sizeof(unsigned long),0,pow(2,(sizeof(unsigned long)*8.0))-1);		
	//	顯示unsigned long資料型態
	printf("%s\t%d\t\t\t%f~%f\n","unsigned short int",sizeof(unsigned short int),0,pow(2,(sizeof(unsigned short int)*8.0))-1);		
	//	顯示unsigned short int資料型態
	printf("%s\t%d\t\t\t%f~%f\n","unsigned long int",sizeof(unsigned long int),0,pow(2,(sizeof(unsigned long int)*8.0))-1);		
	//	顯示unsigned long int資料型態
	printf("%s\t%d\t\t\t%f~%f\n","unsigned long long int",sizeof(unsigned long long int),0,pow(2,(sizeof(unsigned long long int)*8.0))-1);		
	//	顯示unsigned long long int資料型態
	printf("%s\t\t\t%d\t\t\t±3.4×10^(-38)~±3.4×10^38\n","float",sizeof(float));		
	//	顯示float資料型態
	printf("%s\t\t\t%d\t\t\t±1.7×10^(-308)~±1.7×10^308\n","double",sizeof(double));		
	//	顯示double資料型態
	printf("%s\t\t\t%d\t\t\t%s\n","Boolean",sizeof(Boolean),"False/True");
	//	顯示Boolean資料型態
}																		//	結束Show_data_structure副程式
unsigned long long int Count_char_point_str(const char *Input_string, const Boolean Detail)	
//	Count_char_point_str(計算字元指標長度)副程式，計算字元指標(字串)長度
{																		//	進入Count_char_point_str副程式
	unsigned long long int count_num = 0;								//	宣告count_num區域變數，並設定初始值為0
	while (Input_string[count_num] != '\0')								//	若非字串結尾
	{																	//	進入while敘述
		if(Detail == True)												//	若Detail參數為True
		{																//	進入if敘述
			printf("第%d個字元為：%c\n",count_num + 1,Input_string[count_num]);
			//	顯示計數細節
		}																//	結束if敘述
		count_num++;													//	累加count_num變數
    }																	//	結束while敘述
    return count_num;													//	回傳count_num(字元指標長度)變數
}																		//	結束Count_char_point_str副程式

void Show_char_point_str(const char *Input_string)						//	Show_char_point_str(顯示字元指標)副程式
/*	顯示字串內容
	引用副程式：
		Count_char_point_str(計算字元指標長度)副程式
*/
{																		//	進入Show_char_point_str副程式
	unsigned long long int loop_num = 0;								//	宣告loop_num區域變數供迴圈使用，並設定初始值為0
	for(loop_num = 0; loop_num < Count_char_point_str(Input_string, False); loop_num++)
	{																	//	進入for迴圈
		printf("%c",Input_string[loop_num]);							//	依序顯示字元
	}																	//	結束for迴圈
}																		//	結束Show_char_point_str副程式

char *Inverse_char_pint_str(const char *Input_string)					//	Inverse_char_pint_str(反序字元指標)副程式
/*	將字串反序，如輸入字串為"123456abc"，輸出字串為"cba654321"
	引用副程式：
		Count_char_point_str(計算字元指標長度)副程式
*/
{																		//	進入Inverse_char_pint_str(反序字元指標)副程式
	//Show_char_point_str(Input_string);									//	顯示輸入字串
	unsigned long long int str_length = 0;								//	宣告str_length區域變數計算輸入字串長度，並設定初始值為0
	str_length = Count_char_point_str(Input_string, False);				//	取得輸入字串長度
	//printf("str_length：%u\n",str_length);								//	顯示str_length變數
	char *Output_str;													//	宣告Output_str字元指標變數，運算輸出字串
	Output_str = (char*)malloc((str_length + 1) * sizeof(char));		//	設定Output_str字元指標變數長度
	Output_str[str_length] = '\0';										//	設定結束字元
	unsigned long long int loop_num = 0;								//	宣告loop_num區域變數供迴圈使用，並設定初始值為0
	//***初始化Output_str指標變數***
	for(loop_num = 0;loop_num < str_length;loop_num++)					//	以for迴圈依序初始化每一位數
	{																	//	進入for迴圈
		Output_str[loop_num] = '0';										//	初始化Output_str指標變數
	}																	//	結束for迴圈
	for(loop_num = 0;loop_num < str_length;loop_num++)					//	以for迴圈依序反序
	{																	//	進入for迴圈
		Output_str[loop_num] = Input_string[str_length - loop_num - 1];	//	處理字串反序
	}																	//	結束for迴圈
	return Output_str;													//	傳回Output_str
}																		//	結束Inverse_char_pint_str(反序字元指標)副程式

char *StrIntNumberAdd(char *Input_str1, char *Input_str2)				//	StrIntNumberAdd(大數加法運算)副程式
//	雖以指標形式處理字串，但因需計算指標長度(數值位數)採用資料型態為unsigned long long int，故數值長度不得超過unsigned long long int範圍
/*	引用副程式：
		Count_char_point_str(計算字元指標長度)副程式、
		Show_char_point_str(顯示字元指標)副程式(測試用)、
*/	
	
{																		//	進入StrIntNumberAdd(大數加法運算)副程式
	unsigned long long int Input_str1_length = 0;						//	宣告Input_str1_length變數以記錄Input_str1數值長度，並初始化為0
	unsigned long long int Input_str2_length = 0; 						//	宣告Input_str2_length變數以記錄Input_str2數值長度，並初始化為0
	Input_str1_length = Count_char_point_str(Input_str1, False);		//	計算輸入數值字串Input_str1長度(不顯示運算細節)
	Input_str2_length = Count_char_point_str(Input_str2, False);		//	計算輸入數值字串Input_str2長度(不顯示運算細節)
	unsigned long long int Output_str_length = 0;						//	宣告Output_str_length變數 以記錄輸出結果位元數，並初始化為0
	Output_str_length = fmax(Input_str1_length,Input_str2_length) + 1;	//	比較Input_str1及Input_str2資料長度，取出較長者加1(考慮進位情況)
	//printf("Output_str_length：%u\n",Output_str_length);				//	顯示Output_str_length變數
	char *Add_num1,*Add_num2;											//	宣告被加數與加數字串
	char *Carry_str;													//	宣告進位字串
	char *Output_str;													//	宣告運算結果字串
	Add_num1 = (char*)malloc((Output_str_length + 1) * sizeof(char));	//	設定被加數字串長度(含結束字元)
	Add_num2 = (char*)malloc((Output_str_length + 1) * sizeof(char));	//	設定加數字串長度(含結束字元)
	Carry_str = (char*)malloc((Output_str_length + 1) * sizeof(char));	//	設定進位字串長度(含結束字元)
	Output_str = (char*)malloc((Output_str_length + 1) * sizeof(char));	//	設定運算結果字串長度(含結束字元)
	int loop_num = 0;													//	宣告loop_num變數供迴圈使用，並初始化為0
	Add_num1[Output_str_length] = '\0';									//	設定Add_num1字串結束字元
	Add_num2[Output_str_length] = '\0';									//	設定Add_num2字串結束字元
	Carry_str[Output_str_length] = '\0';								//	設定Carry_str字串結束字元
	Output_str[Output_str_length] = '\0';								//	設定Output_str字串結束字元
	//***初始化Add_num1指標變數***
	for(loop_num = 0;loop_num<Output_str_length;loop_num++)				//	處理每一位數 
	{																	//	進入for迴圈
		Add_num1[loop_num] = 48;										//	初始化每位數為數字'0'(ASCII編碼)
	}																	//	結束for迴圈
	//***初始化Add_num2指標變數***
	for(loop_num = 0;loop_num<Output_str_length;loop_num++)				//	處理每一位數 
	{																	//	進入for迴圈
		Add_num2[loop_num] = 48;										//	初始化每位數為數字'0'(ASCII編碼)
	}																	//	結束for迴圈
	//***初始化Carry_str指標變數***
	for(loop_num = 0;loop_num<Output_str_length;loop_num++)				//	處理每一位數 
	{																	//	進入for迴圈
		Carry_str[loop_num] = 48;										//	初始化每位數為數字'0'(ASCII編碼)
	}																	//	結束for迴圈
	//***初始化Output_str指標變數***
	for(loop_num = 0;loop_num < Output_str_length;loop_num++)			//	處理每一位數 
	{																	//	進入for迴圈
		Output_str[loop_num] = 48;										//	初始化每位數為數字'0'(ASCII編碼)
	}																	//	結束for迴圈
	//***將資料填入被加數字串***
	for(loop_num = Output_str_length - Input_str1_length;loop_num<Output_str_length;loop_num++)
	{																	//	進入for迴圈
		Add_num1[loop_num] = Input_str1[loop_num + Input_str1_length - Output_str_length];
		//	將資料填入被加數字串
	}																	//	結束for迴圈
	//***將資料填入加數字串***
	for(loop_num = Output_str_length - Input_str2_length;loop_num<Output_str_length;loop_num++)
	{																	//	進入for迴圈
		Add_num2[loop_num] = Input_str2[loop_num + Input_str2_length - Output_str_length];
		//	將資料填入加數字串
	}																	//	結束for迴圈
	//Show_char_point_str(Add_num1);										//	顯示Add_num1(被加數)資料
	//printf("\n");														//	顯示換行
	//Show_char_point_str(Add_num2);										//	顯示Add_num2(加數)資料
	//printf("\n");														//	顯示換行
	//Show_char_point_str(Carry_str);										//	顯示Carry_str(進位數)資料
	//printf("\n");														//	顯示換行
	//***每一位數依序相加***
	for(loop_num = Output_str_length - 1; loop_num > 0;loop_num--)		//	以for迴圈依序計算每一位數
	{																	//	進入for迴圈
		Carry_str[loop_num - 1] = (((Add_num1[loop_num] - 48) + (Add_num2[loop_num] - 48) + (Carry_str[loop_num] - 48)) / 10) + 48;
		//	計算進位
		Output_str[loop_num] = (((Add_num1[loop_num] - 48) + (Add_num2[loop_num] - 48) + (Carry_str[loop_num] - 48)) % 10) + 48;
		//	計算取餘數(加法和)
		//printf("%c+%c+%c = %c進位%c\n",Add_num1[loop_num],Add_num2[loop_num],Carry_str[loop_num],Output_str[loop_num],Carry_str[loop_num - 1]);
		//	顯示單一位數運算過程
	}																	//	結束for迴圈
	Output_str[0] = Carry_str[0];										//	填入最高位數進位
	//Show_char_point_str(Output_str);									//	顯示運算結果
	free(Add_num1);														//	釋放Add_num1空間
	free(Add_num2);														//	釋放Add_num2空間
	free(Carry_str);													//	釋放Carry_str空間
	return Output_str;													//	回傳Output_str字串(運算結果)
}																		//	結束StrIntNumberAdd(大數加法運算)副程式
void Uint_binary_display(unsigned int Input_num)						//	Uint_binary_display副程式，顯示正整數值二進位表示法
//	Input_num為輸入數值，欲表示二進位之數值
//	呼叫範例：Uint_binary_display(10)，結果：將顯示"1010"(十進位10的二進制數值)
//  ※若使用Visual Studio C++，需引用「<malloc.h>」函式庫，否則無法使用malloc與free函數
{																		//	進入Uint_binary_display副程式
	char *display_string;												//	宣告顯示字串display_string
	unsigned int Input_num_binary_bits;									//	宣告Input_num_binary_bits變數，估計Input_num二進位數值位數
	Input_num_binary_bits = (unsigned int)log2(Input_num) + 1;			//	計算Input_num二進位數值位數填入Input_num_binary_bits變數
	//printf("%d\n",Input_num_binary_bits);								//	顯示Input_num_binary_bits變數計算結果
	display_string = (char*)malloc(Input_num_binary_bits * sizeof(char));
	//	配置display_string字元陣列長度
	int calc_num;														//	宣告calc_num，記錄除法運算商數
	int result_num;														//	宣告result_num變數，記錄取餘數運算餘數
	int calc_bits;														//	宣告calc_bits變數，記錄數值填入位數
	unsigned int loop_num;												//	宣告loop_num變數，供迴圈使用
	calc_bits = 0;														//	初始化calc_bits變數
	loop_num = 0;														//	初始化loop_num變數
	for (loop_num = 0; loop_num < Input_num_binary_bits; loop_num++)	//	以for迴圈初始化display_string資料
	{																	//	進入for迴圈
		display_string[loop_num] = 0;									//	初始化display_string資料為0
	}																	//	結束for迴圈
	calc_num = Input_num;												//	設定calc_num初始為Input_num
	while (calc_num >= 2)												//	若calc_num大於2
	{																	//	進入while敘述
		result_num = calc_num % 2;										//	計算取餘數
		display_string[calc_bits] = (result_num + 48);					//	記錄顯示結果
		calc_bits = calc_bits + 1; 										//	遞增calc_bits變數
		calc_num = (calc_num - result_num) / 2;							//	進位制轉換連續除法
	} 																	//	結束while敘述
	if (calc_num < 2)													//	若欲轉換數值小於2
	{																	//	進入if敘述
		display_string[calc_bits] = (calc_num + 48);					//	記錄顯示結果
	} 																	//	結束if敘述
	for (loop_num = Input_num_binary_bits; loop_num > 0; loop_num--)
	//	以for迴圈顯示運算結果
	{																	//	進入for迴圈
		printf("%c", display_string[loop_num - 1]);						//	顯示運算結果
	}																	//	結束for迴圈
	if(Input_num == 0)													//	若Input_num為0
	{																	//	進入if敘述
		printf("0");													//	顯示"0"
	} 																	//	結束if敘述
	free(display_string);												//	釋放display_string空間
} 																		//	結束Uint_binary_display副程式

void Uint_binary_displayln(unsigned int Input_num)						//	Uint_binary_displayln副程式，顯示正整數值二進位表示法並換行
{																		//	進入Uint_binary_displayln副程式
	Uint_binary_display(Input_num); 									//	呼叫Uint_binary_display進行二進位轉換
	printf("\n");														//	顯示換行
} 																		//	結束Uint_binary_displayln副程式

void Ulong_binary_display(unsigned long Input_num)						//	Uint_binary_display副程式，顯示長正整數值二進位表示法
//	Input_num為輸入數值，欲表示二進位之數值
//	呼叫範例：Uint_binary_display(9999999999)，結果：將顯示"1010"(十進位10的二進制數值)
//  ※若使用Visual Studio C++，需引用「<malloc.h>」函式庫，否則無法使用malloc與free函數
{																		//	進入Ulong_binary_display副程式
	char *display_string;												//	宣告顯示字串display_string
	unsigned int Input_num_binary_bits;									//	宣告Input_num_binary_bits變數，估計Input_num二進位數值位數
	Input_num_binary_bits = (unsigned int)log2(Input_num) + 1;			//	計算Input_num二進位數值位數填入Input_num_binary_bits變數
	//printf("%d\n",Input_num_binary_bits);								//	顯示Input_num_binary_bits變數計算結果
	display_string = (char*)malloc(Input_num_binary_bits * sizeof(char));
	//	配置display_string字元陣列長度
	int calc_num;														//	宣告calc_num，記錄除法運算商數
	int result_num;														//	宣告result_num變數，記錄取餘數運算餘數
	int calc_bits;														//	宣告calc_bits變數，記錄數值填入位數
	unsigned long loop_num;												//	宣告loop_num變數，供迴圈使用
	calc_bits = 0;														//	初始化calc_bits變數
	loop_num = 0;														//	初始化loop_num變數
	for (loop_num = 0; loop_num < Input_num_binary_bits; loop_num++)	//	以for迴圈初始化display_string資料
	{																	//	進入for迴圈
		display_string[loop_num] = 0;									//	初始化display_string資料為0
	}																	//	結束for迴圈
	calc_num = Input_num;												//	設定calc_num初始為Input_num
	while (calc_num >= 2)												//	若calc_num大於2
	{																	//	進入while敘述
		result_num = calc_num % 2;										//	計算取餘數
		display_string[calc_bits] = (result_num + 48);					//	記錄顯示結果
		calc_bits = calc_bits + 1; 										//	遞增calc_bits變數
		calc_num = (calc_num - result_num) / 2;							//	進位制轉換連續除法
	} 																	//	結束while敘述
	if (calc_num < 2)													//	若欲轉換數值小於2
	{																	//	進入if敘述
		display_string[calc_bits] = (calc_num + 48);					//	記錄顯示結果
	} 																	//	結束if敘述
	for (loop_num = Input_num_binary_bits; loop_num > 0; loop_num--)
	//	以for迴圈顯示運算結果
	{																	//	進入for迴圈
		printf("%c", display_string[loop_num - 1]);						//	顯示運算結果
	}																	//	結束for迴圈
	if(Input_num == 0)													//	若Input_num為0
	{																	//	進入if敘述
		printf("0");													//	顯示"0"
	} 																	//	結束if敘述
	free(display_string);												//	釋放display_string空間
} 																		//	結束Ulong_binary_display副程式

void Uint_octal_display(unsigned int Input_num)							//	Uint_octal_display副程式，顯示正整數值八進位表示法
//	Input_num為輸入數值，欲表示八進位之數值
//	呼叫範例：Uint_octal_display(10)，結果：將顯示"12"(十進位10的八進制數值)
//			　Uint_octal_display(100)，結果：將顯示"144"(十進位100的八進制數值)
//  ※若使用Visual Studio C++，需引用「<malloc.h>」函式庫，否則無法使用malloc與free函數
{																		//	進入Uint_binary_display副程式
	char *display_string;												//	宣告顯示字串display_string
	unsigned int Input_num_octal_bits;									//	宣告Input_num_octal_bits變數，估計Input_num八進位數值位數
	Input_num_octal_bits = (unsigned int)(log(Input_num)/log(8)) + 1;	//	計算Input_num八進位數值位數填入Input_num_octal_bits變數
	//printf("%d",Input_num_octal_bits);								  //	顯示Input_num_octal_bits變數計算結果
	display_string = (char*)malloc(Input_num_octal_bits * sizeof(char));
	//	配置display_string字元陣列長度
	int calc_num;														//	宣告calc_num，記錄除法運算商數
	int result_num;														//	宣告result_num變數，記錄取餘數運算餘數
	int calc_bits;														//	宣告calc_bits變數，記錄數值填入位數
	unsigned int loop_num;												//	宣告loop_num變數，供迴圈使用
	calc_bits = 0;														//	初始化calc_bits變數
	loop_num = 0;														//	初始化loop_num變數
	for (loop_num = 0; loop_num < Input_num_octal_bits; loop_num++)		//	以for迴圈初始化display_string資料
	{																	//	進入for迴圈
		display_string[loop_num] = 0;									//	初始化display_string資料為0
	}																	//	結束for迴圈
	calc_num = Input_num;												//	設定calc_num初始為Input_num
	while (calc_num >= 8)												//	若calc_num大於8
	{																	//	進入while敘述
		result_num = calc_num % 8;										//	計算取餘數
		display_string[calc_bits] = (result_num + 48);					//	記錄顯示結果
		calc_bits = calc_bits + 1; 										//	遞增calc_bits變數
		calc_num = (calc_num - result_num) / 8;							//	進位制轉換連續除法
	} 																	//	結束while敘述
	if (calc_num < 8)													//	若欲轉換數值小於8
	{																	//	進入if敘述
		display_string[calc_bits] = (calc_num + 48);					//	記錄顯示結果
	} 																	//	結束if敘述
	for (loop_num = Input_num_octal_bits; loop_num > 0; loop_num--)
	//	以for迴圈顯示運算結果
	{																	//	進入for迴圈
		printf("%c", display_string[loop_num - 1]);						//	顯示運算結果
	}																	//	結束for迴圈
	if(Input_num == 0)													//	若Input_num為0
	{																	//	進入if敘述
		printf("0");													//	顯示"0"
	} 																	//	結束if敘述
	free(display_string);												//	釋放display_string空間
} 																		//	結束Uint_binary_display副程式

Boolean File_exist_check(char *file_name)								//	File_exist_check(檔案存在檢查)副程式
{																		//	進入File_exist_check(檔案存在檢查)副程式
	if( access( file_name, F_OK ) != -1 )								//	若檔案存在
	{																	//	進入if敘述
    	return True;													//	傳回True
	} 																	//	結束if敘述
	else 																//	若檔案不存在
	{																	//	進入else敘述
	    return False;													//	回傳false
	}																	//	結束else敘述
} 																		//	結束File_exist_check(檔案存在檢查)副程式

Boolean File_read_permission_check(char *file_name)						//	File_read_permission_check(檔案讀取權限檢查)副程式
{																		//	進入File_read_permission_check(檔案讀取權限檢查)副程式
	if( access( file_name, R_OK ) != -1 )								//	若檔案可讀取
	{																	//	進入if敘述
    	return True;													//	傳回True
	} 																	//	結束if敘述
	else 																//	若檔案不可讀取
	{																	//	進入else敘述
	    return False;													//	回傳false
	}																	//	結束else敘述
} 																		//	結束File_read_permission_check(檔案讀取權限檢查)副程式

Boolean File_write_permission_check(char *file_name)					//	File_write_permission_check(檔案寫入權限檢查)副程式
{																		//	進入File_write_permission_check(檔案寫入權限檢查)副程式
	if( access( file_name, W_OK ) != -1 )								//	若檔案可讀取
	{																	//	進入if敘述
    	return True;													//	傳回True
	} 																	//	結束if敘述
	else 																//	若檔案不可讀取
	{																	//	進入else敘述
	    return False;													//	回傳false
	}																	//	結束else敘述
} 																		//	結束File_write_permission_check(檔案寫入權限檢查)副程式

struct file_read File_read_data(char *file_name)
//	File_read_data副程式，執行檔案讀取並傳回檔案內容(char指標型態)
{																		//	進入File_read_data副程式
	FILE *file_point;													//	宣告一file_point指標，控制檔案讀寫
	struct file_read file_read1;										//	宣告file_read1結構用於檔案讀取
	long long int file_len_count;										//	宣告file_len_count變數，計算讀取檔案之長度
	file_len_count = 0;													//	初始化file_len_count變數為0
	//***第一次開啟檔案以計算檔案內容長度***
	file_point = fopen(file_name, "rb");								//	以"rb"模式打開檔案
	int read_num;														//	宣告read_num區域變數，用於檔案讀取
	while ((read_num = getc(file_point)) != EOF)						//	Get character from stream(從檔案位元串流取得字元)，尚未讀取至檔案結尾
	{																	//	進入while敘述
		file_len_count = file_len_count + 1;							//	遞增file_len_count變數
	}																	//	結束while敘述
	file_read1.file_len = file_len_count;								//	將file_len_count計算結果填入檔案讀取結構之file_len
	file_read1.file_data = (int*)malloc(file_len_count* sizeof(int));
	//	配置回傳file_read1結構中資料指標(file_data)記憶體大小
	fclose(file_point); 												//	關閉檔案
	//***第二次打開檔案以取出檔案內容***
	file_point = fopen(file_name, "rb");								//	以"rb"模式打開檔案
	long long int loop_number = 0;										//	宣告
	while ((read_num = getc(file_point)) != EOF)						//	Get character from stream(從檔案位元串流取得字元)，尚未讀取至檔案結尾
	{																	//	進入while敘述
		file_read1.file_data[loop_number] = read_num;
	}																	//	結束while敘述
	fclose(file_point); 												//	關閉檔案
	return file_read1;
} 																		//	結束File_read_data副程式

void File_read_display_data(char *file_name,char *mode)					//	File_read_display_data副程式，執行檔案讀取並顯示檔案內容
//	file_name為欲讀取檔案檔名
//	mode為欲讀取檔案模式設定，可傳入"r"或"rb"，"r"為讀取(ANSI)文字檔模式，"rb"為讀取二進位檔模式(以十進位數值顯示檔案內容)
{																		//	進入File_read_display_data副程式
	FILE *file_point;													//	宣告一file_point指標，控制檔案讀寫
	printf("Read file:%s",file_name);									//	顯示"Read file:"與讀取檔案路徑
	printf("\n"); 														//	顯示換行
	if(strcmp(mode, "r") != 0 && strcmp(mode, "rb") != 0 && strcmp(mode, "rb_b") != 0 && strcmp(mode, "rb_o") != 0 &&
	   strcmp(mode, "rb_x") != 0)
	//	若mode參數不為"r"且不為"rb"且不為"rb_b"且不為"rb_o"且不為"rb_x"
	{																	//	進入if敘述
		printf("File_read_display_data:mode ERROR!\n");					//	顯示"File_read_display_data:mode ERROR!"並換行
		return;															//	結束副程式
	}																	//	結束if敘述
	//***處理延伸檔案模式開啟***
	if(strcmp(mode,"r") == 0 || strcmp(mode,"rb") == 0)					//	若mode為常見檔案模式("r"或"rb")
	{																	//	進入if敘述
		file_point = fopen(file_name, mode);							//	直接以mode模式打開檔案
	} 																	//	結束if敘述
	else if(strcmp(mode ,"rb_b") == 0 || strcmp(mode ,"rb_o") == 0 || strcmp(mode ,"rb_x") == 0)
	//	若mode為延伸檔案模式("rb_b"、"rb_o")
	{																	//	進入if敘述
		file_point = fopen(file_name, "rb");							//	以rb模式打開檔案
	} 																	//	結束if敘述
	if(file_point == NULL)												//	若file_point為空指標，file_name不存在該檔案
	{																	//	進入if敘述
		printf("Read file Error!\n");									//	顯示"Read file Error!"並換行
		return;															//	結束副程式
	}																	//	結束if敘述
	if(file_point) 														//	若檔案成功開啟
	{																	//	進入if敘述(檔案成功開啟)
		printf("File data:\n");											//	顯示"File data:"並換行
    	if(strcmp(mode, "r") == 0)										//	若mode為"r"(讀取ANSI文字檔)
		{																//	進入if敘述
			int read_num;												//	宣告read_num區域變數，用於檔案讀取
			while ((read_num = getc(file_point)) != EOF)				//	Get character from stream(從檔案位元串流取得字元)，尚未讀取至檔案結尾
	    	{															//	進入while敘述
	    		putchar(read_num);										//	顯示字元
			}															//	結束while敘述
		}																//	結束if敘述
		else if(strcmp(mode, "rb") == 0)								//	若mode為"rb"(讀取二進位檔，並以十進位數值顯示檔案內容)
		{																//	進入else if敘述
			int read_num;												//	宣告read_num區域變數，用於檔案讀取
			while ((read_num = getc(file_point)) != EOF)				//	Get character from stream(從檔案位元串流取得字元)，尚未讀取至檔案結尾
	    	{															//	進入while敘述
	    		printf("%d\t",read_num);								//	以十進位數值顯示檔案資料並對齊資料(tab)
			}															//	結束while敘述
		}																//	結束else if敘述
		else if(strcmp(mode, "rb_b") == 0)								//	若mode為"rb_b"(讀取二進位檔，並以二進位數值顯示檔案內容)
		{																//	進入else if敘述
			int read_num;												//	宣告read_num區域變數，用於檔案讀取
			while ((read_num = getc(file_point)) != EOF)				//	Get character from stream(從檔案位元串流取得字元)，尚未讀取至檔案結尾
	    	{															//	進入while敘述
	    		Uint_binary_display((unsigned int)read_num);			//	以二進位數值顯示檔案資料
	    		printf("\t");											//	對齊資料(tab)
			}															//	結束while敘述
		}																//	結束else if敘述
		else if(strcmp(mode, "rb_o") == 0)								//	若mode為"rb_o"(讀取二進位檔，並以八進位數值顯示檔案內容)
		{																//	進入else if敘述
			int read_num;												//	宣告read_num區域變數，用於檔案讀取
			while ((read_num = getc(file_point)) != EOF)				//	Get character from stream(從檔案位元串流取得字元)，尚未讀取至檔案結尾
	    	{															//	進入while敘述
	    		printf("%o\t",read_num);								//	以八進位數值顯示檔案資料並對齊資料(tab)
			}															//	結束while敘述
		}																//	結束else if敘述
		else if(strcmp(mode, "rb_x") == 0)								//	若mode為"rb_x"(讀取二進位檔，並以十六進位數值顯示檔案內容)
		{																//	進入else if敘述
			int read_num;												//	宣告read_num區域變數，用於檔案讀取
			while ((read_num = getc(file_point)) != EOF)				//	Get character from stream(從檔案位元串流取得字元)，尚未讀取至檔案結尾
	    	{															//	進入while敘述
	    		printf("%x\t",read_num);								//	以八進位數值顯示檔案資料並對齊資料(tab)
			}															//	結束while敘述
		}																//	結束else if敘述
	}																	//	結束if敘述(檔案成功開啟)
	printf("\n"); 														//	顯示換行
    fclose(file_point); 												//	關閉檔案
}																		//	結束File_read_display_data副程式

Boolean File_write(char *file_name, char *input_str, char *mode)		//	File_write(檔案寫入)副程式，執行文字檔案寫入
//	file_name為欲寫入檔案檔名
// 	input_str為欲寫入檔案之指標形式字串資料
//	mode為寫入檔案模式設定，可傳入"w"或"a"，"w"為新增/覆蓋模式，"a"為擴充模式
//	回傳結果：若檔案寫入成功回傳True，若寫入失敗回傳False
{																		//	進入File_write(檔案寫入)副程式
	FILE *file_point;													//	宣告一file_point指標，控制檔案讀寫
	if (strcmp(mode, "w") != 0 && strcmp(mode, "a") != 0)				//	若mode參數不為"w"亦不為"a"
	{																	//	進入if敘述
		printf("File_write:mode ERROR!\n");								//	顯示"File_write:mode ERROR!"並換行
		return False;													//	回傳False並結束副程式返回
	}																	//	結束if敘述
	if (File_write_permission_check(file_name) == False)				//	若檔案無法寫入
	{																	//	進入if敘述
		printf("File_write:permission ERROR!\n");						//	顯示"File_write:permission ERROR!"並換行
		return False;													//	回傳False並結束副程式返回
	}																	//	結束if敘述
	file_point = fopen(file_name, mode);								//	以mode模式打開檔案
	//	fprintf或fputs語法二擇一使用
	fprintf(file_point, input_str);										//	以fprintf語法寫入檔案
	//fputs(input_str, file_point);										//	以fputs語法寫入檔案
	fclose(file_point); 												//	關閉檔案
	return True;														//	回傳True並結束副程式返回
}																		//	結束File_write(檔案寫入)副程式

void File_delete(const char *file_name)									//	File_delete(檔案刪除)副程式
{																		//	進入File_delete(檔案刪除)副程式
	int status_num=0;													//	宣告整數區域變數status，記錄檔案刪除狀態
	status_num = remove(file_name);										//	刪除指定檔案
} 																		//	結束File_delete(檔案刪除)副程式

void Neural_cell(void)
{
	//***單輸入單輸出分類器模擬***
	double Input1;														//	宣告類神經網路(分類器)輸入
	double A;															//	宣告類神經網路(分類器)狀態
	double W1;															//	宣告權重值
	double Output1;														//	輸出數值
	int training_times = 100;											//	設定訓練次數
	int loop_num, loop_num2;
	//***設定初始值***
	W1=0;
	//***設定訓練資料***
	double Data[2][2]={{-1,-1},{1,1}};
	//***訓練階段***
	for(loop_num = 0;loop_num<training_times;loop_num++)
	{
		for(loop_num2 = 0;loop_num2 < 2;loop_num2++)
		{
			//printf("第%d次訓練第%d資料\n",loop_num+1,loop_num2+1);
			Input1 = Data[loop_num2][0];
			//printf("Input1:%f\n",Input1);
			//printf("W1:%f\n",W1);
			A = W1*Input1;
			//printf("A:%f\n",A);
			Output1 = 1/exp(-A);
			//printf("Output1:%f\n",Output1);
			W1 = (Data[loop_num2][1] - Output1) * ((W1+1)/3);	//	修正權重值
			//printf("W1:%f\n",W1);
			//system("pause");
		}
	}
	//***測試成果***
	printf("Training finish!\n");
	for(loop_num=0;loop_num<10;loop_num++)
	{
		scanf("%f",&Input1);
		A = W1*Input1;
		Output1 = 1/exp(-A);
		printf("%u\n",Output1);
	}	
}

/*char *Random_num()
{
	
}*/
//***影像處理相關副程式***
int Filling_byte_calc(int xsize)										//	計算填充位元組大小副程式
{																		//	進入計算填充位元組大小副程式
	int filling_byte;
	filling_byte = ( xsize % 4);
	return filling_byte;
}																		//	結束計算填充位元組大小副程式
//----BMP圖檔大小(Byte)讀取副程式----
unsigned long BMP_read_filesize(const char *filename)					//	BMP圖檔大小(Byte)讀取副程式
{																		//	進入BMP圖檔大小(Byte)讀取副程式
	unsigned long bmp_read_file_size;									//	宣告讀取BMP圖片檔案大小(Byte)變數，型態為unsigned long 
	char fname_bmp[256];												//	宣告檔案名稱fname_bmp陣列變數(最多256個字元) 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	FILE *fp;															//	宣告檔案指標fp區域變數
	sprintf(fname_bmp, "%s.bmp", filename);								//	產生完整檔案路徑並存放至fname_bmp陣列 
	printf("正在讀取下列檔案大小(Byte)：%s\n", fname_bmp);				//	顯示程式執行狀態
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp == NULL)														//	若開啟檔案失敗 
	{																	//	進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	結束if敘述	 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	bmp_read_file_size = header[2] + (header[3] << 8) + (header[4] << 16) + (header[5] << 24);
	//	計算輸入BMP圖片檔案大小(Byte，位元組) 
	fclose(fp);															//	關閉檔案
	return bmp_read_file_size;											//	將計算出檔案大小數值(Byte)傳回 
}																		//	結束BMP圖檔大小(Byte)讀取副程式 