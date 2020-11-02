`timescale 1ns / 1ps

module tb_textlcd();

reg		[5:0]	i;
reg		[10:0]	j;

// textlcd module 입력
reg				resetn;		// FPGA reset
reg				lcdclk;		// clock for text-lcd
// textlcd module 출력
wire			lcd_rs;		// register select
wire			lcd_rw;		// read/write
wire			lcd_en;		// lcd enable
wire	[7:0]	lcd_data;	// lcd_data

// textlcd module
textlcd	textlcd_u0(	.resetn(resetn),
					.lcdclk(lcdclk),

					.lcd_rs(lcd_rs),
					.lcd_rw(lcd_rw),
					.lcd_en(lcd_en),
					.lcd_data(lcd_data)	);

// 20MHz clock
parameter	PERIOD_25MHZ	=	40;		// 40ns
always begin
	#(PERIOD_25MHZ/2);
	lcdclk <= ~lcdclk;
end

// testbench
initial begin
	i		=	6'd0;
	j		=	11'd0;
	resetn	=	1'b1;
	lcdclk	=	1'b0;
	#(PERIOD_25MHZ*5);
	resetn	=	1'b0;
	#(PERIOD_25MHZ*5);
	resetn	=	1'b1;

	for(i = 6'd0; i < 6'd40; i = i + 6'd1) begin
		for(j = 11'd0; j < 11'd2000; j = j + 11'd1) begin
			#(PERIOD_25MHZ);
		end
	end

	#(PERIOD_25MHZ*100);

	$stop;
end

endmodule
