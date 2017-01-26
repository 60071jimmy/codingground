/* CountCharPointStr副程式
   Develop by Jimmy
 */
//***標頭檔引入區***
#include <stdio.h>								//	引用<stdio.h>函式庫
//***副程式宣告區***
unsigned long long int CountCharPointStr(const char *);
//	宣告CountCharPointStr(計算字元指標長度)副程式

int main()                                      //  主程式
{                                               //  進入主程式
    printf("%u",CountCharPointStr("123456789a"));
    //	呼叫CountCharPointStr副程式計算字串長度，其結果為10。
    return 0;                                   //  回傳0並結束主程式
}                                               //  結束主程式
unsigned long long int CountCharPointStr(const char *Input_string)
//	CountCharPointStr(計算字元指標長度)副程式，計算字元指標(字串)長度 
{											    //	進入CountCharPointStr副程式
	unsigned long long int count_num = 0;
	//	宣告count_num區域變數，並設定初始值為0 
	while (Input_string[count_num] != '\0')		//	若非字串結尾 
	{										    //	進入while敘述 
		count_num++;							//	累加count_num變數 
	}										    //	結束while敘述 
	return count_num;							//	回傳count_num(字元指標長度)變數
}											    //	結束CountCharPointStr副程式

