#!/usr/bin/env python
import dsx
import soclib

def _cluster(arch, no,
             ncpu, nram,
             icache_lines, icache_words,
             dcache_lines, dcache_words):

    #crossbar = arch.create('caba:vci_local_crossbar', 'lc%d'%no)
    crossbar = arch.create('caba:vci_local_crossbar', 'lc0')

    for i in range(ncpu):
        cn = arch.cpu_num
        arch.cpu_num += 1

        cpu = arch.create('caba:vci_xcache_wrapper', 'cpu%d_%d'%(no,i),
                           iss_t='common:mips32el',
                           ident = cn,
                           icache_ways = 1,
                           icache_sets = icache_lines,
                           icache_words = icache_words,
                           dcache_ways = 1,
                           dcache_sets = dcache_lines,
                           dcache_words = dcache_words,
                           )

        crossbar.to_initiator.new() // cpu.vci

    for i in range(nram):
        ram = arch.create('caba:vci_ram', 'ram%d_%d'%(no,i))
        base = 0x10000000*(1+no)+0x1000000*i
        ram.addSegment('cram%d_%d'%(no,i), base, 0x100000, True)
        ram.addSegment('uram%d_%d'%(no,i), base + 0x400000, 0x100000, False)
        ram.vci // crossbar.to_target.new()

    if no == 0:
        ram.addSegment('boot', 0xbfc00000, 0x1000,True)
        ram.addSegment('excep', 0x80000080, 0x1000,True)

    return crossbar

def ClusteredNoirqMulti(
    cpus = [1],
    rams = [1],
    min_latency = 10,
    icache_lines = 32,
    icache_words = 8,
    dcache_lines = 32,
    dcache_words = 8 ):

    arch = soclib.Architecture(
                         cell_size = 4,
                         plen_size = 9,
                         addr_size = 32,
                         rerror_size = 1,
                         clen_size = 1,
                         rflag_size = 1,
                         srcid_size = 8,
                         pktid_size = 1,
                         trdid_size = 1,
                         wrplen_size = 1
                         )
    arch.cpu_num = 0

    mt = arch.create('common:mapping_table',
              'mapping_table',
              addr_bits = [5,3],
              srcid_bits = [4,4],
              cacheability_mask = 0xc00000)

    arch.create('common:loader', 'loader')

    vgmn = arch.create('caba:vci_vgmn', 'vgmn0', min_latency = min_latency, fifo_depth = 8, mt = mt)

    for no, (ncpu, nram) in enumerate(zip(cpus, rams)):
        lc = _cluster(
            arch, no,
            ncpu = ncpu, nram = nram,
            icache_lines = icache_lines, icache_words = icache_words,
            dcache_lines = dcache_lines, dcache_words = dcache_words)
        vgmn.to_initiator.new() // lc.initiator_to_up
        vgmn.to_target.new() // lc.target_to_up

	if no == 0 :
                tg = dsx.TaskModel.getByName('tg').getImpl(soclib.HwTask)
                ctrl, coproc = tg.instanciate(arch, 'tg0')
                ctrl.addSegment('tg_ctrl', 0x16400000, 0x100, False)
                ctrl.vci_initiator // lc.to_initiator.new()
                ctrl.vci_target // lc.to_target.new()

        if no == 3 :
                ramdac = dsx.TaskModel.getByName('ramdac').getImpl(soclib.HwTask)
                ctrl, coproc = ramdac.instanciate(arch, 'ramdac0')
                ctrl.addSegment('ramdac_ctrl', 0x46400000, 0x100, False)
                ctrl.vci_initiator // lc.to_initiator.new()
                ctrl.vci_target // lc.to_target.new()

    tty = arch.create('caba:vci_multi_tty', 'tty', names = ['tty0'])
    tty.addSegment('tty', 0x95400000, 0x20, False)
    tty.vci // lc.to_target.new()

    return arch
