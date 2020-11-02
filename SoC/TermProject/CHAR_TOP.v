`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2014/08/13 10:48:39
// Design Name: 
// Module Name: CHAR_TOP
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module CHAR_TOP (
  // Example Port
  input SYSTEM_CLOCK,    // 100MHz LVTTL SYSTEM CLOCK
  input nSYSPOR,
  output VGA_HSYNCH,     // horizontal sync for the VGA output connector
  output VGA_VSYNCH,     // vertical sync for the VGA output connector
  output [7:3] LCD_R,    // RED DAC data
  output [7:2] LCD_G,    // GREEN DAC data
  output [7:3] LCD_B,    // BLUE DAC data
  output VGA_CLK,
  output VGA_EN,         // VGA Enable
  output lcd_blk_on
);

//wire VGA_HSYNCH;     // horizontal sync for the VGA output connector
//wire VGA_VSYNCH;     // vertical sync for the VGA output connector

//wire system_clock_buffered;  // buffered SYSTEM CLOCK
reg pixel_clock;            // generated from SYSTEM CLOCK      *
wire reset;                  // reset asserted when DCMs are NOT LOCKED     *

wire vga_red_data;           // red video data      *
wire vga_green_data;         // green video data    *
wire vga_blue_data;          // blue video data     *

// internal video timing signals
wire h_synch; // horizontal synch for VGA connector     *
wire v_synch; // vertical synch for VGA connector       *
wire blank;   // composite blanking         *
wire [10:0] pixel_count;    // bit mapped pixel position within the line
wire [9:0] line_count;      // bit mapped line number in a frame lines within the frame
wire [2:0] subchar_pixel;   // pixel position within the character      *
wire [2:0] subchar_line;    // identifies the line number within a character block      *
wire [6:0] char_column;     // character number on the current line     *
wire [6:0] char_line;       // line number on the screen        *

wire TEMP_HSYNCH;
wire TEMP_VSYNCH;
//wire temp_reset;

wire VGA_OUT_RED;
wire VGA_OUT_GREEN;
wire VGA_OUT_BLUE;

assign reset = ~nSYSPOR;

assign LCD_R[7:3] = (VGA_OUT_RED) ? 5'b11111 : 0;
assign LCD_G[7:2] = (VGA_OUT_GREEN) ? 6'b111111 : 0;
assign LCD_B[7:3] = (VGA_OUT_BLUE) ? 5'b11111 : 0;


// instantiate the character generator
CHAR_DISPLAY CHAR_DISPLAY (
  .char_column(char_column),     // character number on the current line
  .char_line(char_line),         // line number on the screen
  .subchar_line(subchar_line),   // the line number within a character block 0-8
  .subchar_pixel(subchar_pixel), // the pixel number within a character block 0-8
  .pixel_clock(pixel_clock),
  .reset(reset),
  .vga_red_data(vga_red_data),
  .vga_green_data(vga_green_data),
  .vga_blue_data(vga_blue_data)
);

// instantiate the video timing generator
SVGA_TIMING_GENERATION SVGA_TIMING_GENERATION(
  .pixel_clock(pixel_clock),     // pixel clock
  .reset(reset),                 // reset
  .h_synch(h_synch),             // horizontal synch for VGA connector
  .v_synch(v_synch),             // vertical synch for VGA connector
  .blank(blank),                 // composite blanking
  .pixel_count(pixel_count),     // counts the pixels in a line
  .line_count(line_count),       // counts the display lines
  .subchar_pixel(subchar_pixel), // pixel position within the character
  .subchar_line(subchar_line),   // identifies the line number within a character block
  .char_column(char_column),     // character number on the current line
  .char_line(char_line)          // line number on the screen
);

// instantiate the video output mux
VIDEO_OUT VIDEO_OUT (
  .pixel_clock(pixel_clock),
  .reset(reset),
  .vga_red_data(vga_red_data),
  .vga_green_data(vga_green_data),
  .vga_blue_data(vga_blue_data),
  .h_synch(h_synch),
  .v_synch(v_synch),
  .blank(blank),
  .VGA_HSYNCH(TEMP_HSYNCH),
  .VGA_VSYNCH(TEMP_VSYNCH),
  .VGA_OUT_RED(VGA_OUT_RED),
  .VGA_OUT_GREEN(VGA_OUT_GREEN),
  .VGA_OUT_BLUE(VGA_OUT_BLUE)
);

always @ (posedge SYSTEM_CLOCK or negedge nSYSPOR)
begin
  if (!nSYSPOR)
    pixel_clock = 1'b0;
  else
    pixel_clock = ~pixel_clock;
end

assign VGA_HSYNCH = ~TEMP_HSYNCH;
assign VGA_VSYNCH = ~TEMP_VSYNCH;
assign VGA_EN = 1'b1;
assign VGA_CLK = ~pixel_clock;
assign lcd_blk_on = 1'b1;

endmodule // MAIN
