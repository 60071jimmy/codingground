# HueToBMP24RGB副程式說明
在常見的RGB色彩空間中，顏色是以紅色(Red)、綠色(Green)與藍色(Blue)三個維度合成；HSV色彩模式則以色調(Hue)、飽和度(Saturation)與明度(Value)三個屬性代表。本副程式將HSV型態影像資料中之Hue資料取出並匯出成圖像。由於HSV色彩空間之Hue值域為0～360(角度)，但BMP圖檔像素之R、G、B之值域為0～255，故令：

| BMP原始圖檔                         |    色調(Hue)                       |
| :------:                           |    :-----------:                   |
|![](https://i.imgur.com/KeLJ0s5.png)<br>圖1 Lenna原始圖檔|![](https://i.imgur.com/v74WQdp.png)<br>圖2 Lenna圖檔之Hue平面|
|![](https://i.imgur.com/ipTGuVe.jpg)|![](https://i.imgur.com/I7yhKpq.png)|
|![](https://i.imgur.com/0FY2YuQ.jpg)|![](https://i.imgur.com/ehMdgaF.png)|
|![](https://i.imgur.com/SbuZwBy.jpg)|![](https://i.imgur.com/vrhYNBF.jpg)|
|![](https://i.imgur.com/hbudyq9.jpg)|![](https://i.imgur.com/IXj0iTT.png)|

