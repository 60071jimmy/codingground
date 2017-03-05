//***本程式執行批次處理BMP圖片檔案之Histogram Equalization***
/*  使用方法：
    於執行檔目錄下建立一BMP資料夾，將欲執行之Histogram Equalization之24位元BMP圖檔放入，
    執行該程式後即可取得各圖檔之灰階之Histogram Equalization後之結果。
 */
/*	數位影像處理程式
	Develop by Jimmy
	This program is licensed under GNU General Public License v3.
	已知Bug：
		影像中值濾波需轉換至HSI色彩空間，對I進行中值濾波後再轉換回RGB，避免色彩改變
 */
//-----引入標頭檔-----
#include <dirent.h>														//	引入標頭檔dirent.h
#include <math.h>														//	引入標頭檔math.h
#include <stdbool.h>													//	引入標頭檔stdbool.h
#include <stdio.h>														//	引入標頭檔stdio.h
#include <stdlib.h>														//	引入標頭檔stdlib.h
#include <string.h>														//	引入標頭檔string.h
#include <sys/types.h>													//	引入標頭檔types.h
#include <unistd.h>														//	引入標頭檔unistd.h
//-----全域定義區-----
#define MAX_PATH 256													//	定義檔案路徑最長長度為256字元
#define FILE_ROOT_PATH "D:\\"
//	定義檔案根目錄絕對路徑位置(用於開啟圖檔)
#define True true
#define False false
#define DebugMode														//	定義程式為DebugMode
//-----全域結構、資料型態宣告區-----
typedef struct BMP24RGB													//	宣告24位元BMP圖檔像素RGB資料結構
{																		//	進入BMP24RGB資料結構
	unsigned char R;													//	宣告R成分變數
	unsigned char G;													//	宣告G成分變數
	unsigned char B;													//	宣告B成分變數
}BMP24RGB;																//	結束BMP24RGB資料結構
typedef struct HSV														//	宣告HSV資料結構
{																		//	進入HSV資料結構
	long double H;														//	宣告H成分變數
	long double S;														//	宣告S成分變數
	long double V;														//	宣告V成分變數
}HSV;																	//	結束HSV資料結構
//-----全域變數宣告區----- 
//int ErrorCode = 0;														//	宣告錯誤狀態紀錄變數ErrorCode
//-----副程式宣告區----- 
void ProcessIMG(const char *filename);
/*	SubFileName副程式用於取出檔案之副檔名
	副程式輸入為欲取得副檔名之檔案完整路徑
	副程式輸出為該檔案之副檔名
 */
char *SubFileName(const char *);										//	宣告SubFileName副程式，用於取得檔案副檔名
/*  BmpReadFilesize副程式將輸入路徑之圖檔大小讀出並傳回
	副程式輸入為欲讀取大小之圖檔路徑(const char *型態)
	副程式輸出為圖檔大小(unsigned long型態)
 */
unsigned long BmpReadFilesize(const char *);							//	宣告BMP圖檔大小(Byte)讀取副程式BmpReadFilesize

/*  BmpReadXSize副程式將輸入路徑之圖檔xsize(寬度)讀出並傳回
	副程式輸入為欲讀取寬度大小之圖檔路徑(const char *型態)
	副程式輸出為圖檔寬度(unsigned long型態)
 */
unsigned long BmpReadXSize(const char *);								//	宣告BMP圖檔xsize(寬度)讀取副程式BmpReadXSize

/*  BmpReadYSize副程式將輸入路徑之圖檔ysize(高度)讀出並傳回
	副程式輸入為欲讀取高度大小之圖檔路徑(const char *型態)
	副程式輸出為圖檔高度(unsigned long型態)
 */
unsigned long BmpReadYSize(const char *);								//	宣告BMP圖檔ysize(高度)讀取副程式BmpReadYSize
/*	BmpRead副程式用於讀取BMP圖檔
	副程式輸入為(欲存放原始圖檔資料之unsigned char型態指標,欲讀取圖檔之寬度,欲讀取圖檔之高度,欲讀取之圖檔路徑)
	副程式輸出：若順利讀取圖檔則傳回0，反之傳回-1
 */
int BmpRead(unsigned char*,const int,const int, const char *);			//	宣告BmpRead(BMP圖檔讀取)副程式
/*	BmpWrite副程式用於寫入BMP圖檔
	副程式輸入為(欲寫入圖檔之unsigned char型態指標資料,欲寫入圖檔之寬度,欲寫入圖檔之高度,欲寫入之圖檔路徑)
	副程式輸出：若順利寫入圖檔則傳回0，反之傳回-1
 */
int BmpWrite(const unsigned char*,const int,const int,const char*); 	//	宣告BmpWrite(BMP圖檔寫入)副程式
/*	ViewBMPImage副程式用於呼叫圖片檢視器開啟圖檔
	副程式輸入為欲開啟檢視之圖檔絕對路徑(配合FILE_ROOT_PATH形成絕對路徑)
	副程式輸入為void(無)
 */
void ViewBMPImage(const char *);										//	宣告ViewBMPImage(BMP圖片檢視)副程式(以Windows圖片檢視器開啟)
/*	InitialIMGArray副程式用於生成BMP24RGB指標變數，並將其資料初始化為0
	副程式輸入為(欲生成BMP24RGB指標變數之圖像寬度,欲生成BMP24RGB指標變數之圖像高度)
	副程式輸出為生成之BMP24RGB指標變數
 */
BMP24RGB *InitialIMGArray(const int, const int);						//	宣告InitialIMGArray副程式
/*	RAWImageToArray副程式將來自BMP圖檔之圖像資料轉換至RGB型態二維陣列，
	轉換後的一個BMP24RGB型態二維陣列代表一張影像，其中：
	__________________
	|                |
	|                |
	|                |
	|                |
	|                |
	|                |
	|________________|
	 ↑為第一像素
	 
 */
BMP24RGB *RAWImageToArray(const unsigned char*, const int, const int);	//	宣告RAWImageToArray(BMP圖檔資料至陣列轉換)副程式
unsigned char *ArrayToRAWImage(const BMP24RGB*,const int,const int);	//	宣告陣列至BMP圖檔資料轉換副程式
/*	BMP24RGBToHSV副程式用於將BMP24RGB型態之RGB像素資料轉換至HSV色彩空間
 */
HSV *BMP24RGBToHSV(const BMP24RGB*,const int,const int);				//	宣告BMP24RGB型態至HSV轉換副程式
/*	ImageDataToTxt副程式用於將圖像影像資料寫入txt檔
	副程式輸入為(欲寫入之txt檔案路徑,欲寫入之圖像影像資料,欲寫入之圖像影像寬度,欲寫入之圖像影像高度)
	副程式輸出：若順利寫入圖檔則傳回0，反之傳回-1
 */
bool ImageDataToTxt(const char*,const BMP24RGB*,const int,const int);
//	宣告圖像影像資料寫入txt檔副程式
BMP24RGB *BmpToGraylevel(const BMP24RGB*,const int,const int);			//	宣告BMP圖片資料轉灰階副程式
BMP24RGB *ImageSmoothing33(const BMP24RGB*,const int,const int);		//	宣告ImageSmoothing33(BMP圖檔3*3Mask平滑濾波)副程式
BMP24RGB *MedianFilter33(const BMP24RGB*,const int,const int);			//	宣告MedianFilter33(BMP圖檔3*3中值濾波)副程式
BMP24RGB *ImageOCR(const BMP24RGB*,const int,const int);				//	宣告ImageOCR(影像OCR)副程式
BMP24RGB *RGBHistogramEqualization(const BMP24RGB*,const int,const int);//	宣告RGBHistogramEqualization(RGB灰階影像直方圖等化)副程式



