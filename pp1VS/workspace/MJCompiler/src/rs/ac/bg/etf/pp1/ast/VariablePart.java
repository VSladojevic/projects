// generated with ast extension for cup
// version 0.8
// 25/7/2018 12:51:17


package rs.ac.bg.etf.pp1.ast;

public class VariablePart extends VarList {

    private VarSingle VarSingle;

    public VariablePart (VarSingle VarSingle) {
        this.VarSingle=VarSingle;
        if(VarSingle!=null) VarSingle.setParent(this);
    }

    public VarSingle getVarSingle() {
        return VarSingle;
    }

    public void setVarSingle(VarSingle VarSingle) {
        this.VarSingle=VarSingle;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(VarSingle!=null) VarSingle.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(VarSingle!=null) VarSingle.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(VarSingle!=null) VarSingle.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("VariablePart(\n");

        if(VarSingle!=null)
            buffer.append(VarSingle.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [VariablePart]");
        return buffer.toString();
    }
}
