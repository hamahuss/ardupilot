#!/usr/bin/env python

import sys, fnmatch
import importlib

# peripheral types that can be shared, wildcard patterns
SHARED_MAP = ["I2C*", "USART*_TX", "UART*_TX", "SPI*", "TIM*_UP"]

ignore_list = []
dma_map = None

debug = False


def check_possibility(periph, dma_stream, curr_dict, dma_map, check_list):
    for other_periph in curr_dict:
        if other_periph != periph:
            if curr_dict[other_periph] == dma_stream:
                ignore_list.append(periph)
                check_str = "%s(%d,%d) %s(%d,%d)" % (
                    other_periph, curr_dict[other_periph][0],
                    curr_dict[other_periph][1], periph, dma_stream[0],
                    dma_stream[1])
                #check if we did this before
                if check_str in check_list:
                    return False
                check_list.append(check_str)
                if debug:
                    print("Trying to Resolve Conflict: ", check_str)
                #check if we can resolve by swapping with other periphs
                for streamchan in dma_map[other_periph]:
                    stream = (streamchan[0], streamchan[1])
                    if stream != curr_dict[other_periph] and \
                       check_possibility(other_periph, stream, curr_dict, dma_map, check_list):
                        curr_dict[other_periph] = stream
                        return True
                return False
    return True

def can_share(periph, noshare_list):
    '''check if a peripheral is in the SHARED_MAP list'''
    for noshare in noshare_list:
        if fnmatch.fnmatch(periph, noshare):
            return False
    for f in SHARED_MAP:
        if fnmatch.fnmatch(periph, f):
            return True
    if debug:
        print("%s can't share" % periph)
    return False


def chibios_dma_define_name(key):
    '''return define name needed for board.h for ChibiOS'''
    if key.startswith('ADC'):
        return 'STM32_ADC_%s_DMA_' % key
    elif key.startswith('SPI'):
        return 'STM32_SPI_%s_DMA_' % key
    elif key.startswith('I2C'):
        return 'STM32_I2C_%s_DMA_' % key
    elif key.startswith('USART'):
        return 'STM32_UART_%s_DMA_' % key
    elif key.startswith('UART'):
        return 'STM32_UART_%s_DMA_' % key
    elif key.startswith('SDIO') or key.startswith('SDMMC'):
        return 'STM32_SDC_%s_DMA_' % key
    elif key.startswith('TIM'):
        return 'STM32_TIM_%s_DMA_' % key
    else:
        print("Error: Unknown key type %s" % key)
        sys.exit(1)

def get_list_index(peripheral, priority_list):
    '''return index into priority_list for a peripheral'''
    for i in range(len(priority_list)):
        str = priority_list[i]
        if fnmatch.fnmatch(peripheral, str):
            return i
    # default to max priority
    return len(priority_list)

def get_sharing_priority(periph_list, priority_list):
    '''get priority of a list of peripherals we could share with'''
    highest = len(priority_list)
    for p in periph_list:
        prio = get_list_index(p, priority_list)
        if prio < highest:
            highest = prio
    return highest