int BmpFillingByteCalc(const int);										//	宣告BmpFillingByteCalc(計算填充位元組大小)副程式
bool FileWritePermissionCheck(const char *);							//	宣告FileWritePermissionCheck(檔案寫入權限檢查)副程式
bool FileWrite(const char *,const char *,const char *);					//	宣告FileWrite(檔案寫入)副程式，執行文字檔案寫入
/*	UCharBubbleSort副程式用於排序unsigned char數值陣列
	副程式輸入為(欲排序之unsigned char數值指標(該指標含結束字元),欲排序數值個數,排序方式)
	在此排序方式輸入0為由小至大排序；輸入1為由大至小排序
	副程式輸出為排序完成之unsigned char數值指標
 */
unsigned char *UCharBubbleSort(const unsigned char *,const unsigned long long int,const bool);
int Compare(const void *,const void *);									//	宣告Compare副程式(供qsort排序使用)
/**	CountCharPointStr副程式
	該副程式用於計算字串指標長度；
	第一項參數為欲計算長度之字串；
	第二項參數為是否顯示計算細節之bool變數，若輸入Ture則顯示計算細節，反之則無
	如：CountCharPointStr("ABC", False)，可得長度為3
	如：CountCharPointStr("123456789a", True)，可顯示
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
unsigned long long int CountCharPointStr(const char *, const bool);
void Show_char_point_str(const char *);									//	宣告Show_char_point_str(顯示字元指標)副程式
/*	ShowUCharPointStr副程式用於顯示無號字元指標內容
	副程式輸入為(欲顯示內容之無號字元指標,無號字元指標長度)
	副程式輸出為void
 */
void ShowUCharPointStr(const unsigned char *,unsigned long long int);	//	宣告ShowUCharPointStr(顯示無號字元指標)副程式
//----主程式---- 
int main(int argc, char** argv)											//	主程式 
{																		//	進入主程式 
	
	//BmpWrite(ArrayToRAWImage(InitialIMGArray(10,10),10,10),10,10,"TestIMG");
	//	測試建立新圖檔
	DIR *dp;
	struct dirent *ep;     
	dp = opendir ("./BMP");

	if (dp != NULL)
	{
		while (ep = readdir (dp))
		{
			//puts (ep->d_name);
			//printf("%s\n",ep->d_name);
			if((strcmp(SubFileName(ep->d_name),"bmp") == 0) || (strcmp(SubFileName(ep->d_name),"BMP") == 0))
			{
				char subbuff[10],IMGFilePath[256];
				memcpy( subbuff, &(ep->d_name), CountCharPointStr((ep->d_name),false)-4 );
				subbuff[CountCharPointStr((ep->d_name),false)-4] = '\0';
				//printf("%s\n",subbuff);
				sprintf(IMGFilePath,"%s%s","./BMP/",subbuff);
				printf("%s\n",IMGFilePath);
				ProcessIMG(IMGFilePath);
			}
		}
		
		(void) closedir (dp);
	}
	else
		perror ("Couldn't open the directory");
		
	
	return 0;															//	傳回0 
}																		//	結束主程式 
void ProcessIMG(const char *filename)									//	ProcessIMG副程式
{																		//	進入ProcessIMG副程式
	unsigned char *image;												//	宣告影像資料指標*image
	/*影像指標可使用陣列方式存取，在一張影像中：
　　　 __________________
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |________________|
　　　  ↑為第一像素 
　　　 	image[0]代表第一像素的藍色(B)，由淡到濃分成8位元，0代表沒有藍色，255代表全藍 
　　　 	image[1]代表第一像素的綠色(G)，由淡到濃分成8位元，0代表沒有綠色，255代表全綠
　　　 	image[2]代表第一像素的紅色(R)，由淡到濃分成8位元，0代表沒有紅色，255代表全紅
　　　 	image[3]代表第二像素的藍色(B)，由淡到濃分成8位元，0代表沒有藍色，255代表全藍 
		image[4]代表第二像素的綠色(G)，由淡到濃分成8位元，0代表沒有綠色，255代表全綠
		image[5]代表第二像素的紅色(R)，由淡到濃分成8位元，0代表沒有紅色，255代表全紅
		但由於BMP檔案格式可能存在填補位元(當影像寬度不為4的倍數時)，image陣列的index對應至圖像像素資料有可能不連續，
		以RAWImageToArray將image陣列轉換至BMP24RGB型態二維陣列，與二維圖像完全對應
	*/
	int FillingByte;													//	宣告填充位元組大小
	unsigned int xsize;													//	宣告X軸像素變數
	unsigned int ysize;													//	宣告Y軸像素變數
	xsize = (unsigned int)BmpReadXSize(filename);						//	讀取輸入BMP圖檔寬度 
	ysize = (unsigned int)BmpReadYSize(filename);						//	讀取輸入BMP圖檔高度 
	if( (xsize == -1) || (ysize == -1) )								//	若xsize或ysize為-1(代表讀取檔案失敗)	
	{																	//	進入if敘述 
		printf("讀取圖檔大小資訊失敗!");								//	顯示"讀取圖檔大小資訊失敗!" 
	}																	//	結束if敘述 
	else																//	若xsize與ysize皆不為-1(正常讀取檔案)
	{																	//	進入else敘述 
		printf("輸入圖檔寬度：%d\n",xsize);								//	顯示輸入圖檔寬度數值 
		printf("輸入圖檔高度：%d\n",ysize);								//	顯示輸入圖檔高度數值 
		printf("輸入影像大小(Byte)：%d\n",(size_t)xsize * ysize * 3);	//	顯示輸入影像大小數值(Byte) 
		FillingByte = BmpFillingByteCalc(xsize);						//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
		image = (unsigned char*)malloc((xsize * 3 + FillingByte) * ysize * sizeof(unsigned char));
		//	計算並建立影像大小空間 
		if (image == NULL) 												//	若建立影像空間失敗 
		{																//	進入if敘述 
			printf("記憶體分配錯誤!");									//	顯示"記憶體分配錯誤!" 
		}																//	結束if敘述 
		else															//	若未發生錯誤 
		{																//	進入else敘述 
			int loop_num = 0;											//	宣告區域變數loop_num供迴圈使用 
			for(loop_num=0;loop_num<((xsize * 3 + FillingByte) * ysize);loop_num++)
			//	以for迴圈初始化每個像素 
			{															//	進入for迴圈 
				image[loop_num]=255;									//	填入預設像素色彩數值 
			}															//	結束for迴圈 
			BmpRead(image, xsize, ysize, filename);						//	讀取圖檔資料
			//***圖像資料轉換至BMP24RGB指標資料結構***
			BMP24RGB *AnalysisData;										//	宣告AnalysisData二維陣列用於圖檔分析
			AnalysisData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));
			//	配置AnalysisData(二維)指標記憶體大小
			if (AnalysisData == NULL) 									//	若建立影像空間失敗 
			{															//	進入if敘述 
				printf("記憶體分配錯誤!");								//	顯示"記憶體分配錯誤!" 
			}															//	結束if敘述 
			AnalysisData = RAWImageToArray(image, xsize, ysize);		//	將BMP圖檔原始資料轉換為BMP24RGB型態二維陣列
			
			//ImageDataToTxt("BMPText",AnalysisData, xsize, ysize);		//	呼叫ImageDataToTxt副程式，將影像資料寫入txt檔
			//BmpWrite(ArrayToRAWImage(AnalysisData,xsize, ysize), xsize, ysize, "BMPOut");
			//	呼叫ArrayToRAWImage副程式將AnalysisData轉換回BMP影像資料格式後以BmpWrite副程式寫入BMP檔
			
			//BmpWrite(ArrayToRAWImage(BmpToGraylevel(AnalysisData,xsize, ysize), xsize, ysize), xsize, ysize, "BMPGraylevelOut");
			//	呼叫BmpToGraylevel將影像轉為灰階後，呼叫ArrayToRAWImage副程式將AnalysisData轉換回BMP影像資料格式後以BmpWrite副程式寫入BMP檔
			
			//BmpWrite(ArrayToRAWImage(ImageSmoothing33(AnalysisData,xsize, ysize), xsize, ysize), xsize, ysize, "BMPImageSmoothing33Out");
			//	呼叫ImageSmoothing33對影像進行平滑濾波後，呼叫ArrayToRAWImage副程式將AnalysisData轉換回BMP影像資料格式後以BmpWrite副程式寫入BMP檔
			
			//BmpWrite(ArrayToRAWImage(MedianFilter33(AnalysisData,xsize, ysize), xsize, ysize), xsize, ysize, "BMPMedianFilter33Out");
			//	呼叫MedianFilter33對影像進行中值濾波後，呼叫ArrayToRAWImage副程式將AnalysisData轉換回BMP影像資料格式後以BmpWrite副程式寫入BMP檔
			
			//BmpWrite(ArrayToRAWImage(ImageOCR(AnalysisData,xsize, ysize), xsize, ysize), xsize, ysize, "BMPImageOCR3");
			//	呼叫ImageOCR副程式後，呼叫ArrayToRAWImage副程式將AnalysisData轉換回BMP影像資料格式後以BmpWrite副程式寫入BMP檔
			
			//BmpWrite(ArrayToRAWImage(RGBHistogramEqualization(AnalysisData,xsize, ysize), xsize, ysize), xsize, ysize, "RGBHisEqual");
			//	呼叫RGBHistogramEqualization對影像進行直方圖等化後，呼叫ArrayToRAWImage副程式將AnalysisData轉換回BMP影像資料格式後以BmpWrite副程式寫入BMP檔
			
			char OutputFileName[50];						//	宣告OutputFileName陣列變數，記錄輸出檔名
			sprintf(OutputFileName,"%s%s",filename,"-RGBGHE");			//	建立輸出檔名字串(OutputFileName)
			BmpWrite(ArrayToRAWImage(RGBHistogramEqualization(BmpToGraylevel(AnalysisData,xsize, ysize),xsize, ysize), xsize, ysize), xsize, ysize, OutputFileName);
			//	呼叫BmpToGraylevel將影像轉為灰階後，再呼叫RGBHistogramEqualization對影像進行直方圖等化後，呼叫ArrayToRAWImage副程式將AnalysisData轉換回BMP影像資料格式後以BmpWrite副程式寫入BMP檔
			
			//***圖像資料由BMP24RGB指標資料結構轉換至HSV色彩空間***
			HSV *AnalysisDataHSV;										//	宣告AnalysisDataHSV指標用於圖檔分析
			AnalysisDataHSV = (HSV*)malloc(xsize * ysize * sizeof(HSV));//	配置AnalysisDataHSV指標記憶體大小
			if (AnalysisDataHSV == NULL) 								//	若建立影像空間失敗 
			{															//	進入if敘述 
				printf("記憶體分配錯誤!");								//	顯示"記憶體分配錯誤!" 
			}															//	結束if敘述 
			//AnalysisDataHSV = BMP24RGBToHSV(AnalysisData, xsize, ysize);//	轉換圖像色彩空間
			
			
			free(image);												//	釋放image記憶體空間
			free(AnalysisData);											//	釋放AnalysisData記憶體空間
			free(AnalysisDataHSV);										//	釋放AnalysisDataHSV記憶體空間
			//ViewBMPImage("RGBGraylevelHisEqual");						//	呼叫ViewBMPImage檢視圖檔
			//system("pause");											//	暫停程式 
		}																//	結束else敘述 
	}																	//	結束else敘述 
}																		//	結束ProcessIMG副程式
char *SubFileName(const char *InputData)								//	SubFileName副程式
{																		//	進入SubFileName副程式
	char OutputData[4];													//	宣告OutputData字元陣列變數，用於記錄取得之副檔名
	OutputData[3] = '\0';												//	於OutputData字串陣列填入結束符號
	OutputData[0] = InputData[CountCharPointStr(InputData,false) - 3];	//	取得副檔名
	OutputData[1] = InputData[CountCharPointStr(InputData,false) - 2];	//	取得副檔名
	OutputData[2] = InputData[CountCharPointStr(InputData,false) - 1];	//	取得副檔名
	return OutputData;													//	回傳取得之副檔名
}																		//	結束SubFileName副程式

