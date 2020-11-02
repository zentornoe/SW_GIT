module clk_divider(
input	wire			resetn,
input	wire			clk,

output	reg		[3:0]	cnt_sec1,
output	reg		[3:0]	cnt_sec10,
output	reg		[3:0]	cnt_min1,
output	reg		[3:0]	cnt_min10,
output	reg		[3:0]	cnt_hour1,
output	reg		[3:0]	cnt_hour10
);

reg		[23:0]	cnt_onesec;
reg				clk_onesec;


always @(posedge clk or negedge resetn)
begin
	if(!resetn) begin
		cnt_onesec <= 24'd0;
	end
	else begin
		if(cnt_onesec < 24'd12499999) begin
			cnt_onesec <= cnt_onesec + 24'd1;
		end
		else begin
			cnt_onesec <= 24'd0;
		end
	end
end

always @(posedge clk or negedge resetn)
begin
	if(!resetn) begin
		clk_onesec <= 1'b1;
	end
	else begin
		if(cnt_onesec == 24'd12499999) begin
			clk_onesec <= ~clk_onesec;
		end
		else begin
			clk_onesec <= clk_onesec;
		end
	end
end


always @(posedge clk_onesec or negedge resetn)
begin
	if(!resetn) begin
		cnt_sec1 <= 4'd9;
	end
	else begin
		if(cnt_sec1 < 4'd9) begin
			cnt_sec1 <= cnt_sec1 + 4'd1;
		end
		else begin
			cnt_sec1 <= 4'd0;
		end
	end
end

always @(posedge clk_onesec or negedge resetn)
begin
	if(!resetn) begin
		cnt_sec10 <= 4'd5;
	end
	else begin
		if(cnt_sec1 == 4'd9) begin
			if(cnt_sec10 < 4'd5) begin
				cnt_sec10 <= cnt_sec10 + 4'd1;
			end
			else begin
				cnt_sec10 <= 4'd0;
			end
		end
		else begin
			cnt_sec10 <= cnt_sec10;
		end
	end
end

always @(posedge clk_onesec or negedge resetn)
begin
	if(!resetn) begin
		cnt_min1 <= 4'd9;
	end
	else begin
		if({cnt_sec10, cnt_sec1} == 8'h59) begin
			if(cnt_min1 < 4'd9) begin
				cnt_min1 <= cnt_min1 + 4'd1;
			end
			else begin
				cnt_min1 <= 4'd0;
			end
		end
		else begin
			cnt_min1 <= cnt_min1;
		end
	end
end

always @(posedge clk_onesec or negedge resetn)
begin
	if(!resetn) begin
		cnt_min10 <= 4'd5;
	end
	else begin
		if( (cnt_min1 == 4'h9) && ({cnt_sec10, cnt_sec1} == 8'h59) ) begin
			if(cnt_min10 < 4'd5) begin
				cnt_min10 <= cnt_min10 + 4'd1;
			end
			else begin
				cnt_min10 <= 4'd0;
			end
		end
		else begin
			cnt_min10 <= cnt_min10;
		end
	end
end

always @(posedge clk_onesec or negedge resetn)
begin
	if(!resetn) begin
		cnt_hour1 <= 4'd3;
	end
	else begin
		if( ({cnt_min10, cnt_min1} == 8'h59) && ({cnt_sec10, cnt_sec1} == 8'h59) ) begin
			if( (cnt_hour1 < 4'd9) && ({cnt_hour10, cnt_hour1} != 8'h23) ) begin
				cnt_hour1 <= cnt_hour1 + 4'd1;
			end
			else begin
				cnt_hour1 <= 4'd0;
			end
		end
		else begin
			cnt_hour1 <= cnt_hour1;
		end
	end
end

always @(posedge clk_onesec or negedge resetn)
begin
	if(!resetn) begin
		cnt_hour10 <= 4'd2;
	end
	else begin
		if( (({cnt_hour10, cnt_hour1} == 8'h23) || (cnt_hour1 == 4'h9)) && ({cnt_min10, cnt_min1} == 8'h59) && ({cnt_sec10, cnt_sec1} == 8'h59) ) begin
			if(cnt_hour10 < 4'd2) begin
				cnt_hour10 <= cnt_hour10 + 4'd1;
			end
			else begin
				cnt_hour10 <= 4'd0;
			end
		end
		else begin
			cnt_hour10 <= cnt_hour10;
		end
	end
end


endmodule

