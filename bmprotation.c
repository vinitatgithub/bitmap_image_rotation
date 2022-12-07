#include<stdio.h>
#include<conio.h>
#include<math.h>
#include"rotation.h"
//#include"bigtolittleendian.h"

typedef struct{
	unsigned char bfType[2];
	unsigned int bfSize;
	unsigned int bfReserved1;
	unsigned int bfReserved2;
	unsigned int bfOffBits;
}bmfh;

typedef struct{
	unsigned int biSize;
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned int biPlanes;
	unsigned int biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biXPelsPerMeter;
	unsigned int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
}bmih;

typedef struct{
    unsigned int biBlue;
    unsigned int biGreen;
    unsigned int biRed;
}acolors[];

int littletobig(unsigned char, unsigned char, unsigned char, unsigned char, unsigned int);

//GLOBALS
FILE *fp1,*fp2,*fp3;
unsigned char filename[100]={'0'};
unsigned char buffer[1000]={'0'},rowdata[5000]={'0'},buf[4]={'0'},headerbuf[40]={'0'};
unsigned int colorintensity=0,paletteentrycount=0,i=0,j=0,bytestoberead=0,noofshades=0;
unsigned int rotationside=0,ImageSize=0,count=0,ImageWidth=0,ImageHeight=0,col=0,biWid=0;
unsigned int biHei=0,FirstTime=1,vertnum=0,horinum=0,paddingbytes=0,acthoriSize=0;
unsigned int tmpOff=0, bytesperpel=0;
//