//----BMP圖檔大小(Byte)讀取副程式----
unsigned long BmpReadFilesize(const char *filename)						//	BmpReadFilesize副程式
{																		//	進入BMP圖檔大小(Byte)讀取副程式
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	sprintf(fname_bmp, "%s.bmp", filename);								//	產生完整檔案路徑並存放至fname_bmp陣列 
	printf("正在讀取下列檔案大小(Byte)：%s\n",fname_bmp);				//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	結束if敘述	 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	unsigned long BmpRead_file_size;									//	宣告讀取BMP圖片檔案大小(Byte)變數，型態為unsigned long 
	BmpRead_file_size= header[2] + (header[3] << 8) + ( header[4] << 16) + ( header[5] << 24);
	//	計算輸入BMP圖片檔案大小(Byte，位元組) 
	fclose(fp);															//	關閉檔案
	return BmpRead_file_size;											//	將計算出檔案大小數值(Byte)傳回 
}																		//	結束BMP圖檔大小(Byte)讀取副程式 
//----BMP圖檔xsize(寬度)讀取副程式----
unsigned long BmpReadXSize(const char *filename)
{																		//	進入BMP圖檔xsize(寬度)讀取副程式
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	sprintf(fname_bmp, "%s.bmp", filename);								//	產生完整檔案路徑並存放至fname_bmp陣列 
	printf("正在讀取下列檔案xsize(寬度)：%s\n",fname_bmp);				//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	結束if敘述	 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	unsigned long BmpReadXSize;											//	宣告讀取BMP圖片檔案的x(寬度)大小(像素)變數，型態為unsigned long 
	BmpReadXSize= header[18] + (header[19] << 8) + ( header[20] << 16) + ( header[21] << 24);
	//	計算輸入BMP圖片檔案x(寬度)大小(像素) 
	fclose(fp);															//	關閉檔案
	return BmpReadXSize;												//	將計算出BMP圖片檔案x(寬度)大小(像素)數值傳回 
}																		//	結束BMP圖檔xsize(寬度)讀取副程式 

//----BMP圖檔ysize(高度)讀取副程式----
unsigned long BmpReadYSize(const char *filename)
{																		//	進入BMP圖檔ysize(高度)讀取副程式
	unsigned long BmpReadYSize;											//	宣告讀取BMP圖片檔案的y(高度)大小(像素)變數，型態為unsigned long 
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	sprintf(fname_bmp, "%s.bmp", filename);								//	產生完整檔案路徑並存放至fname_bmp陣列 
	printf("正在讀取下列檔案ysize(高度)：%s\n",fname_bmp);				//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	結束if敘述	 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	BmpReadYSize= header[22] + (header[23] << 8) + ( header[24] << 16) + ( header[25] << 24);
	//	計算輸入BMP圖片檔案y(高度)大小(像素) 
	fclose(fp);															//	關閉檔案
	return BmpReadYSize;												//	將計算出BMP圖片檔案y(高度)大小(像素)數值傳回 
}																		//	結束BMP圖檔ysize(高度)讀取副程式 
//----BMP圖檔讀取副程式---- 
int BmpRead(unsigned char *image,const int xsize,const int ysize, const char *filename)
{																		//	進入BMP圖檔讀取副程式
	int FillingByte;													//	宣告填充位元組變數
	FillingByte = BmpFillingByteCalc(xsize);							//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
	unsigned long BmpReadYSize;											//	宣告讀取BMP圖片檔案的y(高度)大小(像素)變數，型態為unsigned long
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	sprintf(fname_bmp, "%s.bmp", filename);								//	產生完整檔案路徑並存放至fname_bmp陣列 
	printf("正在讀取檔案：%s\n",fname_bmp);								//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	結束if敘述	 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	fread(image, sizeof(unsigned char), (size_t)(long)(xsize * 3 + FillingByte)*ysize, fp);
	//	讀取圖檔資料 
	fclose(fp);															//	關閉檔案 
	return 0;															//	傳回0並結束副程式 
}																		//	結束BMP圖檔讀取副程式
//----BMP圖檔寫入副程式---- 

