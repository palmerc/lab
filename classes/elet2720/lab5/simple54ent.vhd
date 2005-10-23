-- Cameron L Palmer
-- ELET 2720 Section 305
-- Lab 5 - Simplified Figure 5-4

LIBRARY ieee;
USE ieee.std_logic_1164.all;

--------------------------------------------------

entity simple54ent is
port(
	A: in bit; B: in bit; C: in bit;
	Y: out bit
);
end simple54ent;

--------------------------------------------------

architecture simple54arc of simple54ent is
begin
Y <= (B and (A or C));
end simple54arc;
