-- Copyright (C) 1991-2013 Altera Corporation
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, Altera MegaCore Function License 
-- Agreement, or other applicable license agreement, including, 
-- without limitation, that your use is for the sole purpose of 
-- programming logic devices manufactured by Altera and sold by 
-- Altera or its authorized distributors.  Please refer to the 
-- applicable agreement for further details.

-- PROGRAM		"Quartus II 64-Bit"
-- VERSION		"Version 13.0.1 Build 232 06/12/2013 Service Pack 1 SJ Web Edition"
-- CREATED		"Tue Jan 26 22:19:13 2016"

LIBRARY ieee;
USE ieee.std_logic_1164.all; 

LIBRARY work;

ENTITY SystemTopLevel IS 
	PORT
	(
		CLOCK_50 :  IN  STD_LOGIC;
		GPS_RXD :  IN  STD_LOGIC;
		BlueTooth_RXD :  IN  STD_LOGIC;
		UART_RXD :  IN  STD_LOGIC;
		TouchScreen_RXD :  IN  STD_LOGIC;
		DRAM_DQ :  INOUT  STD_LOGIC_VECTOR(15 DOWNTO 0);
		KEY :  IN  STD_LOGIC_VECTOR(3 DOWNTO 0);
		LCD_DATA :  INOUT  STD_LOGIC_VECTOR(7 DOWNTO 0);
		SRam_DQ :  INOUT  STD_LOGIC_VECTOR(15 DOWNTO 0);
		SW :  IN  STD_LOGIC_VECTOR(17 DOWNTO 0);
		DRAM_CLK :  OUT  STD_LOGIC;
		DRAM_CKE :  OUT  STD_LOGIC;
		DRAM_BA_0 :  OUT  STD_LOGIC;
		DRAM_BA_1 :  OUT  STD_LOGIC;
		DRAM_CS_N :  OUT  STD_LOGIC;
		DRAM_CAS_N :  OUT  STD_LOGIC;
		DRAM_RAS_N :  OUT  STD_LOGIC;
		DRAM_WE_N :  OUT  STD_LOGIC;
		DRAM_UDQM :  OUT  STD_LOGIC;
		DRAM_LDQM :  OUT  STD_LOGIC;
		VGA_HS :  OUT  STD_LOGIC;
		VGA_VS :  OUT  STD_LOGIC;
		SRam_OE_N :  OUT  STD_LOGIC;
		SRam_CE_N :  OUT  STD_LOGIC;
		SRam_WE_N :  OUT  STD_LOGIC;
		SRam_UB_N :  OUT  STD_LOGIC;
		SRam_LB_N :  OUT  STD_LOGIC;
		VGA_BLANK :  OUT  STD_LOGIC;
		GPS_TXD :  OUT  STD_LOGIC;
		BlueTooth_TXD :  OUT  STD_LOGIC;
		UART_TXD :  OUT  STD_LOGIC;
		LCD_ON :  OUT  STD_LOGIC;
		LCD_BLON :  OUT  STD_LOGIC;
		LCD_EN :  OUT  STD_LOGIC;
		LCD_RS :  OUT  STD_LOGIC;
		LCD_RW :  OUT  STD_LOGIC;
		VideoDac_SYNC :  OUT  STD_LOGIC;
		VideoDac_CLK :  OUT  STD_LOGIC;
		TouchScreen_TXD :  OUT  STD_LOGIC;
		DRAM_ADDR :  OUT  STD_LOGIC_VECTOR(11 DOWNTO 0);
		HEX0 :  OUT  STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX1 :  OUT  STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX2 :  OUT  STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX3 :  OUT  STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX4 :  OUT  STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX5 :  OUT  STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX6 :  OUT  STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX7 :  OUT  STD_LOGIC_VECTOR(6 DOWNTO 0);
		LEDG :  OUT  STD_LOGIC_VECTOR(7 DOWNTO 0);
		LEDR :  OUT  STD_LOGIC_VECTOR(17 DOWNTO 0);
		SRAM_ADDR :  OUT  STD_LOGIC_VECTOR(17 DOWNTO 0);
		VGA_B :  OUT  STD_LOGIC_VECTOR(9 DOWNTO 0);
		VGA_G :  OUT  STD_LOGIC_VECTOR(9 DOWNTO 0);
		VGA_R :  OUT  STD_LOGIC_VECTOR(9 DOWNTO 0)
	);
