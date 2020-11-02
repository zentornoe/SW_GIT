`timescale 1ns / 1ns

module tb_top();

// top module 입력
reg				resetn;		// FPGA reset
reg				clk;		// clock
// top module 출력
wire	[7:0]	seg_en;		// 7-segment enable
wire	[7:0]	seg_data;	// 7-segment data
wire			lcd_rs;		// Text LCD register select
wire			lcd_rw;		// Text LCD read/write
wire			lcd_en;		// Text LCD enable
wire	[7:0]	lcd_data;	// Text LCD data

// top module
top	top_u0(	.resetn(resetn),
			.clk(clk),

			.seg_en(seg_en),
			.seg_data(seg_data),
			.lcd_rs(lcd_rs),
			.lcd_rw(lcd_rw),
			.lcd_en(lcd_en),
			.lcd_data(lcd_data)	);

// 20Mhz clock
parameter	PERIOD_25MHZ	=	40;
always begin
	#(PERIOD_25MHZ/2);
	clk <= ~clk;
end

// testbench
initial begin
	clk		=	1'b0;
	resetn	=	1'b1;
	#(PERIOD_25MHZ*5);
	resetn	=	1'b0;
	#(PERIOD_25MHZ*5);
	resetn	=	1'b1;

	#(PERIOD_25MHZ*50000000);
	$stop;
end

endmodule