/*	BmpWrite副程式程式執行BMP圖檔寫入，header陣列為BMP圖檔之檔頭 
 *
 *
 */
 
int BmpWrite(const unsigned char *image,const int xsize,const int ysize,const char *filename) 
{																		//	進入BmpWrite(BMP圖檔寫入)副程式 
	int FillingByte;													//	宣告填充位元組變數(FillingByte)
	FillingByte = BmpFillingByteCalc(xsize);							//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
	unsigned char header[54] = 											//	宣告檔頭設定header陣列(共54Bytes)
	{																	//	header陣列初始值設定 
	0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,									//	header陣列初始值設定
	54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0, 		//	header陣列初始值設定
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		//	header陣列初始值設定
	0, 0, 0, 0															//	header陣列初始值設定
	};																	//	header陣列初始值設定
	unsigned long file_size = (long)xsize * (long)ysize * 3 + 54;		//	宣告file_size變數，設定檔案大小 
	unsigned long width, height;										//	宣告width與height為unsigned long型態變數，用於計算圖像檔案寬度、高度
	char fname_bmp[MAX_PATH];											//	宣告變數用以記錄輸出圖片之完整檔名(最多MAX_PATH個字元) 
	header[2] = (unsigned char)(file_size &0x000000ff);					//	計算檔頭資訊(圖像檔案大小)
	header[3] = (file_size >> 8) & 0x000000ff;							//	計算檔頭資訊(圖像檔案大小)
	header[4] = (file_size >> 16) & 0x000000ff;							//	計算檔頭資訊(圖像檔案大小)
	header[5] = (file_size >> 24) & 0x000000ff;							//	計算檔頭資訊(圖像檔案大小)
	
	width = xsize;														//	將圖像寬度資訊填入width變數
	header[18] = width & 0x000000ff;									//	計算檔頭資訊(圖像寬度)
	header[19] = (width >> 8) &0x000000ff;								//	計算檔頭資訊(圖像寬度)
	header[20] = (width >> 16) &0x000000ff;								//	計算檔頭資訊(圖像寬度)
	header[21] = (width >> 24) &0x000000ff;								//	計算檔頭資訊(圖像寬度)
	 
	height = ysize;														//	將圖像高度資訊填入height變數
	header[22] = height &0x000000ff;									//	計算檔頭資訊(圖像高度)
	header[23] = (height >> 8) &0x000000ff;								//	計算檔頭資訊(圖像高度)
	header[24] = (height >> 16) &0x000000ff;							//	計算檔頭資訊(圖像高度)
	header[25] = (height >> 24) &0x000000ff;							//	計算檔頭資訊(圖像高度)
	sprintf(fname_bmp, "%s.bmp", filename);								//	產生圖檔完整檔名路徑
	FILE *fp;															//	宣告檔案指標fp區域變數，用以記錄寫入圖檔路徑 
	if (!(fp = fopen(fname_bmp, "wb"))) 								//	嘗試以二進位讀檔方式開啟圖檔，若無法成功開啟
	{																	//	進入if敘述
		return -1;														//	回傳-1，並結束副程式
	}																	//	結束if敘述
	
	fwrite(header, sizeof(unsigned char), 54, fp);						//	寫入BMP圖檔檔頭資訊(54Bytes)
	fwrite(image, sizeof(unsigned char), (size_t)(long)(xsize * 3 + FillingByte)*ysize, fp);
	//	寫入BMP圖檔影像資料
	fclose(fp);															//	關閉檔案指標
	return 0;															//	傳回0並結束副程式
}																		//	結束BmpWrite(BMP圖檔寫入)副程式

void ViewBMPImage(const char *filename)
{																		//	進入ViewBMPImage副程式
	if(filename==NULL)													//	若傳入之filename指標為NULL
	{																	//	進入if敘述
		return;															//	結束副程式
	}																	//	結束if敘述
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	sprintf(fname_bmp, "%s%s.BMP",FILE_ROOT_PATH ,filename);			//	建立欲開啟圖檔之完整檔名(最多MAX_PATH個字元)
	char RunCommand[MAX_PATH+100];										//	宣告執行開啟圖檔指令字串變數
	sprintf(RunCommand, "%s%s","\"C:\\WINDOWS\\system32\\rundll32.exe\" C:\\WINDOWS\\system32\\shimgvw.dll,ImageView_Fullscreen ",fname_bmp);
	#ifdef DebugMode													//	若有定義為DebugMode(除錯模式)
		printf("開啟圖檔指令：%s\n",RunCommand);						//	顯示開啟圖檔指令並換行
	#endif																//	結束ifdef敘述
	system(RunCommand);													//	執行system函數，開啟圖檔
}																		//	結束ViewBMPImage副程式