main()
{

    bmfh fileheader;
    bmih bitmapheader;

	printf("Enter the name of file with extension .bmp (BMP image should be uncompressed):\n");
	scanf("%s",&filename);

    fp1=fopen(filename,"rb");
    if(fp1==NULL)
    {
        printf("file not found..\n");
        return 0;
    }

    printf("Enter the number required for operation to be performed.\n");
    printf("1. Rotate Right by 90 degrees.\n");
    printf("2. Rotate Left by 90 degrees.\n");
    printf("3. Rotate Right by 180 degrees.\n");
    printf("4. Rotate Left by 180 degrees.\n");
    printf("5. Rotate Right by 270 degrees.\n");
    printf("6. Rotate Left by 270 degrees.\n");
    printf("7. Rotate Right by 90 degrees and flip wrt horizontal axis.\n");
    printf("8. Flip wrt horizontal axis.\n");
    printf("9. Flip wrt vertical axis.\n");

    scanf("%d",&rotationside);
    if(rotationside <= 0 || rotationside >= 10)
    {
        printf("Invalid number entered.\n");
        printf("Program terminates.\n");
        return 0;
    }

    fp2=fopen("temp.bmp","wb");
	fread(buffer,1,14,fp1);
    fwrite(buffer,1,14,fp2);

//file header information extraction starts here
    fileheader.bfType[0]    =buffer[0];
    fileheader.bfType[1]    =buffer[1];
    fileheader.bfSize       =littletobig(buffer[2],buffer[3],buffer[4],buffer[5],4);
    fileheader.bfReserved1  =littletobig(buffer[6],buffer[7],0,0,2);
    fileheader.bfReserved2  =littletobig(buffer[8],buffer[9],0,0,2);
    fileheader.bfOffBits    =littletobig(buffer[10],buffer[11],buffer[12],buffer[13],4);

    if(fileheader.bfType[0]!='B' || fileheader.bfType[1]!='M' || 
        fileheader.bfReserved1!=0 || fileheader.bfReserved2!=0)
    {
        printf("Corrupted File Header.\n");
        printf("Terminating Program.....\n");
        goto end;
    }
//file header information extraction ends

    printf("BMP HEADER INFORMATION:\n");
    printf("\t %-25s: %c%c\n","File Type",fileheader.bfType[0],fileheader.bfType[1]);
    printf("\t %-25s: %d\n",  "File Size",fileheader.bfSize);
    printf("\t %-25s: %d\n",  "Reserved Bit 1",fileheader.bfReserved1);
    printf("\t %-25s: %d\n",  "Reserved Bit 2",fileheader.bfReserved2);
    printf("\t %-25s: %d\n",  "File Offset for Data",fileheader.bfOffBits);

//bitmap header information extraction starts here
    if((fileheader.bfOffBits-14)<=100)
    {
        fread (buffer,1,fileheader.bfOffBits-14,fp1);
        fwrite(buffer,1,fileheader.bfOffBits-14,fp2);
    }
    else
    {
        fread (buffer,1,40,fp1);
        fwrite(buffer,1,40,fp2);

        tmpOff=fileheader.bfOffBits-54;
        while(tmpOff>0)
        {
            fread(headerbuf,1,1,fp1);
            fwrite(headerbuf,1,1,fp2);
            tmpOff--;
        }
    }

    bitmapheader.biSize         =littletobig(buffer[0],buffer[1],buffer[2],buffer[3],4);
    bitmapheader.biWidth        =littletobig(buffer[4],buffer[5],buffer[6],buffer[7],4);
    bitmapheader.biHeight       =littletobig(buffer[8],buffer[9],buffer[10],buffer[11],4);
    bitmapheader.biPlanes       =littletobig(buffer[12],buffer[13],0,0,2);
    bitmapheader.biBitCount     =littletobig(buffer[14],buffer[15],0,0,2);
    bitmapheader.biCompression  =littletobig(buffer[16],buffer[17],buffer[18],buffer[19],4);
    bitmapheader.biSizeImage    =littletobig(buffer[20],buffer[21],buffer[22],buffer[23],4);
    bitmapheader.biXPelsPerMeter=littletobig(buffer[24],buffer[25],buffer[26],buffer[27],4);
    bitmapheader.biYPelsPerMeter=littletobig(buffer[28],buffer[29],buffer[30],buffer[31],4);   
    bitmapheader.biClrUsed      =littletobig(buffer[32],buffer[33],buffer[34],buffer[35],4);
    bitmapheader.biClrImportant =littletobig(buffer[36],buffer[37],buffer[38],buffer[39],4);
//bitmap header information extraction ends here

    printf("\n");
    printf("BITMAP HEADER INFORMATION:\n");
    printf("\t %-25s: %d\n",  "Bitmap Header Size",bitmapheader.biSize);
    printf("\t %-25s: %d\n",  "Image Width",bitmapheader.biWidth);
    printf("\t %-25s: %d\n",  "Image Height",bitmapheader.biHeight);
    printf("\t %-25s: %d\n",  "Number of Planes",bitmapheader.biPlanes);
    printf("\t %-25s: %d\n",  "Number of Bits per Pel",bitmapheader.biBitCount);
        
    if(bitmapheader.biCompression==0)
        printf("\t %-25s: %d(%s)\n",  "Compression Type",bitmapheader.biCompression,"Uncompressed Data");
    else if (bitmapheader.biCompression==1)
        printf("\t %-25s: %d(%s)\n",  "Compression Type",bitmapheader.biCompression,"RGB_8");
    else if (bitmapheader.biCompression==2)
        printf("\t %-25s: %d(%s)\n",  "Compression Type",bitmapheader.biCompression,"RGB_4");
    else if (bitmapheader.biCompression==3)
        printf("\t %-25s: %d(%s)\n",  "Compression Type",bitmapheader.biCompression,"RGB bitmap with MASK");

    printf("\t %-25s: %d\n",  "Size of Image",      bitmapheader.biSizeImage);
    printf("\t %-25s: %d\n",  "X Pixels per Meter", bitmapheader.biXPelsPerMeter);
    printf("\t %-25s: %d\n",  "Y Pixels per Meter", bitmapheader.biYPelsPerMeter);
    printf("\t %-25s: %d\n",  "Colours Used",       bitmapheader.biClrUsed);
    printf("\t %-25s: %d\n",  "Colours Important",  bitmapheader.biClrImportant);
    
    ImageSize       =bitmapheader.biSizeImage;
    ImageWidth      =bitmapheader.biWidth;
    ImageHeight     =bitmapheader.biHeight;
    biWid           =bitmapheader.biWidth;
    biHei           =bitmapheader.biHeight;
    bytesperpel     =bitmapheader.biBitCount/8;

    vertnum         =(int)(4*ceil((double)(bitmapheader.biHeight*bytesperpel)/4));
    horinum         =(int)(4*ceil((double)(bitmapheader.biWidth*bytesperpel)/4));

    paddingbytes    =biHei*(horinum-biWid*bytesperpel);
    acthoriSize     =horinum;

    printf("Wait....\n");
    if((bitmapheader.biBitCount==32 || bitmapheader.biBitCount==24 || 
        bitmapheader.biBitCount==16 || bitmapheader.biBitCount==8) && 
        bitmapheader.biCompression==0)
    {
        if (rotationside==1 || rotationside==6)
        {
            rotater90l270();
        }
        else if (rotationside==2 || rotationside==5)
        {
            rotatel90r270();
        }
        else if (rotationside==3 || rotationside==4)
        {
            rotater180l180();
        }
        else if (rotationside==7)
        {
            rotater90andflip();
        }
        else if (rotationside==8)
        {
            flipwrthoriaxis();
        }
        else if (rotationside==9)
        {
            flipwrtvertiaxis();
        }
        else
        {
            printf("Unsupported BMP format found...\n");
            printf("Program terminates..\n");
            return 0;
        }
    }

    printf("Generating output image....\n");
    fp2=fopen("temp.bmp","rb");
    fp3=fopen("rotated.bmp","wb");
	fread(buffer,1,fileheader.bfOffBits,fp2);
   
    if(rotationside!=3 && rotationside!=4 && rotationside!=8 && rotationside!=9)
    {
        buf[0]      =buffer[18];
        buf[1]      =buffer[19];
        buf[2]      =buffer[20];
        buf[3]      =buffer[21];

        buffer[18]  =buffer[22];
        buffer[19]  =buffer[23];
        buffer[20]  =buffer[24];
        buffer[21]  =buffer[25];

        buffer[22]  =buf[0];
        buffer[23]  =buf[1];
        buffer[24]  =buf[2];
        buffer[25]  =buf[3];
    }
    
    fwrite(buffer,1,fileheader.bfOffBits,fp3);
    bitmapheader.biSizeImage=bitmapheader.biSizeImage-54;

    ImageSize=bitmapheader.biSizeImage*3;

    while(ImageSize>0)
    {
        fread(buffer,1,1,fp2);
        fwrite(buffer,1,1,fp3);
        ImageSize--;
    }
    
	fclose(fp2);
    fclose(fp3);
end:;
    return 0;
}
