#include "xil_types.h"
#include "xparameters.h"
#include "xiicps.h"
#include "seven_seg.h"
#include "textlcd.h"


#define	IIC_SLAVE_ADDR	0x51
#define	IIC_SCLK_RATE	100000


void	WriteTLCDReg(char *pRegVal, int val);
int		ReadRTC(XIicPs Iic, u8 *SendBuffer, u8 *RecvBuffer);


int main(void)
{
	XIicPs	Iic;			/**< Instance of the IIC Device */
	int 	IicStatus;
	u8		*SendBuffer;	/**< Buffer for Transmitting Data */
	u8		RecvBuffer[3];	/**< Buffer for Receiving Data */

	int		SegReg;
	char	TlcdReg_upline[16];
	char	TlcdReg_downline[16];

	int		i;
	int		wait;


	sprintf(TlcdReg_upline, "Digital Clock   ");

	while(TRUE)
	{
		IicStatus = ReadRTC(Iic, SendBuffer, RecvBuffer);

		if (IicStatus != XST_SUCCESS)
		{
			return XST_FAILURE;
		}
		// Coding
		SegReg = ((RecvBuffer[2] & 0x3f)<<24) + (0xA << 20) + (RecvBuffer[1] & 0x7f)<<12 + (0xA << 8) + (RecvBuffer[0] & 0x7f);
		SEVEN_SEG_mWriteReg(XPAR_SEVEN_SEG__S_AXI_BASEADDR, 0, SegReg);

		WriteTLCDReg(TlcdReg_downline, SegReg);
		for(i = 0; i < 4; i++)
		{	// Coding
			TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4, TlcdReg_upline[i*4+3] + (TlcdReg_upline[i*4+2]<<8) + (TlcdReg_upline[i*4+1]<<16) + (TlcdReg_upline[i*4]<<24));
			TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4+16, TlcdReg_downline[i*4+3] + (TlcdReg_downline[i*4+2]<<8) + (TlcdReg_downline[i*4+1]<<16) + (TlcdReg_downline[i*4]<<24));
		}

		for(wait = 0; wait < 1200; wait++);
	}
}

void WriteTLCDReg(char *pRegVal, int val)
{
	int		i = 0;
	char	temp;

	for(i = 0; i < 8; i++)
	{
		temp = ((val >> ((7-i)*4)) & 0x0f) + 0x30;	// Coding here!
		pRegVal[i] = temp;
		pRegVal[i+8] = 0x20;
	}
}

int ReadRTC(XIicPs Iic, u8 *SendBuffer, u8 *RecvBuffer)
{
	int				Status;
	XIicPs_Config	*Config;

	Config = XIicPs_LookupConfig(XPAR_XIICPS_0_DEVICE_ID);
	if (Config == NULL)
	{
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);

	*SendBuffer		= 0x02;
	RecvBuffer[0]	= 0x00;
	RecvBuffer[1]	= 0x00;
	RecvBuffer[2]	= 0x00;

	Status = XIicPs_MasterSendPolled(&Iic, SendBuffer, 1, IIC_SLAVE_ADDR);
	if(Status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	while(XIicPs_BusIsBusy(&Iic))
	{
		/* NOP */
	}

	Status = XIicPs_MasterRecvPolled(&Iic, RecvBuffer, 3, IIC_SLAVE_ADDR);
	if (Status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
