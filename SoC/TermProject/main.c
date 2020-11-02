#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "xil_exception.h"
#include "xparameters.h"
#include "xscugic.h"
#include "xuartps_hw.h"
#include "xil_types.h"
#include "xiicps.h"
#include "xtime_l.h"
#include "seven_seg.h"
#include "textlcd.h"
#include "pushbutton.h"
#include "led.h"

//			Define interrupt			//
#define INTC_DEVICE_ID		XPAR_SCUGIC_0_DEVICE_ID
#define INTC_DEVICE_INT_ID	31

int GicConfigure(u16 DeviceId);
void ServiceRoutine(void *CallbackRef); //

XScuGic InterruptController; 	     // Instance of the Interrupt Controller
static XScuGic_Config *GicConfig;    // The configuration parameters of the controller

int INTR_F;
int STATE;

//			Define STATE			//
#define	READY	100
#define	RUN1	101
#define	RUN2	110
#define	OVER	111


void 	makeNum(int *ANum);
void	numCompare(int *ANum);

//int		ANum[3];		// Random number buffer
int		Strike;			// # of strike
int		Ball;			// # of ball
int		Out;			// # of out

//			Define	TextLCD				//
char	TlcdReg_upline[16];				// Upline of TextLCD
char	TlcdReg_downline[16];			// Downlline of TextLCD

int 	LCDReg;

void 	WriteTLCDReg(char *pRegVal, int val);
void 	WriteTLCDReg2(char *pRegVal);//show life
void 	WriteTLCDReg3(char *pRegVal);
void 	WriteTLCDReg4(char *pRegVal, int* ANum);
void 	TlcdReady();
void 	TlcdGame();
void 	TlcdSituation();
void 	TlcdSuccess(int* ANum);
void 	TlcdFail(int* ANum);


//			Define	7segment			//
int		num100;
int		num10;
int		num1;
int		Clock;
int		SegReg;

void	state_7Ready();
void	state_7Run1(XTime *xStart, XTime *xEnd, int *ANum);
void	state_7Run2();
void	state_7Over();

void 	numSet();
void	printFND();

//			Define	LED				//
int		life;
int 	LedReg;

void	printLED();


int main(void)
{
	int Status;
	unsigned int	CntrlRegister;

	// Read status register
	CntrlRegister = XUartPs_ReadReg(XPAR_PS7_UART_1_BASEADDR, XUARTPS_CR_OFFSET);

	XUartPs_WriteReg(XPAR_PS7_UART_1_BASEADDR, XUARTPS_CR_OFFSET,
			((CntrlRegister & ~XUARTPS_CR_EN_DIS_MASK) | XUARTPS_CR_TX_EN | XUARTPS_CR_RX_EN));

	Status = GicConfigure(INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("GIC Configure Failed\r\n");
		return XST_FAILURE;
	}

//	Variable Initialize
	int		ANum[3];		// Random number buffer
	STATE	= READY;
	XTime	xStart, xEnd;
	SegReg	= 0x00000000;
	LCDReg	= 0x00000000;

	LedReg	= 0x00;

	xil_printf("GAME START\r\n");
	XTime_GetTime(&xStart);
	srand(xStart);

	while(TRUE)
	{
		switch(STATE) {
			case READY	:
				life 	= 3;
				TlcdReady();
				printLED();			// life = 3
				state_7Ready();

				if(INTR_F == 1) {
					makeNum(ANum);			// random number gen

					Clock 	= 60;
					num100	= 0;
					num10	= 0;
					num1	= 0;
					Strike 	= 0;
					Ball	= 0;
					Out		= 0;		// initialization

					XTime_SetTime(0);
					XTime_GetTime(&xStart);

					STATE 	= RUN1;		// Game start
					INTR_F 	= 0;
				}
				break;

			/**************************************************/

			case RUN1	:
				numSet();		// Get the number from BUTTONS
				state_7Run1(&xStart, &xEnd, ANum);
				printLED();
				TlcdGame();

				if(INTR_F == 1) {
					xil_printf("Decision : %d%d%d", num100, num10, num1);

					numCompare(ANum);

					STATE = RUN2;
					Clock 	= 0;
					life--;
					INTR_F 	= 0;
				}

				break;

			/**************************************************/

			case RUN2	:
				Clock = 60;
				printLED();
				state_7Run2();
				TlcdSituation();

				if((Strike == 3) || (life < 1) ) {
					xil_printf("\nGo to over state\r\n");
					STATE 	= OVER;
				}

				if(INTR_F == 1) {
					XTime_GetTime(&xStart);
					STATE 	= RUN1;

					INTR_F 	= 0;
				}
				break;

			/**************************************************/

			case OVER	:
				if(Strike == 3)	{
					TlcdSuccess(ANum);
				} else {
					TlcdFail(ANum);
				}

				printLED();
				state_7Over();

				if(INTR_F == 1) {
					STATE 	= READY;
					life	= 3;
					Strike 	= 0;
					Ball 	= 0;
					Out		= 0;

					INTR_F 	= 0;
				}
				break;

			/**************************************************/

			default	:
				STATE	= READY;
				INTR_F	= 0;
				life	= 3;
				Strike 	= 0;
				Ball	= 0;
				Out		= 0;
				num100	= 0;
				num10	= 0;
				num1	= 0;
				break;
		}	// end of switch
	}// end of while
	return XST_SUCCESS;
}



