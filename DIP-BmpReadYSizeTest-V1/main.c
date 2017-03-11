/*	數位影像處理程式
	Develop by Jimmy
	This program is licensed under GNU General Public License v3.
 */
//-----引入標頭檔-----
#include <stdbool.h>													//	引入標頭檔stdbool.h
#include <stdio.h>														//	引入標頭檔stdio.h
#include <stdlib.h>														//	引入標頭檔stdlib.h
#include <string.h>														//	引入標頭檔string.h
#include <unistd.h>														//	引入標頭檔unistd.h
//-----全域定義區-----
#define MAX_PATH 256													//	定義檔案路徑最長長度為256字元
#define True true														//	定義True為true
#define False false														//	定義False為false
//-----全域結構、資料型態宣告區-----
typedef struct BMP24RGB													//	宣告24位元BMP圖檔像素RGB資料結構
{																		//	進入BMP24RGB資料結構
	unsigned char R;													//	宣告R成分變數
	unsigned char G;													//	宣告G成分變數
	unsigned char B;													//	宣告B成分變數
}BMP24RGB;																//	結束BMP24RGB資料結構
typedef struct BMPIMAGE													//	宣告BMPIMAGE資料結構
{																		//	進入BMPIMAGE資料結構
	char FILENAME[MAX_PATH];											//	宣告輸入讀取檔案檔名變數FILENAME
	/*	IMAGE_DATA影像指標可使用陣列方式存取，在一張影像中：
　　　 __________________
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |________________|
　　　  ↑為第一像素 
　　　 	IMAGE_DATA[0]代表第一像素的藍色(B)，由淡到濃分成8位元，0代表沒有藍色，255代表全藍 
　　　 	IMAGE_DATA[1]代表第一像素的綠色(G)，由淡到濃分成8位元，0代表沒有綠色，255代表全綠
　　　 	IMAGE_DATA[2]代表第一像素的紅色(R)，由淡到濃分成8位元，0代表沒有紅色，255代表全紅
　　　 	IMAGE_DATA[3]代表第二像素的藍色(B)，由淡到濃分成8位元，0代表沒有藍色，255代表全藍 
		IMAGE_DATA[4]代表第二像素的綠色(G)，由淡到濃分成8位元，0代表沒有綠色，255代表全綠
		IMAGE_DATA[5]代表第二像素的紅色(R)，由淡到濃分成8位元，0代表沒有紅色，255代表全紅
		但由於BMP檔案格式可能存在填補位元(當影像寬度不為4的倍數時)，IMAGE_DATA陣列的index對應至圖像像素資料有可能不連續，
		以RAWImageToArray將IMAGE_DATA陣列轉換至BMP24RGB型態二維陣列，與二維圖像完全對應
	*/
	unsigned int XSIZE;													//	宣告X軸像素變數
	unsigned int YSIZE;													//	宣告Y軸像素變數
	unsigned char FILLINGBYTE;											//	宣告填充位元組大小
	unsigned char *IMAGE_DATA;											//	宣告影像資料指標*IMAGE_DATA
}BMPIMAGE;																//	結束BMPIMAGE資料結構
//-----副程式宣告區----- 
/*  BmpReadYSize副程式將輸入路徑之圖檔ysize(高度)讀出並傳回
	副程式輸入為欲讀取高度大小之圖檔路徑(const char *型態，為欲讀取之檔名,FilenameExtension副檔名資訊)
	副程式輸出為圖檔高度(unsigned long型態)
 */
unsigned long BmpReadYSize(const char *, const bool);					//	宣告BMP圖檔ysize(高度)讀取副程式BmpReadYSize
bool FileExistCheck(char *);											//	宣告FileExistCheck(檔案存在檢查)副程式
/*	BmpReadYSizeTest副程式用於測試BmpReadYSize副程式
	本測試副程式之輸入、輸出皆為void，所有參數皆設計於測試副程式中，
	以觀察使用副程式所需宣告與語法
	副程式輸入為void
	副程式輸出為void
 */