BMP24RGB *InitialIMGArray(const int xsize, const int ysize)				//	InitialIMGArray副程式
{																		//	進入InitialIMGArray副程式
	BMP24RGB *OutputData;												//	宣告OutputData為BMP24RGB指標型態變數
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData(二維)指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序處理圖像像素
		{																//	進入for迴圈
			OutputData[LoopNumber1*xsize + LoopNumber2].R = 0;			//	填入R像素資料
			OutputData[LoopNumber1*xsize + LoopNumber2].G = 0;			//	填入G像素資料
			OutputData[LoopNumber1*xsize + LoopNumber2].B = 0;			//	填入B像素資料
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳生成結果
}																		//	結束InitialIMGArray副程式
BMP24RGB *RAWImageToArray(const unsigned char *image, const int xsize, const int ysize)
//	BMP圖檔資料至陣列轉換副程式
{																		//	進入BMP圖檔資料至陣列轉換副程式
	BMP24RGB *OutputData;												//	宣告OutputData為BMP24RGB指標型態變數
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData(二維)指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int FillingByte;													//	宣告FillingByte區域變數
	FillingByte = BmpFillingByteCalc(xsize);							//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序處理圖像像素
		{																//	進入for迴圈
			OutputData[LoopNumber1*xsize + LoopNumber2].R = 			//	填入R像素資料
			image[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 2];
			OutputData[LoopNumber1*xsize + LoopNumber2].G = 			//	填入G像素資料
			image[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 1];
			OutputData[LoopNumber1*xsize + LoopNumber2].B = 			//	填入B像素資料
			image[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 0];
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳轉換結果
}																		//	結束BMP圖檔資料至陣列轉換副程式

unsigned char *ArrayToRAWImage(const BMP24RGB* InputData,const int xsize,const int ysize)
//	ArrayToRAWImage副程式
{																		//	進入ArrayToRAWImage副程式
	int FillingByte;													//	宣告FillingByte區域變數
	FillingByte = BmpFillingByteCalc(xsize);							//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
	unsigned char *OutputData;											//	宣告OutputData記錄轉換完成資料
	OutputData = (unsigned char*)malloc((xsize * 3 + FillingByte) * ysize * sizeof(unsigned char));
	//	配置OutputData指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	//***初始化OutputData指標資料***
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序初始化OutputData指標資料為0
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<(xsize * 3 + FillingByte);LoopNumber2++)
		//	以for迴圈依序初始化OutputData指標資料為0
		{																//	進入for迴圈
			OutputData[LoopNumber1 * (xsize * 3 + FillingByte) + LoopNumber2] = 0;
		}																//	結束for迴圈
	}																	//	結束for迴圈
	//***填入圖像資料至OutputData***
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序處理圖像像素
		{																//	進入for迴圈
			OutputData[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 2]
			= InputData[LoopNumber1*xsize + LoopNumber2].R;				//	填入R像素資料
			OutputData[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 1]
			= InputData[LoopNumber1*xsize + LoopNumber2].G;				//	填入G像素資料
			OutputData[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 0]
			= InputData[LoopNumber1*xsize + LoopNumber2].B;				//	填入B像素資料
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳轉換結果
}																		//	結束ArrayToRAWImage副程式

HSV *BMP24RGBToHSV(const BMP24RGB *InputData,const int xsize,const int ysize)
//	BMP24RGBToHSV副程式
{																		//	進入BMP24RGBToHSV副程式
	HSV *OutputData;													//	宣告OutputData為HSV指標型態變數
	OutputData = (HSV*)malloc(xsize * ysize * sizeof(HSV));				//	配置OutputData指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	//***OutputData指標初始化***
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序轉換圖像像素資料
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序轉換圖像像素資料
		{																//	進入for迴圈
			OutputData[LoopNumber1 * xsize + LoopNumber2].H = 0;		//	初始化OutputData指標變數
			OutputData[LoopNumber1 * xsize + LoopNumber2].S = 0;		//	初始化OutputData指標變數
			OutputData[LoopNumber1 * xsize + LoopNumber2].V = 0;		//	初始化OutputData指標變數
		}																//	結束for迴圈
	}																	//	結束for迴圈
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序轉換圖像像素資料
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序轉換圖像像素資料
		{																//	進入for迴圈
			unsigned char SortArray[3];									//	宣告SortArray為unsigned char型態陣列，供像素值RGB排序使用
			SortArray[0] = InputData[LoopNumber1 * xsize + LoopNumber2].R;
			//	將像素R資料填入SortArray
			SortArray[1] = InputData[LoopNumber1 * xsize + LoopNumber2].G;
			//	將像素G資料填入SortArray
			SortArray[2] = InputData[LoopNumber1 * xsize + LoopNumber2].B;
			//	將像素B資料填入SortArray
			printf("%d,%d,%d\n",SortArray[0],SortArray[1],SortArray[2]);
			qsort(SortArray, 3, sizeof(unsigned char), Compare);		//	呼叫qsort排序函數(定義於stdlib.h)
			printf("%d,%d,%d\n",SortArray[0],SortArray[1],SortArray[2]);
			system("pause");
			//OutputData[LoopNumber1 * xsize + LoopNumber2]
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳OutputData，並結束副程式
}																		//	結束BMP24RGBToHSV副程式
bool ImageDataToTxt(const char *file_name,const BMP24RGB *image,const int xsize,const int ysize)
//	ImageDataToTxt副程式，用於將圖像影像資料寫入txt檔
{																		//	進入ImageDataToTxt副程式
	char fname_txt[MAX_PATH];											//	宣告圖片資料記錄檔之完整檔名(最多MAX_PATH個字元)
	sprintf(fname_txt, "%s.txt", file_name);							//	建立圖片資料記錄檔之完整檔名(最多MAX_PATH個字元)
	if(FileWrite(fname_txt,"","w") == False)							//	嘗試建立檔案，若檔案建立失敗
	{																	//	進入if敘述
		return False;													//	回傳False並結束副程式返回
	}																	//	結束if敘述
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	char WriteStringTemp[50];											//	宣告WriteStringTemp字串，用於暫存寫入資料
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序寫入圖像資料
	{																	//	進入for迴圈
		sprintf(WriteStringTemp,"第%d個像素R=%d\t",loop_num,image[loop_num].R);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
		sprintf(WriteStringTemp,"第%d個像素G=%d\t",loop_num,image[loop_num].G);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
		sprintf(WriteStringTemp,"第%d個像素B=%d\n",loop_num,image[loop_num].B);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
	}																	//	結束for迴圈
}																		//	結束ImageDataToTxt副程式

BMP24RGB *BmpToGraylevel(const BMP24RGB *image,const int xsize,const int ysize)
//	BMP圖片資料轉灰階副程式
{																		//	進入BMP圖片資料轉灰階副程式
	BMP24RGB *GraylevelImage;											//	宣告GraylevelImage指標變數，記錄灰階轉換後圖像資料
	GraylevelImage = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));
	//	配置GraylevelImage指標記憶體大小
	if (GraylevelImage == NULL) 										//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素灰階值
	{																	//	進入for迴圈
		GraylevelImage[loop_num].R = (image[loop_num].R + image[loop_num].G + image[loop_num].B) / 3;
		//	計算灰階轉換像素值
		GraylevelImage[loop_num].G = (image[loop_num].R + image[loop_num].G + image[loop_num].B) / 3;
		//	計算灰階轉換像素值
		GraylevelImage[loop_num].B = (image[loop_num].R + image[loop_num].G + image[loop_num].B) / 3;
		//	計算灰階轉換像素值
	}																	//	結束for迴圈
    return GraylevelImage;												//	回傳灰階轉換結果
}																		//	結束BMP圖片資料轉灰階副程式
BMP24RGB *ImageSmoothing33(const BMP24RGB *image,const int xsize,const int ysize)
//	ImageSmoothing33(BMP圖檔3*3Mask平滑濾波)副程式
{																		//	進入ImageSmoothing33副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			OutputData[loop_num].R = (
				image[loop_num+xsize-1].R	* 1 + image[loop_num+xsize].R	* 1 + image[loop_num+xsize+1].R	* 1 +
				image[loop_num-1].R 		* 1 + image[loop_num].R			* 1 + image[loop_num+1].R 		* 1 +
				image[loop_num-xsize-1].R	* 1 + image[loop_num-xsize].R	* 1 + image[loop_num-xsize+1].R	* 1
				) / 9;													//	平滑法計算
			OutputData[loop_num].G = (
				image[loop_num+xsize-1].G	* 1 + image[loop_num+xsize].G	* 1 + image[loop_num+xsize+1].G	* 1 +
				image[loop_num-1].G 		* 1 + image[loop_num].G			* 1 + image[loop_num+1].G 		* 1 +
				image[loop_num-xsize-1].G	* 1 + image[loop_num-xsize].G	* 1 + image[loop_num-xsize+1].G	* 1
				) / 9;													//	平滑法計算
			OutputData[loop_num].B = (
				image[loop_num+xsize-1].B	* 1 + image[loop_num+xsize].B	* 1 + image[loop_num+xsize+1].B	* 1 +
				image[loop_num-1].B 		* 1 + image[loop_num].B			* 1 + image[loop_num+1].B 		* 1 +
				image[loop_num-xsize-1].B	* 1 + image[loop_num-xsize].B	* 1 + image[loop_num-xsize+1].B	* 1
				) / 9;													//	平滑法計算
		}																//	結束else敘述
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束ImageSmoothing33副程式

