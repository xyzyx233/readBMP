// readBMP.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <stdio.h>
#include "BmpRot.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>

using namespace std;

//��������
BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];//256ɫ��ɫ��
BITMAPINFOHEADER strInfo;

//��ʾλͼ�ļ�ͷ��Ϣ
void showBmpHead(BITMAPFILEHEADER pBmpHead){
	cout<<"λͼ�ļ�ͷ:"<<endl;
	cout<<"�ļ���С:"<<pBmpHead.bfSize<<endl;
	cout<<"������_1:"<<pBmpHead.bfReserved1<<endl;
	cout<<"������_2:"<<pBmpHead.bfReserved2<<endl;
	cout<<"ʵ��λͼ���ݵ�ƫ���ֽ���:"<<pBmpHead.bfOffBits<<endl<<endl;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){
	cout<<"λͼ��Ϣͷ:"<<endl;
	cout<<"�ṹ��ĳ���:"<<pBmpInforHead.biSize<<endl;
	cout<<"λͼ��:"<<pBmpInforHead.biWidth<<endl;
	cout<<"λͼ��:"<<pBmpInforHead.biHeight<<endl;
	cout<<"biPlanesƽ����:"<<pBmpInforHead.biPlanes<<endl;
	cout<<"biBitCount������ɫλ��:"<<pBmpInforHead.biBitCount<<endl;
	cout<<"ѹ����ʽ:"<<pBmpInforHead.biCompression<<endl;
	cout<<"biSizeImageʵ��λͼ����ռ�õ��ֽ���:"<<pBmpInforHead.biSizeImage<<endl;
	cout<<"X����ֱ���:"<<pBmpInforHead.biXPelsPerMeter<<endl;
	cout<<"Y����ֱ���:"<<pBmpInforHead.biYPelsPerMeter<<endl;
	cout<<"ʹ�õ���ɫ��:"<<pBmpInforHead.biClrUsed<<endl;
	cout<<"��Ҫ��ɫ��:"<<pBmpInforHead.biClrImportant<<endl;
}


int main(){
	//char strFile[LENGTH_NAME_BMP];//bmp�ļ���
	IMAGEDATA *imagedata = NULL;//��̬����洢ԭͼƬ��������Ϣ�Ķ�ά����
	IMAGEDATA *imagedataRot = NULL;//��̬����洢��ת���ͼƬ��������Ϣ�Ķ�ά����
	int width,height;//ͼƬ�Ŀ�Ⱥ͸߶�
	FILE *fpi,*fpw;
	fpi=fopen("lena256.bmp","rb");
	if(fpi != NULL){
		//�ȶ�ȡ�ļ�����
		WORD bfType;
		fread(&bfType,1,sizeof(WORD),fpi);
		if(0x4d42!=bfType)
		{
			cout<<"the file is not a bmp file!"<<endl;
			return NULL;
		}
		//��ȡbmp�ļ����ļ�ͷ����Ϣͷ
		fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpi);
		//showBmpHead(strHead);//��ʾ�ļ�ͷ
		fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);
		//showBmpInforHead(strInfo);//��ʾ�ļ���Ϣͷ

		//��ȡ��ɫ��
		for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
		{
			fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fpi);
			fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fpi);
		}

		width = strInfo.biWidth;
		height = strInfo.biHeight;
		//ͼ��ÿһ�е��ֽ���������4��������
		width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;
		imagedata = (IMAGEDATA*)malloc(width * height);
		imagedataRot = (IMAGEDATA*)malloc(width * height);
		//��ʼ��ԭʼͼƬ����������
		for(int i = 0;i < height;++i)
		{
			for(int j = 0;j < width;++j)
			{
				(*(imagedata + i * width + j)).blue = 0;
			}
		}
		//����ͼƬ����������
		fread(imagedata,sizeof(struct tagIMAGEDATA) * width,height,fpi);
		fclose(fpi);
	}
	else
	{
		cout<<"file open error!"<<endl;
		return NULL;
	}
	for (int i = 0;i < width * height;i++) {
		imagedataRot[i] = imagedata[i];
	}

	//����bmpͼƬ
	if((fpw=fopen("out.bmp","wb"))==NULL)
	{
		cout<<"create the bmp file error!"<<endl;
		return NULL;
	}
	WORD bfType_w=0x4d42;
	fwrite(&bfType_w,1,sizeof(WORD),fpw);
	//fpw +=2;
	fwrite(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpw);
	strInfo.biWidth = width;
	strInfo.biHeight = height;
	fwrite(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpw);
	//�����ɫ������
	for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)
	{
		fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);
		fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);
	}
	//������������
	for(int i =0;i < height;++i)
	{
		for(int j = 0;j < width;++j)
		{
			fwrite( &((*(imagedataRot + i * width + j)).blue),1,sizeof(BYTE),fpw);
		}
	}
	fclose(fpw);
	double* img = new double[width*height];
	for (int i = 0;i < height*width;i++) {
		img[i] = (double)imagedataRot[i].blue;
	}
	ofstream outfile;
	//outfile.open("F:\Pix.txt");
	//for (int i = width-1;i >=0 ;i--) {
	//	for (int j = 0;j < height;j++) {
	//		outfile << img[i * width + j] << '\t';
	//	}
	//	outfile << endl;
	//}
	//�ͷ��ڴ�
	delete[] imagedata;
	delete[] imagedataRot;
}
