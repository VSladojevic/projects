// generated with ast extension for cup
// version 0.8
// 25/7/2018 12:51:17


package rs.ac.bg.etf.pp1.ast;

public class ClassInside implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    private VarDeclList VarDeclList;
    private MethodDeclBrace MethodDeclBrace;

    public ClassInside (VarDeclList VarDeclList, MethodDeclBrace MethodDeclBrace) {
        this.VarDeclList=VarDeclList;
        if(VarDeclList!=null) VarDeclList.setParent(this);
        this.MethodDeclBrace=MethodDeclBrace;
        if(MethodDeclBrace!=null) MethodDeclBrace.setParent(this);
    }

    public VarDeclList getVarDeclList() {
        return VarDeclList;
    }

    public void setVarDeclList(VarDeclList VarDeclList) {
        this.VarDeclList=VarDeclList;
    }

    public MethodDeclBrace getMethodDeclBrace() {
        return MethodDeclBrace;
    }

    public void setMethodDeclBrace(MethodDeclBrace MethodDeclBrace) {
        this.MethodDeclBrace=MethodDeclBrace;
    }

    public SyntaxNode getParent() {
        return parent;
    }

    public void setParent(SyntaxNode parent) {
        this.parent=parent;
    }

    public int getLine() {
        return line;
    }

    public void setLine(int line) {
        this.line=line;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(VarDeclList!=null) VarDeclList.accept(visitor);
        if(MethodDeclBrace!=null) MethodDeclBrace.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(VarDeclList!=null) VarDeclList.traverseTopDown(visitor);
        if(MethodDeclBrace!=null) MethodDeclBrace.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(VarDeclList!=null) VarDeclList.traverseBottomUp(visitor);
        if(MethodDeclBrace!=null) MethodDeclBrace.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("ClassInside(\n");

        if(VarDeclList!=null)
            buffer.append(VarDeclList.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(MethodDeclBrace!=null)
            buffer.append(MethodDeclBrace.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [ClassInside]");
        return buffer.toString();
    }
}
