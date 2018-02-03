all : exes

CFLAGS = -g -O0 -Wall -std=c99 -U__STRICT_ANSI__
LDFLAGS = -g

EXES =
OBJS =

EXES += extract_tape_files
OBJS += extract_tape_files.o
extract_tape_files : extract_tape_files.o bit36.o simhtapes.o
extract_tape_files.o : extract_tape_files.c bit36.h simhtapes.h mst.h


EXES += mfile
OBJS += mfile.o
mfile : mfile.o bit36.o
mfile.o : mfile.c bit36.h mst.h


EXES += dumpTape
DT_OBJS = dumpTape.o 
OBJS += $(DT_OBJS)
dumpTape : $(DT_OBJS) bit36.o
dumpTape.o : dumpTape.c bit36.h

EXES += dumpFile
OBJS += dumpFile.o 
dumpFile : dumpFile.o bit36.o
dumpFile.o : dumpFile.c bit36.h

EXES +=  dumpFile6 
OBJS += dumpFile6.o
dumpFile6 : dumpFile6.o bit36.o
dumpFile6.o : dumpFile6.c bit36.h

EXES +=  restore 
OBJS += restore.o
restore : restore.o bit36.o
restore.o : restore.c bit36.h mst.h

EXES +=  scan 
OBJS += scan.o
scan : scan.o bit36.o
scan.o : scan.c bit36.h mst.h

EXES +=  disasm 
OBJS += disasm.o
disasm : disasm.o bit36.o
disasm.o : disasm.c bit36.h

EXES +=  dis 
OBJS += dis.o
dis : dis.o bit36.o
dis.o : dis.c bit36.h

EXES +=  pack 
OBJS += pack.o
pack : pack.o bit36.o
pack.o : pack.c bit36.h

EXES +=  extract_bound_bootload_0 
OBJS += extract_bound_bootload_0.o
extract_bound_bootload_0 : extract_bound_bootload_0.o bit36.o
extract_bound_bootload_0.o : extract_bound_bootload_0.c mst.h bit36.h

EXES +=  unpack 
OBJS += unpack.o
unpack : unpack.o bit36.o
unpack.o : unpack.c bit36.h

EXES +=  mxarc 
OBJS += mxarc.o
mxarc : mxarc.o bit36.o
mxarc.o : mxarc.c bit36.h mst.h

EXES +=  print_bind_map 
OBJS += print_bind_map.o
print_bind_map : print_bind_map.o bit36.o
print_bind_map.o : print_bind_map.c bit36.h

EXES +=  pack9 
OBJS += pack9.o
pack9 : pack9.o bit36.o
pack9.o : pack9.c bit36.h

EXES +=  restore_toc 
OBJS += restore_toc.o
restore_toc: restore_toc.o bit36.o
restore_toc.o : restore_toc.c bit36.h mst.h

EXES +=  restore_tape 
OBJS += restore_tape.o simhtapes.o
restore_tape: restore_tape.o simhtapes.o bit36.o
restore_tape.o : restore_tape.c bit36.h mst.h simhtapes.h

EXES +=  p72_to_acsii 
OBJS += p72_to_acsii.o
p72_to_acsii: p72_to_acsii.o bit36.o
p72_to_acsii.o: p72_to_acsii.c bit36.h

EXES +=  p72archive_to_acsii 
OBJS += p72archive_to_acsii.o
p72archive_to_acsii: p72archive_to_acsii.o bit36.o
p72archive_to_acsii.o: p72archive_to_acsii.c bit36.h

EXES +=  p72archive_to_segment 
OBJS += p72archive_to_segment.o
p72archive_to_segment: p72archive_to_segment.o bit36.o
p72archive_to_segment.o: p72archive_to_segment.c bit36.h

EXES +=  diskDump 
OBJS += diskDump.o
diskDump: diskDump.o bit36.o
diskDump.o: diskDump.c bit36.h

EXES +=  p72archive_bind_to_ascii 
OBJS += p72archive_bind_to_ascii.o
p72archive_bind_to_ascii: p72archive_bind_to_ascii.o bit36.o
p72archive_bind_to_ascii.o: p72archive_bind_to_ascii.c bit36.h

EXES +=  dumpDisk 
OBJS +=  dumpDisk.o
dumpDisk: dumpDisk.o bit36.o
dumpDisk.o: dumpDisk.c bit36.h

EXES +=  dumpDiskRecords 
OBJS +=  dumpDisk.o
dumpDiskRecords: dumpDiskRecords.o bit36.o
dumpDiskRecords.o: dumpDiskRecords.c bit36.h

EXES +=  purge 
OBJS +=  purge.o
purge : purge.o
purge.o : purge.c

EXES +=  dumpCard 
OBJS +=  dumpCard.o
dumpCard : dumpCard.o
dumpCard.o : dumpCard.c

EXES +=  viipunch 
OBJS +=  viipunch.o
viipunch : viipunch.o
viipunch.o : viipunch.c

EXES +=  pack8 
OBJS +=  pack8.o
pack8 : pack8.o bit36.o
pack8.c : pack8.c bit36.h

EXES +=  cat36 
OBJS +=  cat36.o
cat36: cat36.o bit36.o
cat36.o: cat36.c bit36.h

EXES +=  cdump 
OBJS +=  cdump.o
cdump: cdump.o
cdump.o: cdump.c

EXES +=  cdumpa 
OBJS +=  cdumpa.o
cdumpa: cdumpa.o
cdumpa.o: cdumpa.c 


EXES +=  reload 
OBJS +=  reload.o
reload: reload.o
reload.o: reload.c

EXES +=  p72_to_bcd
OBJS += p72_to_bcd.o
p72_to_bcd: p72_to_bcd.o bit36.o
p72_to_bcd.o: p72_to_bcd.c bit36.h

EXES += p72_to_binary
OBJS += p72_to_binary.o
p72_to_binary: p72_to_binary.o bit36.o
p72_to_binary.o: p72_to_binary.c bit36.h

OBJS += bit36.o 
bit36.o : bit36.c bit36.h

simhtapes.o : simhtapes.c simhtapes.h
OBJS += simhtapes.o 

exes : $(EXES)


clean:
	-@$(RM) -f $(OBJS)  $(EXES)

