// generated with ast extension for cup
// version 0.8
// 25/7/2018 12:51:17


package rs.ac.bg.etf.pp1.ast;

public class ConstSingle implements SyntaxNode {

    private SyntaxNode parent;
    private int line;
    private String I1;
    private ConstName ConstName;

    public ConstSingle (String I1, ConstName ConstName) {
        this.I1=I1;
        this.ConstName=ConstName;
        if(ConstName!=null) ConstName.setParent(this);
    }

    public String getI1() {
        return I1;
    }

    public void setI1(String I1) {
        this.I1=I1;
    }

    public ConstName getConstName() {
        return ConstName;
    }

    public void setConstName(ConstName ConstName) {
        this.ConstName=ConstName;
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
        if(ConstName!=null) ConstName.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(ConstName!=null) ConstName.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(ConstName!=null) ConstName.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("ConstSingle(\n");

        buffer.append(" "+tab+I1);
        buffer.append("\n");

        if(ConstName!=null)
            buffer.append(ConstName.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [ConstSingle]");
        return buffer.toString();
    }
}
