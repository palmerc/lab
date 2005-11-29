-- Cameron L Palmer
-- ELET 2720 Section 305
-- November 25, 2005
-- Final Project - BCD to Seven Segment Display w/ Hex Support

LIBRARY ieee;
USE ieee.std_logic_1164.all;

--------------------------------------------------

entity bcd2seven is
port(
	BCD1,BCD2,BCD4,BCD8: in bit;
	A,B,C,D,E,F,G: out bit
);
end bcd2seven;

--------------------------------------------------

architecture bcd2sevenarc of bcd2seven is
begin
A <= not((BCD8 or BCD4 or BCD2 or not BCD1) and 
	(BCD8 or not BCD4 or BCD2 or BCD1) and 
	(not BCD8 or not BCD4 or BCD2 or not BCD1) and 
	(not BCD8 or BCD4 or not BCD2 or not BCD1));
B <= not ((not BCD8 or not BCD4 or not BCD2) and 
	(not BCD8 or not BCD4 or BCD1) and 
	(not BCD8 or not BCD2 or not BCD1) and 
	(not BCD4 or not BCD2 or BCD1) and 
	(BCD8 or not BCD4 or BCD2 or not BCD1));
C <= not ((BCD8 or BCD4 or not BCD2 or BCD1) and 
	(not BCD8 or not BCD4 or BCD1) and 
	(not BCD8 or not BCD4 or not BCD2));
D <= not ((BCD8 or not BCD4 or BCD2 or BCD1) and 
	(BCD8 or BCD4 or BCD2 or not BCD1) and 
	(not BCD4 or not BCD2 or not BCD1) and 
	(not BCD8 or BCD4 or not BCD2 or BCD1));
E <= not ((BCD8 or not BCD4 or BCD2 or BCD1) and 
	(BCD4 or BCD2 or not BCD1) and 
	(BCD8 or not BCD1));
F <= not ((not BCD8 or not BCD4 or BCD2 or not BCD1) and 
	(BCD8 or BCD4 or not BCD1) and 
	(BCD8 or not BCD2 or not BCD1) and 
	(BCD8 or BCD4 or not BCD2));
G <= not ((BCD8 or BCD4 or BCD2) and 
	(not BCD8 or not BCD4 or BCD2 or BCD1) and 
	(BCD8 or not BCD4 or not BCD2 or not BCD1));
end bcd2sevenarc;
