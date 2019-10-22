#include <stdio.h>
#include <stdlib.h>

// sudoku 3 x 3 possibilities

struct centerSquare{
	
	int centerSquareIntValue;
	
	int possibilityNumber; //it keeps that how many are there possibilities for centerSquare	
	
	int intArray[8];
	
	int remainderArray[10][2];
	
	int remainderArrayLength;
};


int answer = 0;

struct centerSquare *structGenerate(){
	
	struct centerSquare *allcenterSquares;
	
	allcenterSquares = (struct centerSquare*) malloc(9 * sizeof(struct centerSquare));
	
	int minInt;
	for( minInt = 0 ; minInt < 9 ; minInt++ ){ // writing centerSquareIntValue
		(allcenterSquares + minInt) -> centerSquareIntValue = minInt + 1; // because it starts from zero
		(allcenterSquares + minInt) -> possibilityNumber = 0;
	}
	
	int minInt2;
	int structIndex;
	int possibleNUmberCounter = 0;
	for( structIndex = 0 ; structIndex < 9 ; structIndex++){
		for( minInt2 = 1 ; minInt2 < 10 ; minInt2++ ){
			if( minInt2 != ( allcenterSquares + structIndex ) -> centerSquareIntValue){
				(allcenterSquares + structIndex ) -> intArray[ possibleNUmberCounter] = minInt2;
				possibleNUmberCounter += 1 ;
			}
		}
		possibleNUmberCounter = 0 ;
	}
	
	
	calculator(allcenterSquares);
	
	return allcenterSquares;
}


int calculator(struct centerSquare *sudoo){
	
	int forC;
	int forC2;
	int forC3;
	int counter = 0;
	for( forC = 0 ; forC < 9 ; forC++ ){ // find total 15 integer and to append its struct
		int val = (sudoo + forC) -> centerSquareIntValue;
		for( forC2 = 0 ; forC2 < 8 ; forC2++ ){
			for( forC3 = 0 ; forC3 < 8 ; forC3++ ){
				int int1 = (sudoo + forC) -> intArray[forC2];
				int int2 = (sudoo + forC) -> intArray[forC3];
				if( 15 - val == int1 + int2 && int1 != int2 ){
					(sudoo + forC) -> remainderArray[counter][0] = int1 ;
					(sudoo + forC) -> remainderArray[counter][1] = int2 ;
					counter +=1 ;
				}
			}
		}
		(sudoo + forC) -> remainderArrayLength = counter ;
		
		counter = 0 ; 
	}
	
	int for1C;
	int for1C2;
	int for1C3;
	for( for1C = 0 ; for1C < 9 ; for1C++ ){
		for( for1C2 = 0 ; for1C2 < ((sudoo + for1C) -> remainderArrayLength + 1 ) / 2; for1C2++ ){
			for( for1C3 = 0 ; for1C3 < ((sudoo + for1C) -> remainderArrayLength + 1) / 2 ; for1C3++ ){
				if( for1C2 != for1C3){
					
					int p2 = (sudoo + for1C) -> remainderArray[ for1C2 ][ 0 ] ;
					int p8 = (sudoo + for1C) -> remainderArray[ for1C2 ][ 1 ] ;
					int p4 = (sudoo + for1C) -> remainderArray[ for1C3 ][ 0 ] ;
					int p6 = (sudoo + for1C) -> remainderArray[ for1C3 ][ 1 ] ;
					
					int pv2 = (sudoo + for1C) -> remainderArray[ for1C2 ][ 1 ] ;
					int pv8 = (sudoo + for1C) -> remainderArray[ for1C2 ][ 0 ] ;
					int pv4 = (sudoo + for1C) -> remainderArray[ for1C3 ][ 1 ] ;
					int pv6 = (sudoo + for1C) -> remainderArray[ for1C3 ][ 0 ] ;
					answer += correcter( (sudoo + for1C) , p2 , p8 , p4 , p6 );
					answer += correcter( (sudoo + for1C) , pv2 , pv8 , pv4 , pv6 );
				}
			}
		}	
	}
}

int correcter( struct centerSquare *sudoo , int p2 , int p8 , int p4 , int p6 ){ // p meaning is position and integer represents order
	int answer = 0;
	
	int testMatrix[3][3] = {
		{ 0 , p2 , 0 },
		{ p4 , (sudoo -> centerSquareIntValue), p6 },
		{ 0 , p8 , 0 },
	};
	
	int theOtherFour[4];
	int forC;
	int forCounter = 0;
	for( forC = 1; forC < 10 ; forC++ ){
		if(forC != sudoo -> centerSquareIntValue && forC != p2 && forC != p4 && forC != p6 && forC != p8){
			theOtherFour[ forCounter ] = forC;
			forCounter++ ;
		}
	}
	
	int for1C;
	for( for1C = 0 ; for1C < 4 ; for1C++ ){
		testMatrix[0][0] = theOtherFour[ ( for1C ) % 4 ] ;
		testMatrix[0][2] = theOtherFour[ ( for1C + 1 ) % 4 ] ;
		testMatrix[2][0] = theOtherFour[ ( for1C + 2 ) % 4 ] ;
		testMatrix[2][2] = theOtherFour[ ( for1C + 3 ) % 4 ] ;
		
		int CONT1 = testMatrix[0][0] + testMatrix[0][1] + testMatrix[0][2] ;
		int CONT2 = testMatrix[2][0] + testMatrix[2][1] + testMatrix[2][2] ;
		int CONT3 = testMatrix[0][0] + testMatrix[1][0] + testMatrix[2][0] ;
		int CONT4 = testMatrix[0][2] + testMatrix[1][2] + testMatrix[2][2] ;
		
		//printf("%d %d %d %d \n", testMatrix[0][0],testMatrix[0][2],testMatrix[2][0],testMatrix[2][2]);
		//printf("%d %d %d %d \n", CONT1 , CONT2 , CONT3 , CONT4);
		

		if( ( CONT1 == 15 && CONT2 == 15 ) && ( CONT3 == 15 && CONT4 == 15 )){
			int i,k;
			for(i = 0 ; i < 3 ; i ++ ){
				for(k = 0 ; k < 3 ; k ++){
					printf("%d ",testMatrix[i][k]);
				}
				printf("\n");
			}
			printf("\n");
			answer ++;
		}
	}
		
	return answer;

}

int main(int argc , char *argv[]){
	
	structGenerate();
	printf("All options = %d", answer * 8);
}










