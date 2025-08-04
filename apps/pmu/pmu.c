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
    // raw event
    handle_perf_event(&pe, "frontendFlush", 0x00000001, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "ifu_req", 0x00000002, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "ifu_miss", 0x00000003, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "ifu_req_cacheline_0", 0x00000004, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "ifu_req_cacheline_1", 0x00000005, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "ifu_req_cacheline_0_hit", 0x00000006, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "ifu_req_cacheline_1_hit", 0x00000007, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "only_0_hit", 0x00000008, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "only_0_miss", 0x00000009, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "hit_0_hit_1", 0x0000000a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "hit_0_miss_1", 0x0000000b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "miss_0_hit_1", 0x0000000c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "miss_0_miss_1", 0x0000000d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IBuffer_Flushed", 0x0000000e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IBuffer_hungry", 0x0000000f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IBuffer_1_4_valid", 0x00000010, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IBuffer_2_4_valid", 0x00000011, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IBuffer_3_4_valid", 0x00000012, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IBuffer_4_4_valid", 0x00000013, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IBuffer_full", 0x00000014, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Front_Bubble", 0x00000015, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "icache_miss_cnt", 0x00000016, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "icache_miss_penalty", 0x00000017, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bpu_s2_redirect", 0x00000018, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bpu_s3_redirect", 0x00000019, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "fauftb_commit_hit", 0x0000001a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "fauftb_commit_miss", 0x0000001b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "ftb_commit_hits", 0x0000001c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "ftb_commit_misses", 0x0000001d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "decoder_fused_instr", 0x00000100, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "decoder_waitInstr", 0x00000101, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "decoder_stall_cycle", 0x00000102, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "decoder_utilization", 0x00000103, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rename_in", 0x00000104, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rename_waitinstr", 0x00000105, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rename_stall", 0x00000106, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rename_stall_cycle_walk", 0x00000107, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rename_stall_cycle_dispatch", 0x00000108, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rename_stall_cycle_int", 0x00000109, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rename_stall_cycle_vec", 0x0000010a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rename_stall_cycle_v0", 0x0000010b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rename_stall_cycle_vl", 0x0000010c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "me_freelist_1_4_valid", 0x0000010d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "me_freelist_2_4_valid", 0x0000010e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "me_freelist_3_4_valid", 0x0000010f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "me_freelist_4_4_valid", 0x00000110, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_1_4_valid", 0x00000111, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_2_4_valid", 0x00000112, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_3_4_valid", 0x00000113, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_4_4_valid", 0x00000114, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_1_4_valid", 0x00000115, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_2_4_valid", 0x00000116, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_3_4_valid", 0x00000117, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_4_4_valid", 0x00000118, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_1_4_valid", 0x00000119, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_2_4_valid", 0x0000011a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_3_4_valid", 0x0000011b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "std_freelist_4_4_valid", 0x0000011c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch_in", 0x0000011d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch_empty", 0x0000011e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch_utili", 0x0000011f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch_waitinstr", 0x00000120, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch_stall_cycle_lsq", 0x00000121, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch_stall_cycle_rob", 0x00000122, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch_stall_cycle_int_dq", 0x00000123, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch_stall_cycle_fp_dq", 0x00000124, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch_stall_cycle_ls_dq", 0x00000125, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_in", 0x00000126, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_out", 0x00000127, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_out_try", 0x00000128, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_fake_block", 0x00000129, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_1_4_valid", 0x0000012a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_2_4_valid", 0x0000012b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_3_4_valid", 0x0000012c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_4_4_valid", 0x0000012d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_in", 0x0000012e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_out", 0x0000012f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_out_try", 0x00000130, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_fake_block", 0x00000131, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_1_4_valid", 0x00000132, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_2_4_valid", 0x00000133, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_3_4_valid", 0x00000134, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_4_4_valid", 0x00000135, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_in", 0x00000136, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_out", 0x00000137, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_out_try", 0x00000138, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_fake_block", 0x00000139, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_1_4_valid", 0x0000013a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_2_4_valid", 0x0000013b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_3_4_valid", 0x0000013c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatchq_4_4_valid", 0x0000013d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_interrupt_num", 0x0000013e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_exception_num", 0x0000013f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_flush_pipe_num", 0x00000140, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_replay_inst_num", 0x00000141, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_commitUop", 0x00000142, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_commitInstr", 0x00000143, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_commitInstrFused", 0x00000144, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_commitInstrLoad", 0x00000145, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_commitInstrBranch", 0x00000146, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_commitInstrStore", 0x00000147, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_walkInstr", 0x00000148, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_walkCycle", 0x00000149, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_1_4_valid", 0x0000014a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_2_4_valid", 0x0000014b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_3_4_valid", 0x0000014c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "rob_4_4_valid", 0x0000014d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch2Iq_out_fire_cnt", 0x0000014e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "issueQueue_enq_fire_cnt", 0x0000014f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueAluMulBkuBrhJmp_full", 0x00000150, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueAluMulBkuBrhJmp_full", 0x00000151, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueAluBrhJmpI2fVsetriwiVsetriwvfI2v_full", 0x00000152, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueAluCsrFenceDiv_full", 0x00000153, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch2Iq_out_fire_cnt", 0x00000154, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "issueQueue_enq_fire_cnt", 0x00000155, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueVfma64_full", 0x00000156, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueVfalu64Vfcvt64_full", 0x00000157, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueVfdiv_full", 0x00000158, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dispatch2Iq_out_fire_cnt", 0x00000159, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "issueQueue_enq_fire_cnt", 0x0000015a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueStaMou_full", 0x0000015b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueLdu_full", 0x0000015c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueLdu_full", 0x0000015d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueVlduVstuVseglduVsegstu_full", 0x0000015e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "IssueQueueStdMoud_full", 0x0000015f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_1_4_valid", 0x00000160, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_2_4_valid", 0x00000161, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_3_4_valid", 0x00000162, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_4_4_valid", 0x00000163, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_1_4_valid", 0x00000164, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_2_4_valid", 0x00000165, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_3_4_valid", 0x00000166, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_4_4_valid", 0x00000167, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_1_4_valid", 0x00000168, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_2_4_valid", 0x00000169, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_3_4_valid", 0x0000016a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_4_4_valid", 0x0000016b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_1_4_valid", 0x0000016c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_2_4_valid", 0x0000016d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_3_4_valid", 0x0000016e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "bt_std_freelist_4_4_valid", 0x0000016f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_s0_in_fire", 0x00000200, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "stall_dcache", 0x00000201, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_s1_in_fire", 0x00000202, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_s1_tlb_miss", 0x00000203, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_s2_in_fire", 0x00000204, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_s2_dcache_miss", 0x00000205, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_s0_in_fire", 0x00000206, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "stall_dcache", 0x00000207, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_s1_in_fire", 0x00000208, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_s1_tlb_miss", 0x00000209, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_s2_in_fire", 0x0000020a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_s2_dcache_miss", 0x0000020b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_req_valid", 0x0000020c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_req_fire", 0x0000020d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_merge", 0x0000020e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_newline", 0x0000020f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_req_valid", 0x00000210, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_req_fire", 0x00000211, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_idle", 0x00000212, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_flush", 0x00000213, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_replace", 0x00000214, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "mpipe_resp_valid", 0x00000215, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "replay_resp_valid", 0x00000216, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "coh_timeout", 0x00000217, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_1_4_valid", 0x00000218, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_2_4_valid", 0x00000219, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_3_4_valid", 0x0000021a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "sbuffer_full_valid", 0x0000021b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "enq", 0x0000021c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "stld_rollback", 0x0000021d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "enq", 0x0000021e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "deq", 0x0000021f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "deq_block", 0x00000220, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "replay_full", 0x00000221, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "replay_raw_nack", 0x00000222, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "replay_nuke", 0x00000223, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "replay_mem_amb", 0x00000224, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "replay_tlb_miss", 0x00000225, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "replay_bank_conflict", 0x00000226, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "replay_dcache_replay", 0x00000227, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "replay_forward_fail", 0x00000228, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "replay_dcache_miss", 0x00000229, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "full_mask_000", 0x0000022a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "full_mask_001", 0x0000022b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "full_mask_010", 0x0000022c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "full_mask_011", 0x0000022d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "nuke_rollback", 0x0000022e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "mmioCycle", 0x0000022f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "mmioCnt", 0x00000230, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "mmio_wb_success", 0x00000231, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "mmio_wb_blocked", 0x00000232, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "stq_1_4_valid", 0x00000233, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "stq_2_4_valid", 0x00000234, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "stq_3_4_valid", 0x00000235, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "stq_4_4_valid", 0x00000236, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_wbq_req", 0x00000237, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_wbq_1_4_valid", 0x00000238, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_wbq_2_4_valid", 0x00000239, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_wbq_3_4_valid", 0x0000023a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_wbq_4_4_valid", 0x0000023b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_mp_req", 0x0000023c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_mp_total_penalty", 0x0000023d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_missq_req", 0x0000023e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_missq_1_4_valid", 0x0000023f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_missq_2_4_valid", 0x00000240, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_missq_3_4_valid", 0x00000241, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_missq_4_4_valid", 0x00000242, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_probq_req", 0x00000243, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_probq_1_4_valid", 0x00000244, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_probq_2_4_valid", 0x00000245, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_probq_3_4_valid", 0x00000246, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "dcache_probq_4_4_valid", 0x00000247, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_req", 0x00000248, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_replay", 0x00000249, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_replay_for_data_nack", 0x0000024a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_replay_for_no_mshr", 0x0000024b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_replay_for_conflict", 0x0000024c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_req", 0x0000024d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_replay", 0x0000024e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_replay_for_data_nack", 0x0000024f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_replay_for_no_mshr", 0x00000250, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "load_replay_for_conflict", 0x00000251, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000252, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000253, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000254, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000255, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000256, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000257, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000258, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000259, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x0000025a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x0000025b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x0000025c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x0000025d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x0000025e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x0000025f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000260, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000261, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000262, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000263, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "perfEventsPTW", 0x00000264, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "ldDeqCount", 0x00000265, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "stDeqCount", 0x00000266, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_refill", 0x00000300, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_rd_refill", 0x00000301, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_wr_refill", 0x00000302, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_long_miss", 0x00000303, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_access", 0x00000304, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_l2wb", 0x00000305, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_l1wb", 0x00000306, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_wb_victim", 0x00000307, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_wb_cleaning_coh", 0x00000308, PERF_TYPE_RAW, -1);
    
    handle_perf_event(&pe, "Slice0_l2_cache_access_rd", 0x00000309, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_access_wr", 0x0000030a, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice0_l2_cache_inv", 0x0000030b, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_refill", 0x0000030c, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_rd_refill", 0x0000030d, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_wr_refill", 0x0000030e, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_long_miss", 0x0000030f, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_access", 0x00000310, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_l2wb", 0x00000311, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_l1wb", 0x00000312, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_wb_victim", 0x00000313, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_wb_cleaning_coh", 0x00000314, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_access_rd", 0x00000315, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_access_wr", 0x00000316, PERF_TYPE_RAW, -1);
    handle_perf_event(&pe, "Slice1_l2_cache_inv", 0x00000317, PERF_TYPE_RAW, -1);

    struct perf_event_attr pe_inst, pe_cycle, pe_frontend_stall;
    // sbi event
    handle_perf_event(&pe_inst, "instructions", PERF_COUNT_HW_INSTRUCTIONS, PERF_TYPE_HARDWARE, -1);
    handle_perf_event(&pe_cycle, "cycles", PERF_COUNT_HW_CPU_CYCLES, PERF_TYPE_HARDWARE, -1);
    handle_perf_event(&pe_frontend_stall, "frontend stall", PERF_COUNT_HW_STALLED_CYCLES_FRONTEND, PERF_TYPE_HARDWARE, -1);

    __asm__ volatile(".word 0x0005006b");
    return 0;
}