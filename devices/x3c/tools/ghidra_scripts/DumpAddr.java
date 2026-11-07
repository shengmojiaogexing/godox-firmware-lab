// DumpAddr.java -- print decompiled C + disassembly for given addresses.
// Usage: -postScript "DumpAddr.java <outFile> <addr> [<addr> ...]"
// addresses are hex (0x prefix optional).
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.util.task.ConsoleTaskMonitor;

public class DumpAddr extends GhidraScript {

    static String hex(Address a) {
        return String.format("0x%08x", a.getOffset());
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("DumpAddr: usage <outFile> <addr>...");
            return;
        }
        PrintWriter pw = new PrintWriter(new OutputStreamWriter(new FileOutputStream(args[0]), "UTF-8"));
        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor tm = new ConsoleTaskMonitor();
        for (int i = 1; i < args.length; i++) {
            String s = args[i].trim();
            long v;
            try {
                v = Long.parseLong(s.replaceFirst("^0[xX]", ""), 16);
            } catch (Exception e) {
                pw.println("### " + s);
                pw.println("// bad address");
                continue;
            }
            Address a = toAddr(v & 0xFFFFFFFFL);
            pw.println();
            pw.println("==================================================================");
            pw.println("### " + hex(a));
            if (!currentProgram.getMemory().contains(a)) {
                pw.println("// address not in memory");
                continue;
            }
            Function f = getFunctionContaining(a);
            if (f == null) f = getFunctionAt(a);
            if (f == null && getInstructionAt(a) == null) {
                try {
                    disassemble(toAddr(v & ~1L));
                } catch (Exception e) {
                    pw.println("// disassemble failed: " + e);
                }
                f = getFunctionContaining(a);
                if (f == null) f = getFunctionAt(a);
            }
            if (f == null) {
                Instruction in = getInstructionAt(a);
                if (in != null) {
                    pw.println("// no function; instruction: " + in.toString());
                } else {
                    pw.println("// no function and no instruction");
                }
                continue;
            }
            pw.println("// function: " + f.getName() + "  entry=" + hex(f.getEntryPoint()) + "  size="
                    + f.getBody().getNumAddresses() + "  thunk=" + f.isThunk());
            try {
                DecompileResults res = ifc.decompileFunction(f, 120, tm);
                if (res != null && res.decompileCompleted()) {
                    pw.println(res.getDecompiledFunction().getC());
                } else {
                    pw.println("// decompile failed: " + (res == null ? "null result" : res.getErrorMessage()));
                }
            } catch (Exception e) {
                pw.println("// decompile exception: " + e);
            }
            pw.println("// ---- disassembly (max 150 insns) ----");
            InstructionIterator it = currentProgram.getListing().getInstructions(f.getBody(), true);
            int c = 0;
            while (it.hasNext() && c < 150) {
                Instruction in = it.next();
                pw.println(in.getAddress() + "  " + in.toString());
                c++;
            }
        }
        pw.close();
        ifc.dispose();
        println("DumpAddr: wrote " + args[0]);
    }
}
