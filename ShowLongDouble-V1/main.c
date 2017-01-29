/*	ShowLongDouble副程式
	ShowLongDouble副程式用於顯示long double(長浮點數)數值
	Develop by Jimmy Hu
	Date:2017.1.29
 */
//***標頭檔引入區***
#include <stdio.h>								//	引用<stdio.h>函式庫
#include <math.h>								//	引用<math.h>函式庫
//***全域結構定義區***

//***副程式宣告區***
void ShowLongDouble(const long double InputNumber);
int main()										//	主程式
{												//	進入主程式
    long double TestNumber = 4294967296 << 30;
    ShowLongDouble(TestNumber);					//	呼叫ShowLongDouble副程式
    return 0;									//	回傳0並結束主程式
}												//	結束主程式

void ShowLongDouble(const long double InputNumber)
//	ShowLongDouble副程式
{												//	進入ShowLongDouble副程式
	int TimesNumber = 308;						//	宣告TimesNumber為整數(int)變數，並初始化為308
	long double DisplayNumber;					//	宣告DisplayNumber變數，用於記錄顯示數值
	//***處理正負號***
	if(InputNumber < 0)							//	若InputNumber輸入數值為負數
	{											//	進入if敘述
		DisplayNumber = (long double)0.0 - InputNumber;
		//	填入DisplayNumber數值
		printf("-");							//	顯示負號"-"
	}											//	結束if敘述
	else										//	若InputNumber輸入數值不為負數
	{											//	進入else敘述
		DisplayNumber = InputNumber;			//	填入DisplayNumber數值
	}											//	結束else敘述
	//***去除前導0***
	char DisplayChar='\0';						//	宣告DisplayChar為字元(char)型態，並初始化為空字元
	DisplayChar = floor(InputNumber/(long double)pow(10,TimesNumber)) - pow(10,1) * floor(InputNumber/(long double)pow(10,(TimesNumber + 1)));
	//	取出顯示數值
	while((DisplayChar == 0) && (TimesNumber > 0))
	//	若數值為0且TimesNumber(次方)大於0(不為個位數)
	{											//	進入while敘述
		TimesNumber = TimesNumber - 1;			//	遞減TimesNumber變數
		DisplayChar = floor(InputNumber/(long double)pow(10,TimesNumber)) - pow(10,1) * floor(InputNumber/(long double)pow(10,(TimesNumber + 1)));
		//	取出顯示數值
	}											//	結束while敘述
	//***顯示數值***
	int DisplayStartTimes = TimesNumber;		//	宣告DisplayStartTimes為整數(int)變數，用以記錄起始顯示數字次方
	int DisplayDigit;							//	宣告DisplayDigit為整數(int)變數，用以記錄顯示位數
	if(DisplayStartTimes > 14)					//	若DisplayStartTimes(起始顯示數字次方)大於14(即欲顯示數值InputNumber大於10^14)
	{											//	進入if敘述
		DisplayDigit = DisplayStartTimes;		//	給定DisplayDigit顯示位數為起始顯示數字次方數
	}											//	結束if敘述
	else										//	若欲顯示數值InputNumber不大於10^14
	{											//	進入else敘述
		DisplayDigit = 14;						//	給定DisplayDigit顯示位數為14位
	}											//	結束else敘述
	while(TimesNumber >= DisplayStartTimes - DisplayDigit)
	{											//	進入while敘述
		if(TimesNumber == -1)					//	若TimesNumber(次方)為-1時
		{										//	進入if敘述
			printf(".");						//	顯示小數點
		}										//	結束if敘述
		DisplayChar = floor(InputNumber/(long double)pow(10,TimesNumber)) - pow(10,1) * floor(InputNumber/(long double)pow(10,(TimesNumber + 1)));
		//	取出顯示數值
		if( (DisplayChar >= 0) && (DisplayChar <= 9) )
		//	若取出之數值介於0~9之間
		{										//	進入if敘述
			printf("%d",DisplayChar);			//	顯示數值
		}										//	結束if敘述
		else									//	若取出數值錯誤
		{										//	進入else敘述
			break;								//	結束while迴圈
		}										//	結束else敘述
		TimesNumber = TimesNumber - 1;			//	遞減TimesNumber變數
	}											//	結束while敘述
}												//	結束ShowLongDouble副程式
