/*
## @file
#
#  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
#  All intellectual property rights(Copyright, Patent and Trademark) reserved.
#
#  Any violations of copyright or other intellectual property rights of the Loongson Technology
#  Corporation Limited will be held accountable in accordance with the law,
#  if you (or any of your subsidiaries, corporate affiliates or agents) initiate
#  directly or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
#  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
#  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
#  in whole or in part from any software, technology, product or service of Loongson Technology Corporation
#  Limited or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
#  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).
#
#
##
*/
#include <Library/MemConfig.h>
#include <Ppi/Ls3aCpu/Ls3aPlatformTablePpi.h>

#define HIGH_MEM_WIN_BASE_ADDR 0x80000000

void mm_feature_init(OUT DDR_CTRL *mm_ctrl)
{
        mm_ctrl->ddr_freq = DDR_FREQ;
        mm_ctrl->node_offset = NODE_OFFSET;
#ifdef BONITO_100M
        mm_ctrl->ref_clk = 100;
#elif BONITO_25M
        mm_ctrl->ref_clk = 25;
#endif
        mm_ctrl->mem_base = HIGH_MEM_WIN_BASE_ADDR;
        /* mm_ctrl is global variable */
        mm_ctrl->table.ddr_param_store          = 1;
        mm_ctrl->table.ddr3_dimm                = 0;
        mm_ctrl->table.auto_ddr_config          = 1;
        mm_ctrl->table.enable_ddr_leveling      = 1;
        mm_ctrl->table.print_ddr_leveling       = 0;
        mm_ctrl->table.enable_mc_vref_training  = 1;
        mm_ctrl->table.vref_training_debug      = 0;
        mm_ctrl->table.enable_ddr_vref_training = 1;
        mm_ctrl->table.enable_bit_training      = 1;
        mm_ctrl->table.bit_training_debug       = 1;
        mm_ctrl->table.enable_write_training    = 1;
        mm_ctrl->table.debug_write_training     = 0;
        mm_ctrl->table.print_dll_sample         = 0;
        mm_ctrl->table.disable_dq_odt_training  = 1;
        mm_ctrl->table.lvl_debug                = 0;
        mm_ctrl->table.disable_dram_crc         = 1;
        mm_ctrl->table.two_t_mode_enable        = 0;
        mm_ctrl->table.disable_dimm_ecc         = 0;
        mm_ctrl->table.disable_read_dbi         = 1;
        mm_ctrl->table.disable_write_dbi        = 1;
        mm_ctrl->table.disable_dm               = 0;
        mm_ctrl->table.set_by_protocol          = 1;
        mm_ctrl->table.param_set_from_spd_debug = 0;
        mm_ctrl->table.refresh_1x               = 1;
        mm_ctrl->table.spd_only                 = 0;
        mm_ctrl->table.ddr_debug_param          = 0;
        mm_ctrl->table.ddr_soft_clksel          = 1;
        mm_ctrl->table.str                      = 1;
        mm_ctrl->table.pda_mode                 = 1;
        mm_ctrl->data.vref                      = 0x0a01;
        mm_ctrl->data.vref_init                 = 0x20;
        mm_ctrl->data.pl_manualy                = 12;
        mm_ctrl->data.bit_width                 = 0;
        mm_ctrl->data.nc16_map                  = 0;
}
