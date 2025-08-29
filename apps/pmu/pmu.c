#include <err.h>
#include <errno.h>
#include <linux/perf_event.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    int ret;
    ret = syscall(SYS_perf_event_open, hw_event, pid, cpu,
                  group_fd, flags);
    return ret;
}


// 定义一个函数来处理单个性能事件的初始化、打开、操作和读取
static void handle_perf_event(struct perf_event_attr *pe, const char *name, uint64_t config, int type, int group_fd) {
    memset(pe, 0, sizeof(*pe));
    pe->type = type;
    pe->size = sizeof(*pe);
    pe->config = config;
    pe->disabled = 1;

    int fd = perf_event_open(pe, 0, -1, group_fd, 0);
    if (fd == -1) {
        fprintf(stderr, "errno: %d\n", errno);
        err(EXIT_FAILURE, "Error opening %s event %llx\n", name, (unsigned long long)config);
    }

    if (ioctl(fd, PERF_EVENT_IOC_RESET, 0) == -1)
        err(EXIT_FAILURE, "PERF_EVENT_IOC_RESET for %s", name);

    if (ioctl(fd, PERF_EVENT_IOC_ENABLE, 0) == -1)
        err(EXIT_FAILURE, "PERF_EVENT_IOC_ENABLE for %s", name);

    printf("Measuring %s count for this printf\n", name);

    if (ioctl(fd, PERF_EVENT_IOC_DISABLE, 0) == -1)
        err(EXIT_FAILURE, "PERF_EVENT_IOC_DISABLE for %s", name);

    long long count;
    if (read(fd, &count, sizeof(count)) != sizeof(count))
        err(EXIT_FAILURE, "read %s", name);

    printf("Used %lld %s\n", count, name);
    

    if (close(fd) == -1)
        err(EXIT_FAILURE, "close %s fd", name);
}

