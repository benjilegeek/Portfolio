#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>


// (c - sqrt(x²+y²))² + z² = a² --- c = 3 & a = 2 
// the taurus has a width of 10 and a depth of 2

// rotation around y axis
double rotX(double vec[],double deg){
	double rad = (deg / 180) * M_PI;
	double res = vec[0] * cos(rad) + vec[1] * 0 + vec[2] * -1 * sin(rad);  	
	return res;
}

double rotY(double vec[], double deg){
	return vec[1];
}

double rotZ(double vec[],double deg){
	double rad = (deg / 180) * M_PI;
	double res = vec[0] * sin(rad) + vec[1] * 0 + vec[2] * cos(rad);  	
	return res;
}

double roundFirstDec(double num){
	return round(num * 10.0) / 10.0;
}

int main(){
	// field -> brightness of the terminals
	// space -> 3d math space
	double field[50][50] = {0.0};		
	double angle = 0.0;
	double maxZ = 5.0;
	char output[11] = " .,-~:=!*$@";
	// loop 
	// clear screen and hide cursor
	printf("\x1b[2J\x1b[?25l");
	while(1){	
		for(double x=-10.0; x<10.0; x=x+0.4){
			for(double y=-10.0; y<10.0; y=y+0.4){
				double temp = -5 + 6 * sqrt(pow(x,2.0) + pow(y,2.0)) - pow(x,2.0) - pow(y,2.0);
				if(temp >= 0.0){
					double z = sqrt(temp);

					//TODO 
					// 1. implement positive negative part 
					// 2. make the thing bigger

					// rotating
					for(int i=-1; i<2; i=i+2){
						double vec[3] = {x,y,z*i};
						double rX = rotX(vec, angle);
						double rY = rotY(vec, angle);
						double rZ = rotZ(vec, angle);
								
						double brightness = (0.3 + fabs(rZ)) / 5.0;
						if(brightness > 1.0) brightness = 1.0;

						field[(int)round((rY+10.0)*2.5)][(int)round((rX+10.0)*2.5)] = brightness;
					}

				}
			}
		}
		//clear screen	
		printf("\x1b[H");
		for(int h=0; h<50;h++){
			for(int w=0; w<50; w++){
				int idx = (int)(roundFirstDec(field[h][w]) * 10.0);
				//print twice to adjust 2/1 scale of terminal 
				printf("%c%c",output[idx],output[idx]);
			}
			printf("\n");
		}	
		memset(field, 0.0, sizeof(field));
		angle = angle + 1.0;
		usleep(3000);
	}
	return 0;		
}
