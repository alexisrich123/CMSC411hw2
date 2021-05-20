// build a floating point multiply from integer operations

#include <stdint.h>	// for uint32_t
#include <memory.h>	// for memcpy
#include <stdio.h>	// for sscanf, printf

//////////////////////////////////////////////////////////////////////////////
// you write this function
// use only integer types and operations (multiplies, adds, shifts, etc.)
// be sure to handle infinity, NaN and denormalized!
uint32_t add(uint32_t a, uint32_t b)
{
  //mant max and min
  uint32_t max=0x00ffffff;
  uint32_t min=0x00800000;
  //uint32_t bias=0x3f800000;
  // not sure which one it is
  uint32_t Altbais=0x42fe0000;
  // constant
  uint32_t MantAndValue=0x0007fffff;
  uint32_t ExpoAndValue=0x7f800000;
  uint32_t signAndValue=0x80000000;
  uint32_t inf=0x7f800000;
  uint32_t NaN=0x7f800001;

  //adding leading one
   uint32_t leadingOne=0x00800000;

 
  uint32_t neg=0x80000000;

  //bias
  uint32_t bias=0x0000007e;

  
  // A:
  uint32_t aMant=(a&MantAndValue);
  uint32_t aExpo=(a&ExpoAndValue);
  uint32_t aSign=(a & signAndValue);
  // setting expo and Mant


  
  //B
   uint32_t bMant=(b&MantAndValue);
   uint32_t bExpo=(b&ExpoAndValue);
   uint32_t bSign=(b & signAndValue);
   // setting expo and Mant


   
  // result bit
   uint32_t expo;
   uint32_t mant;
   uint32_t sign;

  // final result
   uint32_t finResults;


   ///////////////////////////////////////////////////////////////////////////
   // checking if it is inf or NaN
   // for inf and normal number
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
   ///////////////////////////////////////////////////////////////////////////
   //setting up expo
   aExpo=aExpo>>23;
   bExpo=bExpo>>23;

   aExpo=aExpo-bias;
   bExpo=bExpo-bias;
   //setting up mant
   aMant=aMant+leadingOne;
   bMant=bMant+leadingOne; 

   if(aSign==bSign){
     if(aExpo > bExpo)
       {
	
	 for(uint32_t i=bExpo ; i!=aExpo;i++ ){
	  bExpo= bExpo+1;
	   bMant=bMant>>1;

      
	 }
       }
   
     else
     {
       
       for(uint32_t i=aExpo ; i!= bExpo;i++ ){
	 
	 aExpo=aExpo+1;
           aMant=aMant>>1;

	 
         }
     }

     mant=aMant+bMant;
     expo=aExpo;
     sign=aSign;


   }
   else{

     if(aSign>bSign){

       if(aExpo > bExpo)
	 {

	   for(uint32_t i=bExpo ; i!=aExpo;i++ ){
	     bExpo= bExpo+1;
	     bMant=bMant>>1;

           
	   }
	 }

       else
	 {

	   for(uint32_t i=aExpo ; i!= bExpo;i++ ){

	     aExpo=aExpo+1;
	     aMant=aMant>>1;

          

	   }
	 }
        mant=bMant-aMant;
	expo=aExpo;
	sign=aSign;



     }
     else{

        if(aExpo > bExpo)
       {

         for(uint32_t i=bExpo ; i!=aExpo;i++ ){
          bExpo= bExpo+1;
           bMant=bMant>>1;


         }
       }

     else
     {

       for(uint32_t i=aExpo ; i!= bExpo;i++ ){

         aExpo=aExpo+1;
           aMant=aMant>>1;
       }
     }


	
     }
     mant=aMant-bMant;
        expo=aExpo;
        sign=aSign;

     
   }
   
   //noramlizing 
   while((mant>max) ||(mant<min)){
     

     if(mant>max){
       
       mant=mant>>1;
      
       expo=expo+1;
       
     }
     else if(mant<min){
       mant=mant<<1;
      
       expo=expo-1;
      

     }
   }
   expo=expo+bias;
   expo=expo<<23;
   mant=mant-leadingOne;
   finResults=sign+expo+mant;
   
  return finResults;
}
//////////////////////////////////////////////////////////////////////////////

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
	// get numbers to add
	float f_a, f_b;
	if (argc != 3) {
		printf("must provide two numbers as command line arguments\n");
		return 1;
	}
	sscanf(argv[1], "%f", &f_a);
	sscanf(argv[2], "%f", &f_b);

	// compute result
	float f_result = f_a + f_b;
	uint32_t i_result = add(FtoI(f_a), FtoI(f_b));

	printf("adding %g + %g (0x%08x * 0x%08x)\n", f_a, f_b, FtoI(f_a), FtoI(f_b));
	printf("built-in  says (0x%08x) %g\n", FtoI(f_result), f_result);
	printf("hand-made says (0x%08x) %g\n", i_result, ItoF(i_result));

	return 0;
}
