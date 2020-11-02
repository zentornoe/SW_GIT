
`define LINE_CHAR_NUM	56    // Characters

module CHAR_DISPLAY (
  input [6:0] char_column,   // character number on the current line
  input [6:0] char_line,     // line number on the screen
  input [2:0] subchar_line,  // the line number within a character block 0-8
  input [2:0] subchar_pixel, // the pixel number within a character block 0-8
  input pixel_clock,
  input reset,
  output vga_red_data,
  output vga_green_data,
  output vga_blue_data
);

//// Label Definitions ////

wire [0:8*`LINE_CHAR_NUM-1] text_line[0:32];

assign text_line[0][0:8*`LINE_CHAR_NUM-1] =  "This Project Created by                                 ";
assign text_line[1][0:8*`LINE_CHAR_NUM-1] =  "                                                        ";
assign text_line[2][0:8*`LINE_CHAR_NUM-1] =  "2020 Spring Semester : SoC Term Project - Group 2       ";
assign text_line[3][0:8*`LINE_CHAR_NUM-1] =  "                                                        ";
assign text_line[4][0:8*`LINE_CHAR_NUM-1] =  "                              2017110244   KIM HEESOO   ";
assign text_line[5][0:8*`LINE_CHAR_NUM-1] =  "                                                        ";
assign text_line[6][0:8*`LINE_CHAR_NUM-1] =  "                              2017110379   GO  MIJIN    "; 
assign text_line[7][0:8*`LINE_CHAR_NUM-1] =  "                                                        ";
assign text_line[8][0:8*`LINE_CHAR_NUM-1] =  "                              2016110146   SON DAMIN    ";
assign text_line[9][0:8*`LINE_CHAR_NUM-1] =  "                                                        ";
assign text_line[10][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[11][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[12][0:8*`LINE_CHAR_NUM-1] = "                < NUMBER BASEBALL GAME >                "; 
assign text_line[13][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[14][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[15][0:8*`LINE_CHAR_NUM-1] = " 1. Press BUTTON 0 to start the game                    ";
assign text_line[16][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[17][0:8*`LINE_CHAR_NUM-1] = " 2. You have 3 CHANCES to guess the number              ";
assign text_line[18][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[19][0:8*`LINE_CHAR_NUM-1] = "   * STRIKE : if the number exists and digit is correct ";
assign text_line[20][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[21][0:8*`LINE_CHAR_NUM-1] = "   * BALL   : if the number exists, but digit is wrong  ";
assign text_line[22][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[23][0:8*`LINE_CHAR_NUM-1] = "   * OUT    : No Strike and No Ball               ";
assign text_line[24][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[25][0:8*`LINE_CHAR_NUM-1] = " 3. You have to decide the number in 1 MINUTE           ";
assign text_line[26][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[27][0:8*`LINE_CHAR_NUM-1] = " 4. If you guess correctly, the light will twinkle      ";
assign text_line[28][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[29][0:8*`LINE_CHAR_NUM-1] = " 5. Press the BUTTON 0 to play again                    ";
assign text_line[30][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[31][0:8*`LINE_CHAR_NUM-1] = "                                                        ";
assign text_line[32][0:8*`LINE_CHAR_NUM-1] = "                                                        ";


wire [13:0] char_addr = {char_line[6:0], char_column[6:0]};
wire write_enable;          // character memory is written to on a clock rise when high
wire pixel_on;              // high => output foreground color, low => output background color
reg [7:0] char_write_data;  // the data that will be written to character memory at the clock rise

wire direction;
wire [6:0] inter_char_line;
wire [6:0] inter_char_column;

integer i; // iterator

// always enable writing to character RAM
assign write_enable = 1;

assign direction = 1'b1;

assign inter_char_line = (direction) ? (7'h20 - char_line[6:0]) : char_line[6:0];
assign inter_char_column = (direction) ? (`LINE_CHAR_NUM - char_column[6:0]) : char_column[6:0];

// write the appropriate character data to memory
always @ (inter_char_line or inter_char_column) begin
// insert a space by default
  char_write_data <= 8'h20;
  for (i = 0; i < `LINE_CHAR_NUM; i = i + 1) begin
    if (inter_char_column == i)
      char_write_data <= text_line[inter_char_line][i*8+:8];
  end
end

reg background_red;    // the red component of the background color
reg background_green;  // the green component of the background color
reg background_blue;   // the blue component of the background color
reg foreground_red;    // the red component of the foreground color
reg foreground_green;  // the green component of the foreground color
reg foreground_blue;   // the blue component of the foreground color

// use the result of the character generator module to choose between the foreground and background color
assign vga_red_data = (pixel_on) ? foreground_red : background_red;
assign vga_green_data = (pixel_on) ? foreground_green : background_green;
assign vga_blue_data = (pixel_on) ? foreground_blue : background_blue;

// select the appropriate character colors
always @ (inter_char_line or inter_char_column) begin
// always use a black background with white text
  if ((inter_char_line == 7'h00) && ((inter_char_column >= 26) && (inter_char_column <= 34)))
  begin
    background_red <= 1'b0;
    background_green <= 1'b0;
    background_blue <= 1'b0;
    foreground_red <= 1'b0;
    foreground_green <= 1'b0;
    foreground_blue <= 1'b1;
  end
  else if (((inter_char_line == 7'h00) || (inter_char_line == 7'h05) || (inter_char_line == 7'h1C)) &&
    ((inter_char_column >= 3) && (inter_char_column <= 11))) begin
    background_red <= 1'b0;
    background_green <= 1'b0;
    background_blue <= 1'b0;
    foreground_red <= 1'b0;
    foreground_green <= 1'b1;
    foreground_blue <= 1'b0;
  end
  else begin
    background_red <= 1'b0;
    background_green <= 1'b0;
    background_blue <= 1'b0;
    foreground_red <= 1'b1;
    foreground_green <= 1'b1;
    foreground_blue <= 1'b1;
  end
end

// the character generator block includes the character RAM
// and the character generator ROM
CHAR_GEN CHAR_GEN(
  .pixel_clock(pixel_clock),
  .reset(reset),
  .subchar_line(subchar_line),  // line number within 8 line block
  .char_address(char_addr), // character address "0" is upper left character
  .subchar_pixel(subchar_pixel), // pixel position within 8 pixel block
  .char_write_addr(char_addr),
  .char_write_data(char_write_data),
  .char_write_enable(write_enable),   // pixel position within 8 pixel block
  .char_write_clock(pixel_clock),
  .pixel_on(pixel_on)
);

endmodule //CHAR_DISPLAY
