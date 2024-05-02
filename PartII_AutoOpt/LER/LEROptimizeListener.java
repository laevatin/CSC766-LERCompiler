import java.util.LinkedList;
import java.util.HashMap;
import java.util.Map;

public class LEROptimizeListener extends GloryBaseListener {

    private int loopCount = 0;
    private LinkedList<Integer> loopTypeList;
    private Map<String, LinkedList<String>> valIdxMap;
    
    private class Loop {
        String loopIdx;
        String lBound;
        String uBound;
        int type;

        Loop(String loopIdx, String lBound, String uBound) {
            this.loopIdx = loopIdx;
            this.lBound = lBound;
            this.uBound = uBound;
            this.type = GloryParser.NORMAL;
        }

        public String genLoop() {
            StringBuilder sb = new StringBuilder();
            switch (type) {
                case GloryParser.NORMAL:
                    sb.append("Γ");
                    break;
                case GloryParser.SUMMATION:
                    sb.append("Σ");
                    break;
                case GloryParser.PRODUCT:
                    sb.append("Π");
                    break;
                default:
                    sb.append("Ψ");
                    break;
            }
    
            String l = String.format("%s∫%s,%s∫", loopIdx, lBound, uBound);
            sb.append(l);
            return sb.toString();
        }
    };

    private class ArrayVar {
        String varName;
        LinkedList<String> idxList;

        ArrayVar(String varName, LinkedList<String> idxList) {
            this.varName = varName;
            this.idxList = idxList;
        }

        ArrayVar(String varName) {
            this.varName = varName;
            this.idxList = new LinkedList<>();
        }

        String genVar() {
            if (idxList.size() == 0) {
                return varName;
            }
            StringBuilder sb = new StringBuilder();
            sb.append(varName);
            sb.append("[");
            for (int i = 0; i < idxList.size(); i++) {
                sb.append(idxList.get(i));
                if (i < idxList.size() - 1) {
                    sb.append(",");
                }
            }
            sb.append("]");
            return sb.toString();
        }
    };

    private LinkedList<Loop> loopList;
    private Map<String, LinkedList<String>> relLoops;
    int tmpCount = 0;
    private LinkedList<String> optimizedStatements;

    LEROptimizeListener() {
        valIdxMap = new HashMap<>();
        relLoops = new HashMap<>();
        loopTypeList = new LinkedList<>();
        loopList = new LinkedList<>();
        optimizedStatements = new LinkedList<>();
    }

    @Override
    public void exitStatement(GloryParser.StatementContext ctx) {
        // System.out.println("Statement exited.");

        // update loop type
        for (int i = 0; i < loopList.size(); i++) {
            loopList.get(i).type = loopTypeList.get(i);
        }

        // relLoops
        for (Loop loop : loopList) {
            String loopIdx = loop.loopIdx;
            if (!relLoops.containsKey(loopIdx)) {
                LinkedList<String> list = new LinkedList<>();
                relLoops.put(loopIdx, list);
            } 
            for (Map.Entry<String, LinkedList<String>> entry : valIdxMap.entrySet()) {
                if (entry.getValue().contains(loopIdx)) {
                    for (String val : entry.getValue()) {
                        if (!relLoops.get(loopIdx).contains(val)) {
                            relLoops.get(loopIdx).add(val);
                        }
                    }
                }
            }
        }
        
        LinkedList<Loop> redundantConstantLoop = new LinkedList<>();
        for (Map.Entry<String, LinkedList<String>> entry : relLoops.entrySet()) {
            if (entry.getValue().size() == 0) {
                for (Loop loop : loopList) {
                    if (loop.loopIdx.equals(entry.getKey())) {
                        redundantConstantLoop.add(loop);
                    }
                }
            }
        }

        if (redundantConstantLoop.size() > 0) {
            OptimizeConstantLoopRedundancy(ctx, redundantConstantLoop);
        }
    }

    @Override
    public void enterL(GloryParser.LContext ctx) {

        if (ctx.loopType == null) {
            return;
        }
        loopTypeList.add(ctx.loopType.getType());
        loopCount++;
    }

    @Override
    public void enterFactor(GloryParser.FactorContext ctx) {
        super.enterFactor(ctx);
        // System.out.println("enterFactor: " + ctx.getText());
        String factor = ctx.getText();
        if (factor.contains("[")) {
            LinkedList<String> list = new LinkedList<>();
            
            GloryParser.ExpressionContext expr = ctx.expression();
            if (expr != null) {
                list.add(expr.getText());
            }
            GloryParser.ExprListContext exprlist = ctx.exprList();
            if (exprlist != null) {
                // split by ','
                String [] ids = exprlist.getText().split(",");
                for (String id : ids) {
                    list.add(id);
                }
            }

            valIdxMap.put(ctx.id().getText(), list);
        } 
    }

    @Override
    public void enterId(GloryParser.IdContext ctx) {
        super.enterId(ctx);
    }

    @Override
    public void enterForParam(GloryParser.ForParamContext ctx) {
        loopList.add(new Loop(ctx.id().getText(), ctx.lBound().getText(), ctx.uBound().getText()));
    }

    @Override
    public void exitL(GloryParser.LContext ctx) {
        loopCount--;
        // System.out.println("Exiting loop. Remaining loops: " + loopCount);
    }

    private String idFilter(String string) {

        StringBuilder sb = new StringBuilder();

        boolean andSwitch = true; //
        for (char ch : string.toCharArray()) {
            if (ch == '$') { // when encounter a '&', flip
                andSwitch = !andSwitch;
                continue;
            }

            if (andSwitch) {
                sb.append(ch);
            }
        }
        return sb.toString();
    }

    private String genConstant(String left, ArrayVar rightVar) {
        StringBuilder sb = new StringBuilder();
        sb.append(left);
        sb.append("=");
        sb.append(rightVar.genVar());
        return sb.toString();
    }

    private void OptimizeConstantLoopRedundancy(GloryParser.StatementContext ctx, LinkedList<Loop> redundantConstantLoop) {
        LinkedList<ArrayVar> tmpArrayVars = new LinkedList<>();
        for (Loop loop : redundantConstantLoop) {
            tmpArrayVars.add(new ArrayVar("tmp" + tmpCount));
            tmpCount++;
            if (loop.lBound.equals("0")) {
                optimizedStatements.add(genConstant(loop.uBound, tmpArrayVars.getLast()));
            } else {
                optimizedStatements.add(genConstant(loop.uBound + "-" + loop.lBound, tmpArrayVars.getLast()));
            }
            loopList.remove(loop);
        }

        StringBuilder sb = new StringBuilder();
        for (Loop loop : loopList) {
            sb.append(loop.genLoop());
        }

        for (ArrayVar var : tmpArrayVars) {
            sb.append(var.genVar());
            sb.append("*");
        }

        String r = idFilter(ctx.r().getText());
        String e = idFilter(ctx.e().getText());
        sb.append(e);
        sb.append("=");
        sb.append(r);
        optimizedStatements.add(sb.toString());
        for (String stat : optimizedStatements) {
            System.out.println(stat);
        }
    }


}