BMP24RGB *MedianFilter33(const BMP24RGB *image,const int xsize,const int ysize)
//	BMP圖檔3*3Mask中值濾波副程式
{																		//	進入MedianFilter33(BMP圖檔3*3中值濾波)副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			unsigned char *SortArray;									//	宣告SortArray為unsigned char型態指標，供像素值排序使用
			SortArray = (unsigned char*)malloc(9 * sizeof(unsigned char));
			//---R像素排序---
			SortArray[0] = image[loop_num-xsize-1].R;					//	將像素資料填入SortArray陣列
			SortArray[1] = image[loop_num-xsize].R;						//	將像素資料填入SortArray陣列
			SortArray[2] = image[loop_num-xsize+1].R;					//	將像素資料填入SortArray陣列
			SortArray[3] = image[loop_num-1].R;							//	將像素資料填入SortArray陣列
			SortArray[4] = image[loop_num].R;							//	將像素資料填入SortArray陣列
			SortArray[5] = image[loop_num+1].R;							//	將像素資料填入SortArray陣列
			SortArray[6] = image[loop_num+xsize-1].R;					//	將像素資料填入SortArray陣列
			SortArray[7] = image[loop_num+xsize].R;						//	將像素資料填入SortArray陣列
			SortArray[8] = image[loop_num+xsize+1].R;					//	將像素資料填入SortArray陣列
			/* #ifdef DebugMode
				printf("排序前：");
				ShowUCharPointStr(SortArray,9);
			#endif */
			//qsort(SortArray, 9, sizeof(unsigned char), Compare);		//	呼叫qsort排序函數(定義於stdlib.h)
			SortArray = UCharBubbleSort(SortArray,9,0);
			/* #ifdef DebugMode
				printf("\n排序後：");
				ShowUCharPointStr(SortArray,9);
				system("pause");
			#endif */
			OutputData[loop_num].R = SortArray[4];						//	將排序後中值填入像素資料
			//---G像素排序---
			SortArray[0] = image[loop_num-xsize-1].G;					//	將像素資料填入SortArray陣列
			SortArray[1] = image[loop_num-xsize].G;						//	將像素資料填入SortArray陣列
			SortArray[2] = image[loop_num-xsize+1].G;					//	將像素資料填入SortArray陣列
			SortArray[3] = image[loop_num-1].G;							//	將像素資料填入SortArray陣列
			SortArray[4] = image[loop_num].G;							//	將像素資料填入SortArray陣列
			SortArray[5] = image[loop_num+1].G;							//	將像素資料填入SortArray陣列
			SortArray[6] = image[loop_num+xsize-1].G;					//	將像素資料填入SortArray陣列
			SortArray[7] = image[loop_num+xsize].G;						//	將像素資料填入SortArray陣列
			SortArray[8] = image[loop_num+xsize+1].G;					//	將像素資料填入SortArray陣列
			//qsort(SortArray, 9, sizeof(unsigned char), Compare);		//	呼叫qsort排序函數(定義於stdlib.h)
			UCharBubbleSort(SortArray,9,0);
			OutputData[loop_num].G = SortArray[4];						//	將排序後中值填入像素資料
			//---B像素排序---
			SortArray[0] = image[loop_num-xsize-1].B;					//	將像素資料填入SortArray陣列
			SortArray[1] = image[loop_num-xsize].B;						//	將像素資料填入SortArray陣列
			SortArray[2] = image[loop_num-xsize+1].B;					//	將像素資料填入SortArray陣列
			SortArray[3] = image[loop_num-1].B;							//	將像素資料填入SortArray陣列
			SortArray[4] = image[loop_num].B;							//	將像素資料填入SortArray陣列
			SortArray[5] = image[loop_num+1].B;							//	將像素資料填入SortArray陣列
			SortArray[6] = image[loop_num+xsize-1].B;					//	將像素資料填入SortArray陣列
			SortArray[7] = image[loop_num+xsize].B;						//	將像素資料填入SortArray陣列
			SortArray[8] = image[loop_num+xsize+1].B;					//	將像素資料填入SortArray陣列
			//qsort(SortArray, 9, sizeof(unsigned char), Compare);		//	呼叫qsort排序函數(定義於stdlib.h)
			UCharBubbleSort(SortArray,9,0);
			OutputData[loop_num].B = SortArray[4];						//	將排序後中值填入像素資料
		}																//	結束else敘述		
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束MedianFilter33(BMP圖檔3*3中值濾波)副程式

