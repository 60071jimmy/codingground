### Histogram Equalization(直方圖等化)數位影像處理(Digial Image Processing,DIP)程式
Develop by Jimmy Hu

延續DIP-HistogramEqualization專案功能，可自動讀取BMP資料夾內圖檔進行Histogram Equalization，演算法修正如下：

> 圖像中純白色像素點(R=255且G=255且B=255)不計算至直方圖等化機率統計中，機率計算僅考慮非純白色像素點。