int main(void) {

    struct perf_event_attr pe;

    typedef struct {
        const char *name;
        uint64_t selector;
    } PerfEvent;

    // 定义所有性能事件
    static const PerfEvent perf_events[] = {
    {"noEvent", 0},
    {"frontendFlush", 1},
    {"ifu_req", 2},
    {"ifu_miss", 3},
    {"ifu_req_cacheline_0", 4},
    {"ifu_req_cacheline_1", 5},
    {"ifu_req_cacheline_0_hit", 6},
    {"ifu_req_cacheline_1_hit", 7},
    {"only_0_hit", 8},
    {"only_0_miss", 9},
    {"hit_0_hit_1", 10},
    {"hit_0_miss_1", 11},
    {"miss_0_hit_1", 12},
    {"miss_0_miss_1", 13},
    {"IBuffer_Flushed", 14},
    {"IBuffer_hungry", 15},
    {"IBuffer_1_4_valid", 16},
    {"IBuffer_2_4_valid", 17},
    {"IBuffer_3_4_valid", 18},
    {"IBuffer_4_4_valid", 19},
    {"IBuffer_full", 20},
    {"Front_Bubble", 21},
    {"icache_miss_cnt", 22},
    {"bpu_s2_redirect", 23},
    {"bpu_s3_redirect", 24},
    {"fauftb_commit_hit", 25},
    {"fauftb_commit_miss", 26},
    {"ftb_commit_hits", 27},
    {"ftb_commit_misses", 28},
    {"decoder_fused_instr", 256},
    {"decoder_waitInstr", 257},
    {"decoder_stall_cycle", 258},
    {"decoder_utilization", 259},
    {"rename_in", 260},
    {"rename_waitinstr", 261},
    {"rename_stall", 262},
    {"rename_stall_cycle_walk", 263},
    {"rename_stall_cycle_dispatch", 264},
    {"rename_stall_cycle_int", 265},
    {"rename_stall_cycle_vec", 266},
    {"rename_stall_cycle_v0", 267},
    {"rename_stall_cycle_vl", 268},
    {"me_freelist_1_4_valid", 269},
    {"me_freelist_2_4_valid", 270},
    {"me_freelist_3_4_valid", 271},
    {"me_freelist_4_4_valid", 272},
    {"std_freelist_1_4_valid", 273},
    {"std_freelist_2_4_valid", 274},
    {"std_freelist_3_4_valid", 275},
    {"std_freelist_4_4_valid", 276},
    {"std_freelist_1_4_valid", 277},
    {"std_freelist_2_4_valid", 278},
    {"std_freelist_3_4_valid", 279},
    {"std_freelist_4_4_valid", 280},
    {"std_freelist_1_4_valid", 281},
    {"std_freelist_2_4_valid", 282},
    {"std_freelist_3_4_valid", 283},
    {"std_freelist_4_4_valid", 284},
    {"dispatch_in", 285},
    {"dispatch_empty", 286},
    {"dispatch_utili", 287},
    {"dispatch_waitinstr", 288},
    {"dispatch_stall_cycle_rob", 289},
    {"dispatch_stall_cycle_int_dq", 290},
    {"dispatch_stall_cycle_fp_dq", 291},
    {"dispatch_stall_cycle_ls_dq", 292},
    {"intDq0_dispatchq_in", 293},
    {"intDq0_dispatchq_out", 294},
    {"intDq0_dispatchq_out_try", 295},
    {"intDq0_dispatchq_fake_block", 296},
    {"intDq0_dispatchq_1_4_valid", 297},
    {"intDq0_dispatchq_2_4_valid", 298},
    {"intDq0_dispatchq_3_4_valid", 299},
    {"intDq0_dispatchq_4_4_valid", 300},
    {"intDq1_dispatchq_in", 301},
    {"intDq1_dispatchq_out", 302},
    {"intDq1_dispatchq_out_try", 303},
    {"intDq1_dispatchq_fake_block", 304},
    {"intDq1_dispatchq_1_4_valid", 305},
    {"intDq1_dispatchq_2_4_valid", 306},
    {"intDq1_dispatchq_3_4_valid", 307},
    {"intDq1_dispatchq_4_4_valid", 308},
    {"vecDq_dispatchq_in", 309},
    {"vecDq_dispatchq_out", 310},
    {"vecDq_dispatchq_out_try", 311},
    {"vecDq_dispatchq_fake_block", 312},
    {"vecDq_dispatchq_1_4_valid", 313},
    {"vecDq_dispatchq_2_4_valid", 314},
    {"vecDq_dispatchq_3_4_valid", 315},
    {"vecDq_dispatchq_4_4_valid", 316},
    {"lsDq_dispatchq_in", 317},
    {"lsDq_dispatchq_out", 318},
    {"lsDq_dispatchq_out_try", 319},
    {"lsDq_dispatchq_fake_block", 320},
    {"lsDq_dispatchq_1_4_valid", 321},
    {"lsDq_dispatchq_2_4_valid", 322},
    {"lsDq_dispatchq_3_4_valid", 323},
    {"lsDq_dispatchq_4_4_valid", 324},
    {"rob_interrupt_num", 325},
    {"rob_exception_num", 326},
    {"rob_flush_pipe_num", 327},
    {"rob_replay_inst_num", 328},
    {"rob_commitUop", 329},
    {"rob_commitInstr", 330},
    {"rob_commitInstrFused", 331},
    {"rob_commitInstrLoad", 332},
    {"rob_commitInstrBranch", 333},
    {"rob_commitInstrStore", 334},
    {"rob_walkInstr", 335},
    {"rob_walkCycle", 336},
    {"rob_1_4_valid", 337},
    {"rob_2_4_valid", 338},
    {"rob_3_4_valid", 339},
    {"rob_4_4_valid", 340},
    {"dispatch2Iq_out_fire_cnt", 341},
    {"issueQueue_enq_fire_cnt", 342},
    {"IssueQueueAluMulBkuBrhJmp0_full", 343},
    {"IssueQueueAluMulBkuBrhJmp1_full", 344},
    {"IssueQueueAluBrhJmpI2fVsetriwiVsetriwvfI2v2_full", 345},
    {"IssueQueueAluCsrFenceDiv3_full", 346},
    {"dispatch2Iq_out_fire_cnt", 347},
    {"issueQueue_enq_fire_cnt", 348},
    {"IssueQueueVfma640_full", 349},
    {"IssueQueueVfalu64Vfcvt641_full", 350},
    {"IssueQueueVfdiv2_full", 351},
    {"dispatch2Iq_out_fire_cnt", 352},
    {"issueQueue_enq_fire_cnt", 353},
    {"IssueQueueStaMou0_full", 354},
    {"IssueQueueLdu1_full", 355},
    {"IssueQueueLdu2_full", 356},
    {"IssueQueueStdMoud4_full", 357},
    {"intbt_std_freelist_1_4_valid", 358},
    {"intbt_std_freelist_2_4_valid", 359},
    {"intbt_std_freelist_3_4_valid", 360},
    {"intbt_std_freelist_4_4_valid", 361},
    {"vfbt_std_freelist_1_4_valid", 362},
    {"vfbt_std_freelist_2_4_valid", 363},
    {"vfbt_std_freelist_3_4_valid", 364},
    {"vfbt_std_freelist_4_4_valid", 365},
    {"vlbt_std_freelist_1_4_valid", 366},
    {"vlbt_std_freelist_2_4_valid", 367},
    {"vlbt_std_freelist_3_4_valid", 368},
    {"vlbt_std_freelist_4_4_valid", 369},
    {"v0bt_std_freelist_1_4_valid", 370},
    {"v0bt_std_freelist_2_4_valid", 371},
    {"v0bt_std_freelist_3_4_valid", 372},
    {"ldu0_s0_in_fire", 512},
    {"ldu0_stall_dcache", 513},
    {"ldu0_s1_in_fire", 514},
    {"ldu0_s1_tlb_miss", 515},
    {"ldu0_s2_in_fire", 516},
    {"ldu0_s2_dcache_miss", 517},
    {"ldu1_s0_in_fire", 518},
    {"ldu1_stall_dcache", 519},
    {"ldu1_s1_in_fire", 520},
    {"ldu1_s1_tlb_miss", 521},
    {"ldu1_s2_in_fire", 522},
    {"ldu1_s2_dcache_miss", 523},
    {"sbuffer_req_valid", 524},
    {"sbuffer_req_fire", 525},
    {"sbuffer_merge", 526},
    {"sbuffer_newline", 527},
    {"dcache_req_valid", 528},
    {"dcache_req_fire", 529},
    {"sbuffer_idle", 530},
    {"sbuffer_flush", 531},
    {"sbuffer_replace", 532},
    {"mpipe_resp_valid", 533},
    {"replay_resp_valid", 534},
    {"coh_timeout", 535},
    {"sbuffer_1_4_valid", 536},
    {"sbuffer_2_4_valid", 537},
    {"sbuffer_3_4_valid", 538},
    {"sbuffer_full_valid", 539},
    {"lq_raw", 540},
    {"stld_rollback", 541},
    {"lq_replay_enq", 542},
    {"lq_replay_deq", 543},
    {"lq_replay_deq_block", 544},
    {"replay_full", 545},
    {"replay_raw_nack", 546},
    {"replay_nuke", 547},
    {"replay_mem_amb", 548},
    {"replay_tlb_miss", 549},
    {"replay_bank_conflict", 550},
    {"replay_dcache_replay", 551},
    {"replay_forward_fail", 552},
    {"replay_dcache_miss", 553},
    {"full_mask_000", 554},
    {"full_mask_001", 555},
    {"full_mask_010", 556},
    {"full_mask_011", 557},
    {"nuke_rollback", 558},
    {"mmioCycle", 559},
    {"mmioCnt", 560},
    {"mmio_wb_success", 561},
    {"mmio_wb_blocked", 562},
    {"stq_1_4_valid", 563},
    {"stq_2_4_valid", 564},
    {"stq_3_4_valid", 565},
    {"stq_4_4_valid", 566},
    {"dcache_wbq_req", 567},
    {"dcache_wbq_1_4_valid", 568},
    {"dcache_wbq_2_4_valid", 569},
    {"dcache_wbq_3_4_valid", 570},
    {"dcache_wbq_4_4_valid", 571},
    {"dcache_mp_req", 572},
    {"dcache_mp_total_penalty", 573},
    {"dcache_missq_req", 574},
    {"dcache_missq_1_4_valid", 575},
    {"dcache_missq_2_4_valid", 576},
    {"dcache_missq_3_4_valid", 577},
    {"dcache_missq_4_4_valid", 578},
    {"dcache_probq_req", 579},
    {"dcache_probq_1_4_valid", 580},
    {"dcache_probq_2_4_valid", 581},
    {"dcache_probq_3_4_valid", 582},
    {"dcache_probq_4_4_valid", 583},
    {"loadpipe0_req", 584},
    {"loadpipe0_replay", 585},
    {"loadpipe0_replay_for_data_nack", 586},
    {"loadpipe0_replay_for_no_mshr", 587},
    {"loadpipe0_replay_for_conflict", 588},
    {"loadpipe1_req", 589},
    {"loadpipe1_replay", 590},
    {"loadpipe1_replay_for_data_nack", 591},
    {"loadpipe1_replay_for_no_mshr", 592},
    {"loadpipe1_replay_for_conflict", 593},
    {"tlbllptw_incount", 594},
    {"tlbllptw_inblock", 595},
    {"tlbllptw_memcount", 596},
    {"tlbllptw_memcycle", 597},
    {"access", 598},
    {"l2_hit", 599},
    {"l1_hit", 600},
    {"l0_hit", 601},
    {"sp_hit", 602},
    {"pte_hit", 603},
    {"rwHarzad", 604},
    {"out_blocked", 605},
    {"fsm_count", 606},
    {"fsm_busy", 607},
    {"fsm_idle", 608},
    {"resp_blocked", 609},
    {"mem_count", 610},
    {"mem_cycle", 611},
    {"mem_blocked", 612},
    {"ldDeqCount", 613},
    {"stDeqCount", 614},
    {"Slice0_l2_cache_refill", 768},
    {"Slice0_l2_cache_rd_refill", 769},
    {"Slice0_l2_cache_wr_refill", 770},
    {"Slice0_l2_cache_long_miss", 771},
    {"Slice0_l2_cache_access", 772},
    {"Slice0_l2_cache_l2wb", 773},
    {"Slice0_l2_cache_l1wb", 774},
    {"Slice0_l2_cache_wb_victim", 775},
    {"Slice0_l2_cache_wb_cleaning_coh", 776},
    {"Slice0_l2_cache_access_rd", 777},
    {"Slice0_l2_cache_access_wr", 778},
    {"Slice0_l2_cache_inv", 779},
    {"Slice1_l2_cache_refill", 780},
    {"Slice1_l2_cache_rd_refill", 781},
    {"Slice1_l2_cache_wr_refill", 782},
    {"Slice1_l2_cache_long_miss", 783},
    {"Slice1_l2_cache_access", 784},
    {"Slice1_l2_cache_l2wb", 785},
    {"Slice1_l2_cache_l1wb", 786},
    {"Slice1_l2_cache_wb_victim", 787},
    {"Slice1_l2_cache_wb_cleaning_coh", 788},
    {"Slice1_l2_cache_access_rd", 789},
    {"Slice1_l2_cache_access_wr", 790},
    {"Slice1_l2_cache_inv", 791},
    };

    // 遍历所有性能事件并调用 handle_perf_event
    for (size_t i = 0; i < sizeof(perf_events) / sizeof(perf_events[0]); i++) {
        handle_perf_event(&pe, perf_events[i].name, perf_events[i].selector, PERF_TYPE_RAW, -1);
    }
    

    struct perf_event_attr pe_inst, pe_cycle, pe_frontend_stall;
    // sbi event
    handle_perf_event(&pe_inst, "instructions", PERF_COUNT_HW_INSTRUCTIONS, PERF_TYPE_HARDWARE, -1);
    handle_perf_event(&pe_cycle, "cycles", PERF_COUNT_HW_CPU_CYCLES, PERF_TYPE_HARDWARE, -1);
    handle_perf_event(&pe_frontend_stall, "frontend stall", PERF_COUNT_HW_STALLED_CYCLES_FRONTEND, PERF_TYPE_HARDWARE, -1);

    __asm__ volatile(".word 0x0005006b");
    return 0;
}