-- r-VEX processor
-- Copyright (C) 2008-2012 by TU Delft.
-- All Rights Reserved.

-- THIS IS A LEGAL DOCUMENT, BY USING r-VEX,
-- YOU ARE AGREEING TO THESE TERMS AND CONDITIONS.

-- No portion of this work may be used by any commercial entity, or for any
-- commercial purpose, without the prior, written permission of TU Delft.
-- Nonprofit and noncommercial use is permitted as described below.

-- 1. r-VEX is provided AS IS, with no warranty of any kind, express
-- or implied. The user of the code accepts full responsibility for the
-- application of the code and the use of any results.

-- 2. Nonprofit and noncommercial use is encouraged. r-VEX may be
-- downloaded, compiled, synthesized, copied, and modified solely for nonprofit,
-- educational, noncommercial research, and noncommercial scholarship
-- purposes provided that this notice in its entirety accompanies all copies.
-- Copies of the modified software can be delivered to persons who use it
-- solely for nonprofit, educational, noncommercial research, and
-- noncommercial scholarship purposes provided that this notice in its
-- entirety accompanies all copies.

-- 3. ALL COMMERCIAL USE, AND ALL USE BY FOR PROFIT ENTITIES, IS EXPRESSLY
-- PROHIBITED WITHOUT A LICENSE FROM TU Delft (J.S.S.M.Wong@tudelft.nl).

-- 4. No nonprofit user may place any restrictions on the use of this software,
-- including as modified by the user, by any other authorized user.

-- 5. Noncommercial and nonprofit users may distribute copies of r-VEX
-- in compiled or binary form as set forth in Section 2, provided that
-- either: (A) it is accompanied by the corresponding machine-readable source
-- code, or (B) it is accompanied by a written offer, with no time limit, to
-- give anyone a machine-readable copy of the corresponding source code in
-- return for reimbursement of the cost of distribution. This written offer
-- must permit verbatim duplication by anyone, or (C) it is distributed by
-- someone who received only the executable form, and is accompanied by a
-- copy of the written offer of source code.

-- 6. r-VEX was developed by Stephan Wong, Thijs van As, Fakhar Anjam, Roel Seedorf,
-- Anthony Brandon. r-VEX is currently maintained by TU Delft (J.S.S.M.Wong@tudelft.nl).

-- Copyright (C) 2008-2012 by TU Delft.

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;
use std.textio.all;

library work;
use work.rVEX_pkg.all;
use work.util_pkg.all;


entity rvex_system is
  generic (
    ISSUE_WIDTH        : natural := 4;
    FORWARDING         : boolean := true;
    DMEM_ADDRESS_WIDTH : natural := 12;
    IMEM_ADDRESS_WIDTH : natural := 12);
  port (
    clk               : in  std_logic;
    clk_enable        : in  std_logic;
    reset             : in  std_logic;
    run               : in  std_logic;
    preempt           : in  std_logic;                                    	 -- selects spill routine target address
    ita               : in  std_logic_vector((PC_WIDTH - 1) downto 0);       	 -- start address of spill/restore routine
    mem_select        : in  std_logic_vector(2 downto 0);
    mem_read_address  : in  std_logic_vector(31 downto 0);
    mem_write_address : in  std_logic_vector(31 downto 0);
    mem_write_enable  : in  std_logic;
    mem_byte_enable   : in  std_logic_vector(3 downto 0);
    mem_write_data    : in  std_logic_vector(31 downto 0);
    idle              : out std_logic;
    tca               : out std_logic_vector((PC_WIDTH - 1) downto 0);  -- suspended task resumption address
    done              : out std_logic;
    mem_read_data     : out std_logic_vector(31 downto 0));    
end entity rvex_system;


architecture behavioural of rvex_system is

  signal clk_half : std_logic := '0';

  signal address_dr_s  : std_logic_vector((DMEM_LOGDEP - 3) downto 0) := (others => '0');
  signal address_dw_s  : std_logic_vector((DMEM_LOGDEP - 3) downto 0) := (others => '0');
  signal write_en_dm_s : std_logic_vector(3 downto 0);
  signal read_en_dm_s  : std_logic;

  signal dm2rvex_data_s : std_logic_vector((DMEM_WIDTH - 1) downto 0);
  signal rvex2dm_data_s : std_logic_vector((DMEM_WIDTH - 1) downto 0) := (others => '0');

  signal done_s         : std_logic                                    := '0';
  signal cycles_s       : std_logic_vector(31 downto 0)                := (others => '0');
  signal address_uart_s : std_logic_vector((DMEM_LOGDEP - 1) downto 0) := (others => '0');
  signal data_uart_s    : std_logic_vector(31 downto 0);

  signal flush_s : std_logic                                     := '0';  -- flush for fetch stage
  signal mpc     : std_logic_vector((PC_WIDTH - 1) downto 0);  -- pc to read instruction of i_mem
  signal mpc_r   : std_logic_vector((PC_WIDTH - 1) downto 0);  -- pc to read instruction of i_mem
  signal instr_s : std_logic_vector(ISSUE_WIDTH*32 - 1 downto 0) := (others => '0');  -- instruction from i_mem

  signal dmem_write_enable : std_logic_vector(3 downto 0);
  signal dmem_address      : std_logic_vector((DMEM_LOGDEP - 3) downto 0);
  signal dmem_write_data   : std_logic_vector(31 downto 0);
  signal dmem_read_data    : std_logic_vector(31 downto 0);

  signal imem_write_address : std_logic_vector(ADDR_WIDTH + 1 downto 0);
  signal imem_write_enable  : std_logic;
  signal imem_write_data    : std_logic_vector(31 downto 0);
  signal imem_read_data     : std_logic_vector(31 downto 0);
  signal status_data_out_s  : std_logic_vector(31 downto 0);
  
  constant NUM_CYCLES : integer := 28527;
  FILE     file_report : TEXT open write_mode is "report_control_error.txt";
  FILE     file_simulation : TEXT open write_mode is "report_simulation.txt";
  signal   control_flow_error : std_logic := '0';

