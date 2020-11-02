`timescale 1ns / 1ps

module testbench_CLA();


integer			fid	=	0;

reg		[15:0]	x;
reg		[15:0]	y;
wire	[15:0]	sum;
wire			carry;
reg				cin;
reg				clk;

wire	[16:0]	solution;
genvar idx;
genvar idy;

cla_16bits
cla_16bits_U0
(
 	.x		(	x		),
	.y		(	y		),
	.c_in	(	cin		),

	.s		(	sum		),
	.c_out	(	carry	)
);

initial begin
	fid = $fopen("result.txt");
	cin = 0;
	clk = 0;
	x = 16'd65535;
	y = 16'd3;
	#3
	x = 0;
	y = 0;
end

always begin
	#5 clk = ~clk;
end

always @(posedge clk)
begin
	if(x < 16'd65535) begin
		if(y < 16'd65535) begin
			y <= y + 1'b1;
		end
		else begin
			y <= 16'd0;
			x <= x + 1'b1;
		end
	end
	else begin
		x <= 16'd65535;
		y <= 16'd65535;
	end
end

assign	solution	=	{1'b0, x} + {1'b0, y};

always @(posedge clk)
begin
	#5;
	$fwrite(fid, "X=%05d\tY=%05d\t\tSum=%05d\tCout=%b\t(Solution\t:\tSum=%02d\tCout=%b)", x,y,sum,carry,solution[15:0],solution[16]);
	if( (sum != solution[15:0]) || (carry != solution[16]) ) begin
		$fwrite(fid, "\t\tERROR\n");
	end
	else begin
		$fwrite(fid, "\n");
	end
end

endmodule
