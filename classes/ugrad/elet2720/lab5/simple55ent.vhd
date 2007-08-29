-- Cameron L Palmer
-- ELET 2720 Section 305
-- Lab 5 - Simplified Figure 5-5

LIBRARY ieee;
USE ieee.std_logic_1164.all;

--------------------------------------------------

entity simple55ent is
port(
	A: in bit; B: in bit; C: in bit;
	Y: out bit
);
end simple55ent;

--------------------------------------------------

architecture simple55arc of simple55ent is
begin
Y <= A and (B nand C);
end simple55arc;