begin
  
  i_mem_1 : entity work.i_mem
    generic map (
      ISSUE_WIDTH => ISSUE_WIDTH,
      ADDR_WIDTH  => ADDR_WIDTH)
    port map (
      clk            => clk,
      clk_enable     => clk_enable,
      reset          => flush_s,
      write_enable_a => imem_write_enable,
      address_a      => imem_write_address,
      write_data_a   => imem_write_data,
      address_b      => mpc_r(2 + log2c(ISSUE_WIDTH) + ADDR_WIDTH - 1 downto 2 + log2c(ISSUE_WIDTH)),
      read_data_a    => imem_read_data,
      read_data_b    => instr_s);

  rvex_1 : entity work.rvex
    generic map (
      ISSUE_WIDTH => ISSUE_WIDTH,
      FORWARDING  => FORWARDING)
    port map (
      clk         => clk,
      clk_enable  => clk_enable,
      reset       => reset,
      run         => run,
      preempt     => preempt,
      ita         => ita,
      data_in     => dm2rvex_data_s,
      instr_s     => instr_s,
      address_dm  => address_dr_s,
      write_en_dm => write_en_dm_s,
      read_en_dm  => read_en_dm_s,
      data_out    => rvex2dm_data_s,
      Flush       => flush_s,
      mpc         => mpc,
      mpc_r       => mpc_r,
      tca         => tca,           --  suspended task  resumption address
      done        => done_s);

  d_mem_1 : entity work.d_mem
    port map (
      clk          => clk,
      write_en_a   => write_en_dm_s,
      write_en_b   => dmem_write_enable,
      address_a    => address_dr_s,
      address_b    => dmem_address,
      write_data_a => rvex2dm_data_s,
      write_data_b => dmem_write_data,
      read_data_a  => dm2rvex_data_s,
      read_data_b  => dmem_read_data);

  dmem_write_enable <= mem_byte_enable when mem_select(2) = '1' else (others => '0');
  dmem_address      <= mem_write_address(DMEM_LOGDEP - 1 downto 2);
  dmem_write_data   <= mem_write_data;

  imem_write_address <= mem_write_address(ADDR_WIDTH + 1 downto 0);
  imem_write_enable  <= mem_write_enable when mem_select(1) = '1' else '0';
  imem_write_data    <= mem_write_data;

  -- selection of 3 memory spaces
  mem_read_data <= dmem_read_data when mem_select(2) = '1' else
                   imem_read_data when mem_select(1) = '1' else
                   status_data_out_s;

  done <= done_s;
  idle <= done_s or (not run);

  status_process : process (cycles_s, mem_read_address, mpc_r)
  begin  -- process status_process
    case mem_read_address(3 downto 2) is
      when "00" =>
        status_data_out_s <= cycles_s;
      when "01" =>
        status_data_out_s <= mpc_r;
      when others =>
        status_data_out_s <= (others => '0');
    end case;
  end process status_process;

  -- Counts running cycles
  cycle_counter : process(clk)
    variable l : line;
  begin
    if rising_edge(clk) then
      if reset = '1' then
        cycles_s <= (others => '0');
      elsif clk_enable = '1' then
        if (done_s = '0' and run = '1') then
          cycles_s <= std_logic_vector(unsigned(cycles_s) + 1);
        end if;
        if (unsigned(cycles_s)) > (NUM_CYCLES) then
          report "Control flow error detected " & integer'image(to_integer(unsigned(cycles_s))) severity WARNING;
          write(l,string'("control flow error detected"));
          writeline(file_report, l);
          control_flow_error <= '1';
        end if;
      end if;
    end if;
  end process cycle_counter;
  
  -- print total cycles for code execution
  print_cycles : process(clk)
  	variable l : line;
  begin
    if (rising_edge(clk)) then
      if (done_s = '1' or control_flow_error = '1') then
        file_close(file_report);
        write(l,string'("simulation OK"));
        writeline(file_simulation, l);
        file_close(file_simulation);
        report "the number of cycles is " & integer'image(to_integer(unsigned(cycles_s))) severity FAILURE;
      end if;
    end if;
  end process print_cycles;
  
end architecture behavioural;