int GicConfigure(u16 DeviceId)
{
	int Status;

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	GicConfig = XScuGic_LookupConfig(DeviceId);
	if (NULL == GicConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&InterruptController, GicConfig,
					GicConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the ARM processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
			&InterruptController);

	/*
	 * Enable interrupts in the ARM
	 */
	Xil_ExceptionEnable();

	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler performs
	 * the specific interrupt processing for the device
	 */
	Status = XScuGic_Connect(&InterruptController, INTC_DEVICE_INT_ID,
			   (Xil_ExceptionHandler)ServiceRoutine,
			   (void *)&InterruptController);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the interrupt for the device and then cause (simulate) an
	 * interrupt so the handlers will be called
	 */
	XScuGic_Enable(&InterruptController, INTC_DEVICE_INT_ID);

	return XST_SUCCESS;
}

void ServiceRoutine(void *CallbackRef)
{
	char pb;

	pb = PUSHBUTTON_mReadReg(XPAR_PUSHBUTTON_0_S00_AXI_BASEADDR, 0);
	PUSHBUTTON_mWriteReg(XPAR_PUSHBUTTON_0_S00_AXI_BASEADDR, 0, 0);

	if ((pb & 1) == 1){
		xil_printf("S1 \t");
		INTR_F = 1;
	}
	else if ((pb & 2) == 2){
		xil_printf("S2 \t");
		INTR_F = 2;
	}
	else if ((pb & 4) == 4){
		xil_printf("S3 \t");
		INTR_F = 4;
	}
	else if ((pb & 8) == 8){
		xil_printf("S4 \t");
		INTR_F = 8;
	}

}

void	state_7Ready() {

	SegReg = (0x1A00 << 16) + (0xAAAA);
	SEVEN_SEG_mWriteReg(XPAR_SEVEN_SEG_0_S00_AXI_BASEADDR, 0, SegReg);
}

void	state_7Run1(XTime *xStart, XTime *xEnd, int *ANum) {

	XTime_GetTime(xEnd);

	if((*xEnd - *xStart) >= COUNTS_PER_SECOND) {
		Clock--;
		XTime_GetTime(xStart);
	}

	if(Clock <= 0) {
		STATE = RUN2;
		life--;
		numCompare(ANum);
		Clock = 60;
	}

	printFND();

}

void	state_7Run2() {
	printFND();
}


void	state_7Over() {
	printFND();
}

