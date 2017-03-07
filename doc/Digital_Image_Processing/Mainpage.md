### 純C語言數位影像處理程式
#### BMP圖檔檔頭格式資訊
|    結構體名稱    |    可選        |    大小                    |    用途                    |    備註                |
|    :------:    |    :-----:    |    :------:                |    :-----:                |    :-----:            |
|    點陣圖檔案頭   |    否         |    14位元組                |    儲存點陣圖檔案通用資訊      |    僅在讀取檔案時有用    |
|    DIB頭        |    否         |    固定<br>(存在7種不同版本)  |    儲存點陣圖詳細資訊及像素格式|    緊接在點陣圖檔案頭後   |
|    附加位遮罩    |    是         |     3或4 DWORD<br>(12或16位元組)|    定義像素格式              |僅在DIB頭是BITMAPINFOHEADER時存在|
#### 讀取BMP圖檔與RGB、HSV色彩空間的轉換
> https://github.com/60071jimmy/codingground/tree/master/DIP-RGB2HSV-2

BMP原始圖檔與轉換至HSV色彩空間結果列表如下：

| BMP原始圖檔      |    轉換至HSV色彩空間結果 |
| :------:        |    :-----------:     |
|![](https://i.imgur.com/KeLJ0s5.png)|![](https://i.imgur.com/v74WQdp.png)<br>色調(Hue)<br>![](https://i.imgur.com/0uCSAjv.png)<br>飽和度(Saturation)<br>![](https://i.imgur.com/0Jgv6bN.png)<br>明度(Value)|
|![](https://i.imgur.com/KZYqcN4.png)|![](https://i.imgur.com/gkzhE2e.png)<br>色調(Hue)<br>![](https://i.imgur.com/jaKf1Hm.png)<br>飽和度(Saturation)<br>![](https://i.imgur.com/aPG6ZFj.png)<br>明度(Value)|
|![](https://i.imgur.com/ipTGuVe.jpg)|![](https://i.imgur.com/I7yhKpq.png)<br>色調(Hue)<br>![](https://i.imgur.com/8jYTfWl.png)<br>飽和度(Saturation)<br>![](https://i.imgur.com/xW9qjYD.png)<br>明度(Value)|
|![](https://i.imgur.com/SbuZwBy.jpg)|![](https://i.imgur.com/vrhYNBF.jpg)<br>色調(Hue)<br>![](https://i.imgur.com/I1r2gUr.jpg)<br>飽和度(Saturation)<br>![](https://i.imgur.com/LvdDNYC.jpg)<br>明度(Value)|
|![](https://i.imgur.com/ro291Qg.jpg)|![](https://i.imgur.com/2jHlFHX.jpg)<br>色調(Hue)<br>![](https://i.imgur.com/9mlN2Cg.jpg)<br>飽和度(Saturation)<br>![](https://i.imgur.com/udsaIeI.jpg)<br>明度(Value)|
|![](https://i.imgur.com/0FY2YuQ.jpg)|![](https://i.imgur.com/ehMdgaF.png)<br>色調(Hue)<br>![](https://i.imgur.com/Gu0W4r3.jpg)<br>飽和度(Saturation)<br>![](https://i.imgur.com/RIRId2t.jpg)<br>明度(Value)|
|![](https://i.imgur.com/hbudyq9.jpg)|![](https://i.imgur.com/IXj0iTT.png)<br>色調(Hue)<br>![](https://i.imgur.com/YrzQ5lN.jpg)<br>飽和度(Saturation)<br>![](https://i.imgur.com/tihRDcG.jpg)<br>明度(Value)|

上表所呈現之"轉換至HSV色彩空間結果"分別以灰階方式顯示色調(Hue)、飽和度(Saturation)與明度(Value)三項色彩特性。其結果分別由HueToBMP24RGB副程式、SaturationToBMP24RGB副程式與ValueToBMP24RGB副程式生成；副程式功能說明如下。

- HueToBMP24RGB副程式
    本副程式將HSV型態影像資料中之Hue資料取出並匯出成圖像。由於HSV色彩空間之Hue值域為0～360(角度)，但BMP圖檔像素之R、G、B之值域為0～255，故令：
$$ R=Hue \times {255 \over 360} $$$$ G=Hue \times {255 \over 360} $$$$ B=Hue \times {255 \over 360} $$
完整程式如下：
    > Github：
    >  https://github.com/60071jimmy/codingground/tree/master/DIP-HueToBMP24RGB-V1

- SaturationToBMP24RGB副程式
    本副程式用於將HSV型態影像資料中之Saturation資料取出並匯出成圖像。由於HSV色彩空間之Saturation值域為0～1，但一般24位元圖像R、G、B之值域為0～255，故令：
$$ R=Saturation \times 255 $$$$ G=Saturation \times 255 $$$$ B=Saturation \times 255 $$
完整程式如下：
    > Github：
    >https://github.com/60071jimmy/codingground/tree/master/DIP-SaturationToBMP24RGB1

- ValueToBMP24RGB副程式
    本副程式用於將HSV型態影像資料中之Value資料取出並匯出成圖像。由於HSV色彩空間之值域為0～255(整數數值)，一般24位元圖像R、G、B之值域為0～255，故令：
$$ R=Value $$$$ G=Value $$$$ B=Value $$
完整程式如下：
    > Github：
    > https://github.com/60071jimmy/codingground/tree/master/DIP-ValueToBMP24RGB-V1


#### 離散小波變換(Discrete Wavelet Transform，DWT)
##### 二維離散小波轉換
![](https://upload.wikimedia.org/wikipedia/commons/thumb/e/ea/2D_DWT.jpg/500px-2D_DWT.jpg)

圖片來自維基百科

| 圖像名稱 | 結果 |
| :------: | :-----------: |
|原始影像|![](https://i.imgur.com/KeLJ0s5.png)|
|水平方向低通濾波、垂直方向低通濾波<br>**有模糊之效果**<br>邊界像素點未處理|![](https://i.imgur.com/rbinJEh.png)|
|水平方向低通濾波、垂直方向高通濾波<br>**保留水平紋理**<br>邊界像素點未處理|![](https://i.imgur.com/FFJo5lI.png)|
|水平方向高通濾波、垂直方向低通濾波<br>**保留垂直紋理**<br>邊界像素點未處理|![](https://i.imgur.com/B3FrAMD.png)|
|水平方向高通濾波、垂直方向高通濾波<br>邊界像素點未處理|![](https://i.imgur.com/8wFK50v.png)|

將(水平方向低通濾波、垂直方向高通濾波)與(水平方向高通濾波、垂直方向低通濾波)與(水平方向高通濾波、垂直方向高通濾波)三者之圖像結果執行各像素點or運算如下：

![](https://i.imgur.com/Ao3vn6j.png)

##### 二維離散小波轉換高頻區與Sobel運算子測邊比較
![](https://i.imgur.com/4sLPgK0.png)

![](https://i.imgur.com/VE9YVq9.jpg)


#### 圖像處理結果合併

![](https://i.imgur.com/IZ2GNfw.jpg)
