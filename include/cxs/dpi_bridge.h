#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


enum {
    CXS_DPI_OK = 0,
    CXS_DPI_FULL = -1,
    CXS_DPI_BADARG = -2,
    CXS_DPI_EMPTY = 0,
    CXS_DPI_GOT = 1,
};


enum {
    CXS_LINK_DOWN = 0,
    CXS_LINK_UP = 1,
    CXS_LINK_DEACT = 2,
    CXS_LINK_ERR = 3,
};


enum {
    CXS_CMD_NONE = 0,
    CXS_CMD_ACTIVATE = 1,
    CXS_CMD_DEACTIVATE = 2,
    CXS_CMD_RESET = 3,
    CXS_CMD_RETRAIN = 4,
};


int cxs_tx_push(uint64_t d0, uint64_t d1, uint64_t d2, uint64_t d3,
                uint64_t d4, uint64_t d5, uint64_t d6, uint64_t d7,
                uint32_t cntl, uint32_t prcltype, unsigned char last,
                uint64_t t_fs);


void cxs_link_event(int ev, uint64_t t_fs);


int cxs_rx_pop(uint64_t* d0, uint64_t* d1, uint64_t* d2, uint64_t* d3,
               uint64_t* d4, uint64_t* d5, uint64_t* d6, uint64_t* d7,
               uint32_t* cntl, uint32_t* prcltype, unsigned char* last);


int cxs_dpi_tx_pop(uint64_t* d0, uint64_t* d1, uint64_t* d2, uint64_t* d3,
                   uint64_t* d4, uint64_t* d5, uint64_t* d6, uint64_t* d7,
                   uint32_t* cntl, uint32_t* prcltype, unsigned char* last,
                   uint64_t* t_fs);


int cxs_dpi_rx_push(uint64_t d0, uint64_t d1, uint64_t d2, uint64_t d3,
                    uint64_t d4, uint64_t d5, uint64_t d6, uint64_t d7,
                    uint32_t cntl, uint32_t prcltype, unsigned char last,
                    uint64_t t_fs);


int cxs_credit_return(int n);


int cxs_link_cmd(int cmd);


int cxs_dpi_set_loopback(int enable);
int cxs_dpi_set_capacity(int n);
int cxs_dpi_reset(void);


int cxs_dpi_get_stats(int* tx_pushed, int* rx_popped, int* link_events,
                      int* credit_returns, int* link_cmds, int* errors,
                      int* loopback_drops);


int cxs_dpi_get_last_tx(uint64_t* d0, uint64_t* d1, uint64_t* d2, uint64_t* d3,
                        uint64_t* d4, uint64_t* d5, uint64_t* d6,
                        uint64_t* d7, uint32_t* cntl, uint32_t* prcltype,
                        unsigned char* last, uint64_t* t_fs);


int cxs_dpi_get_tx(int index, uint64_t* d0, uint64_t* d1, uint64_t* d2,
                   uint64_t* d3, uint64_t* d4, uint64_t* d5, uint64_t* d6,
                   uint64_t* d7, uint32_t* cntl, uint32_t* prcltype,
                   unsigned char* last, uint64_t* t_fs);


int cxs_tx_push_batch(const uint64_t* rec, int n_flits);
int cxs_dpi_rx_push_batch(const uint64_t* rec, int n_flits);
int cxs_dpi_tx_pop_batch(uint64_t* rec, int max_flits);
int cxs_rx_pop_batch(uint64_t* rec, int max_flits);

int cxs_dpi_drain(int max_flits);


int cxs_dpi_bench_start(void);
int cxs_dpi_bench_stop(void);
int cxs_dpi_bench_get(uint64_t* calls_single, uint64_t* calls_batch,
                      uint64_t* flits_single, uint64_t* flits_batch,
                      uint64_t* drain_calls, uint64_t* drained,
                      uint64_t* dpi_ns, uint64_t* wall_ns);

#ifdef __cplusplus
}
#endif
