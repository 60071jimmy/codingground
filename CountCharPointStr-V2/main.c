/* Develop by Jimmy
   This program is licensed under GNU General Public License v3.
 */
//***標頭檔引入區***
#include <stdio.h>								//	引用<stdio.h>函式庫
#include <stdbool.h>							//	引用<stdbool.h>函式庫
//***全域結構定義區***
#define True true								//	定義True為true
#define False false								//	定義False為false
//***副程式宣告區***
unsigned long long int CountCharPointStr(const char *, const bool);
//	宣告CountCharPointStr(計算字元指標長度)副程式

int main()                                      //  主程式
{                                               //  進入主程式
    printf("%u",CountCharPointStr("123456789a", True));
	//	呼叫CountCharPointStr副程式計算字串長度並顯示計算細節，其結果為10。
    return 0;                                   //  回傳0並結束主程式
}                                               //  結束主程式
unsigned long long int CountCharPointStr(const char *Input_string, const bool Detail)
//	CountCharPointStr(計算字元指標長度)副程式，計算字元指標(字串)長度
{												//	進入CountCharPointStr副程式
	unsigned long long int count_num = 0;
	//	宣告count_num區域變數，並設定初始值為0
	while (Input_string[count_num] != '\0')					//	若非字串結尾
	{											//	進入while敘述
		if(Detail == True)								//	若Detail參數為True
		{										//	進入if敘述
			printf("第%d個字元為：%c\n",count_num + 1,Input_string[count_num]);
			//	顯示計數細節
		}										//	結束if敘述
		count_num++;								//	累加count_num變數
    }											//	結束while敘述
    return count_num;									//	回傳count_num(字元指標長度)變數
}												//	結束CountCharPointStr副程式