BMP24RGB *ImageOCR(const BMP24RGB *image,const int xsize,const int ysize)
//	ImageOCR(影像OCR)副程式
{																		//	進入ImageOCR(影像OCR)副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述
	//***初始化OutputData指標資料***
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序初始化OutputData圖像像素
	{																	//	進入for迴圈
		OutputData[loop_num].R = 128;									//	設定像素初始值
		OutputData[loop_num].G = 128;									//	設定像素初始值
		OutputData[loop_num].B = 128;									//	設定像素初始值
	}																	//	結束for迴圈
	bool XArray[1700];													//	宣告XArray布林變數陣列
	bool YArray[2340];													//	宣告YArray布林變數陣列
	//***初始化布林變數陣列***
	for(loop_num = 0; loop_num < 1700; loop_num++)						//	以for迴圈初始化XArray布林變數陣列
	{																	//	進入for迴圈
		XArray[loop_num] = false;										//	將false填入XArray布林變數陣列
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num < 2340; loop_num++)						//	以for迴圈初始化YArray布林變數陣列
	{																	//	進入for迴圈
		YArray[loop_num] = false;										//	將false填入YArray布林變數陣列
	}																	//	結束for迴圈
	//***水平掃描OCR圖像***
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序掃描圖像像素
	{																	//	進入for迴圈
		//---忽略邊界像素點(將index轉換為二維座標)---
		int PixelLocationX;												//	宣告PixelLocationX，記錄像素點位於圖像二維座標中之X方向位置
		int PixelLocationY;												//	宣告PixelLocationY，記錄像素點位於圖像二維座標中之Y方向位置
		PixelLocationX = loop_num % xsize;								//	計算PixelLocationX
		PixelLocationY = floor(loop_num / xsize);						//	計算PixelLocationY
		if( (PixelLocationX < 150) || (PixelLocationX > (xsize - 150)) || (PixelLocationY < 150) || (PixelLocationY > (ysize - 150)))
		//	檢測邊界點像素(像素點與邊界最短距離若小於150)
		{																//	進入if敘述
			
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			if( (image[loop_num].R == 0) || (image[loop_num].G == 0) || (image[loop_num].B == 0) )
			//	若R、G、B像素值任一為0(可能為文字)
			{															//	進入if敘述
				YArray[PixelLocationY] = true;							//	將布林變數改為true
			}															//	結束if敘述
		}																//	結束else敘述
	}																	//	結束for迴圈	
	//***垂直掃描OCR圖像***
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序掃描圖像像素
	{																	//	進入for迴圈
		int PixelLocationX;												//	宣告PixelLocationX，記錄像素點位於圖像二維座標中之X方向位置
		int PixelLocationY;												//	宣告PixelLocationY，記錄像素點位於圖像二維座標中之Y方向位置
		PixelLocationX = loop_num % xsize;								//	計算PixelLocationX
		PixelLocationY = floor(loop_num / xsize);						//	計算PixelLocationY
		if( (PixelLocationX < 150) || (PixelLocationX > (xsize - 150)) || (PixelLocationY < 150) || (PixelLocationY > (ysize - 150)))
		//	檢測邊界點像素(像素點與邊界最短距離若小於150)
		{																//	進入if敘述
			
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			if( (image[loop_num].R == 0) || (image[loop_num].G == 0) || (image[loop_num].B == 0) )
			//	若R、G、B像素值任一為0(可能為文字)
			{															//	進入if敘述
				YArray[PixelLocationY] = true;							//	將布林變數改為true
			}															//	結束if敘述
		}																//	結束else敘述
	}																	//	結束for迴圈	
	//---輸出掃描結果---
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序初始化OutputData圖像像素
	{																	//	進入for迴圈
		int PixelLocationX;												//	宣告PixelLocationX，記錄像素點位於圖像二維座標中之X方向位置
		int PixelLocationY;												//	宣告PixelLocationY，記錄像素點位於圖像二維座標中之Y方向位置
		PixelLocationX = loop_num % xsize;								//	計算PixelLocationX
		PixelLocationY = floor(loop_num / xsize);						//	計算PixelLocationY
		if( (XArray[PixelLocationX] == true) && (YArray[PixelLocationY] == true))
		{
			OutputData[loop_num].R = image[loop_num].R;								
			OutputData[loop_num].G = image[loop_num].G;								
			OutputData[loop_num].B = image[loop_num].B;								
		}
	}																	//	結束for迴圈
	return OutputData;
	#ifdef DebugMode													//	若有定義為DebugMode(除錯模式)
	//---顯示掃描結果---
	printf("OCR test\n");
	bool temp;
	int CountNum,CountNum2;
	/*temp = XArray[0];
	CountNum=0;
	CountNum2=0;
	printf("XArray:\n%d:",(int)temp);
	for(loop_num = 0; loop_num < 1700; loop_num++)						//	以for迴圈初始化XArray布林變數陣列
	{																	//	進入for迴圈
		if(XArray[loop_num] == temp)
		{
			CountNum++;
			CountNum2++;
		}
		else
		{
			temp = XArray[loop_num];
			printf("%d(%d)\n%d:",CountNum,CountNum2,(int)temp);
			CountNum=1;
		}
	}																	//	結束for迴圈
	printf("%d",CountNum);*/
	temp = YArray[0];
	CountNum=0;
	CountNum2=0;
	printf("\nYArray:\n%d:",(int)temp);
	for(loop_num = 0; loop_num < 2340; loop_num++)						//	以for迴圈初始化YArray布林變數陣列
	{																	//	進入for迴圈
		if(YArray[loop_num] == temp)
		{
			CountNum++;
			CountNum2++;
		}
		else
		{
			temp = YArray[loop_num];
			printf("%d(%d)\n%d:",CountNum,ysize-CountNum2,(int)temp);
			CountNum=1;
		}
	}																	//	結束for迴圈
	printf("%d",CountNum);
	#endif																//	結束ifdef敘述
}																		//	結束ImageOCR(影像OCR)副程式
BMP24RGB *RGBHistogramEqualization(const BMP24RGB *image,const int xsize,const int ysize)
//	RGBHistogramEqualization(RGB灰階影像直方圖等化)副程式
{																		//	進入RGBHistogramEqualization(RGB灰階影像直方圖等化)副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述
	//***初始化OutputData指標資料***
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序初始化OutputData圖像像素
	{																	//	進入for迴圈
		OutputData[loop_num].R = 0;										//	設定像素初始值
		OutputData[loop_num].G = 0;										//	設定像素初始值
		OutputData[loop_num].B = 0;										//	設定像素初始值
	}																	//	結束for迴圈
	long long int CountPixel[256];										//	宣告CountPixel陣列統計各像素值出現次數
	//***以for迴圈依序初始化CountPixel統計值為0***
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈初始化CountPixel統計值為0
	{																	//	進入for迴圈
		CountPixel[loop_num] = 0;										//	初始化CountPixel統計值為0
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		CountPixel[image[loop_num].R] = CountPixel[image[loop_num].R] + 1;
		//	計算R像素值出現次數
	}																	//	結束for迴圈
	unsigned long long int PDF[256],CDF[256];							//	宣告PDF與CDF陣列，計算個別像素值機率與累計機率
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序計算PDF
	{																	//	進入for迴圈
		PDF[loop_num] = CountPixel[loop_num];							//	計算像素值PDF
	}																	//	結束for迴圈
	//system("pause");
	CDF[0] = PDF[0];													//	填入像素值為0的CDF
	for(loop_num = 1; loop_num < 256; loop_num++)						//	以for迴圈依序計算CDF
	{																	//	進入for迴圈
		CDF[loop_num] = CDF[loop_num - 1] + PDF[loop_num];				//	計算像素值CDF
	}																	//	結束for迴圈
	unsigned char ResultPixel[256];										//	宣告ResultPixel陣列記錄HistogramEqualization後像素值
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序HistogramEqualization後像素值
	{																	//	進入for迴圈
		ResultPixel[loop_num] = CDF[loop_num] * 255 / (xsize * ysize);	//	計算HistogramEqualization後像素值
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈輸出結果至OutputData
	{																	//	進入for迴圈
		OutputData[loop_num].R = ResultPixel[image[loop_num].R];		//	輸出結果至OutputData
	}																	//	結束for迴圈
	
	//***以for迴圈依序初始化CountPixel統計值為0***
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈初始化CountPixel統計值為0
	{																	//	進入for迴圈
		CountPixel[loop_num] = 0;										//	初始化CountPixel統計值為0
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		CountPixel[image[loop_num].G] = CountPixel[image[loop_num].G] + 1;
		//	計算G像素值出現次數
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序計算PDF
	{																	//	進入for迴圈
		PDF[loop_num] = CountPixel[loop_num];							//	計算像素值PDF
	}																	//	結束for迴圈
	CDF[0] = PDF[0];													//	填入像素值為0的CDF
	for(loop_num = 1; loop_num < 256; loop_num++)						//	以for迴圈依序計算CDF
	{																	//	進入for迴圈
		CDF[loop_num] = CDF[loop_num - 1] + PDF[loop_num];				//	計算像素值CDF
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序HistogramEqualization後像素值
	{																	//	進入for迴圈
		ResultPixel[loop_num] = CDF[loop_num] * 255 / (xsize * ysize);	//	計算HistogramEqualization後像素值
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈輸出結果至OutputData
	{																	//	進入for迴圈
		OutputData[loop_num].G = ResultPixel[image[loop_num].G];		//	輸出結果至OutputData
	}																	//	結束for迴圈
	
	//***以for迴圈依序初始化CountPixel統計值為0***
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈初始化CountPixel統計值為0
	{																	//	進入for迴圈
		CountPixel[loop_num] = 0;										//	初始化CountPixel統計值為0
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		CountPixel[image[loop_num].B] = CountPixel[image[loop_num].B] + 1;
		//	計算B像素值出現次數
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序計算PDF
	{																	//	進入for迴圈
		PDF[loop_num] = CountPixel[loop_num];							//	計算像素值PDF
	}																	//	結束for迴圈
	CDF[0] = PDF[0];													//	填入像素值為0的CDF
	for(loop_num = 1; loop_num < 256; loop_num++)						//	以for迴圈依序計算CDF
	{																	//	進入for迴圈
		CDF[loop_num] = CDF[loop_num - 1] + PDF[loop_num];				//	計算像素值CDF
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序HistogramEqualization後像素值
	{																	//	進入for迴圈
		ResultPixel[loop_num] = CDF[loop_num] * 255 / (xsize * ysize);	//	計算HistogramEqualization後像素值
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈輸出結果至OutputData
	{																	//	進入for迴圈
		OutputData[loop_num].B = ResultPixel[image[loop_num].B];		//	輸出結果至OutputData
	}																	//	結束for迴圈
	return OutputData;													//	回傳HistogramEqualization運算結果
}																		//	結束RGBHistogramEqualization(RGB灰階影像直方圖等化)副程式
int BmpFillingByteCalc(const int xsize)									//	BmpFillingByteCalc(BMP圖檔填補位元計算)副程式
{																		//	進入BmpFillingByteCalc副程式
	int FillingByte;													//	宣告FillingByte記錄填補位元組計算結果
	FillingByte = ( xsize % 4);											//	計算填補位元組數量
	return FillingByte;													//	傳回計算結果
}																		//	結束BmpFillingByteCalc副程式

bool FileWritePermissionCheck(const char *file_name)					//	FileWritePermissionCheck(檔案寫入權限檢查)副程式
{																		//	進入FileWritePermissionCheck(檔案寫入權限檢查)副程式
	if( access( file_name, W_OK ) != -1 )								//	若檔案可讀取
	{																	//	進入if敘述
    	return True;													//	傳回True
	} 																	//	結束if敘述
	else 																//	若檔案不可讀取
	{																	//	進入else敘述
	    return False;													//	回傳false
	}																	//	結束else敘述
} 																		//	結束FileWritePermissionCheck(檔案寫入權限檢查)副程式

bool FileWrite(const char *file_name,const char *input_str,const char *mode)
//	FileWrite(檔案寫入)副程式，執行文字檔案寫入
//	file_name為欲寫入檔案檔名
// 	input_str為欲寫入檔案之指標形式字串資料
//	mode為寫入檔案模式設定，可傳入"w"或"a"，"w"為新增/覆蓋模式，"a"為擴充模式
//	回傳結果：若檔案寫入成功回傳True，若寫入失敗回傳False
{																		//	進入FileWrite(檔案寫入)副程式
	FILE *file_point;													//	宣告一file_point指標，控制檔案讀寫
	if (strcmp(mode, "w") != 0 && strcmp(mode, "a") != 0 && strcmp(mode, "w+") != 0 && strcmp(mode, "a+") != 0)
	//	若mode參數不為"w"亦不為"a"亦不為"w+"亦不為"a+"
	{																	//	進入if敘述
		printf("FileWrite:mode ERROR!\n");								//	顯示"FileWrite:mode ERROR!"並換行
		return False;													//	回傳False並結束副程式返回
	}																	//	結束if敘述
	if( strcmp(mode, "a") == 0 || strcmp(mode, "a+") == 0 )				//	若mode參數為"a"或"a+"(擴充模式)
	{																	//	進入if敘述
		if (FileWritePermissionCheck(file_name) == False)				//	若檔案無法寫入
		{																//	進入if敘述
			printf("FileWrite:permission ERROR!\n");					//	顯示"FileWrite:permission ERROR!"並換行
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
	}																	//	結束if敘述
	file_point = fopen(file_name, mode);								//	以mode模式打開檔案
	//	fprintf或fputs語法二擇一使用
	fprintf(file_point, input_str);										//	以fprintf語法寫入檔案
	//fputs(input_str, file_point);										//	以fputs語法寫入檔案
	fclose(file_point); 												//	關閉檔案
	return True;														//	回傳True並結束副程式返回
}																		//	結束FileWrite(檔案寫入)副程式

unsigned char *UCharBubbleSort(	const unsigned char *InputData,
								const unsigned long long int InputDataNum,
								const bool Mode)
//	UCharBubbleSort副程式
{																		//	進入UCharBubbleSort副程式
	unsigned char *OutputData;											//	宣告OutputData記錄輸出資料
	OutputData = (unsigned char*)malloc( InputDataNum * sizeof(unsigned char) );
	//	配置OutputData輸出資料指標記憶體空間
	unsigned long long int LoopNumber1,LoopNumber2;						//	宣告LoopNumber1與LoopNumber2，用於迴圈執行次數計算
	for(LoopNumber1=0;LoopNumber1 < InputDataNum;LoopNumber1++)			//	以迴圈依序
	{																	//	進入for迴圈
		OutputData[LoopNumber1] = InputData[LoopNumber1];				//	複製資料至輸出指標記憶體空間
	}																	//	結束for迴圈
	for(LoopNumber2 = 1;LoopNumber2 < InputDataNum;LoopNumber2++)		//	以迴圈依序比較數值
	{																	//	進入for迴圈
		for(LoopNumber1=0;LoopNumber1 < InputDataNum - LoopNumber2;LoopNumber1++)
		//	以迴圈依序比較數值
		{																//	進入for迴圈
			if( Mode == 0 )												//	若模式為由小排至大
			{															//	進入if敘述
				if(OutputData[LoopNumber1] > OutputData[LoopNumber1 + 1])
				//	若較大的數值在前
				{														//	進入if敘述
					unsigned char TempNumber;							//	宣告TempNumber暫存變數(用於數值交換)
					TempNumber = OutputData[LoopNumber1];				//	數值交換
					OutputData[LoopNumber1] = OutputData[LoopNumber1 + 1];
					//	數值交換
					OutputData[LoopNumber1 + 1] = TempNumber;			//	數值交換
				}														//	結束if敘述
			}															//	結束if敘述
			else if( Mode == 1 )										//	若模式為由大排至小
			{															//	進入else if敘述
				if(OutputData[LoopNumber1] < OutputData[LoopNumber1 + 1])
				//	若較小的數值在前
				{														//	進入if敘述
					unsigned char TempNumber;							//	宣告TempNumber暫存變數(用於數值交換)
					TempNumber = OutputData[LoopNumber1];				//	數值交換
					OutputData[LoopNumber1] = OutputData[LoopNumber1 + 1];
					//	數值交換
					OutputData[LoopNumber1 + 1] = TempNumber;			//	數值交換
				}														//	結束if敘述
			}															//	結束else if敘述
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳輸出資料
}																		//	結束UCharBubbleSort副程式

int Compare(const void *data1,const void *data2)						//	Compare副程式
{																		//	進入Compare副程式
	int *ptr1 = (int*)data1;											//	宣告*ptr1整數指標變數
	int *ptr2 = (int*)data2;											//	宣告*ptr2整數指標變數
	if(*ptr1 < *ptr2)													//	若*ptr1數值較小
	{																	//	進入if敘述
		return -1;														//	傳回-1，並結束副程式
	}																	//	結束if敘述
	else if(*ptr1 > *ptr2)												//	若*ptr1數值較大
	{																	//	進入else if敘述
		return 1;														//	傳回1，並結束副程式
	}																	//	結束else if敘述
	else																//	若*ptr1與*ptr2數值相等
	{																	//	進入else敘述
		return 0;														//	傳回0，並結束副程式
	}																	//	結束else敘述	
}																		//	結束Compare副程式
unsigned long long int CountCharPointStr(const char *Input_string, const bool Detail)	
//	CountCharPointStr(計算字元指標長度)副程式，計算字元指標(字串)長度
{																		//	進入CountCharPointStr副程式
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
}																		//	結束CountCharPointStr副程式
void Show_char_point_str(const char *Input_string)						//	Show_char_point_str(顯示字元指標)副程式
/*	顯示字串內容
	引用副程式：
		CountCharPointStr(計算字元指標長度)副程式
*/
{																		//	進入Show_char_point_str副程式
	unsigned long long int loop_num = 0;								//	宣告loop_num區域變數供迴圈使用，並設定初始值為0
	for(loop_num = 0; loop_num < CountCharPointStr(Input_string, False); loop_num++)
	{																	//	進入for迴圈
		printf("%c",Input_string[loop_num]);							//	依序顯示字元
	}																	//	結束for迴圈
}																		//	結束Show_char_point_str副程式
void ShowUCharPointStr(const unsigned char *InputData,unsigned long long int InputDataLen)
//	ShowUCharPointStr(顯示無號字元指標)副程式
{																		//	進入ShowUCharPointStr副程式
	unsigned long long int LoopNumber = 0;								//	宣告LoopNumber變數，用於迴圈計數
	for(LoopNumber=0;LoopNumber<InputDataLen;LoopNumber++)				//	以for迴圈依序顯示每項元素
	{																	//	進入for迴圈
		printf("%u,",InputData[LoopNumber]);							//	顯示無號字元指標內容
	}																	//	結束for迴圈
}																		//	結束ShowUCharPointStr副程式
