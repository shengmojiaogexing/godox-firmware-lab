// ExportAll.java -- export a full analysis database from a Ghidra program.
// Usage: -postScript "ExportAll.java <outDir>"
// Produces: functions.csv, decompiled/<addr>_<name>.c, strings.csv, datarefs.csv,
//           callgraph.json, memory_map.csv, vector_table.csv, gaps.csv, overview.json
//
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CodingErrorAction;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DWordDataType;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.util.task.ConsoleTaskMonitor;
import ghidra.util.task.TaskMonitor;

public class ExportAll extends GhidraScript {

    private static final long FLASH_LO = 0x08000000L;
    private static final long FLASH_HI = 0x08200000L;
    private static final long RAM_LO = 0x20000000L;
    private static final long RAM_HI = 0x20100000L;
    private static final long PERIPH_LO = 0x40000000L;
    private static final long PERIPH_HI = 0x60000000L;
    private static final long SCS_LO = 0xE0000000L;
    private static final long SCS_HI = 0xE0100000L;
    private static final long FMC_LO = 0xA0000000L;
    private static final long FMC_HI = 0xA0001000L;

    private PrintWriter out;
    private String outDir;
    private TaskMonitor tm = new ConsoleTaskMonitor();
    private TreeMap<Long, String> stringMap = new TreeMap<Long, String>();
    private TreeMap<Long, String> stringKind = new TreeMap<Long, String>();

    static String hex(Address a) {
        return String.format("0x%08x", a.getOffset());
    }

    static String hex(long v) {
        return String.format("0x%08x", v & 0xFFFFFFFFL);
    }

    static String csv(String s) {
        if (s == null) return "";
        if (s.indexOf(',') >= 0 || s.indexOf('"') >= 0 || s.indexOf('\n') >= 0 || s.indexOf('\r') >= 0) {
            return "\"" + s.replace("\"", "\"\"") + "\"";
        }
        return s;
    }

    static String cls(long v) {
        long x = v & 0xFFFFFFFFL;
        if (x >= FLASH_LO && x < FLASH_HI) return "flash";
        if (x >= RAM_LO && x < RAM_HI) return "ram";
        if (x >= PERIPH_LO && x < PERIPH_HI) return "peripheral";
        if (x >= SCS_LO && x < SCS_HI) return "scs";
        if (x >= FMC_LO && x < FMC_HI) return "fmc";
        return "const";
    }

