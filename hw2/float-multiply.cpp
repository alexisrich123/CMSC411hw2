// build a floating point multiply from integer operations

#include <stdint.h>	// for uint32_t
#include <memory.h>	// for memcpy
#include <stdio.h>	// for sscanf, printf

////////////////////////////////////////
// you write this function
// use only integer types and operations (multiplies, adds, shifts, etc.)
// be sure to handle infinity, NaN and denormalized!
uint32_t multiply(uint32_t a, uint32_t b)
{

  //final result
  uint32_t finResults;

  // for buiding finall result
  //uint64_t mant;
  uint64_t mant;
  uint32_t expo;
  uint32_t sign;

  // constants
  uint32_t bias=0x0000007e;
  uint32_t bias64=0x000003ff;
  uint32_t neg=0x80000000;
  uint32_t leadingOne=0x00800000;
  uint32_t MantAndValue=0x0007fffff;
  uint32_t ExpoAndValue=0x7f800000;
  uint32_t signAndValue=0x80000000;
  uint32_t inf=0x7f800000;
  uint32_t NaN=0x7f800001;

  uint32_t max=0x00ffffff;
  uint32_t min=0x00800000;

  //A value
  uint32_t aMant=(a & MantAndValue);
  uint32_t aExpo=(a & ExpoAndValue);
  uint32_t aSign=(a & signAndValue);


  // B value
  uint32_t bMant=(b & MantAndValue);
  uint32_t bExpo=(b & ExpoAndValue);
  uint32_t bSign=(b & signAndValue);

  ////////////////////////For INF and NAN////////////////////////////////////////
  if(((aExpo==inf)&(aMant==0))&(bExpo<inf))
    {
      finResults=a;
      return finResults;


    }
  else if(((bExpo==inf)&(bMant==0))&(aExpo<inf))
    {
      finResults=a;
      return finResults;

    }// if both inf loginf if netiva
  else if(((bExpo==inf)&&(bMant==0))&&((aExpo==inf)&&(bMant==0)))
    {
      if(bSign==aSign)
	{
	  finResults= bSign+inf;

	  return finResults;


	}
      else if(bSign>aSign)
	{
	  finResults=bSign+inf;
	  return finResults;


	}
      else if(bSign<aSign){
	finResults=aSign+inf;
	return finResults;
      }
    }
  else if((aExpo==inf)&(aMant>0))
    {
      finResults=aExpo+aMant;
      return finResults;

    }
  else if((aExpo==inf)&(aMant>0))
    {
      finResults=bExpo+bMant;
      return finResults;

    }



  //////////////////////////////////END//////////////////////////////////////////


  
   //formating
  //sorting sign expo and mant
  // seting up expo

  aExpo=aExpo>>23;
  bExpo=bExpo>>23;

  aExpo=aExpo-bias;
  bExpo=bExpo-bias;

//setting up mant
  aMant=aMant+leadingOne;
  bMant=bMant+leadingOne;

  // if the sign are not equal then the number is -
  if(aExpo > bExpo)
    {

      for(uint32_t i=bExpo ; i!=aExpo;i++ ){
	bExpo= bExpo+1;
	bMant=bMant>>1;

	printf("first for loop\n");
      }
      expo=aExpo;
    }

  else if( aExpo <= bExpo)
    {

      for(uint32_t i=aExpo ; i!= bExpo;i++ ){
	aExpo=aExpo+1;
	aMant=aMant>>1;
	printf("second for loop\n");
      }
      expo=bExpo;
    }

 
  if(aSign==bSign){
    sign=0;
  }
  else{
    sign=neg;
  }
  // mant=(aMant * bMant);
  // mant =5;
  mant=aMant *bMant;
  mant=mant<<23;
  
 
     //noramlizing


  if( mant != 0){

     while((mant>max) ||(mant<min)){


     if(mant>max){
      
       mant=mant>>1;

       expo=expo+1;
       printf("mant > max in while mant=%d max=%d\n",mant,max);
       
     }
     else if(mant<min){
       mant=mant<<1;

       expo=expo-1;
       printf(" mant < min  in while mant=%d max=%d\n",mant,min);



     }
   }
     
  }
  expo=expo+bias;
   expo=expo<<23;
   mant=mant-leadingOne;
   finResults=sign+expo+mant;



  
	return finResults;
}
////////////////////////////////////////

// used to be able to use casting or union to convert
// now memcpy is supposed to be the only safe way

// get integer encoding for a float
uint32_t FtoI(float f) { 
	uint32_t i;
	memcpy(&i, &f, sizeof(float)); 
	return i;
}

// get float from its integer encoding
float ItoF(uint32_t i) { 
	float f;
	memcpy(&f, &i, sizeof(float)); 
	return f;
}

int main(int argc, char *argv[])
{
	// get numbers to multiply
	float f_a, f_b;
	if (argc != 3) {
		printf("must provide two numbers as command line arguments\n");
		return 1;
	}
	sscanf(argv[1], "%f", &f_a);
	sscanf(argv[2], "%f", &f_b);

	// compute result
	float f_result = f_a * f_b;
	uint32_t i_result = multiply(FtoI(f_a), FtoI(f_b));

	printf("multiplying %g * %g (0x%08x * 0x%08x)\n", f_a, f_b, FtoI(f_a), FtoI(f_b));
	printf("built-in  says (0x%08x) %g\n", FtoI(f_result), f_result);
	printf("hand-made says (0x%08x) %g\n", i_result, ItoF(i_result));

	return 0;
}