void	numCompare(int *ANum) {

	int Answer[3];

	Strike 	= 0;
	Ball 	= 0;
	Out		= 0;

	Answer[0] = num100;
	Answer[1] = num10;
	Answer[2] = num1;


	xil_printf("\n\n*** Compare with : %d%d%d    ", Answer[0], Answer[1], Answer[2]);
	xil_printf("***    ANum : %d%d%d ***\n\n", ANum[0], ANum[1], ANum[2]);
	//xil_printf("\nAnswer = %d%d%d\r\n",ANum[0], ANum[1], ANum[2]);

    if(ANum[0] == Answer[0]) Strike++;
    if(ANum[1] == Answer[1]) Strike++;
    if(ANum[2] == Answer[2]) Strike++;

    if(ANum[0] == Answer[1] || ANum[0] == Answer[2]) Ball++;
    if(ANum[1] == Answer[0] || ANum[1] == Answer[2]) Ball++;
    if(ANum[2] == Answer[0] || ANum[2] == Answer[1]) Ball++;


	if((Strike == 0) && (Ball == 0))
		Out = 0x0F;		// Out : O
	else
		Out = 0x18;		// Out : X

}


void	numSet() {

	if(INTR_F == 2) {
		num100++;
		if(num100 > 9 ) {
			num100 = 0;
		}
	}
	if(INTR_F == 4) {
		num10++;
		if(num10 > 9) {
			num10 = 0;
		}
	}
	if(INTR_F == 8) {
		num1++;
		if(num1 > 9) {
			num1 = 0;
		}
	}
	INTR_F = 0;
}

void	printFND() {

	int upline;
	int downline;

	if(Clock == 60)	{
		upline = 0x1A00;
	} else {
		upline = (0xAA00) + ((Clock/10) << 4) + (Clock%10);
	}

	downline = 0xA000 + (num100 << 8) + (num10 << 4) + num1;

	SegReg = (upline << 16) + (downline);
	SEVEN_SEG_mWriteReg(XPAR_SEVEN_SEG_0_S00_AXI_BASEADDR, 0, SegReg);

}


void	makeNum(int *ANum) {

	for (int i = 0; i < 3; i++)
	{
		ANum[i] = (random() % 9) + 1;
		for (int j = 0; j < i; j++){
			if (ANum[i] == ANum[j]){
				i--;
				break;
			}
		}
	}

	xil_printf("\nAnswer = %d%d%d\r\n",ANum[0], ANum[1], ANum[2]);
}

void	printLED() {

	switch(life) {
		case 1 :
			LedReg = 0x01;
			break;
		case 2 :
			LedReg = 0x05;
			break;
		case 3 :
			LedReg = 0x15;
			break;
		default :
			LedReg = 0x00;
			break;
	}
	LED_mWriteReg(XPAR_LED_0_S00_AXI_BASEADDR, 0, LedReg);

}


void WriteTLCDReg(char *pRegVal, int val)
{
	int		i = 0;
	//char	temp;

	for(i = 0; i < 3; i++){
		pRegVal[i] = 0x20;
	}

	pRegVal[3] = 0x30 + Strike;

	for(i = 4; i < 9; i++){
		pRegVal[i] = 0x20;
	}

	pRegVal[9] = 0x30 + Ball;

	for(i = 10; i < 13; i++){
		pRegVal[i] = 0x20;
	}

	pRegVal[13] = 0x40 + Out;

	for(i = 14; i < 16; i++){
		pRegVal[i] = 0x20;
	}

}

void WriteTLCDReg2(char *pRegVal)
{
	int		i = 0;
	//int 	life;

	pRegVal[0] = 0x30+life;

	for(i = 1; i < 16; i++){
		pRegVal[i] = 0x20; //set space
	}
}

void WriteTLCDReg3(char *pRegVal)
{
	int		i = 0;

	for(i = 0; i < 16; i++){
		pRegVal[i] = 0x20; //set space
	}
}

void WriteTLCDReg4(char* pRegVal, int* ANum){
	pRegVal[0] = 0x41;	// A
	pRegVal[1] = 0x6e;	// n
	pRegVal[2] = 0x73;	// s
	pRegVal[3] = 0x77;	// w
	pRegVal[4] = 0x65;	// e
	pRegVal[5] = 0x72;	// r
	pRegVal[6] = 0x3a;	// :
	pRegVal[7] = 0x30 + ANum[0];	// 100
	pRegVal[8] = 0x30 + ANum[1];	// 10
	pRegVal[9] = 0x30 + ANum[2];	// 1

	for(int i = 10; i<16; i++)
		pRegVal[i] = 0x20;

}