void BmpReadYSizeTest(void);											//	宣告BmpReadYSizeTest副程式
//----主程式---- 
int main(int argc, char** argv)											//	主程式 
{																		//	進入主程式 
	BmpReadYSizeTest();													//	呼叫BmpReadYSizeTest副程式
	return 0;															//	傳回0 
}																		//	結束主程式 

bool FileExistCheck(char *file_name)									//	FileExistCheck(檔案存在檢查)副程式
{																		//	進入FileExistCheck(檔案存在檢查)副程式
	if( access( file_name, F_OK ) != -1 )								//	若檔案存在
	{																	//	進入if敘述
    	return True;													//	傳回True
	} 																	//	結束if敘述
	else 																//	若檔案不存在
	{																	//	進入else敘述
	    return False;													//	回傳false
	}																	//	結束else敘述
} 																		//	結束FileExistCheck(檔案存在檢查)副程式
//----BMP圖檔ysize(高度)讀取副程式----
unsigned long BmpReadYSize(const char *filename, const bool FilenameExtension)
//	BMP圖檔ysize(高度)讀取副程式
{																		//	進入BMP圖檔ysize(高度)讀取副程式
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	if(FilenameExtension == false)										//	若輸入參數fname_bmp不具副檔名
	{																	//	進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	printf("正在讀取下列檔案ysize(高度)：%s\n",fname_bmp);				//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	結束if敘述	 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	unsigned long BmpReadYSize;											//	宣告讀取BMP圖片檔案的y(高度)大小(像素)變數，型態為unsigned long 
	BmpReadYSize= header[22] + (header[23] << 8) + ( header[24] << 16) + ( header[25] << 24);
	//	計算輸入BMP圖片檔案y(高度)大小(像素) 
	fclose(fp);															//	關閉檔案
	return BmpReadYSize;												//	將計算出BMP圖片檔案y(高度)大小(像素)數值傳回 
}																		//	結束BMP圖檔ysize(高度)讀取副程式 
void BmpReadYSizeTest(void)												//	BmpReadYSizeTest副程式 
{																		//	進入BmpReadYSizeTest副程式
	BMPIMAGE BMPImage1;													//	宣告BMP影像1(BMPImage1)
	printf("請輸入BMP圖檔(test.bmp)：");								//	顯示訊息"請輸入BMP圖檔(test.bmp)："
	scanf("%s",&BMPImage1.FILENAME);									//	使用者輸入圖檔名稱 
	bool FileCheck;														//	宣告FileCheck布林變數，用以記錄檔案是否存在(若檔案存在為true)
	FileCheck = FileExistCheck(BMPImage1.FILENAME);						//	呼叫FileExistCheck副程式檢查檔案是否存在
	if(FileCheck == false)												//	若檔案不存在
	{																	//	進入if敘述
		printf("圖檔不存在!");											//	顯示"圖檔不存在!"
		return;															//	程式結束 
	}																	//	結束if敘述
	BMPImage1.YSIZE = (unsigned int)BmpReadYSize(BMPImage1.FILENAME,true);
	//	讀取輸入BMP圖檔高度 
	if(BMPImage1.YSIZE == -1)											//	若YSIZE為-1(代表讀取檔案失敗)	
	{																	//	進入if敘述 
		printf("讀取圖檔大小資訊失敗!");								//	顯示"讀取圖檔大小資訊失敗!"
		return;															//	程式結束 
	}																	//	結束if敘述 
	else																//	若YSIZE不為-1(正常讀取檔案)
	{																	//	進入else敘述
		printf("輸入圖檔高度：%d\n",BMPImage1.YSIZE);					//	顯示輸入圖檔高度數值 
		return;															//	程式結束 
	}																	//	結束else敘述 
}																		//	結束BmpReadYSizeTest副程式
