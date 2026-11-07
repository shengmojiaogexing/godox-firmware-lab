/* ###
 * InitFirmware.java - pre-analysis script for Godox AD100 Pro V1.2 firmware (STM32F0 / Cortex-M0)
 *
 * The firmware is imported as a raw binary:
 *   - processor  ARM:LE:32:Cortex
 *   - loader     BinaryLoader
 *   - base       0x08003000   (application image; 0x08000000..0x08002FFF is the
 *                              bootloader which is NOT part of the supplied file)
 *
 * It stores a 48-entry vector table at 0x08003000 (16 system + 32 IRQ).  For each
 * non-zero entry this script:
 *   1. masks the Thumb bit (bit0),
 *   2. sets the TMode context register to 1 so Ghidra disassembles Thumb code,
 *   3. disassembles the target,
 *   4. creates a function and gives it the canonical CMSIS handler name,
 *   5. marks the address as an external entry point.
 *
 * The named entry points give the auto-analysis a solid root set for following
 * BL/BX calls through the whole image.
 *
 * @category Ghidra
 */

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.SourceType;

public class InitFirmware extends GhidraScript {

    /** Base address of the supplied application image (not 0x08000000!). */
    private static final long BASE = 0x08003000L;
    /** Size of the supplied image in bytes (AD100pro_V1.2.bin = 50248 = 0xC448). */
    private static final long IMG_SIZE = 0xC448L;
    /** Vector table entry count (16 system + 32 IRQ). */
    private static final int VEC_COUNT = 48;

    /** Canonical CMSIS names for the 16 system vectors (index 0 = initial SP). */
    private static final String[] SYSTEM_NAMES = {
        "Initial_SP",          // 0  (not a function)
        "Reset_Handler",       // 1
        "NMI_Handler",         // 2
        "HardFault_Handler",   // 3
        "MemManage_Handler",   // 4
        "BusFault_Handler",    // 5
        "UsageFault_Handler",  // 6
        "Reserved7",           // 7
        "Reserved8",           // 8
        "Reserved9",           // 9
        "Reserved10",          // 10
        "SVC_Handler",         // 11
        "DebugMon_Handler",    // 12
        "Reserved13",          // 13
        "PendSV_Handler",      // 14
        "SysTick_Handler"      // 15
    };

    private String vectorName(int i) {
        if (i < SYSTEM_NAMES.length) {
            return SYSTEM_NAMES[i];
        }
        return "IRQ" + (i - 16) + "_Handler";
    }

    @Override
    public void run() throws Exception {
        Address imageStart = toAddr(BASE);
        Address imageEnd = toAddr(BASE + IMG_SIZE - 1);
        println("InitFirmware: image range 0x" + Long.toHexString(BASE)
                + " - 0x" + Long.toHexString(BASE + IMG_SIZE - 1));

        Memory mem = currentProgram.getMemory();
        // Basic sanity checks on the imported block.
        if (mem.getBlock(imageStart) == null) {
            printerr("InitFirmware: no memory block containing 0x" + Long.toHexString(BASE));
            return;
        }
        println("InitFirmware: block = " + mem.getBlock(imageStart).getName()
                + " start=0x" + mem.getBlock(imageStart).getStart()
                + " size=" + mem.getBlock(imageStart).getSize());

        int txId = currentProgram.startTransaction("InitFirmware: vector table -> functions");
        int created = 0;
        try {
            Register tmode = currentProgram.getProgramContext().getRegister("TMode");
            if (tmode == null) {
                printerr("InitFirmware: WARNING - TMode context register not found");
            } else {
                println("InitFirmware: TMode register = " + tmode.getName());
            }

            // Collect entries first: (target address, canonical name, vector index)
            List<long[]> targets = new ArrayList<>();
            List<String> names = new ArrayList<>();
            for (int i = 1; i < VEC_COUNT; i++) {
                Address slot = imageStart.add(i * 4L);
                long word;
                try {
                    word = mem.getInt(slot) & 0xFFFFFFFFL;
                }
                catch (Exception e) {
                    printerr("InitFirmware: cannot read vector[" + i + "]: " + e);
                    continue;
                }
                if (word == 0) {
                    continue;
                }
                long target = word & ~1L; // strip Thumb bit
                if (target < BASE || target > BASE + IMG_SIZE - 1) {
                    printerr(String.format(
                        "InitFirmware: vector[%d] = 0x%08X points outside the image, skipped",
                        i, word));
                    continue;
                }
                targets.add(new long[] { target, i });
                names.add(vectorName(i));
                println(String.format("InitFirmware: vector[%2d] 0x%08X -> 0x%08X  %s",
                        i, word, target, vectorName(i)));
            }

            for (int k = 0; k < targets.size(); k++) {
                long target = targets.get(k)[0];
                int idx = (int) targets.get(k)[1];
                String name = names.get(k);
                Address entry = toAddr(target);

                // Thumb code: TMode = 1 is required for correct disassembly.
                if (tmode != null) {
                    currentProgram.getProgramContext().setValue(tmode, entry, entry,
                            BigInteger.ONE);
                }

                // Disassemble first (CreateFunctionCmd also disassembles, but doing it
                // explicitly gives us a clear log entry and a stable root).
                DisassembleCommand dcmd = new DisassembleCommand(entry, null, true);
                dcmd.applyTo(currentProgram, monitor);

                if (getFunctionAt(entry) == null) {
                    CreateFunctionCmd ccmd = new CreateFunctionCmd(entry);
                    ccmd.applyTo(currentProgram, monitor);
                }
                Function f = getFunctionAt(entry);
                if (f == null) {
                    printerr("InitFirmware: failed to create function at 0x"
                            + Long.toHexString(target) + " (vector " + idx + ")");
                    continue;
                }
                // A shared default handler may be referenced by several vectors; keep
                // the first canonical name, append aliases as labels.
                if (f.getName().startsWith("FUN_") || f.getName().startsWith("thunk_")) {
                    f.setName(name, SourceType.USER_DEFINED);
                } else if (!f.getName().equals(name)) {
                    try {
                        createLabel(entry, name, true);
                    }
                    catch (Exception e) {
                        // duplicate label - ignore
                    }
                }
                try {
                    currentProgram.getSymbolTable().addExternalEntryPoint(entry);
                }
                catch (Exception e) {
                    // duplicate / invalid - ignore
                }
                created++;
            }
            println("InitFirmware: processed " + created + " vector functions");
        }
        finally {
            currentProgram.endTransaction(txId, true);
        }
    }
}
