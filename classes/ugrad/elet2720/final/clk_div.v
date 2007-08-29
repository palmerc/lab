//
// by Baihong Fang
// Copyright (c) 2002
// Department of Electrical and Computer Engineering
// McMaster University
//

// clock divider
module clk_div (clock_25MHz, clock_1MHz, clock_100KHz, clock_10KHz, clock_1KHz,
		clock_100Hz, clock_10Hz, clock_1Hz);
	input	clock_25MHz;
	output	clock_1MHz, clock_100KHz, clock_10KHz, clock_1KHz;
	output	clock_100Hz, clock_10Hz, clock_1Hz;
	reg		clock_1MHz, clock_100KHz, clock_10KHz, clock_1KHz;
	reg		clock_100Hz, clock_10Hz, clock_1Hz;
	
	reg [4:0] count_1MHz;
	reg [2:0] count_100KHz, count_10KHz, count_1KHz;
	reg [2:0] count_100Hz, count_10Hz, count_1Hz;
	
	// Generate 1Mhz clock
	always @ (posedge clock_25MHz) begin
		if (count_1MHz < 5'b 11000) count_1MHz <= count_1MHz + 1'b 1;
		else count_1MHz <= 5'b 00000;
	
		if (count_1MHz < 5'h 0C) clock_1MHz <= 1'b 0;
		else clock_1MHz <= 1'b 1;
	end
	
	// Generate 100Khz clock
	always @ (posedge clock_1MHz) begin
		if (count_100KHz == 3'b 100) begin
			clock_100KHz <= !clock_100KHz;
			count_100KHz <= 3'b 000;
		end
		else count_100KHz <= count_100KHz + 1'b 1;
	end
	
	// Generate 10Khz clock
	always @ (posedge clock_100KHz) begin
		if (count_10KHz == 3'b 100) begin
			clock_10KHz <= !clock_10KHz;
			count_10KHz <= 3'b 000;
		end
		else count_10KHz <= count_10KHz + 1'b 1;
	end
	
	// Generate 1Khz clock
	always @ (posedge clock_10KHz) begin
		if (count_1KHz == 3'b 100) begin
			clock_1KHz <= !clock_1KHz;
			count_1KHz <= 3'b 000;
		end
		else count_1KHz <= count_1KHz + 1'b 1;
	end
	
	// Generate 100Hz clock
	always @ (posedge clock_1KHz) begin
		if (count_100Hz == 3'b 100) begin
			clock_100Hz <= !clock_100Hz;
			count_100Hz <= 3'b 000;
		end
		else count_100Hz <= count_100Hz + 1'b 1;
	end
	
	// Generate 10Hz clock
	always @ (posedge clock_100Hz) begin
		if (count_10Hz == 3'b 100) begin
			clock_10Hz <= !clock_10Hz;
			count_10Hz <= 3'b 000;
		end
		else count_10Hz <= count_10Hz + 1'b 1;
	end
	
	// Generate 1Hz clock
	always @ (posedge clock_10Hz) begin
		if (count_1Hz == 3'b 100) begin
			clock_1Hz <= !clock_1Hz;
			count_1Hz <= 3'b 000;
		end
		else count_1Hz <= count_1Hz + 1'b 1;
	end
	
endmodule	// End module

