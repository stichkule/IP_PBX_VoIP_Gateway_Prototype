/* 
* This routine decodes a u-law encoded audio file to a 16-bit
* linear PCM audio file. The function 'Snack_Mulaw2Lin()' has 
* been implemented in it's original form by referring to the 
* following resource:
*http://www.opensource.apple.com/source/tcl/tcl-20/tcl_ext/snack/s*nack/generic/g711.c
*---------------------------------------------------------------
* Authors: Shiril Tichkule, Srivishnu Alvakonda, Rahul Yamasani
* Date: 12 December 2017
*---------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define	SIGN_BIT	(0x80)	/* Sign bit for a A-law byte. */
#define	QUANT_MASK	(0xf)		/* Quantization field mask. */
#define	SEG_SHIFT	(4)	/* Left shift for segment number */
#define BIAS	(0x84)		/* Bias for linear code. */
#define	SEG_MASK	(0x70)		/* Segment field mask. */


/*
 * Snack_Mulaw2Lin() - Convert a u-law value to 16-bit linear PCM
 *
 * First, a biased linear code is derived from the code word. An unbiased
 * output can then be obtained by subtracting 33 from the biased code.
 *
 * Note that this function expects to be passed the complement of the
 * original code word. This is in keeping with ISDN conventions.
 */
short
Snack_Mulaw2Lin(
	unsigned char	u_val)
{
	short t;

	/* Complement to obtain normal u-law value. */
	u_val = ~u_val;

	/*
	 * Extract and bias the quantization bits. Then
	 * shift up by the segment number and subtract out the bias.
	 */
	t = ((u_val & QUANT_MASK) << 3) + BIAS;
	t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

	return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
}

uint8_t header[]={
0x52, //R
0x49, //I
0x46, //F
0x46, //F
0xda,0xff, 0x04,0x00, //File size
0x57, //W
0x41, //A
0x56, //V
0x45, //E
0x66,0x6d, 0x74,0x20, //FMT
0x10,0x00, 0x00,0x00, //Length of format data
0x01,0x00, //Format type -- PCM
0x01,0x00, //Number of channels 
0x40,0x1f,0x00,0x00, //Sample rate
0x00,0x7d,0x00,0x00, //Cumulative data rate
0x02,0x00, //8-bit stereo/16-bit mono
0x10,0x00, //bits-per-sample
0x64,0x61,0x74,0x61, //data chunk header
0xb6,0xff, 0x04,0x00}; //File size

int main(){
  FILE *fp_read, *fp_write; //file pointers for audio files
  uint32_t cnt=0,i;
  uint8_t buffer;
  fp_read = fopen("Au8A_eng_f5.wav","rb"); //read file pointer
  int flag = 0;
  uint16_t decoded;
  fp_write = fopen("decoded_output.wav","wb+"); //write-file
  for(i=0;i<44;i++) //read header from given WAV file
  {                 //and write it to the output WAV file
	buffer = header[i];
	fwrite(&buffer,sizeof(buffer),1,fp_write);
  }

  if(fp_read != NULL)
  {
    fseek(fp_read,44,SEEK_SET); //move over header data
    while(!feof(fp_read)) //while EOF is not reached
    {
      fread(&buffer,sizeof(buffer),1,fp_read); //read input
      decoded = Snack_Mulaw2Lin(buffer); //decode audio data
      fwrite(&decoded,sizeof(decoded),1,fp_write); 
    
    }
    fclose(fp_read); //close file pointers
    fclose(fp_write);
  }
}
