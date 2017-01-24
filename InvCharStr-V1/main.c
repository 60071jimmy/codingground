//***標頭檔引入區***
#include <stdio.h>														//	引用<stdio.h>函式庫
#include <stdbool.h>													//	引用<stdbool.h>函式庫
//***全域結構定義區***

//***副程式宣告區***
unsigned long long int CountCharPointStr(const char *, const bool);
//	宣告CountCharPointStr(計算字元指標長度)副程式
char *InvCharStr(const char *);											//	宣告InvCharStr(反序字元指標)副程式
/**	PrintString副程式
	該副程式用於顯示字串內容，如PrintString("123456abcdefghijklmn")，則顯示123456abcdefghijklmn
**/
void PrintString(const char *);											//	宣告PrintString(顯示字元指標)副程式

int main()																//	主程式 
{																		//	進入主程式 
    char test_str[] = "123456abcdefghijklmn";							//	宣告test_str測試字串
	PrintString(InvCharStr(test_str));                                  //	呼叫InvCharStr副程式將字串反序
    return 0;															//	回傳0並結束主程式
}																		//	結束主程式 
unsigned long long int CountCharPointStr(const char *Input_string, const bool Detail)	
//	CountCharPointStr(計算字元指標長度)副程式，計算字元指標(字串)長度
{																		//	進入CountCharPointStr副程式
	unsigned long long int count_num = 0;								//	宣告count_num區域變數，並設定初始值為0
	while (Input_string[count_num] != '\0')								//	若非字串結尾
	{																	//	進入while敘述
		if(Detail == true)												//	若Detail參數為True
		{																//	進入if敘述
			printf("第%d個字元為：%c\n",count_num + 1,Input_string[count_num]);
			//	顯示計數細節
		}																//	結束if敘述
		count_num++;													//	累加count_num變數
    }																	//	結束while敘述
    return count_num;													//	回傳count_num(字元指標長度)變數
}																		//	結束CountCharPointStr副程式
char *InvCharStr(const char *Input_string)								//	InvCharStr(反序字元指標)副程式
{																		//	進入InvCharStr(反序字元指標)副程式
	unsigned long long int str_length = 0;								//	宣告str_length區域變數計算輸入字串長度，並設定初始值為0
	str_length = CountCharPointStr(Input_string, false);				//	取得輸入字串長度
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
}																		//	結束InvCharStr(反序字元指標)副程式
void PrintString(const char *Input_string)
//	PrintString(顯示字元指標)副程式
/*	PrintString副程式顯示字串內容
*/
{																		//	進入Show_char_point_str副程式
	unsigned long long int loop_num = 0;
	//	宣告loop_num區域變數供迴圈使用，並設定初始值為0
	for(loop_num = 0; loop_num < strlen(Input_string); loop_num++)
	{																	//	進入for迴圈
		printf("%c",Input_string[loop_num]);							//	依序顯示字元
	}																	//	結束for迴圈
}																		//	結束Show_char_point_str副程式
