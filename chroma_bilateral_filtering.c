#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//filter parameters
#define sigmaR 30 //Parameter to control the range filtering weight
#define sigmaD 50 //Parameter to control the domain filtering weight
#define neighbourLimit 5 //Define the maximum city block distance between the neighbouring pixels and the central pixel(pixel considered for filtering)

//input image path
#define input_path "D://noisyimg.yuv"
//output image path
#define output_path "D://chroma_bilateral_filtered_img.yuv"

//Function to return gaussian distributed weight based on the euclidean distance between the pixel locations
double domain_weight(int i,int j,int k,int l,double sigd){
    return exp(-0.5*pow(pow(pow(i-k,2)+pow(j-l,2),0.5)/sigd,2));
}
//Function to return gaussian distributed weight based on the euclidean distance between the pixel values
double range_weight(int x,int s,double sigr){
    return exp(-0.5*pow((x-s)/sigr,2));
}
int main(){
   
    FILE *file,*file1;
    unsigned char *yuvData,*uDataUp,*vDataUp,*uDataUpNew,*vDataUpNew;
    int width = 800;//Width of the YUV image
    int height = 500;//Height of the YUV image

    //Size of y,u and v plane for YUV420p
    int ySize = width * height;               
    int uSize = (width / 2) * (height / 2);    
    int vSize = (width / 2) * (height / 2);    

    file = fopen(input_path, "rb");//Specify the required YUV image's path
    if (!file) {
        perror("Error opening file");
        printf("Could not open the file.\n");
        return 1;
    }

    //Allocating memory to read the yuv image file
    yuvData = (unsigned char *)malloc(ySize + uSize + vSize);
    if (!yuvData) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return 1;
    }
    //Reading the yuv image data into buffer
    size_t bytesRead = fread(yuvData, 1, ySize + uSize + vSize, file);
    printf("Bytes read=%d\n",bytesRead);
    //Checking whether the yuv image data was read completely
    if (bytesRead != (ySize + uSize + vSize)) {
        printf("Error reading the file.\n");
        free(yuvData);
        fclose(file);
        return 1;
    }
    //Allocating memory space to store the upsampled u and v plane data
    uDataUp=(unsigned char *)malloc(uSize*4);
    vDataUp=(unsigned char *)malloc(4*vSize);
    //Allocating memory space to store the results of the bilateral filtering performed on the upsampled u and v plane data
    uDataUpNew=(unsigned char *)malloc(uSize*4);
    vDataUpNew=(unsigned char *)malloc(4*vSize);
    unsigned char *yPlane = yuvData;                               // Y plane
    unsigned char *uPlane = yuvData + ySize;                       // U plane
    unsigned char *vPlane = yuvData + ySize + uSize;               // V plane
    //Upsampling u plane data
    for(int i=0;i<height/2;i++){
        for(int j=0;j<width/2;j++){
            uDataUp[i*2*width+j*2]=uPlane[i*width/2+j];
            uDataUp[i*2*width+width+j*2]=uPlane[i*width/2+j];
            uDataUp[i*2*width+j*2+1]=uPlane[i*width/2+j];
            uDataUp[i*2*width+width+j*2+1]=uPlane[i*width/2+j];
        }
    }
    //Upsampling v plane data
    for(int i=0;i<height/2;i++){
        for(int j=0;j<width/2;j++){
            vDataUp[i*2*width+j*2]=vPlane[i*width/2+j];
            vDataUp[i*2*width+width+j*2]=vPlane[i*width/2+j];
            vDataUp[i*2*width+j*2+1]=vPlane[i*width/2+j];
            vDataUp[i*2*width+width+j*2+1]=vPlane[i*width/2+j];
        }
    }

    //Performing bilateral filtering on u plane data
    double weightedSum,totalWeight,weight;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            weightedSum=0,totalWeight=0;
            for(int k=-neighbourLimit;k<neighbourLimit;k++){
                if(i+k<0 || i+k>=height){
                    continue;
                }
                for(int l=-neighbourLimit;l<neighbourLimit;l++){
                    if(l+j<0 || l+j>=width){
                        continue;
                    }
                    weight=domain_weight(i,j,k,l,sigmaD)*range_weight(uDataUp[i*width+j],uDataUp[(i+k)*width+j+l],sigmaR);
                    totalWeight+=weight;
                    weightedSum+=weight*uDataUp[(i+k)*width+j+l];
                }
            }
            uDataUpNew[i*width+j]=round(weightedSum/totalWeight);
        }
    }
    //Performing bilateral filtering on v plane data
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            weightedSum=0,totalWeight=0;
            for(int k=-neighbourLimit;k<neighbourLimit;k++){
                if(i+k<0 || i+k>=height){
                    continue;
                }
                for(int l=-neighbourLimit;l<neighbourLimit;l++){
                    if(l+j<0 || l+j>=width){
                        continue;
                    }
                    weight=domain_weight(i,j,k,l,sigmaD)*range_weight(vDataUp[i*width+j],vDataUp[(i+k)*width+j+l],sigmaR);
                    totalWeight+=weight;
                    weightedSum+=weight*vDataUp[(i+k)*width+j+l];
                }
            }
            vDataUpNew[i*width+j]=round(weightedSum/totalWeight);
        }
    }
    int k=0;
    //Storing the filtered data back together
    for(int i=0;i<height;i+=2){
        for(int j=0;j<width;j+=2){
        yuvData[ySize+(k++)]=uDataUpNew[i*width+j];
        }
    }
    for(int i=0;i<height;i+=2){
        for(int j=0;j<width;j+=2){
            yuvData[ySize+(k++)]=vDataUpNew[i*width+j];
        }
    }
    file1 = fopen(output_path, "wb");//Specify the output image name and location of choice
    size_t bytesWrote=fwrite(yuvData,1,ySize + uSize + vSize, file1);//Storing the filtered yuv image
    printf("Bytes wrote=%d\n",bytesWrote);

    free(yuvData);
    free(uDataUp);
    free(vDataUp);
    free(uDataUpNew);
    free(vDataUpNew);
    fclose(file1);
    fclose(file);
    
}