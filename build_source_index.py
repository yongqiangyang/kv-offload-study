from pathlib import Path
import hashlib
import json
import re
import subprocess
import zipfile

root = Path(__file__).resolve().parent
a = 'repos/vllm-ascend/'
m = 'repos/memfabric_hybrid/'
manager = a + 'vllm_ascend/distributed/kv_transfer/sparse_kv_offload/sparse_kv_offload_manager.py'
attn = a + 'vllm_ascend/attention/sfa_kv_offload.py'
te = a + 'vllm_ascend/distributed/kv_transfer/utils/memfabric_transfer_engine.py'
pd = a + 'vllm_ascend/distributed/kv_transfer/kv_p2p/sfa_pd_rd2h/'
off = m + 'src/acc_offload/'
shared = off + 'csrc/acc_offload_shared_dram_entry.cpp'
pyte = m + 'src/smem/csrc/python_wrapper/mk_transfer_adapter/csrc/pytransfer.cpp'
trans = m + 'src/smem/csrc/smem_trans/'
specs = {
    'runner': (a+'vllm_ascend/worker/v2/model_runner.py', 'self.sparse_kv_offload_manager = init_sparse_kv_offload_manager('),
    'budget': (manager, 'def plan_sparse_kv_offload_memory('),
    'upstream': ('repos/vllm/docs/design/hisparse.md', '# HiSparse'),
    'manager': (manager, 'class SparseKVOffloadManager:'),
    'manager_init': (manager, 'config = offload.OffloadConfig()'),
    'offload_bind': (off+'csrc/python_wrapper/pymf_acc_offload.cpp', 'void DefineAccOffloadConfig('),
    'offload_c': (off+'csrc/acc_offload.cpp', 'OFFLOAD_API int32_t offload_init('),
    'shared_init': (shared, 'int32_t AccOffloadSharedDramEntry::Initialize('),
    'aligned_alloc': (manager, 'def empty_aligned_int8_cpu_tensors('),
    'offload_py': (m+'src/smem/python/memfabric_hybrid/memfabric_hybrid/mf_acc_offload.py', 'def empty(sizes,'),
    'writeback': (manager, 'def _offload_new_kv_on_current_stream('),
    'tail_copy': (manager, 'def copy_sfa_kv('),
    'sparse_c': (off+'csrc/acc_offload.cpp', 'OFFLOAD_API int32_t offload_sparse_copy('),
    'sparse_launch': (off+'csrc/launch/acc_offload_operators_launch.cpp', 'void AccOffloadSparseCopy('),
    'te_build': (te, 'def _build_engine('),
    'te_cpp_init': (pyte, 'int TransferAdapterPy::Initialize('),
    'te_port': (pyte, 'int TransferAdapterPy::GetRpcPort('),
    'te_register': (te, 'def register_buffer('),
    'producer_register': (pd+'worker.py', 'global_memfabric_te.register_buffer(register_regions.ptrs, register_regions.lengths)'),
    'te_cpp_register': (pyte, 'int TransferAdapterPy::RegisterMemory('),
    'pd_read': (pd+'read_thread.py', 'ret = self.engine.batch_transfer_sync_read(p_session, all_local_ptrs, all_peer_ptrs, all_lengths)'),
    'te_cpp_read': (pyte, 'int TransferAdapterPy::BatchTransferSyncRead('),
    'trans_c': (trans+'smem_trans.cpp', 'SMEM_API int32_t smem_trans_batch_read('),
    'trans_connection': (trans+'smem_trans_connection.cpp', 'Result SmemTransConnection::BatchSyncTransfer('),
    'te_adapter': (te, 'class MemfabricBackend:'),
    'shared_reserve': (shared, 'int32_t AccOffloadSharedDramEntry::ReserveSharedPool('),
    'shared_create': (shared, 'int32_t AccOffloadSharedDramEntry::CreateEntityAndPool('),
    'shared_map': (shared, 'int32_t AccOffloadSharedDramEntry::MapPool('),
    'vmm_map': (m+'src/hybm/csrc/mm/hybm_vmm_based_segment.cpp', 'Result HybmVmmBasedSegment::MapMemBySetAccess('),
    'register_caches': (manager, 'def register_kv_caches('),
    'restore_view': (manager, 'def _restore_bfloat16_tensor('),
    'release_py': ('releases/memfabric-1.3.0-python/memfabric_hybrid/mf_acc_offload.py', 'def empty(sizes,'),
    'entry_manager': (off+'csrc/acc_offload_entry_manager.cpp', 'AccOffloadEntryManager::~AccOffloadEntryManager('),
    'exec_kv': (attn, 'def exec_kv('),
    'sparse_kernel_entry': (off+'csrc/operators/acc_offload_sparse_copy.cpp', 'extern "C" __global__ __aicore__ void OffloadSparseCopyOps('),
    'sparse_kernel': (off+'csrc/operators/acc_offload_sparse_copy.h', 'class OffloadSparseCopyKernel'),
    'offload_new': (manager, 'def offload_new_kv('),
    'wait_writeback': (manager, 'def wait_for_current_kv_writeback('),
    'lim': (attn, 'def _lim_select('),
    'copy_sfa': (attn, 'def _copy_sfa_attention('),
    'copy_sfa_kernel': (a+'csrc/attention/fused_scatter_copy_sparse_flash_attention/op_kernel/first_fill_scatter_copy_kernel.h', 'class FirstFillScatterCopyKernel'),
    'copy_sfa_readme': (a+'csrc/attention/fused_scatter_copy_sparse_flash_attention/README.md', '# FusedScatterCopySparseFlashAttention'),
    'trans_register': (trans+'smem_trans_entry.cpp', 'Result SmemTransEntry::RegisterAndExportSlice('),
    'consumer_register': (pd+'worker.py', 'def _register_memfabric_pull('),
    'send_wait': (pd+'send_thread.py', 'p_save_event.synchronize()'),
    'read_ack': (pd+'read_thread.py', 'sock.send_multipart((identity, b"", encoder.encode((READ_DONE, layer_idx))))'),
    'reuse_gate': (pd+'worker.py', 'def wait_for_layer_reuse('),
    'get_dva': (shared, 'int32_t AccOffloadSharedDramEntry::GetDva('),
    'offload_header': (off+'include/host/acc_offload.h', 'uint64_t offload_malloc('),
    'memcache_backend': (a+'vllm_ascend/distributed/kv_transfer/kv_pool/ascend_store/backend/memcache_backend.py', 'from memcache_hybrid import DistributedObjectStore'),
}
index = {}
for key, (rel, needle) in specs.items():
    lines = (root / rel).read_text().splitlines()
    hits = [i + 1 for i, line in enumerate(lines) if needle in line]
    if len(hits) != 1:
        raise ValueError((key, needle, hits))
    index[key] = {'file': rel, 'line': hits[0], 'symbol': needle}
(root / 'source-index.json').write_text(json.dumps(index, ensure_ascii=False, indent=2) + '\n')
print(f'Validated {len(index)} source anchors. Snapshot provenance remains in manifest.json.')