END SystemTopLevel;

ARCHITECTURE bdf_type OF SystemTopLevel IS 

COMPONENT hexdisplays
	PORT(Hex0_1 : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		 Hex2_3 : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		 Hex4_5 : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		 Hex6_7 : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		 HEX0 : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		 HEX1 : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		 HEX2 : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		 HEX3 : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		 HEX4 : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		 HEX5 : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		 HEX6 : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		 HEX7 : OUT STD_LOGIC_VECTOR(6 DOWNTO 0)
	);
END COMPONENT;

COMPONENT nios_ii_system
	PORT(RESET : IN STD_LOGIC;
		 CLOCK_50Mhz : IN STD_LOGIC;
		 IO_acknowledge : IN STD_LOGIC;
		 IO_irq : IN STD_LOGIC;
		 STDIN : IN STD_LOGIC;
		 DRAM_DQ : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
		 IO_read_data : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
		 LCD_DATA : INOUT STD_LOGIC_VECTOR(7 DOWNTO 0);
		 Push_Buttons : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
		 Switches : IN STD_LOGIC_VECTOR(17 DOWNTO 0);
		 DRAM_CLK : OUT STD_LOGIC;
		 DRAM_CKE : OUT STD_LOGIC;
		 DRAM_BA_0 : OUT STD_LOGIC;
		 DRAM_BA_1 : OUT STD_LOGIC;
		 DRAM_CS_N : OUT STD_LOGIC;
		 DRAM_CAS_N : OUT STD_LOGIC;
		 DRAM_RAS_N : OUT STD_LOGIC;
		 DRAM_WE_N : OUT STD_LOGIC;
		 DRAM_UDQM : OUT STD_LOGIC;
		 DRAM_LDQM : OUT STD_LOGIC;
		 IO_bus_enable : OUT STD_LOGIC;
		 IO_rw : OUT STD_LOGIC;
		 STDOUT : OUT STD_LOGIC;
		 LCD_ON : OUT STD_LOGIC;
		 LCD_BLON : OUT STD_LOGIC;
		 LCD_EN : OUT STD_LOGIC;
		 LCD_RS : OUT STD_LOGIC;
		 LCD_RW : OUT STD_LOGIC;
		 DRAM_ADDR : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
		 Green_Leds : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
		 Hex0_1 : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
		 Hex2_3 : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
		 Hex4_5 : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
		 Hex6_7 : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
		 IO_address : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
		 IO_byte_enable : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
		 IO_write_data : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
		 Red_Leds : OUT STD_LOGIC_VECTOR(17 DOWNTO 0)
	);
END COMPONENT;

COMPONENT video_controller800x480
	PORT(Clock : IN STD_LOGIC;
		 Reset_L : IN STD_LOGIC;
		 IOEnable_L : IN STD_LOGIC;
		 WriteEnable_L : IN STD_LOGIC;
		 LowerByteSelect_L : IN STD_LOGIC;
		 UpperByteSelect_L : IN STD_LOGIC;
		 Clock_50Mhz : IN STD_LOGIC;
		 GraphicsCS_L : IN STD_LOGIC;
		 Address : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
		 DataIn : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
		 SRam_Data : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
		 horiz_sync_out : OUT STD_LOGIC;
		 vert_sync_out : OUT STD_LOGIC;
		 VideoBlanking_L : OUT STD_LOGIC;
		 SRam_OE_L : OUT STD_LOGIC;
		 SRam_CE_L : OUT STD_LOGIC;
		 SRam_WE_L : OUT STD_LOGIC;
		 SRam_LB_L : OUT STD_LOGIC;
		 SRam_UB_L : OUT STD_LOGIC;
		 BlueOut : OUT STD_LOGIC_VECTOR(9 DOWNTO 0);
		 DataOut : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
		 GreenOut : OUT STD_LOGIC_VECTOR(9 DOWNTO 0);
		 RedOut : OUT STD_LOGIC_VECTOR(9 DOWNTO 0);
		 SRamAddress : OUT STD_LOGIC_VECTOR(17 DOWNTO 0)
	);
