# SaturationToBMP24RGB副程式

在常見的RGB色彩空間中，顏色是以紅色(Red)、綠色(Green)與藍色(Blue)三個維度合成；HSV色彩模式則以色調(Hue)、飽和度(Saturation)與明度(Value)三個屬性代表。本副程式用於將HSV型態影像資料中之Saturation資料取出並匯出成圖像。由於HSV色彩空間之Saturation值域為0～1，但一般24位元圖像R、G、B之值域為0～255，故令：

R=Saturation * 255；G=Saturation * 255；B=Saturation * 255

以下為部分BMP輸入圖檔與飽和度(Saturation)屬性平面輸出結果。

| BMP原始圖檔                         |    飽和度(Saturation)                       |
| :------:                           |    :-----------:                   |
|![](https://i.imgur.com/KeLJ0s5.png)<br>圖1 Lenna原始圖檔|![](https://i.imgur.com/0uCSAjv.png)<br>圖2 Lenna圖檔之飽和度(Saturation)平面|
|![](https://i.imgur.com/ipTGuVe.jpg)|![](https://i.imgur.com/8jYTfWl.png)<br>飽和度(Saturation)|
|![](https://i.imgur.com/SbuZwBy.jpg)|![](https://i.imgur.com/I1r2gUr.jpg)<br>飽和度(Saturation)|
|![](https://i.imgur.com/0FY2YuQ.jpg)|![](https://i.imgur.com/Gu0W4r3.jpg)<br>飽和度(Saturation)|
|![](https://i.imgur.com/hbudyq9.jpg)|![](https://i.imgur.com/YrzQ5lN.jpg)<br>飽和度(Saturation)|