    static String escape(String s) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c == '\\') sb.append("\\\\");
            else if (c == '\n') sb.append("\\n");
            else if (c == '\r') sb.append("\\r");
            else if (c == '\t') sb.append("\\t");
            else if (c == '"') sb.append("\\\"");
            else if (c < 0x20) sb.append(String.format("\\x%02x", (int) c));
            else sb.append(c);
        }
        return sb.toString();
    }

    PrintWriter open(String name) throws IOException {
        return new PrintWriter(new OutputStreamWriter(new FileOutputStream(new File(outDir, name)), "UTF-8"));
    }

    String sanitize(String name) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < name.length(); i++) {
            char c = name.charAt(i);
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '.' || c == '-') {
                sb.append(c);
            } else {
                sb.append('_');
            }
        }
        String s = sb.toString();
        if (s.length() > 80) s = s.substring(0, 80);
        return s;
    }

    String funcNameAt(Address a) {
        Function f = getFunctionAt(a);
        if (f == null) f = getFunctionContaining(a);
        if (f == null) return null;
        return f.getName();
    }

    public void run() throws Exception {
        String[] args = getScriptArgs();
        outDir = (args.length > 0) ? args[0] : ".";
        File d = new File(outDir);
        d.mkdirs();
        new File(d, "decompiled").mkdirs();

        Memory mem = currentProgram.getMemory();
        Listing listing = currentProgram.getListing();
        FunctionManager fm = currentProgram.getFunctionManager();
        ReferenceManager rm = currentProgram.getReferenceManager();

        // ---------- 1. raw string scan (independent of Ghidra's analysis) ----------
        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isInitialized()) continue;
            long size = b.getSize();
            if (size <= 0 || size > 64L * 1024 * 1024) continue;
            byte[] buf = new byte[(int) size];
            try {
                mem.getBytes(b.getStart(), buf);
            } catch (Exception e) {
                continue;
            }
            scanStrings(buf, b.getStart().getOffset());
        }

        // ---------- 2. Ghidra-defined strings (override raw classification) ----------
        DataIterator di = listing.getDefinedData(true);
        while (di.hasNext() && !monitor.isCancelled()) {
            Data data = di.next();
            Object v = data.getValue();
            if (v instanceof String) {
                long a = data.getAddress().getOffset();
                stringMap.put(a, (String) v);
                stringKind.put(a, "ghidra:" + data.getDataType().getName());
            }
        }

        // ---------- 3. instructions: data refs from operands + refs ----------
        out = open("datarefs.csv");
        out.println("src_addr,kind,value,value_class,target_addr,target_kind,target_symbol");
        int insnCount = 0;
        InstructionIterator ii = listing.getInstructions(true);
        while (ii.hasNext() && !monitor.isCancelled()) {
            Instruction ins = ii.next();
            insnCount++;
            Address ia = ins.getAddress();
            int nops = ins.getNumOperands();
            for (int op = 0; op < nops; op++) {
                Object[] objs = ins.getOpObjects(op);
                if (objs == null) continue;
                for (Object o : objs) {
                    if (o instanceof ghidra.program.model.scalar.Scalar) {
                        long val = ((ghidra.program.model.scalar.Scalar) o).getUnsignedValue();
                        long x = val & 0xFFFFFFFFL;
                        if (x >= FLASH_LO && x < FLASH_HI) {
                            String tk = "unknown";
                            Address ta = toAddr(x);
                            if (listing.getInstructionAt(ta) != null) tk = "code";
                            else if (stringMap.containsKey(x)) tk = "string";
                            else if (listing.getDataAt(ta) != null) tk = "data";
                            else if (getFunctionAt(ta) != null) tk = "code";
                            out.println(hex(ia) + ",insn_scalar," + hex(x) + ",flash," + hex(x) + "," + tk + "," + csv(funcNameAt(ta)));
                        } else if (cls(x).equals("peripheral") || cls(x).equals("scs") || cls(x).equals("ram") || cls(x).equals("fmc")) {
                            out.println(hex(ia) + ",insn_scalar," + hex(x) + "," + cls(x) + "," + hex(x) + ",,"
                                    + csv(symbolAt(ta(x))));
                        }
                    } else if (o instanceof Address) {
                        Address ta = (Address) o;
                        long x = ta.getOffset();
                        String tk = "unknown";
                        if (listing.getInstructionAt(ta) != null) tk = "code";
                        else if (stringMap.containsKey(x)) tk = "string";
                        out.println(hex(ia) + ",insn_addr," + hex(x) + "," + cls(x) + "," + hex(x) + "," + tk + ","
                                + csv(funcNameAt(ta)));
                    }
                }
            }
        }
        out.close();

        // ---------- 4. literal pool scan (all 4-byte aligned words) ----------
        out = new PrintWriter(new OutputStreamWriter(
                new FileOutputStream(new File(outDir, "datarefs.csv"), true), "UTF-8"));
        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isInitialized()) continue;
            long start = b.getStart().getOffset();
            long size = b.getSize();
            if (size <= 0 || size > 64L * 1024 * 1024) continue;
            byte[] buf = new byte[(int) size];
            try {
                mem.getBytes(b.getStart(), buf);
            } catch (Exception e) {
                continue;
            }
            for (int off = 0; off + 4 <= buf.length; off += 4) {
                long v = (((long) buf[off]) & 0xFF) | ((((long) buf[off + 1]) & 0xFF) << 8)
                        | ((((long) buf[off + 2]) & 0xFF) << 16) | ((((long) buf[off + 3]) & 0xFF) << 24);
                String c = cls(v);
                if (c.equals("const")) continue;
                long src = start + off;
                String tk = "";
                String sym = "";
                if (c.equals("flash")) {
                    Address ta = toAddr(v);
                    if (listing.getInstructionAt(ta) != null) tk = "code";
                    else if (stringMap.containsKey(v)) tk = "string";
                    else {
                        Function f = getFunctionAt(ta);
                        if (f != null) {
                            tk = "funcstart";
                            sym = f.getName();
                        } else if (listing.getDataAt(ta) != null) tk = "data";
                    }
                }
                out.println(hex(src) + ",literal," + hex(v) + "," + c + "," + hex(v) + "," + tk + "," + csv(sym));
            }
        }
        out.close();

        // ---------- 5. functions.csv + decompiled/ + callgraph ----------
        List<Function> funcs = new ArrayList<Function>();
        FunctionIterator fi = fm.getFunctions(true);
        while (fi.hasNext()) funcs.add(fi.next());
        println("ExportAll: functions=" + funcs.size());

        DecompInterface ifc = new DecompInterface();
        ifc.toggleCCode(true);
        ifc.toggleSyntaxTree(false);
        ifc.openProgram(currentProgram);
        DecompileOptions dopts = new DecompileOptions();
        ifc.setOptions(dopts);

        out = open("functions.csv");
        out.println("addr,name,size,thunk,external,insn_count,param_count,caller_count,callers,callee_count,callees,comment");
        PrintWriter index = open("decompiled/_index.tsv");
        index.println("addr\tname\tsize\tfile");
        PrintWriter failed = open("decomp_failed.csv");
        failed.println("addr,name,reason");
        TreeMap<Long, String[]> nodes = new TreeMap<Long, String[]>();
        StringBuilder edges = new StringBuilder();
        int decompiled = 0, failedCount = 0, i = 0;
        for (Function f : funcs) {
            if (monitor.isCancelled()) break;
            long ea = f.getEntryPoint().getOffset();
            long size = f.getBody().getNumAddresses();
            Set<Function> callers = f.getCallingFunctions(tm);
            Set<Function> callees = f.getCalledFunctions(tm);
            StringBuilder cb = new StringBuilder();
            for (Function c : callers) {
                if (cb.length() > 0) cb.append(';');
                cb.append(hex(c.getEntryPoint())).append(' ').append(c.getName());
            }
            StringBuilder ce = new StringBuilder();
            for (Function c : callees) {
                if (ce.length() > 0) ce.append(';');
                ce.append(hex(c.getEntryPoint())).append(' ').append(c.getName());
                edges.append(edges.length() > 0 ? ",\n" : "")
                        .append("{\"from\":\"").append(hex(f.getEntryPoint())).append("\",\"to\":\"")
                        .append(hex(c.getEntryPoint())).append("\",\"to_name\":\"").append(jsonEsc(c.getName())).append("\"}");
            }
            int insnCountF = 0;
            InstructionIterator iit = listing.getInstructions(f.getBody(), true);
            while (iit.hasNext()) {
                iit.next();
                insnCountF++;
            }
            out.println(hex(f.getEntryPoint()) + "," + csv(f.getName()) + "," + size + "," + f.isThunk() + ","
                    + f.isExternal() + "," + insnCountF + "," + f.getParameterCount() + "," + callers.size() + ","
                    + csv(cb.toString()) + "," + callees.size() + "," + csv(ce.toString()) + ","
                    + csv(f.getComment()));
            nodes.put(ea, new String[] { hex(f.getEntryPoint()), f.getName(), Long.toString(size) });

            // decompile
            String reason = "ok";
            String code = null;
            if (f.isExternal()) {
                reason = "external";
            } else if (f.isThunk()) {
                reason = "thunk";
            } else {
                try {
                    DecompileResults res = ifc.decompileFunction(f, 90, tm);
                    if (res == null) reason = "null result";
                    else if (!res.decompileCompleted()) reason = "incomplete: " + res.getErrorMessage();
                    else code = res.getDecompiledFunction().getC();
                } catch (Exception e) {
                    reason = "exception: " + e;
                }
            }
            if (code != null) {
                String fname = hex(f.getEntryPoint()) + "_" + sanitize(f.getName()) + ".c";
                PrintWriter fo = new PrintWriter(new OutputStreamWriter(
                        new FileOutputStream(new File(outDir, "decompiled/" + fname)), "UTF-8"));
                fo.println("// " + hex(f.getEntryPoint()) + " " + f.getName() + " size=" + size);
                fo.print(code);
                fo.close();
                index.println(hex(f.getEntryPoint()) + "\t" + f.getName() + "\t" + size + "\t" + fname);
                decompiled++;
            } else {
                failed.println(hex(f.getEntryPoint()) + "," + csv(f.getName()) + "," + csv(reason));
                failedCount++;
            }
            if ((++i % 200) == 0) {
                println("ExportAll: decompiled " + i + "/" + funcs.size());
            }
        }
        out.close();
        index.close();
        failed.close();
        ifc.dispose();

        // callgraph.json
        PrintWriter cg = open("callgraph.json");
        cg.println("{\"program\":\"" + jsonEsc(currentProgram.getName()) + "\",\"nodes\":[");
        boolean first = true;
        for (Map.Entry<Long, String[]> e : nodes.entrySet()) {
            if (!first) cg.println(",");
            first = false;
            cg.print("{\"addr\":\"" + e.getValue()[0] + "\",\"name\":\"" + jsonEsc(e.getValue()[1]) + "\",\"size\":"
                    + e.getValue()[2] + "}");
        }
        cg.println("],\"edges\":[");
        cg.println(edges.toString());
        cg.println("]}");
        cg.close();

        // memory map
        out = open("memory_map.csv");
        out.println("name,start,end,size,initialized,read,write,execute");
        for (MemoryBlock b : mem.getBlocks()) {
            out.println(csv(b.getName()) + "," + hex(b.getStart()) + "," + hex(b.getEnd()) + "," + b.getSize() + ","
                    + b.isInitialized() + "," + b.isRead() + "," + b.isWrite() + "," + b.isExecute());
        }
        out.close();

        // ---------- 6. vector table analysis (first 0x400 bytes of program) ----------
        out = open("vector_table.csv");
        out.println("index,vector_addr,raw_value,target,valid,function_at_target,containing_function,is_function_start,insn_at_target,default_loop,symbol");
        long vbase = mem.getMinAddress().getOffset();
        for (int vi = 0; vi < 256; vi++) {
            Address va = toAddr(vbase + 4L * vi);
            if (!mem.contains(va)) break;
            long raw = ((long) mem.getInt(va)) & 0xFFFFFFFFL;
            if (vi > 0 && raw == 0 && vi > 160) break;
            String target = "", valid = "no", fnAt = "", containing = "", isStart = "", insnAt = "";
            String defaultLoop = "", sym = "";
            if (raw >= FLASH_LO && raw < FLASH_HI) {
                long t = raw & ~1L;
                Address ta = toAddr(t);
                target = hex(t);
                valid = mem.contains(ta) ? "yes" : "no";
                Function fa = getFunctionAt(ta);
                if (fa != null) { fnAt = fa.getName(); isStart = "yes"; }
                Function fc = getFunctionContaining(ta);
                if (fc != null) containing = hex(fc.getEntryPoint()) + " " + fc.getName();
                Instruction in = listing.getInstructionAt(ta);
                if (in != null) {
                    insnAt = in.toString();
                    if (in.getMnemonicString().equals("b") && in.getNumOperands() == 1) {
                        Object[] oo = in.getOpObjects(0);
                        if (oo != null && oo.length == 1 && oo[0] instanceof Address) {
                            if (((Address) oo[0]).getOffset() == t) defaultLoop = "yes";
                        }
                    }
                }
                sym = symbolAt(ta);
            } else if (vi == 0 && raw >= RAM_LO && raw < RAM_HI) {
                target = hex(raw);
                valid = "sp";
            }
            out.println(vi + "," + hex(va) + "," + hex(raw) + "," + target + "," + valid + "," + csv(fnAt) + ","
                    + csv(containing) + "," + isStart + "," + csv(insnAt) + "," + defaultLoop + "," + csv(sym));
        }
        out.close();

        // ---------- 7. strings.csv ----------
        out = open("strings.csv");
        out.println("addr,length,kind,text,xref_count,xrefs");
        int scount = 0;
        for (Map.Entry<Long, String> e : stringMap.entrySet()) {
            long sa = e.getKey();
            String text = e.getValue();
            if (text == null || text.length() == 0) continue;
            Address a = toAddr(sa);
            java.util.List<Reference> refList = new java.util.ArrayList<Reference>();
            try {
                ghidra.program.model.symbol.ReferenceIterator refIt = rm.getReferencesTo(a);
                while (refIt.hasNext()) {
                    refList.add(refIt.next());
                }
            } catch (Exception ex) {
                // ignore
            }
            StringBuilder rb = new StringBuilder();
            for (Reference r : refList) {
                if (rb.length() > 0) rb.append(';');
                String fn = funcNameAt(r.getFromAddress());
                rb.append(hex(r.getFromAddress()));
                if (fn != null) rb.append(' ').append(fn);
            }
            String kind = stringKind.containsKey(sa) ? stringKind.get(sa) : "raw";
            out.println(hex(sa) + "," + text.length() + "," + csv(kind) + "," + csv(escape(text)) + ","
                    + refList.size() + "," + csv(rb.toString()));
            scount++;
        }
        out.close();

        // ---------- 8. undefined-code gaps ----------
        out = open("gaps.csv");
        out.println("block,start,end,length,initialized");
        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isInitialized()) {
                out.println(csv(b.getName()) + "," + hex(b.getStart()) + "," + hex(b.getEnd()) + "," + b.getSize() + ",no");
                continue;
            }
            long size = b.getSize();
            if (size <= 0 || size > 64L * 1024 * 1024) continue;
            long start = b.getStart().getOffset();
            long gapStart = -1;
            long gapLen = 0;
            for (long off = 0; off < size; off++) {
                Address a = toAddr(start + off);
                CodeUnit cu = listing.getCodeUnitAt(a);
                if (cu == null) {
                    if (gapStart < 0) {
                        gapStart = start + off;
                        gapLen = 0;
                    }
                    gapLen++;
                } else {
                    if (gapLen >= 64) {
                        out.println(csv(b.getName()) + "," + hex(gapStart) + "," + hex(gapStart + gapLen - 1) + ","
                                + gapLen + ",yes");
                    }
                    gapStart = -1;
                    gapLen = 0;
                }
            }
            if (gapLen >= 64) {
                out.println(csv(b.getName()) + "," + hex(gapStart) + "," + hex(gapStart + gapLen - 1) + "," + gapLen
                        + ",yes");
            }
        }
        out.close();

        // ---------- 9. overview ----------
        PrintWriter ov = open("overview.json");
        ov.println("{");
        ov.println("  \"program\": \"" + jsonEsc(currentProgram.getName()) + "\",");
        ov.println("  \"language\": \"" + currentProgram.getLanguageID() + "\",");
        ov.println("  \"image_base\": \"" + hex(currentProgram.getImageBase()) + "\",");
        ov.println("  \"functions\": " + funcs.size() + ",");
        ov.println("  \"decompiled\": " + decompiled + ",");
        ov.println("  \"decompile_failed\": " + failedCount + ",");
        ov.println("  \"instructions\": " + insnCount + ",");
        ov.println("  \"strings\": " + scount + ",");
        ov.println("  \"memory_blocks\": " + mem.getBlocks().length);
        ov.println("}");
        ov.close();
        println("ExportAll: done functions=" + funcs.size() + " decompiled=" + decompiled + " failed=" + failedCount
                + " strings=" + scount + " insns=" + insnCount + " out=" + outDir);
    }

    private Address ta(long v) {
        return toAddr(v);
    }

    private String symbolAt(Address a) {
        Function f = getFunctionAt(a);
        if (f != null) return f.getName();
        try {
            ghidra.program.model.symbol.Symbol[] syms = currentProgram.getSymbolTable().getSymbols(a); if (syms != null && syms.length > 0) return syms[0].getName();
        } catch (Exception e) {
            // ignore
        }
        return "";
    }

    private static String jsonEsc(String s) {
        if (s == null) return "";
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c == '"') sb.append("\\\"");
            else if (c == '\\') sb.append("\\\\");
            else if (c < 0x20) sb.append(String.format("\\u%04x", (int) c));
            else sb.append(c);
        }
        return sb.toString();
    }

    // scan a buffer for printable ASCII / valid UTF-8 runs
    private void scanStrings(byte[] buf, long baseAddr) {
        int i = 0;
        int n = buf.length;
        while (i < n) {
            int b = buf[i] & 0xFF;
            if (isStringStart(b)) {
                int j = i;
                while (j < n && isStringByte(buf[j] & 0xFF)) j++;
                if (j - i >= 4) {
                    int start = i;
                    int len = j - i;
                    String s = decodeUtf8(buf, start, len);
                    if (s != null) {
                        long addr = baseAddr + start;
                        if (!stringMap.containsKey(addr) || stringKind.get(addr).startsWith("raw")) {
                            stringMap.put(addr, s);
                            stringKind.put(addr, "raw");
                        }
                    }
                }
                i = j;
            } else {
                i++;
            }
        }
    }

    private static boolean isStringStart(int b) {
        return (b >= 0x20 && b < 0x7F) || b >= 0xC2;
    }

    private static boolean isStringByte(int b) {
        return (b >= 0x20 && b < 0x7F) || (b >= 0x80);
    }

    private static String decodeUtf8(byte[] buf, int off, int len) {
        try {
            CharsetDecoder dec = Charset.forName("UTF-8").newDecoder();
            dec.onMalformedInput(CodingErrorAction.REPORT);
            dec.onUnmappableCharacter(CodingErrorAction.REPORT);
            CharBuffer cb = dec.decode(ByteBuffer.wrap(buf, off, len));
            String s = cb.toString();
            if (s.length() < 4) return null;
            int printable = 0;
            for (int k = 0; k < s.length(); k++) {
                char c = s.charAt(k);
                if (c >= 0x20 && c != 0x7F) printable++;
            }
            if (printable * 10 < s.length() * 6) return null;
            return s;
        } catch (Exception e) {
            return null;
        }
    }
}

