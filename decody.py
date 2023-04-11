def to_bin(h):
    return format(h, '016b')

def walk_map(m):
    for (start_offset, identifier) in sorted(m, key=lambda p: p[0]):
        print(to_bin(start_offset) + ' ' + identifier)

        # identify set bits
        set_bits = []

        address = 0
        while address <= 15:
            if(start_offset) & 1 != 0:
                set_bits.append(address)
            address += 1 # increment address line
            start_offset = start_offset >> 1 # shift out the lowest bit

        formatted_addresses = map(lambda a: f'A{a}', set_bits)
        print(' bits set:', ', '.join(formatted_addresses))

def set_bits(address_lines):
	val = 0x0
	for address_line in address_lines:
		val |= (0x1 << address_line)
	return val

memory_map = [
    ( 0x0, 'work RAM' ),
    ( set_bits([12]), 'PIA' ),
    ( set_bits([13]), 'VDP Read' ),
    ( set_bits([12, 13]), 'VDP Write'),
    ( set_bits([14]), 'ROM 2'),
    ( set_bits([15]), 'ROM 1'),
    ( set_bits([14, 15]), 'ROM 0')
]

print('Memory Map:')
walk_map(memory_map)

#print('I/O Map:')
#walk_map(io_map)
