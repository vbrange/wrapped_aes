`default_nettype none

module AesControl #(
    parameter BITS = 32
)(
    input wb_clk_i,
    input wb_rst_i,
    input wbs_stb_i,
    input wbs_cyc_i,
    input wbs_we_i,
    input [3:0] wbs_sel_i,
    input [31:0] wbs_dat_i,
    input [31:0] wbs_adr_i,
    output wbs_ack_o,
    output [31:0] wbs_dat_o
);
    reg wbs_ack_o;
    reg [31:0] wbs_dat_o;

    reg [127:0] aes_enc_in;
    reg [127:0] aes_enc_key;
    wire[127:0] aes_enc_out;
    wire aes_enc_rdy;
    reg  aes_enc_nrst;

    Aes128Encrypt aes_enc (
        wb_clk_i, // clk
        aes_enc_nrst,
        aes_enc_in,
        aes_enc_key,
        aes_enc_out,
        aes_enc_rdy
    );

    wire[31:0] aes_status;
    assign aes_status[0] = aes_enc_rdy;
    assign aes_status[1] = aes_enc_nrst;
    assign aes_status[31:2] = 0;

    always @(posedge wb_clk_i) begin

        if (wb_rst_i) begin
            aes_enc_in <= 0;
            aes_enc_key <= 0;
            aes_enc_nrst <= 0;
        end

        if (wbs_stb_i && wbs_cyc_i && !wbs_we_i) begin
            //$display("Monitor: READ");

            case (wbs_adr_i[11:0])
                12'h000: wbs_dat_o <= aes_status;

                12'h020: wbs_dat_o <= aes_enc_in[ 31: 0];
                12'h024: wbs_dat_o <= aes_enc_in[ 63:32];
                12'h028: wbs_dat_o <= aes_enc_in[ 95:64];
                12'h02C: wbs_dat_o <= aes_enc_in[127:96];

                12'h030: wbs_dat_o <= aes_enc_out[ 31: 0];
                12'h034: wbs_dat_o <= aes_enc_out[ 63:32];
                12'h038: wbs_dat_o <= aes_enc_out[ 95:64];
                12'h03C: wbs_dat_o <= aes_enc_out[127:96];

                default: wbs_dat_o <= 32'hDEADB00F;
            endcase

            wbs_ack_o <= 1;
        end
        else if (wbs_stb_i && wbs_cyc_i && wbs_we_i) begin
            //$display("Monitor: WRITE");

            case (wbs_adr_i[11:0])
                12'h004: aes_enc_nrst <= 0;
                12'h008: aes_enc_nrst <= 1;

                12'h010: aes_enc_key[ 31: 0] <= wbs_dat_i;
                12'h014: aes_enc_key[ 63:32] <= wbs_dat_i;
                12'h018: aes_enc_key[ 95:64] <= wbs_dat_i;
                12'h01C: aes_enc_key[127:96] <= wbs_dat_i;

                12'h020: aes_enc_in[ 31: 0] <= wbs_dat_i;
                12'h024: aes_enc_in[ 63:32] <= wbs_dat_i;
                12'h028: aes_enc_in[ 95:64] <= wbs_dat_i;
                12'h02C: aes_enc_in[127:96] <= wbs_dat_i;
            endcase
            wbs_ack_o <= 1;
        end
        else
            wbs_ack_o <= 0;
    end

endmodule

`default_nettype wire
