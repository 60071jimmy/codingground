### Histogram Equalization(直方圖等化)數位影像處理(Digial Image Processing,DIP)程式
Develop by Jimmy Hu

This project is adjusted algorithm from DIP-HistogramEqualization project. This main program could read all BMP files in the folder which named "BMP", and implement Histogram Equalization algorithm to each BMP file.The information of algorithm adjustment is following

> The pure white pixel (R = 255, G = 255, B = 255) is excluded when implement Histogram Equalization algorithm.

延續DIP-HistogramEqualization專案功能，該主程式可自動讀取BMP資料夾內圖檔進行Histogram Equalization，演算法修正如下：

> 圖像中純白色像素點(R=255且G=255且B=255)不計算至直方圖等化機率統計中，機率計算僅考慮非純白色像素點。
