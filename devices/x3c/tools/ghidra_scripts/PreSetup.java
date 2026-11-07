// PreSetup.java -- Ghidra pre-analysis script for raw ARM Cortex-M firmware images.
// Marks the vector table as pointer data, adds references, creates function starts
// for every non-zero vector target and seeds disassembly from the reset handler.
//
// Usage (analyzeHeadless):
//   -preScript PreSetup.java <vectorBaseHex> <vectorCount> <resetEntryHex>
// e.g. -preScript "PreSetup.java 0x08004800 131 0x08004AF9"
//
import java.io.File;
import java.util.ArrayList;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataUtilities;
import ghidra.program.model.data.DWordDataType;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.SymbolTable;

public class PreSetup extends GhidraScript {

    private static final long FLASH_LO = 0x08000000L;
    private static final long FLASH_HI = 0x08200000L;

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("PreSetup: need args <vectorBase> <count> [resetEntry]");
            return;
        }
        long base = Long.decode(args[0]);
        int count = Integer.parseInt(args[1]);
        Long reset = (args.length > 2) ? Long.decode(args[2]) : null;

        Memory mem = currentProgram.getMemory();
        Listing listing = currentProgram.getListing();
        SymbolTable st = currentProgram.getSymbolTable();
        ReferenceManager rm = currentProgram.getReferenceManager();
        DataType ptrDt = currentProgram.getDataTypeManager().getPointer(new DWordDataType());

        int created = 0;
        int zero = 0;
        ArrayList<Long> targets = new ArrayList<Long>();
        for (int i = 0; i < count; i++) {
            long addrVal = base + 4L * i;
            Address a = toAddr(addrVal);
            if (!mem.contains(a)) {
                println("PreSetup: vector " + i + " address outside memory: " + Long.toHexString(addrVal));
                continue;
            }
            long v = ((long) mem.getInt(a)) & 0xFFFFFFFFL;
            Data d = listing.getDataAt(a);
            if (d == null) {
                try {
                    DataUtilities.createData(currentProgram, a, ptrDt, -1, false,
                            DataUtilities.ClearDataMode.CLEAR_ALL_UNDEFINED_CONFLICT_DATA);
                } catch (Exception e) {
                    println("PreSetup: could not create pointer data at " + Long.toHexString(addrVal) + ": " + e);
                }
            }
            if (v == 0) {
                zero++;
                continue;
            }
            if (v >= FLASH_LO && v < FLASH_HI) {
                long t = v & ~1L;
                Address ta = toAddr(t);
                if (!mem.contains(ta)) {
                    println("PreSetup: vector " + i + " target outside memory: " + Long.toHexString(v));
                    continue;
                }
                targets.add(t);
                if (!rm.hasReferencesFrom(a)) {
                    try {
                        rm.addMemoryReference(a, ta, RefType.DATA, SourceType.USER_DEFINED, 0);
                    } catch (Exception e) {
                        println("PreSetup: ref fail " + Long.toHexString(addrVal) + ": " + e);
                    }
                }
            }
        }

        // create function starts for vector targets (analysis will expand bodies)
        int fcount = 0;
        for (long t : targets) {
            Address ta = toAddr(t);
            Function f = getFunctionAt(ta);
            if (f == null) {
                String name = null;
                if (reset != null && t == (reset & ~1L)) {
                    name = "Reset_Handler";
                }
                try {
                    f = createFunction(ta, name);
                    fcount++;
                } catch (Exception e) {
                    println("PreSetup: createFunction fail at " + Long.toHexString(t) + ": " + e);
                }
            }
            try {
                disassemble(ta);
            } catch (Exception e) {
                println("PreSetup: disassemble fail at " + Long.toHexString(t) + ": " + e);
            }
        }

        // set reset handler as external entry point too, and its name
        if (reset != null) {
            Address ra = toAddr(reset & ~1L);
            try {
                st.addExternalEntryPoint(ra);
            } catch (Exception e) {
                println("PreSetup: addExternalEntryPoint fail: " + e);
            }
            Function rf = getFunctionAt(ra);
            if (rf != null && rf.getName().startsWith("FUN_")) {
                try {
                    rf.setName("Reset_Handler", SourceType.USER_DEFINED);
                } catch (Exception e) {
                    println("PreSetup: rename reset fail: " + e);
                }
            }
            try {
                disassemble(ra);
            } catch (Exception e) {
                println("PreSetup: disassemble reset fail: " + e);
            }
        }

        println("PreSetup: vectors=" + count + " zero=" + zero + " targets=" + targets.size()
                + " functions_created=" + fcount);
    }
}
