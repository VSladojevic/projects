// generated with ast extension for cup
// version 0.8
// 25/7/2018 12:51:17


package rs.ac.bg.etf.pp1.ast;

public class VariableList extends VarList {

    private VarList VarList;
    private VarSingle VarSingle;

    public VariableList (VarList VarList, VarSingle VarSingle) {
        this.VarList=VarList;
        if(VarList!=null) VarList.setParent(this);
        this.VarSingle=VarSingle;
        if(VarSingle!=null) VarSingle.setParent(this);
    }

    public VarList getVarList() {
        return VarList;
    }

    public void setVarList(VarList VarList) {
        this.VarList=VarList;
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
        if(VarList!=null) VarList.accept(visitor);
        if(VarSingle!=null) VarSingle.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(VarList!=null) VarList.traverseTopDown(visitor);
        if(VarSingle!=null) VarSingle.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(VarList!=null) VarList.traverseBottomUp(visitor);
        if(VarSingle!=null) VarSingle.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("VariableList(\n");

        if(VarList!=null)
            buffer.append(VarList.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(VarSingle!=null)
            buffer.append(VarSingle.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [VariableList]");
        return buffer.toString();
    }
}
