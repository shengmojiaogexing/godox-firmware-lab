/* ###
 * ExportAll.java - post-analysis script for Godox AD100 Pro V1.2 firmware.
 *
 * Decompiles every function in the program with Ghidra's DecompInterface and
 * writes the following files under <outDir>:
 *
 *   functions.tsv              addr \t size \t name \t ncallers \t signature
 *   decompiled/<addr>_<name>.c one file per function (header comment + C body)
 *   decompile_all.c            all functions concatenated, ordered by address
 *   strings.tsv                ASCII / UTF-16LE strings found in memory
 *   refs.tsv                   call/jump references between functions
 *   memory_map.md              memory block table
 *   export_summary.txt         export statistics
 *
 * Script arguments:  <outDir>   (required)
 *
 * @category Ghidra
 */

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.SourceType;

public class ExportAll extends GhidraScript {

    /** Address of main() in the AD100 Pro V1.2 image (supplied by prior analysis). */
    private static final long MAIN_ADDR = 0x0800CF74L;
    /** Per-function decompiler timeout in seconds. */
    private static final int DECOMP_TIMEOUT = 60;

    private static String hex(long v) {
        return String.format("0x%08X", v);
    }

    private static String addr(Function f) {
        return hex(f.getEntryPoint().getOffset());
    }

