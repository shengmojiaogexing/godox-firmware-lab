// VectorCheck.java -- validate the Cortex-M vector table against Ghidra's function database.
// Usage: -postScript "VectorCheck.java <outDir> <vectorCount>"
// Writes <outDir>/vector_table.csv and prints summary counts.
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;

public class VectorCheck extends GhidraScript {

    private static final long FLASH_LO = 0x08000000L;
    private static final long FLASH_HI = 0x08200000L;

    static String hex(long v) {
        return String.format("0x%08x", v & 0xFFFFFFFFL);
    }

    static String hex(Address a) {
        return String.format("0x%08x", a.getOffset());
    }

    static String csv(String s) {
        if (s == null) return "";
        if (s.indexOf(',') >= 0 || s.indexOf('"') >= 0 || s.indexOf('\n') >= 0) {
            return "\"" + s.replace("\"", "\"\"") + "\"";
        }
        return s;
    }

    String symAt(Address a) {
        SymbolTable st = currentProgram.getSymbolTable();
        Symbol[] syms = st.getSymbols(a); if (syms != null && syms.length > 0) return syms[0].getName();
        return "";
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String outDir = args.length > 0 ? args[0] : ".";
        int count = args.length > 1 ? Integer.parseInt(args[1]) : 131;
        Memory mem = currentProgram.getMemory();
        long vbase = mem.getMinAddress().getOffset();
        PrintWriter out = new PrintWriter(new OutputStreamWriter(
                new FileOutputStream(outDir + "/vector_table.csv"), "UTF-8"));
        out.println("index,vector_addr,raw_value,target,valid,function_at_target,containing_function,"
                + "is_function_start,insn_at_target,default_loop,symbol");
        int valid = 0, fnStart = 0, defaultLoops = 0, midFunction = 0;
        for (int i = 0; i < count; i++) {
            Address va = toAddr(vbase + 4L * i);
            if (!mem.contains(va)) break;
            long raw = ((long) mem.getInt(va)) & 0xFFFFFFFFL;
            String target = "", validStr = "no", fnAt = "", containing = "", isStart = "";
            String insnAt = "", defaultLoop = "", sym = "";
            if (raw >= FLASH_LO && raw < FLASH_HI) {
                long t = raw & ~1L;
                Address ta = toAddr(t);
                target = hex(t);
                validStr = mem.contains(ta) ? "yes" : "no";
                if (validStr.equals("yes")) valid++;
                Function fa = getFunctionAt(ta);
                if (fa != null) {
                    fnAt = fa.getName();
                    isStart = "yes";
                    fnStart++;
                } else {
                    Function fc = getFunctionContaining(ta);
                    if (fc != null) {
                        containing = hex(fc.getEntryPoint()) + " " + fc.getName();
                        midFunction++;
                    }
                }
                Instruction in = getInstructionAt(ta);
                if (in != null) {
                    insnAt = in.toString();
                    if (in.getMnemonicString().equals("b") && in.getNumOperands() == 1) {
                        Object[] oo = in.getOpObjects(0);
                        if (oo != null && oo.length == 1 && oo[0] instanceof Address) {
                            if (((Address) oo[0]).getOffset() == t) {
                                defaultLoop = "yes";
                                defaultLoops++;
                            }
                        }
                    }
                }
                sym = symAt(ta);
            } else if (i == 0 && raw >= 0x20000000L && raw < 0x20100000L) {
                target = hex(raw);
                validStr = "sp";
            }
            out.println(i + "," + hex(va) + "," + hex(raw) + "," + target + "," + validStr + "," + csv(fnAt) + ","
                    + csv(containing) + "," + isStart + "," + csv(insnAt) + "," + defaultLoop + "," + csv(sym));
        }
        out.close();
        println("VectorCheck: count=" + count + " valid_flash=" + valid + " function_start=" + fnStart
                + " mid_function=" + midFunction + " default_b_loops=" + defaultLoops);
    }
}