<<<<<<< HEAD
=======
def generate_DMAMUX_map_mask(peripheral_list, channel_mask, noshare_list, dma_exclude):
    '''
    generate a dma map suitable for a board with a DMAMUX

    In principle any peripheral can use any stream, but we need to
    ensure that a peripheral doesn't try to use the same stream as its
    partner (eg. a RX/TX pair)
    '''
    dma_map = {}
    idsets = {}

    # first unshareable peripherals
    available = channel_mask
    for p in peripheral_list:
        dma_map[p] = []
        idsets[p] = set()

    for p in peripheral_list:
        if can_share(p, noshare_list) or p in dma_exclude:
            continue
        for i in range(16):
            mask = (1<<i)
            if available & mask != 0:
                available &= ~mask
                dma = (i // 8) + 1
                stream = i % 8
                dma_map[p].append((dma,stream,0))
                idsets[p].add(i)
                break

    if debug:
        print('dma_map1: ', dma_map)
        print('available: 0x%04x' % available)

    # now shareable
    idx = 0
    for p in peripheral_list:
        if not can_share(p, noshare_list) or p in dma_exclude:
            continue
        base = idx % 16
        for i in range(16):
            found = None
            for ii in list(range(base,16)) + list(range(0,base)):
                if (1<<ii) & available == 0:
                    continue

                dma = (ii // 8) + 1
                stream = ii % 8

                if (dma,stream) in dma_map[p]:
                    continue
                
                # prevent attempts to share with other half of same peripheral
                if p.endswith('RX'):
                    other = p[:-2] + 'TX'
                elif p.endswith('TX'):
                    other = p[:-2] + 'RX'
                else:
                    other = None

                if other is not None and ii in idsets[other]:
                    if len(idsets[p]) >= len(idsets[other]) and len(idsets[other]) > 0:
                        continue
                    idsets[other].remove(ii)
                    dma_map[other].remove((dma,stream))
                found = ii
                break
            if found is None:
                continue
            base = (found+1) % 16
            dma = (found // 8) + 1
            stream = found % 8
            dma_map[p].append((dma,stream))
            idsets[p].add(found)
        idx = (idx+1) % 16
    if debug:
        print('dma_map: ', dma_map)
        print('idsets: ', idsets)
        print('available: 0x%04x' % available)
    return dma_map

def generate_DMAMUX_map(peripheral_list, noshare_list, dma_exclude):
    '''
    generate a dma map suitable for a board with a DMAMUX1 and DMAMUX2
    '''
    # first split peripheral_list into those for DMAMUX1 and those for DMAMUX2
    dmamux1_peripherals = []
    dmamux2_peripherals = []
    for p in peripheral_list:
        if dma_name(p) == 'BDMA':
            dmamux2_peripherals.append(p)
        else:
            dmamux1_peripherals.append(p)
    map1 = generate_DMAMUX_map_mask(dmamux1_peripherals, 0xFFFF, noshare_list, dma_exclude)
    map2 = generate_DMAMUX_map_mask(dmamux2_peripherals, 0xFF, noshare_list, dma_exclude)
    # translate entries from map2 to "DMA controller 3", which is used for BDMA
    for p in map2.keys():
        streams = []
        for (controller,stream) in map2[p]:
            streams.append((3,stream))
        map2[p] = streams
    both = map1
    both.update(map2)
    if debug:
        print('dma_map_both: ', both)
    return both

>>>>>>> upstream/master
def write_dma_header(f, peripheral_list, mcu_type, dma_exclude=[],
                     dma_priority='', dma_noshare=''):
    '''write out a DMA resolver header file'''
    global dma_map

    # form a list of DMA priorities
    priority_list = dma_priority.split()

    # sort by priority
    peripheral_list = sorted(peripheral_list, key=lambda x: get_list_index(x, priority_list))

    # form a list of peripherals that can't share
    noshare_list = dma_noshare.split()

    try:
        lib = importlib.import_module(mcu_type)
        dma_map = lib.DMA_Map
    except ImportError:
        print("Unable to find module for MCU %s" % mcu_type)
        sys.exit(1)

    print("Writing DMA map")
    unassigned = []
    curr_dict = {}

    for periph in peripheral_list:
        if periph in dma_exclude:
            continue
        assigned = False
        check_list = []
        if not periph in dma_map:
            print("Unknown peripheral function %s in DMA map for %s" %
                  (periph, mcu_type))
            sys.exit(1)
        for streamchan in dma_map[periph]:
            stream = (streamchan[0], streamchan[1])
            if check_possibility(periph, stream, curr_dict, dma_map,
                                 check_list):
                curr_dict[periph] = stream
                assigned = True
                break
        if assigned == False:
            unassigned.append(periph)

    # now look for shared DMA possibilities
    stream_assign = {}
    for k in curr_dict.keys():
        stream_assign[curr_dict[k]] = [k]

    unassigned_new = unassigned[:]
    for periph in unassigned:
        share_possibility = []
        for streamchan in dma_map[periph]:
            stream = (streamchan[0], streamchan[1])
            share_ok = True
            for periph2 in stream_assign[stream]:
                if not can_share(periph, noshare_list) or not can_share(periph2, noshare_list):
                    share_ok = False
            if share_ok:
                share_possibility.append(stream)
        if share_possibility:
            # sort the possible sharings so minimise impact on high priority streams
            share_possibility = sorted(share_possibility, key=lambda x: get_sharing_priority(stream_assign[x], priority_list))
            # and take the one with the least impact (lowest value for highest priority stream share)
            stream = share_possibility[-1]
            if debug:
                print("Sharing %s on %s with %s" % (periph, stream,
                                                    stream_assign[stream]))
            curr_dict[periph] = stream
            stream_assign[stream].append(periph)
            unassigned_new.remove(periph)
    unassigned = unassigned_new

    f.write("\n\n// auto-generated DMA mapping from dma_resolver.py\n")

    if unassigned:
        f.write(
            "\n// Note: The following peripherals can't be resolved for DMA: %s\n\n"
            % unassigned)

    for key in sorted(curr_dict.keys()):
        stream = curr_dict[key]
        shared = ''
        if len(stream_assign[stream]) > 1:
            shared = ' // shared %s' % ','.join(stream_assign[stream])
        f.write("#define %-30s STM32_DMA_STREAM_ID(%u, %u)%s\n" %
                (chibios_dma_define_name(key)+'STREAM', curr_dict[key][0],
                 curr_dict[key][1], shared))
        for streamchan in dma_map[key]:
            if stream == (streamchan[0], streamchan[1]):
                f.write("#define %-30s %u\n" %
                        (chibios_dma_define_name(key)+'CHAN', streamchan[2]))
                break

# now generate UARTDriver.cpp DMA config lines
    f.write("\n\n// generated UART DMA configuration lines\n")
    for u in range(1, 9):
        key = None
        if 'USART%u_TX' % u in peripheral_list:
            key = 'USART%u' % u
        if 'UART%u_TX' % u in peripheral_list:
            key = 'UART%u' % u
        if 'USART%u_RX' % u in peripheral_list:
            key = 'USART%u' % u
        if 'UART%u_RX' % u in peripheral_list:
            key = 'UART%u' % u
        if key is None:
            continue
        f.write("#define STM32_%s_RX_DMA_CONFIG " % key)
        if key + "_RX" in curr_dict:
            f.write(
                "true, STM32_UART_%s_RX_DMA_STREAM, STM32_%s_RX_DMA_CHN\n" %
                (key, key))
        else:
            f.write("false, 0, 0\n")
        f.write("#define STM32_%s_TX_DMA_CONFIG " % key)
        if key + "_TX" in curr_dict:
            f.write(
                "true, STM32_UART_%s_TX_DMA_STREAM, STM32_%s_TX_DMA_CHN\n" %
                (key, key))
        else:
            f.write("false, 0, 0\n")

if __name__ == '__main__':
    import optparse

    parser = optparse.OptionParser("dma_resolver.py")
    parser.add_option("-M", "--mcu", default=None, help='MCU type')
    parser.add_option(
        "-D", "--debug", action='store_true', help='enable debug')
    parser.add_option(
        "-P",
        "--peripherals",
        default=None,
        help='peripheral list (comma separated)')

    opts, args = parser.parse_args()

    if opts.peripherals is None:
        print("Please provide a peripheral list with -P")
        sys.exit(1)

    if opts.mcu is None:
        print("Please provide a MCU type with -<")
        sys.exit(1)

    debug = opts.debug

    plist = opts.peripherals.split(',')
    mcu_type = opts.mcu

    f = open("dma.h", "w")
    write_dma_header(f, plist, mcu_type)