    private static String sanitize(String s) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                    || (c >= '0' && c <= '9') || c == '_' || c == '.' || c == '-') {
                sb.append(c);
            }
            else {
                sb.append('_');
            }
        }
        return sb.toString();
    }

    private static PrintWriter openWriter(File f) throws Exception {
        return new PrintWriter(new OutputStreamWriter(new FileOutputStream(f),
                StandardCharsets.UTF_8));
    }

    private static String esc(String s, boolean tsv) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            switch (c) {
                case '\\': sb.append("\\\\"); break;
                case '\n': sb.append("\\n"); break;
                case '\r': sb.append("\\r"); break;
                case '\t': sb.append(tsv ? "\\t" : "    "); break;
                default:
                    if (c < 0x20) {
                        sb.append(String.format("\\x%02x", (int) c));
                    }
                    else {
                        sb.append(c);
                    }
            }
        }
        return sb.toString();
    }

    private String callerList(Function f, List<Function> cache) {
        StringBuilder sb = new StringBuilder();
        for (Function c : cache) {
            if (sb.length() > 0) {
                sb.append(", ");
            }
            sb.append(addr(c)).append("(").append(c.getName()).append(")");
        }
        return sb.toString();
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            printerr("ExportAll: usage: ExportAll.java <outDir>");
            return;
        }
        File outDir = new File(args[0]);
        File decDir = new File(outDir, "decompiled");
        if (!decDir.exists() && !decDir.mkdirs()) {
            printerr("ExportAll: cannot create " + decDir);
            return;
        }
        println("ExportAll: output dir = " + outDir.getAbsolutePath());

        // ---- make sure main() has its well known name ------------------------
        int txId = currentProgram.startTransaction("ExportAll: rename main");
        try {
            Address mainAddr = toAddr(MAIN_ADDR);
            Function mf = getFunctionAt(mainAddr);
            if (mf == null) {
                // main may not have been reached by auto analysis; create it now.
                try {
                    disassemble(mainAddr);
                }
                catch (Exception e) {
                    println("ExportAll: disassemble(main) failed: " + e);
                }
                try {
                    mf = createFunction(mainAddr, "main");
                }
                catch (Exception e) {
                    println("ExportAll: createFunction(main) failed: " + e);
                }
                println("ExportAll: created function main at " + hex(MAIN_ADDR));
            }
            else {
                mf.setName("main", SourceType.USER_DEFINED);
                println("ExportAll: renamed existing function at " + hex(MAIN_ADDR) + " -> main");
            }
        }
        finally {
            currentProgram.endTransaction(txId, true);
        }

        DecompInterface decomp = new DecompInterface();
        decomp.toggleCCode(true);
        decomp.toggleSyntaxTree(false);
        decomp.setSimplificationStyle("decompile");
        if (!decomp.openProgram(currentProgram)) {
            printerr("ExportAll: DecompInterface.openProgram failed");
            return;
        }

        FunctionManager fm = currentProgram.getFunctionManager();
        Listing listing = currentProgram.getListing();
        List<Function> funcs = new ArrayList<>();
        for (Function f : fm.getFunctions(true)) {
            funcs.add(f);
        }
        funcs.sort(Comparator.comparingLong(f -> f.getEntryPoint().getOffset()));
        println("ExportAll: function count = " + funcs.size());

        PrintWriter tsv = openWriter(new File(outDir, "functions.tsv"));
        PrintWriter all = openWriter(new File(outDir, "decompile_all.c"));
        all.println("/*");
        all.println(" * Godox AD100 Pro V1.2 firmware - complete Ghidra decompilation");
        all.println(" * binary: AD100pro_V1.2.bin  base: 0x08003000  size: 0xC448");
        all.println(" * generated by Ghidra 12.2 analyzeHeadless + ExportAll.java");
        all.println(" */");
        all.println();

        int total = 0;
        int nonEmpty = 0;
        int failed = 0;
        int thunks = 0;
        long totalBytes = 0;
        List<String> milestones = new ArrayList<>();

        for (Function f : funcs) {
            total++;
            String name = f.getName();
            String a = addr(f);
            long sizeBytes = f.getBody().getNumAddresses();
            totalBytes += sizeBytes;
            if (f.isThunk()) {
                thunks++;
            }
            Set<Function> callerSet = f.getCallingFunctions(monitor);
            List<Function> callers = new ArrayList<>();
            if (callerSet != null) {
                callers.addAll(callerSet);
                callers.sort(Comparator.comparingLong(x -> x.getEntryPoint().getOffset()));
            }
            Set<Function> calleeSet = f.getCalledFunctions(monitor);
            List<Function> callees = new ArrayList<>();
            if (calleeSet != null) {
                callees.addAll(calleeSet);
                callees.sort(Comparator.comparingLong(x -> x.getEntryPoint().getOffset()));
            }

            int insnCount = 0;
            InstructionIterator it = listing.getInstructions(f.getBody(), true);
            while (it.hasNext()) {
                it.next();
                insnCount++;
            }

            String signature;
            try {
                signature = f.getPrototypeString(false, false);
            }
            catch (Exception e) {
                signature = "void " + name + "(void)";
            }

            String code = "";
            String err = "";
            try {
                DecompileResults res = decomp.decompileFunction(f, DECOMP_TIMEOUT, monitor);
                if (res != null && res.decompileCompleted()) {
                    if (res.getDecompiledFunction() != null) {
                        code = res.getDecompiledFunction().getC();
                    }
                    else {
                        err = "no DecompiledFunction";
                    }
                }
                else {
                    err = (res == null) ? "decompileFunction returned null" : res.getErrorMessage();
                }
            }
            catch (Exception e) {
                err = e.toString();
            }
            if (code == null) {
                code = "";
            }
            boolean empty = code.trim().isEmpty();
            String bodyCheck = code;
            int b1 = bodyCheck.indexOf('{');
            int b2 = bodyCheck.lastIndexOf('}');
            boolean emptyBody = true;
            if (b1 >= 0 && b2 > b1) {
                emptyBody = bodyCheck.substring(b1 + 1, b2).trim().isEmpty();
            }
            if (!empty && !emptyBody) {
                nonEmpty++;
            }
            if (!err.isEmpty()) {
                failed++;
                code = "/* Ghidra decompilation failed for " + a + " " + name + ": "
                        + esc(err, false) + " */\n";
                emptyBody = true;
            }

            // ---- functions.tsv --------------------------------------------
            tsv.println(a + "\t" + sizeBytes + "\t" + name + "\t" + callers.size()
                    + "\t" + esc(signature, true));

            // ---- per function .c file -------------------------------------
            File cf = new File(decDir, a + "_" + sanitize(name) + ".c");
            PrintWriter pw = openWriter(cf);
            pw.println("/*");
            pw.println(" * Godox AD100 Pro V1.2 - decompiled function");
            pw.println(" * -------------------------------------------------------------");
            pw.println(" * Address   : " + a);
            pw.println(" * Name      : " + name);
            pw.println(" * Size      : " + sizeBytes + " bytes, " + insnCount + " instructions");
            pw.println(" * Signature : " + signature);
            pw.println(" * Callers   : " + callers.size());
            for (Function c : callers) {
                pw.println(" *             " + addr(c) + "  " + c.getName());
            }
            pw.println(" * Callees   : " + callees.size());
            for (Function c : callees) {
                pw.println(" *             " + addr(c) + "  " + c.getName());
            }
            pw.println(" * -------------------------------------------------------------");
            pw.println(" */");
            pw.println();
            pw.println(code);
            pw.close();

            // ---- combined output ------------------------------------------
            all.println("/* =====================================================================");
            all.println(" * " + a + "  " + name + "  (" + sizeBytes + " bytes, " + insnCount
                    + " insns, " + callers.size() + " callers)");
            all.println(" * " + signature);
            if (!emptyBody) {
                all.println(" * ===================================================================== */");
            }
            else {
                all.println(" * ===================================================================== */");
            }
            all.println(code);
            all.println();
        }

        tsv.close();
        all.close();
        decomp.dispose();

        // ---- strings.tsv ------------------------------------------------------
        int stringCount = writeStrings(new File(outDir, "strings.tsv"));
        // ---- refs.tsv ---------------------------------------------------------
        int refCount = writeRefs(new File(outDir, "refs.tsv"));
        // ---- memory_map.md ----------------------------------------------------
        writeMemoryMap(new File(outDir, "memory_map.md"));

        // ---- summary ----------------------------------------------------------
        double pct = total == 0 ? 0.0 : (100.0 * nonEmpty / total);
        PrintWriter sum = openWriter(new File(outDir, "export_summary.txt"));
        sum.println("Ghodra export summary for AD100pro_V1.2.bin (base 0x08003000)");
        sum.println("functions total      : " + total);
        sum.println("functions non-empty  : " + nonEmpty + " (" + String.format("%.1f", pct) + "%)");
        sum.println("functions failed     : " + failed);
        sum.println("thunks               : " + thunks);
        sum.println("total body bytes     : " + totalBytes);
        sum.println("strings              : " + stringCount);
        sum.println("call/jump refs       : " + refCount);
        sum.println("decompiled dir       : " + decDir.getAbsolutePath());
        sum.close();
        println("ExportAll: total=" + total + " nonEmpty=" + nonEmpty + " (" + String.format("%.1f", pct)
                + "%) failed=" + failed + " strings=" + stringCount + " refs=" + refCount);
    }

    /* ------------------------------------------------------------------ */
    /* strings.tsv                                                         */
    /* ------------------------------------------------------------------ */
    private int writeStrings(File f) throws Exception {
        PrintWriter pw = openWriter(f);
        Memory mem = currentProgram.getMemory();
        int count = 0;
        for (MemoryBlock block : mem.getBlocks()) {
            if (!block.isInitialized()) {
                continue;
            }
            long start = block.getStart().getOffset();
            long end = block.getEnd().getOffset();
            long pos = start;
            while (pos <= end) {
                // ASCII run
                StringBuilder sb = new StringBuilder();
                long p = pos;
                while (p <= end) {
                    int bv = mem.getByte(toAddr(p)) & 0xFF;
                    if (bv >= 0x20 && bv < 0x7F) {
                        sb.append((char) bv);
                        p++;
                    }
                    else {
                        break;
                    }
                }
                if (sb.length() >= 4) {
                    pw.println(hex(pos) + "\t" + sb.length() + "\tascii\t" + esc(sb.toString(), true));
                    count++;
                    pos = p;
                    continue;
                }
                // UTF-16LE run (printable ASCII chars with NUL high byte)
                StringBuilder u = new StringBuilder();
                long q = pos;
                while (q + 1 <= end) {
                    int lo = mem.getByte(toAddr(q)) & 0xFF;
                    int hi = mem.getByte(toAddr(q + 1)) & 0xFF;
                    if (hi == 0 && lo >= 0x20 && lo < 0x7F) {
                        u.append((char) lo);
                        q += 2;
                    }
                    else {
                        break;
                    }
                }
                if (u.length() >= 4) {
                    pw.println(hex(pos) + "\t" + u.length() + "\tutf16le\t" + esc(u.toString(), true));
                    count++;
                    pos = q;
                    continue;
                }
                pos++;
            }
        }
        pw.close();
        return count;
    }

    /* ------------------------------------------------------------------ */
    /* refs.tsv                                                            */
    /* ------------------------------------------------------------------ */
    private int writeRefs(File f) throws Exception {
        PrintWriter pw = openWriter(f);
        Listing listing = currentProgram.getListing();
        FunctionManager fm = currentProgram.getFunctionManager();
        int count = 0;
        InstructionIterator it = listing.getInstructions(true);
        while (it.hasNext()) {
            Instruction insn = it.next();
            Address from = insn.getAddress();
            Function fromFunc = fm.getFunctionContaining(from);
            String fromName = fromFunc == null ? "?" : fromFunc.getName();
            Reference[] refs = insn.getReferencesFrom();
            for (Reference r : refs) {
                if (r == null) {
                    continue;
                }
                Address to = r.getToAddress();
                if (to == null) {
                    continue;
                }
                boolean isCall = r.getReferenceType().isCall();
                boolean isJump = r.getReferenceType().isJump();
                if (!isCall && !isJump) {
                    continue;
                }
                Function toFunc = fm.getFunctionAt(to);
                String toName = toFunc == null ? "" : toFunc.getName();
                pw.println(hex(from.getOffset()) + "\t" + fromName + "\t"
                        + hex(to.getOffset()) + "\t" + toName + "\t"
                        + r.getReferenceType());
                count++;
            }
        }
        pw.close();
        return count;
    }

    /* ------------------------------------------------------------------ */
    /* memory_map.md                                                       */
    /* ------------------------------------------------------------------ */
    private void writeMemoryMap(File f) throws Exception {
        PrintWriter pw = openWriter(f);
        pw.println("# Memory map - AD100pro_V1.2.bin");
        pw.println();
        pw.println("| Block | Start | End | Size | Initialized | R | W | X | Source |");
        pw.println("|-------|-------|-----|------|-------------|---|---|---|--------|");
        Memory mem = currentProgram.getMemory();
        for (MemoryBlock b : mem.getBlocks()) {
            pw.println("| " + b.getName() + " | " + hex(b.getStart().getOffset())
                    + " | " + hex(b.getEnd().getOffset())
                    + " | " + b.getSize()
                    + " | " + b.isInitialized()
                    + " | " + b.isRead()
                    + " | " + b.isWrite()
                    + " | " + b.isExecute()
                    + " | " + b.getSourceInfos() + " |");
        }
        pw.println();
        pw.close();
    }
}