END COMPONENT;

COMPONENT onchipm68xxio
	PORT(Clock_50Mhz : IN STD_LOGIC;
		 IOSelect_H : IN STD_LOGIC;
		 ByteSelect_L : IN STD_LOGIC;
		 WE_L : IN STD_LOGIC;
		 Reset_L : IN STD_LOGIC;
		 RS232_RxData : IN STD_LOGIC;
		 GPS_RxData : IN STD_LOGIC;
		 BlueTooth_RxData : IN STD_LOGIC;
		 TouchScreen_RxData : IN STD_LOGIC;
		 Address : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
		 DataIn : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		 RS232_TxData : OUT STD_LOGIC;
		 ACIA_IRQ : OUT STD_LOGIC;
		 GPS_TxData : OUT STD_LOGIC;
		 BlueTooth_TxData : OUT STD_LOGIC;
		 TouchScreen_TxData : OUT STD_LOGIC;
		 DataOut : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END COMPONENT;

SIGNAL	Clock25Mhz :  STD_LOGIC;
SIGNAL	IO_byte_enable :  STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL	IO_read_data :  STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL	IO_write_data :  STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_0 :  STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_1 :  STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_2 :  STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_3 :  STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_4 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_5 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_6 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_7 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_22 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_9 :  STD_LOGIC;
SIGNAL	DFF_inst14 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_11 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_23 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_24 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_14 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_15 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_25 :  STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_19 :  STD_LOGIC;


BEGIN 
VideoDac_SYNC <= '0';
SYNTHESIZED_WIRE_4 <= '1';
SYNTHESIZED_WIRE_6 <= '1';
SYNTHESIZED_WIRE_7 <= '1';
SYNTHESIZED_WIRE_9 <= '1';



b2v_inst : hexdisplays
PORT MAP(Hex0_1 => SYNTHESIZED_WIRE_0,
		 Hex2_3 => SYNTHESIZED_WIRE_1,
		 Hex4_5 => SYNTHESIZED_WIRE_2,
		 Hex6_7 => SYNTHESIZED_WIRE_3,
		 HEX0 => HEX0,
		 HEX1 => HEX1,
		 HEX2 => HEX2,
		 HEX3 => HEX3,
		 HEX4 => HEX4,
		 HEX5 => HEX5,
		 HEX6 => HEX6,
		 HEX7 => HEX7);


PROCESS(CLOCK_50,SYNTHESIZED_WIRE_4,SYNTHESIZED_WIRE_6)
BEGIN
IF (SYNTHESIZED_WIRE_4 = '0') THEN
	Clock25Mhz <= '0';
ELSIF (SYNTHESIZED_WIRE_6 = '0') THEN
	Clock25Mhz <= '1';
ELSIF (RISING_EDGE(CLOCK_50)) THEN
	Clock25Mhz <= SYNTHESIZED_WIRE_5;
END IF;
END PROCESS;


PROCESS(CLOCK_50,SYNTHESIZED_WIRE_7,SYNTHESIZED_WIRE_9)
BEGIN
IF (SYNTHESIZED_WIRE_7 = '0') THEN
	DFF_inst14 <= '0';
ELSIF (SYNTHESIZED_WIRE_9 = '0') THEN
	DFF_inst14 <= '1';
ELSIF (RISING_EDGE(CLOCK_50)) THEN
	DFF_inst14 <= SYNTHESIZED_WIRE_22;
END IF;
END PROCESS;







SYNTHESIZED_WIRE_23 <= NOT(SYNTHESIZED_WIRE_22);



SYNTHESIZED_WIRE_14 <= NOT(IO_byte_enable(0));



b2v_inst3 : nios_ii_system
PORT MAP(RESET => KEY(0),
		 CLOCK_50Mhz => CLOCK_50,
		 IO_acknowledge => DFF_inst14,
		 IO_irq => SYNTHESIZED_WIRE_11,
		 DRAM_DQ => DRAM_DQ,
		 IO_read_data => IO_read_data,
		 LCD_DATA => LCD_DATA,
		 Push_Buttons => KEY(3 DOWNTO 1),
		 Switches => SW,
		 DRAM_CLK => DRAM_CLK,
		 DRAM_CKE => DRAM_CKE,
		 DRAM_BA_0 => DRAM_BA_0,
		 DRAM_BA_1 => DRAM_BA_1,
		 DRAM_CS_N => DRAM_CS_N,
		 DRAM_CAS_N => DRAM_CAS_N,
		 DRAM_RAS_N => DRAM_RAS_N,
		 DRAM_WE_N => DRAM_WE_N,
		 DRAM_UDQM => DRAM_UDQM,
		 DRAM_LDQM => DRAM_LDQM,
		 IO_bus_enable => SYNTHESIZED_WIRE_22,
		 IO_rw => SYNTHESIZED_WIRE_24,
		 LCD_ON => LCD_ON,
		 LCD_BLON => LCD_BLON,
		 LCD_EN => LCD_EN,
		 LCD_RS => LCD_RS,
		 LCD_RW => LCD_RW,
		 DRAM_ADDR => DRAM_ADDR,
		 Green_Leds => LEDG,
		 Hex0_1 => SYNTHESIZED_WIRE_0,
		 Hex2_3 => SYNTHESIZED_WIRE_1,
		 Hex4_5 => SYNTHESIZED_WIRE_2,
		 Hex6_7 => SYNTHESIZED_WIRE_3,
		 IO_address => SYNTHESIZED_WIRE_25,
		 IO_byte_enable => IO_byte_enable,
		 IO_write_data => IO_write_data,
		 Red_Leds => LEDR);


SYNTHESIZED_WIRE_15 <= NOT(IO_byte_enable(1));



SYNTHESIZED_WIRE_19 <= NOT(IO_byte_enable(0));



b2v_inst4 : video_controller800x480
PORT MAP(Clock => Clock25Mhz,
		 Reset_L => KEY(0),
		 IOEnable_L => SYNTHESIZED_WIRE_23,
		 WriteEnable_L => SYNTHESIZED_WIRE_24,
		 LowerByteSelect_L => SYNTHESIZED_WIRE_14,
		 UpperByteSelect_L => SYNTHESIZED_WIRE_15,
		 Clock_50Mhz => CLOCK_50,
		 GraphicsCS_L => SYNTHESIZED_WIRE_23,
		 Address => SYNTHESIZED_WIRE_25,
		 DataIn => IO_write_data,
		 SRam_Data => SRam_DQ,
		 horiz_sync_out => VGA_HS,
		 vert_sync_out => VGA_VS,
		 VideoBlanking_L => VGA_BLANK,
		 SRam_OE_L => SRam_OE_N,
		 SRam_CE_L => SRam_CE_N,
		 SRam_WE_L => SRam_WE_N,
		 SRam_LB_L => SRam_LB_N,
		 SRam_UB_L => SRam_UB_N,
		 BlueOut => VGA_B,
		 DataOut => IO_read_data,
		 GreenOut => VGA_G,
		 RedOut => VGA_R,
		 SRamAddress => SRAM_ADDR);


b2v_inst5 : onchipm68xxio
PORT MAP(Clock_50Mhz => CLOCK_50,
		 IOSelect_H => SYNTHESIZED_WIRE_22,
		 ByteSelect_L => SYNTHESIZED_WIRE_19,
		 WE_L => SYNTHESIZED_WIRE_24,
		 Reset_L => KEY(0),
		 RS232_RxData => UART_RXD,
		 GPS_RxData => GPS_RXD,
		 BlueTooth_RxData => BlueTooth_RXD,
		 TouchScreen_RxData => TouchScreen_RXD,
		 Address => SYNTHESIZED_WIRE_25,
		 DataIn => IO_write_data(7 DOWNTO 0),
		 RS232_TxData => UART_TXD,
		 ACIA_IRQ => SYNTHESIZED_WIRE_11,
		 GPS_TxData => GPS_TXD,
		 BlueTooth_TxData => BlueTooth_TXD,
		 TouchScreen_TxData => TouchScreen_TXD,
		 DataOut => IO_read_data(7 DOWNTO 0));


SYNTHESIZED_WIRE_5 <= NOT(Clock25Mhz);


VideoDac_CLK <= Clock25Mhz;

END bdf_type;