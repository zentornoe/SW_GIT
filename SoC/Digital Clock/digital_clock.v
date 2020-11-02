module digital_clock(
input	wire			resetn,
input	wire			clk,		// 25MHz

output	wire	[31:0]	segdata,
output	wire	[31:0]	textdata_a,
output	wire	[31:0]	textdata_b,
output	wire	[31:0]	textdata_c,
output	wire	[31:0]	textdata_d,
output	wire	[31:0]	textdata_e,
output	wire	[31:0]	textdata_f,
output	wire	[31:0]	textdata_g,
output	wire	[31:0]	textdata_h
);

wire	[3:0]	cnt_sec1;
wire	[3:0]	cnt_sec10;
wire	[3:0]	cnt_min1;
wire	[3:0]	cnt_min10;
wire	[3:0]	cnt_hour1;
wire	[3:0]	cnt_hour10;

assign segdata = { cnt_hour10, cnt_hour1, 4'hA, cnt_min10, cnt_min1, 4'hA, cnt_sec10, cnt_sec1 };					// hh-mm-ss

assign textdata_a =  32'h44_69_67_69;				// Digi
assign textdata_b =  32'h74_61_6C_20;				// tal
assign textdata_c =  32'h43_6C_6F_63;				// Cloc
assign textdata_d =  32'h6B_20_20_20;				// k
assign textdata_e =  {4'h3, cnt_hour10, 4'h3, cnt_hour1, 8'h3A, 4'h3, cnt_min10};   		// hh:m
assign textdata_f =  {4'h3, cnt_min1, 8'h3A, 4'h3, cnt_sec10, 4'h3, cnt_sec1};				// m:ss
assign textdata_g =  32'h20_20_20_20;
assign textdata_h =  32'h20_20_20_20;


clk_divider	clk_divider_u0(	.resetn(resetn),
							.clk(clk),

							.cnt_sec1(cnt_sec1),
							.cnt_sec10(cnt_sec10),
							.cnt_min1(cnt_min1),
							.cnt_min10(cnt_min10),
							.cnt_hour1(cnt_hour1),
							.cnt_hour10(cnt_hour10)	);

endmodule