void TlcdReady()
{

	int i;
	int wait;

	sprintf(TlcdReg_upline, "Game Ready      ");
	WriteTLCDReg3(TlcdReg_downline);
			for(i = 0; i < 4; i++)
			{
				TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4, TlcdReg_upline[i*4+3] + (TlcdReg_upline[i*4+2]<<8) + (TlcdReg_upline[i*4+1]<<16) + (TlcdReg_upline[i*4]<<24));
				TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4+16, TlcdReg_downline[i*4+3] + (TlcdReg_downline[i*4+2]<<8) + (TlcdReg_downline[i*4+1]<<16) + (TlcdReg_downline[i*4]<<24));
			}
	for(wait = 0; wait < 1200; wait++);
}


void TlcdGame()
{

	int i;
	int wait;

	sprintf(TlcdReg_upline, "Game Try :      ");
	WriteTLCDReg2(TlcdReg_downline);
			for(i = 0; i < 4; i++)
			{
				TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4, TlcdReg_upline[i*4+3] + (TlcdReg_upline[i*4+2]<<8) + (TlcdReg_upline[i*4+1]<<16) + (TlcdReg_upline[i*4]<<24));
				TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4+16, TlcdReg_downline[i*4+3] + (TlcdReg_downline[i*4+2]<<8) + (TlcdReg_downline[i*4+1]<<16) + (TlcdReg_downline[i*4]<<24));
			}
	for(wait = 0; wait < 1200; wait++);
}

void TlcdSituation()
{

	int i;
	int wait;

	sprintf(TlcdReg_upline, "STRIKE BALL OUT ");
	WriteTLCDReg(TlcdReg_downline, LCDReg);
			for(i = 0; i < 4; i++)
			{
				TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4, TlcdReg_upline[i*4+3] + (TlcdReg_upline[i*4+2]<<8) + (TlcdReg_upline[i*4+1]<<16) + (TlcdReg_upline[i*4]<<24));
				TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4+16, TlcdReg_downline[i*4+3] + (TlcdReg_downline[i*4+2]<<8) + (TlcdReg_downline[i*4+1]<<16) + (TlcdReg_downline[i*4]<<24));
			}
	for(wait = 0; wait < 1200; wait++);
}

void TlcdSuccess(int* ANum){
	int i;
	int wait;

	sprintf(TlcdReg_upline, "YOU SUCCEEDED   ");
	WriteTLCDReg4(TlcdReg_downline, ANum);
			for(i = 0; i < 4; i++)
			{
				TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4, TlcdReg_upline[i*4+3] + (TlcdReg_upline[i*4+2]<<8) + (TlcdReg_upline[i*4+1]<<16) + (TlcdReg_upline[i*4]<<24));
				TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4+16, TlcdReg_downline[i*4+3] + (TlcdReg_downline[i*4+2]<<8) + (TlcdReg_downline[i*4+1]<<16) + (TlcdReg_downline[i*4]<<24));
			}
	for(wait = 0; wait < 1200; wait++);
}

void TlcdFail(int* ANum){
	int i;
	int wait;

	sprintf(TlcdReg_upline, "YOU FAILED      ");
	WriteTLCDReg4(TlcdReg_downline, ANum);
			for(i = 0; i < 4; i++)
			{
				TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4, TlcdReg_upline[i*4+3] + (TlcdReg_upline[i*4+2]<<8) + (TlcdReg_upline[i*4+1]<<16) + (TlcdReg_upline[i*4]<<24));
				TEXTLCD_mWriteReg(XPAR_TEXTLCD_0_S00_AXI_BASEADDR, i*4+16, TlcdReg_downline[i*4+3] + (TlcdReg_downline[i*4+2]<<8) + (TlcdReg_downline[i*4+1]<<16) + (TlcdReg_downline[i*4]<<24));
			}
	for(wait = 0; wait < 1200; wait++);
}


